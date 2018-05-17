#include"trie.h"
#include"validate_expression.h"
#include"entity_instantiation.h"
#ifndef INPUT_TESTCASES_H
#define INPUT_TESTCASES_H

trie *
input_signal_to_trie (t_signal * signal_list_head,
		      t_entity * parent_entity_ptr)
{
  bool isempty_signal_list = 1;
  trie *signal_trie_root = trie_constructor ();
  char char_signal_value;
  char signal_name[50], temp_string_end_buffer[2];
  while (scanf (" %1[sS]", temp_string_end_buffer) == 1)
    {
      isempty_signal_list = 0;
      t_signal *temp_signal_list_head = signal_list_head;
      if (scanf
	  ("%*1[iI]%*1[gG]%*1[nN]%*1[aA]%*1[lL]%1[ \n\t]",
	   temp_string_end_buffer) < 1)
	{
	  printf ("** ERROR: Undefined Declaration in Architecture\n");
	  exit (EXIT_FAILURE);
	}
      if (scanf (" %[a-zA-Z0-9_]", signal_name) < 1)
	{
	  printf ("** ERROR:invalid signal name\n");
	  exit (EXIT_FAILURE);
	}
      temp_signal_list_head->name = NULL;
      set_signal_name (temp_signal_list_head, signal_name);
      if (trie_search (signal_name, signal_trie_root) != NULL)
	{
	  printf ("** ERROR:duplicate signal name\n");
	  exit (EXIT_FAILURE);
	}
      if (trie_search (signal_name, parent_entity_ptr->in_port_trie_root) !=
	  NULL
	  || trie_search (signal_name,
			  parent_entity_ptr->out_port_trie_root) != NULL)
	{
	  printf ("** ERROR:port name and signal name are same\n");
	  exit (EXIT_FAILURE);
	}
      trie_insert ((void *) temp_signal_list_head, signal_name,
		   signal_trie_root);
      temp_signal_list_head->next = (t_signal *) malloc (sizeof (t_signal));
      temp_signal_list_head->triggered_processes = NULL;
      temp_signal_list_head->driver_head_ptr = NULL;
      temp_signal_list_head = temp_signal_list_head->next;
      while (scanf (" %1[,]", temp_string_end_buffer) == 1)
	{
	  if (scanf (" %[a-zA-Z0-9_]", signal_name) < 1)
	    {
	      printf ("** ERROR:invalid signal name\n");
	      exit (EXIT_FAILURE);
	    }
	  temp_signal_list_head->name = NULL;
	  set_signal_name (temp_signal_list_head, signal_name);
	  if (trie_search (signal_name, signal_trie_root) != NULL)
	    {
	      printf ("** ERROR:duplicate signal name\n");
	      exit (EXIT_FAILURE);
	    }
	  if (trie_search (signal_name, parent_entity_ptr->in_port_trie_root)
	      != NULL
	      || trie_search (signal_name,
			      parent_entity_ptr->out_port_trie_root) != NULL)
	    {
	      printf ("** ERROR:port name and signal name are same\n");
	      exit (EXIT_FAILURE);
	    }
	  trie_insert ((void *) temp_signal_list_head, signal_name,
		       signal_trie_root);
	  temp_signal_list_head->next =
	    (t_signal *) malloc (sizeof (t_signal));
	  temp_signal_list_head->triggered_processes = NULL;
	  temp_signal_list_head->driver_head_ptr = NULL;
	  temp_signal_list_head = temp_signal_list_head->next;
	}
      if (scanf (" %1[:]", temp_string_end_buffer) < 1)
	{
	  printf ("** ERROR:invalid signal name\n");
	  exit (EXIT_FAILURE);
	}
      if (scanf (" %1[sS]", temp_string_end_buffer) == 1)
	{
	  if (scanf
	      ("%*1[tT]%*1[dD]%*1[_]%*1[lL]%*1[oO]%*1[gG]%*1[iI]%1[cC]",
	       temp_string_end_buffer) < 1)
	    {
	      printf ("** ERROR: invalid signal type\n");
	      exit (EXIT_FAILURE);
	    }
	  if (scanf (" %1[:]", temp_string_end_buffer) == 1)
	    {
	      if (scanf (" %*1[=] %*1[']%c", &char_signal_value) < 1)
		{
		  printf ("** ERROR:invalid signals' value \n");
		  exit (EXIT_FAILURE);
		}
	      if (scanf ("%1[']", temp_string_end_buffer) < 1)
		{
		  printf ("** ERROR:invalid signals' value \n");
		  exit (EXIT_FAILURE);
		}
	      while (temp_signal_list_head != signal_list_head)
		{
		  if (char_signal_value == '0')
		    signal_list_head->value = STD_0;
		  else if (char_signal_value == '1')
		    signal_list_head->value = STD_1;
		  else if (char_signal_value == 'U')
		    signal_list_head->value = STD_U;
		  else if (char_signal_value == 'X')
		    signal_list_head->value = STD_X;
		  else if (char_signal_value == 'W')
		    signal_list_head->value = STD_W;
		  else if (char_signal_value == 'L')
		    signal_list_head->value = STD_L;
		  else if (char_signal_value == 'H')
		    signal_list_head->value = STD_H;
		  else if (char_signal_value == 'Z')
		    signal_list_head->value = STD_Z;
		  else if (char_signal_value == '-')
		    signal_list_head->value = STD_DCARE;
		  else
		    {
		      printf ("** ERROR:invalid signals' value \n");
		      exit (EXIT_FAILURE);
		    }
		  signal_list_head = signal_list_head->next;
		}
	    }
	  else
	    {
	      while (temp_signal_list_head != signal_list_head)
		{
		  signal_list_head->value = STD_U;
		  signal_list_head = signal_list_head->next;
		}
	    }
	}
      else
	{
	  printf ("** ERROR:invalid signal type\n");
	  exit (EXIT_FAILURE);
	}
      if (scanf (" %1[;]", temp_string_end_buffer) < 1)
	{
	  printf ("** ERROR:';' missing after signal type\n");
	  exit (EXIT_FAILURE);
	}
    }
  signal_list_head->name = NULL;
  set_signal_name (signal_list_head, "SIGNAL_SENTINAL");
  signal_list_head->next = NULL;
  return signal_trie_root;
}

