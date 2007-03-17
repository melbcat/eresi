/*
** version.c for elfsh
** 
** Started     Nov 25 2003 mayhem
** Last update Nov 25 2003 mayhem
** Last update Sep 27 2005 mxatone
** 
**
** $Id: version.c,v 1.6 2007-03-17 17:26:06 mxatone Exp $
**
*/
#include "libelfsh.h"

static int version_parent = -1;
static int version_need = -1;

/**
 * Check an equivalence between a version def entry and a version need entry on 
 * two different files
 * @param file file for version need section
 * @param deffile file for version def section
 * @param need version need entry
 * @param def version def entry
 * @return need auxiliary entry
 */
elfsh_Vernaux		*elfsh_check_defneed_eq(elfshobj_t *file, elfshobj_t *deffile,
					       elfsh_Verneed *need, elfsh_Verdef *def)
{
  elfsh_Vernaux		*needaux;
  elfsh_Verdaux		*defaux;
  void			*data;
  void			*defdata;
  u_int			offset;
  u_int			defoffset;
  char			*needname;
  char			*defname;
  u_int			index, defndx;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (!file || !deffile || !need || !def)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		 "Invalid parameters", NULL);

  data = need;
  for (index = 0, offset = need->vn_aux; 
       index < need->vn_cnt; 
       index++, offset += needaux->vna_next)
    {
      needaux = data + offset;

      /* We find a first correc entry */
      if (needaux->vna_hash == def->vd_hash)
	{
	  needname = elfsh_get_vernauxname(file, needaux);

	  /* The definition as multiple entry, we need a correct one */
	  defdata = def;
	  for (defndx = 0, defoffset = def->vd_aux; 
	       defndx < def->vd_cnt; 
	       defndx++, defoffset += defaux->vda_next)
	    {
	      defaux = defdata + defoffset;

	      defname = elfsh_get_verdauxname(deffile, defaux);

	      /* Check the name */
	      if (!strcmp(needname, defname))
		  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, needaux);

	      if (defaux->vda_next == 0)
		break;
	    }
	}

      if (needaux->vna_next == 0)
	break;
    }

  PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
	       "Cannot find equivalent entries", NULL);
}

/**
 * Return a Version need entry value for aux (aux) 
 */
elfsh_Word    		elfsh_get_verneed_aux(elfsh_Verneed *need)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 2)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (need == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (need->vn_aux));
}

/**
 * Set a Version need entry value for aux (aux) 
 */
int			elfsh_set_verneed_aux(elfsh_Verneed *need, elfsh_Word val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 2)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (need == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  need->vn_aux = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}


/**
 * Return a Version need (aux) entry value for count (count) 
 */
elfsh_Half    		elfsh_get_verneed_cnt(elfsh_Verneed *need)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 2)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (need == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (need->vn_cnt));
}

/**
 * Set a Version need entry value for count (count) 
 */
int			elfsh_set_verneed_cnt(elfsh_Verneed *need, elfsh_Half val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 2)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (need == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  need->vn_cnt = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Return a Version need entry value for file (file) 
 */
elfsh_Word    		elfsh_get_verneed_file(elfsh_Verneed *need)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 2)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (need == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (need->vn_file));
}

/**
 * Set a Version need entry value for file (file) 
 */
int			elfsh_set_verneed_file(elfsh_Verneed *need, elfsh_Word val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 2)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (need == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  need->vn_file = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Return a Version need (aux) entry value for next (next) normal & parent 
 */
elfsh_Word    		elfsh_get_verneed_next(void *p)
{
  elfsh_Verneed		*need;
  elfsh_Vernaux		*aux;
  
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);  

  if (p == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  if (version_need == 1)
    {
      aux = p;
      PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (aux->vna_next));
    } 

  need = p;
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (need->vn_next));
}


/**
 * Set a Version need (aux) entry value for next (next) normal & parent 
 */
