#include"utility_function_definition.h"
#include"file_input.h"
#include <string.h>

int
main ()
{
  trie *entity_trie_root = trie_constructor ();
  trie *architecture_trie_root = trie_constructor ();
  t_entity *entity_ptr;
  t_architecture *architecture_ptr;

  //taking input from file
  file_input (entity_trie_root, architecture_trie_root);

  //initialize lookup table
  initialize_lookup_table ();

  clearerr (stdin);
  if (!freopen ("/dev/tty", "r", stdin))
    {
      perror ("/dev/tty");
      exit (1);
    }
  char simulator_command[50], unit_name[50];
  while (1)
    {
      printf ("(mySimulator)");
      if (scanf ("%s", simulator_command) < 1)
	{
	  printf ("** ERROR: STDIN UNSET\n");
	  exit (EXIT_FAILURE);
	}
      if (strcmpci (simulator_command, "vsim") == 0)
	{
	  if (scanf ("%*1[ \n\t] %[a-zA-Z0-9_]", unit_name) < 1)
	    printf ("** ERROR: incorrect unit name for simulation\n");
	  else
	    {
	      entity_ptr =
		(t_entity *) trie_search (unit_name, entity_trie_root);
	      if (entity_ptr == NULL)
		printf ("** ERROR: unit '%s' not found\n", unit_name);
	      else
		{
		  if (entity_ptr->default_architecture_ptr == NULL)
		    printf ("** NOTE: Architecture unit for '%s' not found\n",
			    unit_name);
		  else
		    {
		      printf
			("** NOTE: Simulating architecture %s of entity %s\n",
			 entity_ptr->default_architecture_ptr->name,
			 entity_ptr->name);
		      t_testcase_handle testcase_handle_1;
		      testcase_handle_1 =
			*(entity_ptr->default_architecture_ptr->
			  architecture_testcase_handle);
		      t_signal *signal_list_head =
			testcase_handle_1.signal_list_head;
		      t_process *process_list_head =
			testcase_handle_1.process_list_head;
		      t_time_line *time_line_head =
			create_init_event_queue (signal_list_head,
						 process_list_head);
		      run_simulation (time_line_head, signal_list_head);
		    }
		}
	    }
	}
      else if (strcmpci (simulator_command, "quit") == 0)
	{
	  printf ("** NOTE: Simulator closing\n");
	  exit (EXIT_SUCCESS);
	}
      else if (strcmpci (simulator_command, "display") == 0)
	{
	  if (scanf ("%*1[ \n\t] %[a-zA-Z0-9_]", unit_name) < 1)
	    printf ("** ERROR: incorrect unit name for simulation\n");
	  else
	    {
	      entity_ptr =
		(t_entity *) trie_search (unit_name, entity_trie_root);
	      if (entity_ptr == NULL)
		printf ("** ERROR: unit '%s' not found\n", unit_name);
	      else
		{
		  if (entity_ptr->default_architecture_ptr == NULL)
		    printf ("** NOTE: Architecture unit for '%s' not found\n",
			    unit_name);
		  else
		    {
		      printf
			("** NOTE: Displaying architecture %s of entity %s\n",
			 entity_ptr->default_architecture_ptr->name,
			 entity_ptr->name);
		      t_testcase_handle testcase_handle_1;
		      testcase_handle_1 =
			*(entity_ptr->default_architecture_ptr->
			  architecture_testcase_handle);
		      if (scanf
			  (" %*1[pP]%*1[rR]%*1[oO]%*1[cC]%*1[eE]%*1[sS]%1[sS]",
			   unit_name) == 1)
			{
			  printf ("** NOTE: Displaying processes\n");
			  t_process *process_list_head =
			    testcase_handle_1.process_list_head;
			  display_process_names (process_list_head);
			}
		      else
			if (scanf
			    (" %*1[sS]%*1[iI]%*1[gG]%*1[nN]%*1[aA]%1[lL]",
			     unit_name) == 1)
			{
			  printf ("** NOTE: Displaying signals\n");
			  t_signal *signal_list_head =
			    testcase_handle_1.signal_list_head;
			  display_signal_values (signal_list_head);
			}
		    }
		}
	    }
	}
    }

  return 0;
}