void
add_trigger (t_signal * signal_ptr, t_process * process_ptr)
{
  t_trigger *trigger_list_head = signal_ptr->triggered_processes;
  if (trigger_list_head == NULL)
    {
      signal_ptr->triggered_processes =
	(t_trigger *) malloc (sizeof (t_trigger));
      signal_ptr->triggered_processes->next = NULL;
      signal_ptr->triggered_processes->scheduled_process = process_ptr;
      return;
    }
  while (trigger_list_head->next != NULL)
    trigger_list_head = trigger_list_head->next;
  trigger_list_head->next = (t_trigger *) malloc (sizeof (t_trigger));
  trigger_list_head->next->next = NULL;
  trigger_list_head->next->scheduled_process = process_ptr;
}

void *
create_parse_tree (trie * signal_trie_root, trie * in_port_trie_root,
		   t_process * parent_process, node_type * root_type)
{
  //new
  t_token *head = (t_token *) malloc (sizeof (t_token));
  t_token *tail =
    tokenizer (head, parent_process, signal_trie_root, in_port_trie_root);
  t_token_dll *token_dlist = (t_token_dll *) malloc (sizeof (t_token_dll));
  token_dlist->head = head;
  token_dlist->tail = tail;
  validate_expression (token_dlist);
  if (token_dlist->head->type == TOKEN_SENTINAL)
    {
      printf ("** ERROR:empty expression\n");
      exit (EXIT_FAILURE);
    }
  //printf("## expression for %s parsable\n",parent_process->name);
  (*root_type) = token_dlist->head->brace_root_type;
  return token_dlist->head->token_node_ptr;
}