int			elfsh_set_verneed_next(void *p, elfsh_Word val)
{
  elfsh_Verneed		*need;
  elfsh_Vernaux		*aux;
  
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (p == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  if (version_need == 1)
    {
      aux = p;
      aux->vna_next = val;
    } 
  else
    {
      need = p;
      need->vn_next = val;
    }

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Return a Version need (aux) entry value for flags (flag) 
 */
elfsh_Half    		elfsh_get_verneed_flags(elfsh_Vernaux *naux)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (naux == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (naux->vna_flags));
}

/**
 * Set a Version need (aux) entry value for flags (flag) 
 */
int			elfsh_set_verneed_flags(elfsh_Vernaux *naux, elfsh_Half val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (naux == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  naux->vna_flags = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Return a Version need (aux) entry value for hash (hash) 
 */
elfsh_Word    		elfsh_get_verneed_hash(elfsh_Vernaux *naux)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (naux == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (naux->vna_hash));
}

/**
 * Set a Version need (aux) entry value for hash (hash) 
 */
int			elfsh_set_verneed_hash(elfsh_Vernaux *naux, elfsh_Word val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (naux == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  naux->vna_hash = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/* Return a Version need (aux) entry value for name (name) */
elfsh_Word    		elfsh_get_verneed_name(elfsh_Vernaux *naux)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (naux == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (naux->vna_name));
}

/**
 * Set a Version need (aux) entry value for name (name) 
 */
int			elfsh_set_verneed_name(elfsh_Vernaux *naux, elfsh_Word val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (naux == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  naux->vna_name = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Return a Version need (aux) entry value for other (index) 
 */
elfsh_Half    		elfsh_get_verneed_ndx(elfsh_Vernaux *naux)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (naux == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (naux->vna_other));
}


/**
 * Set a Version need (aux) entry value for ndx (index) 
 */
int			elfsh_set_verneed_ndx(elfsh_Vernaux *naux, elfsh_Half val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_need != 1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);    

  if (naux == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  naux->vna_other = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Get Version need by name 
 */
elfsh_Verneed		*elfsh_get_verneed_by_name(elfshobj_t *file, char *name)
{
  elfsh_Verneed		*table;
  char			*filename;
  u_int			offset = 0;
  int			versize;
  void			*ps;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (file == NULL || name == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", NULL);

  ps = elfsh_get_verneedtab(file, &versize);
  if (ps == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Unable to get VERNEED", NULL);

  version_need = 2;

  do
    {
      table = ps + offset;

      filename = elfsh_get_verneedfile(file, table);

      if (!strcmp(name, filename))
	  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (table));

      if (table->vn_next == NULL)
	break;

      offset += table->vn_next;
    } while (table != NULL);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, NULL);  
}

/**
 * Get Version need by index (aux) 
 */
elfsh_Vernaux        	*elfsh_get_verneed_entry_by_index(void *ps, 
						      elfsh_Half index)
{
  elfsh_Verneed		*table;
  elfsh_Vernaux		*tableaux;
  u_int			i, offset = 0, auxset = 0;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  version_need = 1;

  do
    {
      table = ps + offset;

      auxset = offset + table->vn_aux;
      for(i = 0; i < table->vn_cnt; i++)
	{
	  tableaux = ps + auxset;

	  if (tableaux->vna_other == index)
	    PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (tableaux));

	  if (tableaux->vna_next == NULL)
	    break;

	  auxset += tableaux->vna_next;
	}
      if (table->vn_next == NULL)
	break;

      offset += table->vn_next;
    } while (table != NULL);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, NULL);
}

/**
 * Return a Version def entry value for cname (name) [child] 
 */
elfsh_Word    		elfsh_get_verdef_cname(elfsh_Verdaux *def)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent <= -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (def->vda_name));
}

/**
 * Set a Version def entry value for cname (name) [child] 
 */
int			elfsh_set_verdef_cname(elfsh_Verdaux *def, elfsh_Word val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent <= -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  def->vda_name = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}


/**
 * Return a Version def entry value for cnext (next) [child]
 */
elfsh_Word    		elfsh_get_verdef_cnext(elfsh_Verdaux *def)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent <= -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (def->vda_next));
}

