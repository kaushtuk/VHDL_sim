#include"input_report_statement.h"
#include<string.h>
#ifndef ENTITY_INSTANTIATION_H
#define ENTITY_INSTANTIATION_H

void* copy_parse_tree(void* root_ptr,node_type root_type,trie* dup_signal_trie_root,trie* port_map_trie_root){
	if(root_type == SIGNAL_NODE_TYPE){
		void* uncast_signal_ptr;
		uncast_signal_ptr = trie_search(((t_signal*)(root_ptr))->name,port_map_trie_root);
		if(uncast_signal_ptr == NULL)
			uncast_signal_ptr = trie_search(((t_signal*)(root_ptr))->name,dup_signal_trie_root);
		else
			uncast_signal_ptr = (void*)(((t_port*)uncast_signal_ptr)->signal_ptr);
		return uncast_signal_ptr;
	}
	t_operator_node* temp_optr = (t_operator_node*)malloc(sizeof(t_operator_node));
	temp_optr->type = ((t_operator_node*)root_ptr)->type ;
	temp_optr->e_operator = ((t_operator_node*)root_ptr)->e_operator ;
	if(temp_optr->type == BINARY_OPERATOR){
		temp_optr->LHS_type = ((t_operator_node*)root_ptr)->LHS_type ;
		temp_optr->LHS = copy_parse_tree(((t_operator_node*)root_ptr)->LHS,((t_operator_node*)root_ptr)->LHS_type,dup_signal_trie_root,port_map_trie_root);
	}
	temp_optr->RHS_type = ((t_operator_node*)root_ptr)->RHS_type ;
	temp_optr->RHS = copy_parse_tree(((t_operator_node*)root_ptr)->RHS,((t_operator_node*)root_ptr)->RHS_type,dup_signal_trie_root,port_map_trie_root);
	return (void*)temp_optr;	
}