int
input_processes (t_process * process_list_head,
		 t_signal ** signal_tail_address, trie * signal_trie_root,
		 t_entity * parent_entity_ptr, trie * entity_trie_root,
		 trie * architecture_trie_root)
{
  char process_name[50], triggering_signal_name[50], driven_signal_name[50],
    after_choice[5], temp_string_end_buffer[2];
  t_process *previous_process_ptr = NULL;
  int is_process_list_empty = 1, process_list_recreate = 0;
  //parsed
  if (scanf
      (" %*1[bB]%*1[eE]%*1[gG]%*1[iI]%*1[nN]%1[ \n\t]",
       temp_string_end_buffer) < 1)
    {
      printf ("** ERROR:invalid begin statement\n");
      exit (EXIT_FAILURE);
    }
  if (scanf (" %[a-zA-Z0-9_]", process_name) < 1)
    {
      printf ("** ERROR: invalid process name\n");
      exit (EXIT_FAILURE);
    }
  while (strcmpci (process_name, "end") != 0)
    {
      if (scanf (" %1[:]", temp_string_end_buffer) < 1)
	{
	  printf ("** ERROR: invalid concurrent statement\n");
	  exit (EXIT_FAILURE);
	}
      if (scanf (" %1[pP]", temp_string_end_buffer) == 1)
	{
	  //process
	  if (scanf
	      ("%*1[rR]%*1[oO]%*1[cC]%*1[eE]%*1[sS]%1[sS]",
	       temp_string_end_buffer) < 1)
	    {
	      printf ("** ERROR: invalid concurrent statement\n");
	      exit (EXIT_FAILURE);
	    }
	  process_list_head->name = NULL;
	  set_process_name (process_list_head, process_name);

	  //triggers
	  if (scanf (" %1[(]", temp_string_end_buffer) < 1)
	    {
	      printf ("** ERROR: invalid concurrent statement\n");
	      exit (EXIT_FAILURE);
	    }
	  if (scanf (" %[a-zA-Z0-9_]", triggering_signal_name) < 1)
	    {
	      printf ("** ERROR: invalid triggering signal name\n");
	      exit (EXIT_FAILURE);
	    }
	  t_signal *temp_triggering_signal =
	    (t_signal
	     *) (trie_search (triggering_signal_name, signal_trie_root));
	  if (temp_triggering_signal == NULL)
	    {
	      t_port *temp_triggering_port =
		(t_port
		 *) (trie_search (triggering_signal_name,
				  parent_entity_ptr->in_port_trie_root));
	      if (temp_triggering_port == NULL
		  || temp_triggering_port->signal_ptr == NULL)
		{
		  printf ("** ERROR: invalid triggering signal name\n");
		  exit (EXIT_FAILURE);
		}
	      temp_triggering_signal = temp_triggering_port->signal_ptr;
	    }
	  add_trigger (temp_triggering_signal, process_list_head);
	  while (scanf (" %1[,]", temp_string_end_buffer) == 1)
	    {
	      if (scanf (" %[a-zA-Z0-9_]", triggering_signal_name) < 1)
		{
		  printf ("** ERROR:invalid triggering signal name\n");
		  exit (EXIT_FAILURE);
		}
	      t_signal *temp_triggering_signal =
		(t_signal
		 *) (trie_search (triggering_signal_name, signal_trie_root));
	      if (temp_triggering_signal == NULL)
		{
		  t_port *temp_triggering_port =
		    (t_port
		     *) (trie_search (triggering_signal_name,
				      parent_entity_ptr->in_port_trie_root));
		  if (temp_triggering_port == NULL
		      || temp_triggering_port->signal_ptr == NULL)
		    {
		      printf ("** ERROR: invalid triggering signal name\n");
		      exit (EXIT_FAILURE);
		    }
		  temp_triggering_signal = temp_triggering_port->signal_ptr;
		}
	      add_trigger (temp_triggering_signal, process_list_head);
	    }
	  if (scanf
	      (" %*1[)] %*1[bB]%*1[eE]%*1[gG]%*1[iI]%*1[nN]%1[ \n\t]",
	       temp_string_end_buffer) < 1)
	    {
	      printf ("** ERROR: invalid triggering signal name\n");
	      exit (EXIT_FAILURE);
	    }
	  //signal assignment
	  if (scanf (" %[a-zA-Z0-9_]", driven_signal_name) < 1)
	    {
	      printf ("** ERROR: invalid process driven signal name\n");
	      exit (EXIT_FAILURE);
	    }
	  process_list_head->driven_signal_ptr =
	    (t_signal *) (trie_search (driven_signal_name, signal_trie_root));
	  if (process_list_head->driven_signal_ptr == NULL)
	    {
	      t_port *temp_driven_port =
		(t_port
		 *) (trie_search (driven_signal_name,
				  parent_entity_ptr->out_port_trie_root));
	      if (temp_driven_port == NULL
		  || temp_driven_port->signal_ptr == NULL)
		{
		  printf ("** ERROR: invalid process driven signal name\n");
		  exit (EXIT_FAILURE);
		}
	      process_list_head->driven_signal_ptr =
		temp_driven_port->signal_ptr;
	    }
	  add_process_driver_to_signal (process_list_head);
	  process_list_head->is_enqueued = 1;
	  if (scanf (" %*1[<]%1[=]", temp_string_end_buffer) < 1)
	    {
	      printf ("** ERROR: invalid  driven signal assignment\n");
	      exit (EXIT_FAILURE);
	    }
	  //expression parser function
	  process_list_head->parse_tree_root_ptr =
	    create_parse_tree (signal_trie_root,
			       parent_entity_ptr->in_port_trie_root,
			       process_list_head,
			       &(process_list_head->root_type));

	  //report statement
	  if (scanf (" %1[rR]", temp_string_end_buffer) == 1)
	    {
	      if (scanf
		  ("%*1[eE]%*1[pP]%*1[oO]%*1[rR]%*1[tT]%1[ \n\t]",
		   temp_string_end_buffer) < 1)
		{
		  printf ("** ERROR:invalid report statement\n");
		  exit (EXIT_FAILURE);
		}
	      process_list_head->report_head_ptr =
		input_report_statement (signal_trie_root,
					parent_entity_ptr->in_port_trie_root);
	    }
	  else
	    {
	      process_list_head->report_head_ptr = NULL;
	    }
	  //end process statement
	  if (scanf
	      (" %*1[eE]%*1[nN]%*1[dD] %*1[pP]%*1[rR]%*1[oO]%*1[cC]%*1[eE]%*1[sS]%*1[sS] %1[;]",
	       temp_string_end_buffer) < 1)
	    {
	      printf ("** ERROR:invalid expression ending\n");
	      exit (EXIT_FAILURE);
	    }
	  process_list_head->next = (t_process *) malloc (sizeof (t_process));
	  previous_process_ptr = process_list_head;
	  process_list_head = process_list_head->next;
	  is_process_list_empty = 0;
	}
      else if (scanf (" %1[eE]", temp_string_end_buffer) == 1)
	{
	  //entity instantiation
	  if (scanf
	      ("%*1[nN]%*1[tT]%*1[iI]%*1[tT]%*1[yY]%*1[ \n\t] %*1[wW]%*1[oO]%*1[rR]%*1[kK] %1[.]",
	       temp_string_end_buffer) < 1)
	    {
	      printf ("** ERROR: invalid concurrent statement\n");
	      exit (EXIT_FAILURE);
	    }
	  //limitation
	  if (is_process_list_empty == 1)
	    {
	      process_list_recreate = 1;
	      t_process *temp_recreate_process =
		(t_process *) malloc (sizeof (t_process));
	      process_list_head->next = temp_recreate_process;
	      previous_process_ptr =
		entity_instantiation (process_name, process_list_head,
				      signal_tail_address, signal_trie_root,
				      parent_entity_ptr, entity_trie_root,
				      architecture_trie_root);
	      process_list_head = temp_recreate_process;
	      //printf("** ERROR: There should be a process before an entity instantiation\n");
	      //exit(EXIT_FAILURE);
	    }
	  else
	    {
	      previous_process_ptr =
		entity_instantiation (process_name, previous_process_ptr,
				      signal_tail_address, signal_trie_root,
				      parent_entity_ptr, entity_trie_root,
				      architecture_trie_root);
	    }
	  is_process_list_empty = 0;
	}
      else
	{
	  printf ("** ERROR: invalid concurrent statement\n");
	  exit (EXIT_FAILURE);
	}
      if (scanf (" %[a-zA-Z0-9_]", process_name) < 1)
	{
	  printf ("** ERROR: invalid process name\n");
	  exit (EXIT_FAILURE);
	}
    }
  process_list_head->name = NULL;
  set_process_name (process_list_head, "PROCESS_SENTINAL");
  process_list_head->next = NULL;
  if (scanf
      ("%*1[ \n\t] %*1[aA]%*1[rR]%*1[cC]%*1[hH]%*1[iI]%*1[tT]%*1[eE]%*1[cC]%*1[tT]%*1[uU]%*1[rR]%*1[eE] %1[;]",
       temp_string_end_buffer) < 1)
    {
      printf ("** ERROR: invalid end architecture statement\n");
      exit (EXIT_FAILURE);
    }
  return process_list_recreate;
}

