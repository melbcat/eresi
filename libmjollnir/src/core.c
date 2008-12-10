/*
** (C) 2006-2008 The ERESI team
**
** @file core.c
** @brief Implement low-level functions of the libmjollnir library
**
*/
#include "libmjollnir.h"


/**
 * @brief Core control flow analysis function at a given address
 * @param sess Mjollnir session
 * @param ptr Code buffer pointer to analyze
 * @param offset Offset into the code buffer to analyze
 * @param vaddr Entry point address for analysis
 * @param len Size of code to analyse
 * @param curdepth current depth of cfg being analyzed
 * @param maxdepth depth limit of analysis (== MJR_MAX_DEPTH for limitless)
 * @return Success (0) or error (-1).
 */
int		mjr_analyse_rec(mjrsession_t *sess, eresi_Addr vaddr, int curdepth, int maxdepth)
{
  asm_instr     instr;
  unsigned int  curr;
  int		ilen;
  eresi_Addr	trueaddr, falseaddr;
  container_t	*curblock;
  mjrblock_t	*block;
  u_int		delayslotsize;
  u_char	*ptr;
  u_int		curlen;
  u_int		restlen;
  elfshsect_t	*sect;
  elfsh_SAddr	off;
  u_char	eos;
  u_int		depthinc;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  /* Check if we have reached the limit bound */
  if (maxdepth > 0 && curdepth >= maxdepth)
    PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);

  trueaddr = falseaddr = MJR_BLOCK_INVALID;
  delayslotsize = 0;

  /* Create a new block if current address is out of all existing ones */
  curblock = (container_t *) hash_get(&sess->cur->blkhash, _vaddr2str(vaddr));
  
#if __DEBUG_MJOLLNIR__
  fprintf(D_DESC, "[D] core.c:analyse_code: bloc requested at vaddr " XFMT " offset %u\n", 
	  vaddr, offset);
#endif

  assert(curblock != NULL);

  block = (mjrblock_t *) curblock->data;

#if __DEBUG_MJOLLNIR__
  fprintf(D_DESC, "[D] %s: bloc " XFMT ": seen %hhd\n",
          __FUNCTION__, block->vaddr, block->seen);
#endif

  /* Avoid loops */
  if (block->seen == 1)
    PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
  block->seen = 1;

  /* Read data at this block's addr */
  sect = elfsh_get_parent_section(sess->cur->obj, vaddr, &off);
  curlen = MJR_MIN(sect->shdr->sh_size - off, MJR_MAX_BLOCK_SIZE);
  eos = (curlen != MJR_MAX_BLOCK_SIZE ? 1 : 0);
  XALLOC(__FILE__, __FUNCTION__, __LINE__, ptr, curlen, -1);
  ptr = elfsh_readmema(sess->cur->obj, vaddr, ptr, curlen);

  /* Read all instructions, making sure we never override section's boundaries */
  for (curr = 0; vaddr + curr < sect->shdr->sh_addr + sect->shdr->sh_size; curr += ilen)
    {

      /* Reallocate the buffer if necessary */
      if (!eos && curr + 20 >= curlen)
	{
	  sect = elfsh_get_parent_section(sess->cur->obj, vaddr + curr, &off);
	  restlen = MJR_MIN(sect->shdr->sh_size - off, MJR_MAX_BLOCK_SIZE);
	  eos = (restlen != MJR_MAX_BLOCK_SIZE ? 1 : 0);
	  XREALLOC(__FILE__, __FUNCTION__, __LINE__, ptr, ptr, curlen + restlen, -1);
	  curlen += restlen;
	  ptr = elfsh_readmema(sess->cur->obj, vaddr + curr, ptr + curr, curlen - curr);
	  vaddr += curr;
	  curlen -= curr;
	  curr = 0;
	}

      /* Analyse current instruction */
      ilen = asm_read_instr(&instr, ptr + curr, curlen - curr, &sess->cur->proc);

#if __DEBUG_READ__
      fprintf(D_DESC,"[D] %s/%s,%d: ilen=%d first byte=%02x\n", 
	      __FUNCTION__, __FILE__, __LINE__, ilen, *(ptr + curr));
#endif
      
      /* This is an error that should never happens, or we have a libasm bug */
      if (ilen <= 0) 
	{
	  printf(" [D] asm_read_instr returned -1 at address " XFMT "\n", vaddr + curr);
	  exit(-1);
	}

      mjr_history_shift(sess->cur, instr, vaddr + curr);
      block->size += ilen;	  

#if __DEBUG_READ__
      fprintf(stderr, " [D] curaddr WILL BE analyzed: "XFMT" (curr = %d, ilen = %d) \n", 
	      vaddr + curr, curr, ilen);
#endif

      /* Increase block size for delay slot if any */
      delayslotsize = mjr_trace_control(sess->cur, curblock, sess->cur->obj, &instr, 
					vaddr + curr, &trueaddr, &falseaddr);

#if __DEBUG_READ__
      fprintf(stderr, " [D] curaddr analyzed: "XFMT" (trueaddr = "XFMT", falseaddr = "XFMT")\n",
	      vaddr + curr, trueaddr, falseaddr);
#endif

      /* If we have found a contiguous block, stop this recursion now */
      if (trueaddr == MJR_BLOCK_EXIST)
	{
	  block->size -= ilen;
	  break;
	}

      /* Recurse on next blocks */
      if (trueaddr != MJR_BLOCK_INVALID) 
	{
	  depthinc = (instr.type & ASM_TYPE_CALLPROC ? 1 : 0);
	  mjr_analyse_rec(sess, trueaddr, curdepth + depthinc, maxdepth);
	}

      if (falseaddr != MJR_BLOCK_INVALID) 
	mjr_analyse_rec(sess, falseaddr, curdepth, maxdepth);

      /* If we have recursed, the current block is over */
      if (falseaddr != MJR_BLOCK_INVALID || trueaddr != MJR_BLOCK_INVALID || 
	  (instr.type & ASM_TYPE_RETPROC) || (instr.type & ASM_TYPE_STOP))
	{

#if __DEBUG_MJOLLNIR__
	  fprintf(D_DESC, "[D] core.c:analyse_code: bloc at vaddr " XFMT " with delayslot size %u\n", 
		  block->vaddr, delayslotsize);
#endif
	  
	  block->size += delayslotsize;
	  //only free in kernsh/kedbg in runtime mode
	  //XFREE(__FILE__, __FUNCTION__, __LINE__, ptr);
	  break;
	}
    }
  
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}