/**
 * Set a Version def entry value for cnext (next) [child] 
 */
int			elfsh_set_verdef_cnext(elfsh_Verdaux *def, elfsh_Word val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent <= -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  def->vda_next = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Return a Version def entry value for next (next) normal & aux 
 */
elfsh_Word    		elfsh_get_verdef_next(void *p)
{
  elfsh_Verdef		*def;
  elfsh_Verdaux		*aux;
  
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (p == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  if (version_parent > -1)
    {
      aux = p;
      PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (aux->vda_next));
    }

  def = p;
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (def->vd_next));    
}

/**
 * Set a Version def entry value for next (next) normal & aux 
 */
int			elfsh_set_verdef_next(void *p, elfsh_Word val)
{
  elfsh_Verdef		*def;
  elfsh_Verdaux		*aux;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (p == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  if (version_parent > -1)
    {
      aux = p;
      aux->vda_next = val;
    }
  else
    {
      def = p;
      def->vd_next = val;
    }

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Return a Version def entry value for aux (aux) 
 */
elfsh_Word    		elfsh_get_verdef_aux(elfsh_Verdef *def)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent > -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (def->vd_aux));
}

/**
 * Set a Version def entry value for aux (aux) 
 */
int			elfsh_set_verdef_aux(elfsh_Verdef *def, elfsh_Word val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent > -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  def->vd_aux = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Return a Version def entry value for flags (flags) 
 */
elfsh_Half    		elfsh_get_verdef_flags(elfsh_Verdef *def)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent > -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (def->vd_flags));
}

/**
 * Set a Version def entry value for flags (flags) 
 */
int			elfsh_set_verdef_flags(elfsh_Verdef *def, elfsh_Half val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent > -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  def->vd_flags = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Return a Version def entry value for hash (hash) 
 */
elfsh_Word    		elfsh_get_verdef_hash(elfsh_Verdef *def)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent > -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (def->vd_hash));
}

/**
 * Set a Version def entry value for hash (hash) 
 */
int			elfsh_set_verdef_hash(elfsh_Verdef *def, elfsh_Word val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent > -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  def->vd_hash = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Return a Version def entry value for cnt (count) 
 */
elfsh_Half    		elfsh_get_verdef_cnt(elfsh_Verdef *def)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent > -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (def->vd_cnt));
}

/**
 * Set a Version def entry value for cnt (count) 
 */
int			elfsh_set_verdef_cnt(elfsh_Verdef *def, elfsh_Half val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent > -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  def->vd_cnt = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Return a Version def entry value for ndx (index) 
 */
elfsh_Half    		elfsh_get_verdef_ndx(elfsh_Verdef *def)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent > -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (def->vd_ndx));
}


/**
 * Set a Version def entry value for ndx (index) 
 */
int			elfsh_set_verdef_ndx(elfsh_Verdef *def, elfsh_Half val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (version_parent > -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid L2 request object", -1);   

  if (def == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  def->vd_ndx = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Get Version def by index 
 */
void	           	*elfsh_get_verdef_entry_by_index(void *ps, 
						      elfsh_Half index)
{
  elfsh_Verdef		*table;
  elfsh_Verdaux		*tableaux;
  u_int			i = 0, offset = 0, auxset = 0;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  do
    {
      table = ps + offset;

      if (table->vd_ndx == index && version_parent <= -1)
	    PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (table));

      if (table->vd_ndx == version_parent && version_parent > -1)
	{
	  auxset = offset + table->vd_aux;
	  for(i = 0; i <= index; i++)
	    {
	      tableaux = ps + auxset;

	      if (i >= index)
		break;

	      if (tableaux->vda_next == NULL)
		break;
		  
	      auxset += tableaux->vda_next;
	    }

	  if (i != index)
	    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
			      "Subindex not found", NULL);

	  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (tableaux));
	}

      if (table->vd_next == NULL)
	break;

      offset += table->vd_next;
    } while (table != NULL);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, NULL);
}