t_signal *
port_signal_instantiate (t_signal * signal_list_head,
			 t_entity * parent_entity_ptr)
{
  //out port
  t_port *port_head = parent_entity_ptr->out_port_list_head;
  while (port_head != NULL)
    {
      signal_list_head->name = NULL;
      set_signal_name (signal_list_head, port_head->name);
      port_head->signal_ptr = signal_list_head;
      signal_list_head->value = port_head->port_value;
      signal_list_head->triggered_processes = NULL;
      signal_list_head->driver_head_ptr = NULL;
      signal_list_head->next = (t_signal *) malloc (sizeof (t_signal));
      signal_list_head = signal_list_head->next;
      port_head = port_head->next;
    }
  //in port
  port_head = parent_entity_ptr->in_port_list_head;
  while (port_head != NULL)
    {
      signal_list_head->name = NULL;
      set_signal_name (signal_list_head, port_head->name);
      port_head->signal_ptr = signal_list_head;
      signal_list_head->value = port_head->port_value;
      signal_list_head->triggered_processes = NULL;
      signal_list_head->driver_head_ptr = NULL;
      signal_list_head->next = (t_signal *) malloc (sizeof (t_signal));
      signal_list_head = signal_list_head->next;
      port_head = port_head->next;
    }
  return signal_list_head;
}

