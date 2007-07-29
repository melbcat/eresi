/*
** sct.c for libkernsh
**
** $Id: sct.c,v 1.4 2007-07-29 16:54:36 pouik Exp $
**
*/
#include "libkernsh.h"

/* Put syscalls inside the list lsct */
int kernsh_sct(list_t *lsct)
{
  int ret;
  u_int         dim[3];
  vector_t      *sct;
  int          (*fct)();

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if(lsct == NULL)
    {
      PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		   "List is NULL !", -1);
    }
  
  sct = aspect_vector_get("sct");
  dim[0] = libkernshworld.arch;
  dim[1] = libkernshworld.os;

  fct = aspect_vectors_select(sct, dim);

  ret = fct(lsct);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, ret);
}


/* Get syscalls on Linux */
int kernsh_sct_linux(list_t *lsct)
{
  int i;
  char *key;
  libkernshsyscall_t *syscall;
  elfsh_SAddr   offset;
  unsigned long start;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

#if __DEBUG_KERNSH__
  printf("SCT LINUX\n");
#endif


  /* Get syscalls in the static kernel */
  if (kernsh_is_static_mode())
    {
      offset = 0;
      for (i = (int) config_get_data(LIBKERNSH_VMCONFIG_NB_SYSCALLS) - 1;
	   i >= 0;
	   i--)
	{
	  XALLOC(__FILE__, 
		 __FUNCTION__, 
		 __LINE__, 
		 syscall,
		 sizeof(libkernshsyscall_t), 
		 -1);
	  
	  XALLOC(__FILE__, 
		 __FUNCTION__, 
		 __LINE__, 
		 key,
		 BUFSIZ, 
		 -1);
	  
	  memset(syscall, '\0', sizeof(libkernshsyscall_t)); 
	  memset(key, '\0', BUFSIZ);
	  snprintf(key,
		   BUFSIZ,
		   "%d",
		   i);

	  start = elfsh_get_foffset_from_vaddr(libkernshworld.root, 
					       libkernshworld.sct + sizeof(unsigned long)*i);

	  elfsh_raw_read(libkernshworld.root,
			 start,
			 &syscall->addr,
			 sizeof(unsigned long));

	  memcpy(syscall->name, 
		 elfsh_reverse_symbol(libkernshworld.root, 
				      (elfsh_Addr) syscall->addr, 
				      &offset),
		 sizeof(syscall->name));

	  /* Add syscall to the list */
	  list_add(lsct, key, (void *) syscall);
	}
    }
  /* Get syscalls in the memory */
  else {
      if (!libkernshworld.open)
	{
	  PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		       "Memory not open !", -1);
	}

      for (i = (int) config_get_data(LIBKERNSH_VMCONFIG_NB_SYSCALLS) - 1;
	   i >= 0;
	   i--)
	{
	  XALLOC(__FILE__, 
		 __FUNCTION__, 
		 __LINE__, 
		 syscall,
		 sizeof(libkernshsyscall_t), 
		 -1);

	  XALLOC(__FILE__, 
		 __FUNCTION__, 
		 __LINE__, 
		 key,
		 BUFSIZ, 
		 -1);

	  memset(syscall, '\0', sizeof(libkernshsyscall_t)); 
	  memset(key, '\0', BUFSIZ);
	  snprintf(key,
		   BUFSIZ,
		   "%d",
		   i);

	  kernsh_readmem(libkernshworld.sct + sizeof(unsigned long) * i, 
			 &syscall->addr, 
			 sizeof(unsigned long));
	  
	  kernsh_resolve_systemmap(syscall->addr, 
				   syscall->name, 
				   sizeof(syscall->name));

	  /* Add syscall in the list */
	  list_add(lsct, key, (void *) syscall);
	}
    }

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0); 
}

int kernsh_sct_netbsd(list_t *lsct)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

int kernsh_sct_freebsd(list_t *lsct)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

int kernsh_syscall(int num, int argc, unsigned int argv[])
{
  int __ret = 0;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);
  
  if (argc > 5 || argc < 0)
    {
      PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		   "invalid number of arguments", -1);
    }

  switch(argc) 
    {
    case 0 :
      SYSCALL0(num);
      break;
    case 1 :
      SYSCALL1(num, argv[0]);
      break;
    case 2:
      SYSCALL2( num, argv[0], argv[1] );
      break;
    case 3:
      SYSCALL3 (num, argv[0], argv[1], argv[2] );
      break;
    case 4:
      SYSCALL4 (num, argv[0], argv[1], argv[2], argv[3] );
      break;
    case 5:
      SYSCALL5 (num, argv[0], argv[1], argv[2], argv[3], argv[4] );
      break;
    }
  
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, __ret);
}