/**
 * Get Version symbols by index 
 */
elfsh_Half    		*elfsh_get_versym_entry_by_index(elfsh_Half *s, 
						      u_int index)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (s + index));
}


/**
 * Return a Version symbol entry value 
 */
elfsh_Half    		elfsh_get_versym_val(elfsh_Half *sym)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (sym == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (*sym));
}


/**
 * Set a Version symbol entry value 
 */
int			elfsh_set_versym_val(elfsh_Half *sym, elfsh_Half val)
{
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (sym == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", -1);

  *sym = val;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}


/**
 * Return the version symbol entry giving its name 
 */
elfsh_Half		*elfsh_get_versym_by_name(elfshobj_t *file, char *name)
{
  elfsh_Sym		*ret;
  void			*versym;
  elfsh_Half		*verh;
  int			index;
  int			size = 0, versize = 0;
  char			*actual;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (file == NULL || name == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Invalid NULL parameter", NULL);

  ret = elfsh_get_dynsymtab(file, &size);
  if (ret == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Unable to get DYNSYM", NULL);

  versym = elfsh_get_versymtab(file, &versize);
  if (versym == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Unable to get VERSSYM", NULL);

  verh = versym;
  for (index = 0; index < size; index++)
    {
      actual = elfsh_get_dynsymbol_name(file, ret + index);
      if (actual && !strcmp(actual, name))
	  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (verh + index));
    }
  
  PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		    "Symbol not found", NULL);
}


/**
 * Missing
 */
int 			elfsh_get_verdauxnamelist(elfshobj_t *file, 
						  hashdef_t *hdef, 
						  char **names, 
						  int numnames)
{
  elfsh_Verdaux		*table;
  void			*ps;
  u_int			dindex;
  u_int			offset;
  
  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  ps = hdef->ps;
  offset = (u_int) hdef->aux;
  for (dindex = 0; dindex < numnames; dindex++)
    names[dindex] = NULL;

  for (dindex = 0; dindex < hdef->def->vd_cnt; dindex++)
    {
      table = ps + offset;
      if (dindex < numnames)
	  names[dindex] = elfsh_get_verdauxname(file, table);

      if (table->vda_next == NULL)
	break;

      offset += table->vda_next;
    }

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/**
 * Missing
 */
int			elfsh_load_needtable(hash_t *t, void *ps, 
					     u_int size)
{
  elfsh_Verneed		*table;
  elfsh_Vernaux		*tableaux;
  hashneed_t		*pneed;
  u_int			offset;
  u_int			auxset;
  u_int			index;
  u_int			aux;
  char			s_temp[8];

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  for (index = 0, offset = 0; offset < size; index++)
    {
      table = ps + offset;

      auxset = offset + table->vn_aux;
      for (aux = 0; aux < table->vn_cnt; aux++) 
	{
	  tableaux = ps + auxset;

	  snprintf(s_temp, 8, "%u", tableaux->vna_other);
	  XALLOC(__FILE__, __FUNCTION__, __LINE__,pneed, sizeof(hashneed_t), -1);
	  pneed->need = table;
	  pneed->aux = tableaux;
	  hash_add(t, strdup(s_temp), (void*) pneed);
 
	  if (tableaux->vna_next == 0)
	    break;

	  auxset += tableaux->vna_next;
	}
      
      if (table->vn_next == 0)
	break;

      offset += table->vn_next;
    }

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}

/** 
 * Missing
 */
int			elfsh_load_deftable(hash_t *t, void *ps, u_int size)
{
  elfsh_Verdef		*table;
  hashdef_t		*pdef;
  u_int			offset;
  u_int			index;
  char			s_temp[8];

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);
  for (index = 0, offset = 0; offset < size; index++)
    {
      table = ps + offset;
      snprintf(s_temp, 8, "%u", table->vd_ndx);
      XALLOC(__FILE__, __FUNCTION__, __LINE__,pdef, sizeof(hashdef_t), -1);
      pdef->ps  = ps;
      pdef->def = table;
      pdef->aux = (elfsh_Word *) offset + table->vd_aux;
      hash_add(t, strdup(s_temp), (void *) pdef);
 
      if (table->vd_next == NULL)
	break;
      offset += table->vd_next;
    }
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}


/**
 * Missing
 */
void    		*elfsh_get_verstrtable(elfshobj_t *file)
{
  elfshsect_t		*dynstr;
  int	       		strindex, dnum;
  void			*ret;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  if (file->secthash[ELFSH_SECTION_DYNSTR] == NULL)
    {

      dynstr = elfsh_get_section_by_name(file, ELFSH_SECTION_NAME_DYNSTR, 
					 NULL, &strindex, &dnum);
      if (dynstr == NULL)
	PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
			  "Section not found", NULL); 

      if (dynstr->data == NULL)
	{
	  dynstr->data = elfsh_load_section(file, dynstr->shdr);
	  if (dynstr->data == NULL)
	    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
			      "Data section not found", NULL); 
	}

      file->secthash[ELFSH_SECTION_DYNSTR] = dynstr;
    
    }

  ret = elfsh_get_raw(file->secthash[ELFSH_SECTION_DYNSTR]);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (ret));
}