t_testcase_handle
input_testcase (t_entity * parent_entity_ptr, trie * entity_trie_root,
		trie * architecture_trie_root)
{
  //entity and architecture trie root are for entity instantiation
  //parent entity ptr is for port to signal instantiation
  t_testcase_handle testcase_handle_1;
  t_signal *signal_list_head = (t_signal *) malloc (sizeof (t_signal));
  t_process *process_list_head = (t_process *) malloc (sizeof (t_process));
  testcase_handle_1.signal_list_head = signal_list_head;
  testcase_handle_1.process_list_head = process_list_head;

  //port to signal list head addition
  signal_list_head =
    port_signal_instantiate (signal_list_head, parent_entity_ptr);

  //main data structure---------------------------------------------------------------------------------------------
  //signal input
  trie *signal_trie_root =
    input_signal_to_trie (signal_list_head, parent_entity_ptr);

  //signal sentinal removal
  if (strcmp (testcase_handle_1.signal_list_head->name, "SIGNAL_SENTINAL") ==
      0)
    {
      free (testcase_handle_1.signal_list_head);
      signal_list_head = NULL;
      testcase_handle_1.signal_list_head = NULL;
    }
  else
    {
      signal_list_head = testcase_handle_1.signal_list_head;
      while (strcmp (signal_list_head->next->name, "SIGNAL_SENTINAL") != 0)
	signal_list_head = signal_list_head->next;
      free (signal_list_head->next);
      signal_list_head->next = NULL;
    }

  //process input
  int process_list_recreate = 0;
  if (signal_list_head == NULL)
    {
      process_list_recreate =
	input_processes (process_list_head, &signal_list_head,
			 signal_trie_root, parent_entity_ptr,
			 entity_trie_root, architecture_trie_root);
      testcase_handle_1.signal_list_head = signal_list_head;
    }
  else
    process_list_recreate =
      input_processes (process_list_head, &signal_list_head, signal_trie_root,
		       parent_entity_ptr, entity_trie_root,
		       architecture_trie_root);

  if (process_list_recreate == 1)
    {
      // to be done
      t_process *temp_pro = testcase_handle_1.process_list_head;
      testcase_handle_1.process_list_head =
	(testcase_handle_1.process_list_head)->next;
      free (temp_pro);
    }

  //process sentinal removal
  if (strcmp (testcase_handle_1.process_list_head->name, "PROCESS_SENTINAL")
      == 0)
    {
      free (testcase_handle_1.process_list_head);
      testcase_handle_1.process_list_head = NULL;
    }
  else
    {
      process_list_head = testcase_handle_1.process_list_head;
      while (strcmp (process_list_head->next->name, "PROCESS_SENTINAL") != 0)
	process_list_head = process_list_head->next;
      free (process_list_head->next);
      process_list_head->next = NULL;
    }
  testcase_handle_1.signal_trie_root = signal_trie_root;
  //------------------------------------------------------------------------------------------------------------------
  return testcase_handle_1;
}

#endif
