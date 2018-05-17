#ifndef INPUT_REPORT_STATEMENT_H
#define INPUT_REPORT_STATEMENT_H

t_report *
input_report_statement (trie * signal_trie_root, trie * in_port_trie_root)
{
  t_report *report_head_ptr = NULL;
  int i = 0;
  t_report *itr_report_head_ptr = NULL;
  char report_string[50], signal_name[50], temp_buff[2];
  while (scanf (" %1[;]", temp_buff) < 1)
    {
      if (i == 0)
	{
	  report_head_ptr = (t_report *) malloc (sizeof (t_report));
	  report_head_ptr->next = NULL;
	  itr_report_head_ptr = report_head_ptr;
	  i++;
	}
      else
	{
	  itr_report_head_ptr->next = (t_report *) malloc (sizeof (t_report));
	  itr_report_head_ptr = itr_report_head_ptr->next;
	  itr_report_head_ptr->next = NULL;
	}
      if (scanf (" %1[\"]", temp_buff) == 1)
	{
	  if (scanf ("%[^\"]", report_string) < 1)
	    {
	      printf ("ERROR: invalid report statement\n");
	      exit (EXIT_FAILURE);
	    }
	  if (scanf ("%1[\"]", temp_buff) < 1)
	    {
	      printf ("ERROR: invalid report statement\n");
	      exit (EXIT_FAILURE);
	    }
	  itr_report_head_ptr->string_ptr = NULL;
	  set_report_string (itr_report_head_ptr, report_string);
	  itr_report_head_ptr->type = 0;
	}
      else if (scanf (" %1[sS]", temp_buff) == 1)
	{
	  if (scanf
	      ("%*1[tT]%*1[dD]%*1[_]%*1[lL]%*1[oO]%*1[gG]%*1[iI]%*1[cC]%*1[']%*1[iI]%*1[mM]%*1[aA]%*1[gG]%*1[eE]%1[(]",
	       temp_buff) < 1)
	    {
	      printf ("ERROR: invalid report statement\n");
	      exit (EXIT_FAILURE);
	    }
	  if (scanf (" %[a-zA-Z0-9_]", signal_name) < 1)
	    {
	      printf ("ERROR: invalid report statement\n");
	      exit (EXIT_FAILURE);
	    }
	  if (scanf (" %1[)]", temp_buff) < 1)
	    {
	      printf ("ERROR: invalid report statement\n");
	      exit (EXIT_FAILURE);
	    }
	  itr_report_head_ptr->type = 1;
	  itr_report_head_ptr->signal_ptr =
	    (t_signal *) trie_search (signal_name, signal_trie_root);
	  if (itr_report_head_ptr->signal_ptr == NULL)
	    {
	      itr_report_head_ptr->signal_ptr =
		(t_signal *) trie_search (signal_name, in_port_trie_root);
	      if (itr_report_head_ptr->signal_ptr == NULL)
		{
		  printf ("ERROR: invalid report statement\n");
		  exit (EXIT_FAILURE);
		}
	      itr_report_head_ptr->signal_ptr =
		((t_port *) ((void *) (itr_report_head_ptr->signal_ptr)))->
		signal_ptr;
	    }
	}
      else
	{
	  printf ("ERROR: invalid report statement\n");
	  exit (EXIT_FAILURE);
	}
      if (scanf (" %1[&]", temp_buff) < 1)
	{
	  if (scanf (" %1[;]", temp_buff) == 1)
	    break;
	  printf ("ERROR: invalid report statement\n");
	  exit (EXIT_FAILURE);
	}
    }
  return report_head_ptr;
}

t_report *
copy_report_statement (t_report * org_report_head_ptr,
		       trie * port_map_trie_root, trie * dup_signal_trie_root)
{
  t_report *dup_report_head_ptr = NULL;
  int i = 0;
  t_report *itr_report_head_ptr;
  while (org_report_head_ptr != NULL)
    {
      if (i == 0)
	{
	  dup_report_head_ptr = (t_report *) malloc (sizeof (t_report));
	  itr_report_head_ptr = dup_report_head_ptr;
	  i++;
	}
      else
	{
	  itr_report_head_ptr->next = (t_report *) malloc (sizeof (t_report));
	  itr_report_head_ptr = itr_report_head_ptr->next;
	}
      itr_report_head_ptr->next = NULL;
      itr_report_head_ptr->type = org_report_head_ptr->type;
      if (itr_report_head_ptr->type == 0)
	{
	  itr_report_head_ptr->string_ptr = NULL;
	  set_report_string (itr_report_head_ptr,
			     org_report_head_ptr->string_ptr);
	}
      else if (itr_report_head_ptr->type == 1)
	{
	  itr_report_head_ptr->signal_ptr =
	    (t_signal *) trie_search (org_report_head_ptr->signal_ptr->name,
				      port_map_trie_root);
	  if (itr_report_head_ptr->signal_ptr == NULL)
	    {
	      itr_report_head_ptr->signal_ptr =
		(t_signal *) trie_search (org_report_head_ptr->signal_ptr->
					  name, dup_signal_trie_root);
	    }
	  else
	    itr_report_head_ptr->signal_ptr =
	      ((t_port *) ((void *) itr_report_head_ptr->signal_ptr))->
	      signal_ptr;
	}
      else
	{
	  printf ("ERROR: invalid report statement\n");
	  exit (EXIT_FAILURE);
	}
      org_report_head_ptr = org_report_head_ptr->next;
    }
  return dup_report_head_ptr;
}

#endif