/**
 * Missing
 */
char	       		*elfsh_get_verneedfile(elfshobj_t *file, elfsh_Verneed *n)
{
  void 	    		*str;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);  

  str = elfsh_get_verstrtable(file);
  if (str == NULL)
       PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Unable to find STR table for verneedfile", NULL);    

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (str + n->vn_file));
}

/**
 * Missing
 */
char			*elfsh_get_vernauxname(elfshobj_t *file, elfsh_Vernaux *a)
{
  void 			*str;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);  

  str = elfsh_get_verstrtable(file);
  if (str == NULL)
       PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Unable to find STR table for verneedfile", NULL);    

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (str + a->vna_name));
}

/**
 * Missing
 */
char			*elfsh_get_verdauxname(elfshobj_t *file, elfsh_Verdaux *a)
{
  void 			*str;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);  

  str = elfsh_get_verstrtable(file);
  if (str == NULL)
       PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Unable to find STR table for verdeffile", NULL);    

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (str + a->vda_name));
}

/**
 * Missing
 */
void			*elfsh_get_versymtab(elfshobj_t *file, int *num)
{
  elfshsect_t		*sect;
  int			strindex, nbr;
  void			*ret;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);  

  if (file->secthash[ELFSH_SECTION_GNUVERSYM] == NULL)
    {
      sect = elfsh_get_section_by_type(file, SHT_GNU_versym, 0, 
				       NULL, &strindex, &nbr);

      if (sect == NULL)
        PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
			  "Unable to find Symbol Version table", NULL);;

      if (sect->data == NULL)
	{
	  sect->data = elfsh_load_section(file, sect->shdr);
	  if (sect->data == NULL)
	    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
			      "Unable to find data for Symbol Version table", 
			      NULL); 
	}

      file->secthash[ELFSH_SECTION_GNUVERSYM] = sect;
    }

  nbr = file->secthash[ELFSH_SECTION_GNUVERSYM]->shdr->sh_size;
  nbr /= sizeof(elfsh_Half);

  if (num)
    *num = nbr;

  ret = elfsh_get_raw(file->secthash[ELFSH_SECTION_GNUVERSYM]);
  
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (ret));
}

elfshsect_t		*elfsh_get_versymtab_by_range(elfshobj_t *file, elfsh_Addr range, int *num)
{
  elfshsect_t		*sect;
  int			nbr;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);  

  sect = elfsh_get_section_by_type(file, SHT_GNU_versym, range, NULL, NULL, NULL);
  
  if (sect == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		 "Unable to find Symbol Version table", NULL);

  if (sect->data == NULL)
    {
      sect->data = elfsh_load_section(file, sect->shdr);
      if (sect->data == NULL)
	PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		     "Unable to find data for Symbol Version table", 
		     NULL); 
    }
  
  nbr = sect->shdr->sh_size;
  nbr /= sizeof(elfsh_Half);

  if (num)
    *num = nbr;

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, sect);
}