/**
 * @brief Analyse control flow at a given address
 * @param sess Mjollnir session
 * @param addr Entry point address for analysis
 * @param maxdepth Maximum depth of the analysis in the CFG
 * @param flags <FIXME:NotImplemented>
 * @return Success (0) or Error (-1).
 */
int		mjr_analyse(mjrsession_t *sess, eresi_Addr addr, int maxdepth, int flags)
{
  elfshsect_t   *parent;
  elfsh_SAddr   offset;
  eresi_Addr	e_entry;
  eresi_Addr	main_addr;
  u_char	*ptr;
  container_t	*cont;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);
  if (!addr || !sess)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, "Invalid null parameters", -1);

  /* Make sure the input address is mapped somewhere */
  parent = elfsh_get_parent_section(sess->cur->obj, addr, &offset);
  if (!parent)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, "Unable to find parent section", -1);

  /* Create a container for the starting block */
  cont = mjr_create_block_container(sess->cur, 0, addr, 0, 0);
  if (!cont)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__,
		 "Can't create initial block", -1);
  hash_add(&sess->cur->blkhash, _vaddr2str(addr), cont);

  /* Check if we are starting at the entry point -- if yes, special treatment is to be done */
  e_entry = elfsh_get_entrypoint(elfsh_get_hdr(sess->cur->obj));
  if (addr == e_entry)
    {
      printf(" [*] Entry point: " AFMT "\n", e_entry);
      XALLOC(__FILE__, __FUNCTION__, __LINE__, ptr, MJR_MAX_BLOCK_SIZE * 2, -1);
      elfsh_readmema(sess->cur->obj, addr, ptr, MJR_MAX_BLOCK_SIZE * 2);
      main_addr = mjr_trace_start(sess->cur, ptr, MJR_MAX_BLOCK_SIZE * 2, e_entry);
      XFREE(__FILE__, __FUNCTION__, __LINE__, ptr);
      printf(" [*] main located at " AFMT "\n", main_addr);
    }

  /* We are not analyzing the entry point */
  else   
    {
      main_addr = 0;
      sess->cur->func_stack = elist_empty(sess->cur->func_stack->name);
      cont = mjr_create_function_container(sess->cur, addr, 0, _vaddr2str(addr), 0, NULL);
      sess->cur->curfunc = cont;
      mjr_function_register(sess->cur, addr, cont);
      elist_push(sess->cur->func_stack, cont);
    }

  /* Analyse recursively, starting at requested address */
  if (mjr_analyse_rec(sess, addr, 0, maxdepth) < 0)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__,
                 "Error during code analysis", -1);

  /* If requested address was the entry point, also analyse the infered main() address */
  if (main_addr)
    {
      sess->cur->func_stack = elist_empty(sess->cur->func_stack->name);
      cont = mjr_function_get_by_vaddr(sess->cur, main_addr);
      sess->cur->curfunc = cont;
      elist_push(sess->cur->func_stack, cont);
      if (mjr_analyse_rec(sess, main_addr, 0, maxdepth) < 0)
	PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__,
		     "Error during code analysis", -1);
    }

  /* Store analysis on disk */
  if (mjr_analyse_finished(sess) < 0)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__,
                 "Error during storage of analysis info", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/** Store control-flow analysis information on disk */
int             mjr_analyse_finished(mjrsession_t *sess)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  /* Store analyzed functions in file */
  if (mjr_flow_store(sess->cur, ASPECT_TYPE_FUNC) <= 0)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__,
                 "Unable to store functions in file", -1);

  /* Store analyzed blocks in file */
  if (mjr_flow_store(sess->cur, ASPECT_TYPE_BLOC) <= 0)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__,
                 "Unable to store blocks in file", -1);

  /* Set the flag and return */
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}


/** Indicate if a given address has already been analyzed */
int		mjr_analysed(mjrsession_t *sess, eresi_Addr addr)
{
  int		analysed;
  container_t	*cont;
  char		*str;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);
  str = _vaddr2str(addr);
  cont = (container_t *) hash_get(&sess->cur->blkhash, str);
  XFREE(__FILE__, __FUNCTION__, __LINE__, str);
  analysed = (cont ? 1 : 0);
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, analysed);
}