t_testcase_handle copy_testcase(t_testcase_handle* org_testcase_ptr,trie* port_map_trie_root,t_signal** parent_signal_tail_address,char* instance_name){
	t_testcase_handle process_copy_handle;
	t_signal* org_signal_head=org_testcase_ptr->signal_list_head;
	t_process* org_process_head=org_testcase_ptr->process_list_head;
	trie* dup_signal_trie_root=trie_constructor();
	trie* dup_process_trie_root=trie_constructor();
	t_signal* dup_signal_head=NULL;int i=0;t_signal* itr_signal_head;

	//signal copy
	while(org_signal_head!=NULL){

		//copy triggers
		t_trigger* org_trigger_head = org_signal_head->triggered_processes;
                t_trigger* dup_trigger_head = NULL;int j=0;t_trigger* itr_trigger_head = NULL;
                while(org_trigger_head != NULL){
                        if(j==0){
                                dup_trigger_head=(t_trigger*)malloc(sizeof(t_trigger));
                                itr_trigger_head = dup_trigger_head;j++;
                        }
                        else{
                                itr_trigger_head->next=(t_trigger*)malloc(sizeof(t_trigger));
                                itr_trigger_head = itr_trigger_head->next;
                        }
			//modify process name with instance name for dup_process_trie***
                        itr_trigger_head->scheduled_process = (t_process*)trie_search(org_trigger_head->scheduled_process->name,dup_process_trie_root);
                        if(itr_trigger_head->scheduled_process == NULL){
                                t_process* temp_process=(t_process*)malloc(sizeof(t_process));
                                itr_trigger_head->scheduled_process=temp_process;
                                trie_insert((void*)temp_process,org_trigger_head->scheduled_process->name,dup_process_trie_root);
                        }
                        itr_trigger_head->next=NULL;
                        org_trigger_head = org_trigger_head->next;
                }
		if(trie_search(org_signal_head->name,port_map_trie_root) == NULL){
			if(i==0){
                        	dup_signal_head=(t_signal*)malloc(sizeof(t_signal));
	                        itr_signal_head = dup_signal_head;i++;
        	        }
	                else{
        	                itr_signal_head->next=(t_signal*)malloc(sizeof(t_signal));
                	        itr_signal_head = itr_signal_head->next;
	                }

        	        //modify signal name with instance name***dd
        	        //copy name of signal
                	itr_signal_head->name = NULL;
	                set_signal_name(itr_signal_head,org_signal_head->name);
	                itr_signal_head->value = org_signal_head->value;
			//add copied trigger list
	                itr_signal_head->triggered_processes=dup_trigger_head;
	                itr_signal_head->driver_head_ptr = NULL;
	                itr_signal_head->next=NULL;
	                trie_insert((void*)itr_signal_head,itr_signal_head->name,dup_signal_trie_root);
		}

		//it also checks duplicate triggers
		if(trie_search(org_signal_head->name,port_map_trie_root) != NULL){
                	t_signal* port_mapped_signal = ((t_port*)trie_search(org_signal_head->name,port_map_trie_root))->signal_ptr;
			//resolving port values and parent signal value to parent signal value(only for out ports)***
	                while(dup_trigger_head != NULL){
        	                t_trigger* port_mapped_trigger_head = port_mapped_signal->triggered_processes;
                	        bool found_duplicate_trigger = 0;
                        	while(port_mapped_trigger_head != NULL){
                                	if(port_mapped_trigger_head->scheduled_process == dup_trigger_head->scheduled_process){
                                        	found_duplicate_trigger = 1;
	                                        break;
        	                        }
                	                port_mapped_trigger_head = port_mapped_trigger_head->next;
	                        }
        	                if(found_duplicate_trigger == 0){
                	                t_trigger* temp_trigg = (t_trigger*)malloc(sizeof(t_trigger));
                        	        temp_trigg->scheduled_process = dup_trigger_head->scheduled_process;
                                	temp_trigg->next = port_mapped_signal->triggered_processes;
	                                port_mapped_signal->triggered_processes = temp_trigg;
        	                }
                	        dup_trigger_head = dup_trigger_head->next;
	                }
                }
		org_signal_head = org_signal_head->next;
	}
	if((*parent_signal_tail_address) == NULL){
		printf("** ERROR:An entity instantiation exits in an empty entity\n");
                exit(EXIT_FAILURE);
	}
	else if(dup_signal_head != NULL){
		(*parent_signal_tail_address)->next = dup_signal_head;
		(*parent_signal_tail_address) = itr_signal_head;
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	//process copy
	t_process* dup_process_head=NULL;int k=0;t_process* itr_process_head=NULL;
	while(org_process_head != NULL){
		//modified process name trie_search***dd
		if(k==0){
			dup_process_head = (t_process*)trie_search(org_process_head->name,dup_process_trie_root);
			itr_process_head=dup_process_head;k++;
		}
		else{
                        itr_process_head->next = (t_process*)trie_search(org_process_head->name,dup_process_trie_root);
                        itr_process_head=itr_process_head->next;
                }
		if(itr_process_head == NULL){
			printf("** ERROR:A process without triggeres exits in the entity instantiation\n");
	                exit(EXIT_FAILURE);
		}
		//modified process name set_process_name***dd
		itr_process_head->name = NULL;
		set_process_name(itr_process_head,org_process_head->name);
		itr_process_head->is_enqueued = org_process_head->is_enqueued;
		itr_process_head->root_type = org_process_head->root_type;
		itr_process_head->is_after = org_process_head->is_after;
		itr_process_head->after_time_stamp = org_process_head->after_time_stamp;
		itr_process_head->next = NULL;

		//redirect driven signal
		itr_process_head->driven_signal_ptr = (t_signal*)trie_search(org_process_head->driven_signal_ptr->name,port_map_trie_root);
		if(itr_process_head->driven_signal_ptr == NULL){
			itr_process_head->driven_signal_ptr = (t_signal*)trie_search(org_process_head->driven_signal_ptr->name,dup_signal_trie_root);
		}
		else{
			itr_process_head->driven_signal_ptr = ((t_port*)((void*)(itr_process_head->driven_signal_ptr)))->signal_ptr;
		}
		//driver copy
		add_process_driver_to_signal(itr_process_head);
		//report copy
		itr_process_head->report_head_ptr = copy_report_statement(org_process_head->report_head_ptr,port_map_trie_root,dup_signal_trie_root);
		//parse tree copy
		itr_process_head->parse_tree_root_ptr = copy_parse_tree(org_process_head->parse_tree_root_ptr,org_process_head->root_type,dup_signal_trie_root,port_map_trie_root);
		itr_process_head->next = NULL;
		//printf("-- copying process %s\n",itr_process_head->name);
		org_process_head = org_process_head->next;
	}
	process_copy_handle.process_list_head = dup_process_head;
	process_copy_handle.signal_trie_root = (trie*)((void*)itr_process_head);
	process_copy_handle.signal_list_head = dup_signal_head;
	
	while(dup_signal_head != NULL){
		char* dup_signal_name;int name_length;
		name_length = strlen(instance_name);
		name_length = name_length + strlen(dup_signal_head->name);
		dup_signal_name = (char*)malloc((name_length+2)*sizeof(char));
		strcpy(dup_signal_name,instance_name);
		strcat(dup_signal_name,"\\");
		strcat(dup_signal_name,dup_signal_head->name);
		set_signal_name(dup_signal_head,dup_signal_name);
		free(dup_signal_name);
		dup_signal_head = dup_signal_head->next;
	}
	while(dup_process_head != NULL){
                char* dup_process_name;int name_length;
                name_length = strlen(instance_name);
                name_length = name_length + strlen(dup_process_head->name);
                dup_process_name = (char*)malloc((name_length+2)*sizeof(char));
                strcpy(dup_process_name,instance_name);
                strcat(dup_process_name,"\\");
                strcat(dup_process_name,dup_process_head->name);
                set_process_name(dup_process_head,dup_process_name);
		free(dup_process_name);
                dup_process_head = dup_process_head->next;
        }
	
	return process_copy_handle;
}

t_process* entity_instantiation(char* instance_name,t_process* previous_process_ptr,t_signal** parent_signal_tail_address,trie* signal_trie_root,t_entity* parent_entity_ptr,trie* entity_trie_root,trie* architecture_trie_root){
	char entity_name[50],architecture_name[50],temp_string_end_buffer[2];
	t_entity* entity_ptr;
	t_architecture* architecture_ptr;
	if(scanf(" %[a-zA-Z0-9_]",entity_name)<1){
		printf("** ERROR:invalid instance entity name\n");
                exit(EXIT_FAILURE);
	}
	entity_ptr=(t_entity*)trie_search(entity_name,entity_trie_root);
	if(entity_ptr==NULL){
                printf("** ERROR:invalid instance entity name\n");
                exit(EXIT_FAILURE);
        }
	if(scanf(" %1[(]",temp_string_end_buffer)==1){
		if(scanf(" %[a-zA-Z0-9_]",architecture_name)<1){
			printf("** ERROR:invalid instance architecture name\n");
        	        exit(EXIT_FAILURE);
		}
		architecture_ptr=(t_architecture*)trie_search(architecture_name,architecture_trie_root);
		if(architecture_ptr==NULL){
			printf("** ERROR:invalid instance architecture name\n");
                        exit(EXIT_FAILURE);
		}
		if(scanf(" %1[)]",temp_string_end_buffer)<1){
			printf("** ERROR:invalid instance architecture name\n");
                        exit(EXIT_FAILURE);
		}
	}
	else{
		architecture_ptr = entity_ptr->default_architecture_ptr;
		if(architecture_ptr==NULL){
                        printf("** ERROR:invalid instance architecture name\n");
                        exit(EXIT_FAILURE);
                }
	}
	if(architecture_ptr->parent_entity_ptr != entity_ptr){
		printf("** ERROR:invalid instantiation\n");
                exit(EXIT_FAILURE);
	}

	//-------------------------------------------------------------------------------------------------------
	trie* port_map_trie_root = trie_constructor();
	char port_name[50],signal_name[50];
	t_port* port_ptr;
	signal_value port_value;
	if(scanf(" %1[pP]",temp_string_end_buffer)==1){
		if(scanf("%*1[oO]%*1[rR]%*1[tT] %*1[mM]%*1[aA]%*1[pP] %1[(]",temp_string_end_buffer)<1){
			printf("** ERROR:invalid port map\n");
                        exit(EXIT_FAILURE);
		}
		if(scanf(" %[a-zA-Z0-9_]",port_name)<1){
			printf("** ERROR:invalid port name in port map\n");
                        exit(EXIT_FAILURE);
		}
		if(scanf(" %*1[=]%1[>]",temp_string_end_buffer)<1){
                        printf("** ERROR:invalid port name in port map\n");
                        exit(EXIT_FAILURE);
                }
		if(scanf(" %[a-zA-Z0-9_]",signal_name)<1){
                        printf("** ERROR:invalid signal name in port map\n");
                        exit(EXIT_FAILURE);
                }
		port_ptr=(t_port*)trie_search(port_name,entity_ptr->in_port_trie_root);
		if(port_ptr==NULL){
			port_ptr=(t_port*)trie_search(port_name,entity_ptr->out_port_trie_root);
			if(port_ptr==NULL){
				printf("** ERROR:invalid port name for the port map\n");
	                        exit(EXIT_FAILURE);
			}
		}
		port_value = port_ptr->port_value;
		port_ptr=(t_port*)malloc(sizeof(t_port));
		port_ptr->name = NULL;
		set_port_name(port_ptr,port_name);
		port_ptr->port_value = port_value;
		port_ptr->signal_ptr=(t_signal*)trie_search(signal_name,signal_trie_root);
		if(port_ptr->signal_ptr ==NULL){
			if(trie_search(port_name,entity_ptr->in_port_trie_root)!=NULL){
				port_ptr->next=(t_port*)trie_search(signal_name,parent_entity_ptr->in_port_trie_root);
				if(port_ptr->next ==NULL){
                                        printf("** ERROR:invalid port name for the port map\n");
                                        exit(EXIT_FAILURE);
                                }
				port_ptr->signal_ptr = port_ptr->next->signal_ptr;
			}
			else{				
				port_ptr->next=(t_port*)trie_search(signal_name,parent_entity_ptr->out_port_trie_root);
				if(port_ptr->next ==NULL){
					printf("** ERROR:invalid port name for the port map\n");
	                                exit(EXIT_FAILURE);
				}
				port_ptr->signal_ptr = port_ptr->next->signal_ptr;
				port_ptr->signal_ptr->value = lookup_RES[port_ptr->signal_ptr->value][port_ptr->port_value];
			}
		}
		port_ptr->next = NULL;
		trie_insert((void*)port_ptr,port_ptr->name,port_map_trie_root);
		while(scanf(" %1[,]",temp_string_end_buffer)==1){
			if(scanf(" %[a-zA-Z0-9_]",port_name)<1){
	                        printf("** ERROR:invalid port name in port map\n");
	                        exit(EXIT_FAILURE);
	                }
	                if(scanf(" %*1[=]%1[>]",temp_string_end_buffer)<1){
	                        printf("** ERROR:invalid port name in port map\n");
	                        exit(EXIT_FAILURE);
	                }
	                if(scanf(" %[a-zA-Z0-9_]",signal_name)<1){
	                        printf("** ERROR:invalid signal name in port map\n");
	                        exit(EXIT_FAILURE);
	                }
	                port_ptr=(t_port*)trie_search(port_name,entity_ptr->in_port_trie_root);
	                if(port_ptr==NULL){
	                        port_ptr=(t_port*)trie_search(port_name,entity_ptr->out_port_trie_root);
	                        if(port_ptr==NULL){
	                                printf("** ERROR:invalid port name for the port map\n");
	                                exit(EXIT_FAILURE);
	                        }
	                }
			port_value = port_ptr->port_value;
	                port_ptr=(t_port*)malloc(sizeof(t_port));
			port_ptr->name = NULL;
	                set_port_name(port_ptr,port_name);
			port_ptr->port_value = port_value;
	                port_ptr->signal_ptr=(t_signal*)trie_search(signal_name,signal_trie_root);
	                if(port_ptr->signal_ptr ==NULL){
				if(trie_search(port_name,entity_ptr->in_port_trie_root)!=NULL){
	                        	port_ptr->next=(t_port*)trie_search(signal_name,parent_entity_ptr->in_port_trie_root);
					if(port_ptr->next ==NULL){
                                                printf("** ERROR:invalid port name for the port map\n");
                                                exit(EXIT_FAILURE);
                                        }
				}
	                        else{
                        	        port_ptr->next=(t_port*)trie_search(signal_name,parent_entity_ptr->out_port_trie_root);
                	                if(port_ptr->next ==NULL){
        	                                printf("** ERROR:invalid port name for the port map\n");
	                                        exit(EXIT_FAILURE);
	                                }
	                        }
	                        port_ptr->signal_ptr = port_ptr->next->signal_ptr;
        	        }
	                port_ptr->next = NULL;
	                trie_insert((void*)port_ptr,port_ptr->name,port_map_trie_root);
		}
		if(scanf(" %1[)]",temp_string_end_buffer)<1){
			printf("** ERROR:invalid port map closing brace\n");
                        exit(EXIT_FAILURE);
		}
	}
	if(scanf(" %1[;]",temp_string_end_buffer)<1){
                printf("** ERROR:invalid instance\n");
                exit(EXIT_FAILURE);
        }
	//---------------------------------------------------------------------------------------------------------------------------
	t_testcase_handle process_copy_handle;
	process_copy_handle = copy_testcase(architecture_ptr->architecture_testcase_handle,port_map_trie_root,parent_signal_tail_address,instance_name);
	if(process_copy_handle.process_list_head != NULL){
		if(previous_process_ptr != NULL){
			((t_process*)((void*)(process_copy_handle.signal_trie_root)))->next = previous_process_ptr->next;
			previous_process_ptr->next = process_copy_handle.process_list_head;

		}
		else{
			printf("** ERROR: in process copying??\n");
                        exit(EXIT_FAILURE);
		}
		previous_process_ptr = (t_process*)((void*)(process_copy_handle.signal_trie_root));
	}
	printf("-- loading Entity %s\n",entity_ptr->name);
	return previous_process_ptr;
}
#endif