/**
 * Missing
 */
void			*elfsh_get_verneedtab(elfshobj_t *file, int *num)
{
  elfshsect_t		*sect;
  int			strindex, nbr;
  void			*ret;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);  

  version_need = -1;

  if (file->secthash[ELFSH_SECTION_GNUVERNEED] == NULL)
    {
      sect = elfsh_get_section_by_type(file, SHT_GNU_verneed, 0, 
				       NULL, &strindex, &nbr);

      if (sect == NULL)
        PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
			  "Unable to find Needed Version section", NULL);

      if (sect->data == NULL)
	{
	  sect->data = elfsh_load_section(file, sect->shdr);
	  if (sect->data == NULL)
	    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
			      "Unable to find data for Needed Version section", 
			      NULL); 
	}

      file->secthash[ELFSH_SECTION_GNUVERNEED] = sect;
    }

  nbr = file->secthash[ELFSH_SECTION_GNUVERNEED]->shdr->sh_size;
  nbr /= sizeof(elfsh_Verneed);

  if (num)
    *num = nbr;

  ret = elfsh_get_raw(file->secthash[ELFSH_SECTION_GNUVERNEED]);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (ret));
}


/**
 * Missing
 */ 
void			*elfsh_get_verdeftab(elfshobj_t *file, int *num)
{
  elfshsect_t		*sect;
  int			strindex, nbr;
  void			*ret;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);  

  version_parent = -1;

  if (file->secthash[ELFSH_SECTION_GNUVERDEF] == NULL)
    {
      sect = elfsh_get_section_by_type(file, SHT_GNU_verdef, 0, 
				       NULL, &strindex, &nbr);

      if (sect == NULL)
        PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
			  "Unable to find Definition Version section", NULL);

      if (sect->data == NULL)
	{
	  sect->data = elfsh_load_section(file, sect->shdr);
	  if (sect->data == NULL)
	    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
			      "Unable to find data for Definition "
			      "Version section", NULL); 
	}

      file->secthash[ELFSH_SECTION_GNUVERDEF] = sect;
    }

  nbr = file->secthash[ELFSH_SECTION_GNUVERDEF]->shdr->sh_size;
  nbr /= sizeof(elfsh_Verdef);

  *num = nbr;

  ret = elfsh_get_raw(file->secthash[ELFSH_SECTION_GNUVERDEF]);

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (ret));
}

/**
 * Missing
 */
elfshsect_t	       	*elfsh_get_verdeftab_by_idx(elfshobj_t *file, elfsh_Addr range, int *num)
{
  elfsh_Verdef		*table;
  void			*ps;
  elfshsect_t		*sect;
  int			strindex, nbr;
  u_int			offset = 0;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);  

  version_parent = -1;

  sect = elfsh_get_section_by_type(file, SHT_GNU_verdef, 0, 
				   NULL, &strindex, &nbr);

  if (sect == NULL)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Unable to find Definition Version section", NULL);

  if (sect->data == NULL)
    {
      sect->data = elfsh_load_section(file, sect->shdr);
      if (sect->data == NULL)
	PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
			  "Unable to find data for Definition "
			  "Version section", NULL); 
    }

  ps = elfsh_get_raw(sect);

  do
    {
      table = ps + offset;

      if (table->vd_ndx == range)
	{
	  version_parent = range;
	  break;
	}
      
      if (table->vd_next == NULL)
	break;

      offset += table->vd_next;
    } while(table != NULL);

  nbr = sect->shdr->sh_size;
  nbr /= sizeof(elfsh_Verdef);
  *num = nbr;

  if (version_parent == -1)
    PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		      "Unable to find data for Definition "
		      "Version section with index", NULL);    

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, (sect));
}

