#include"trie.h"
 
#ifndef INPUT_PORTS_H
#define INPUT_PORTS_H

void attach_port_list(t_entity* entity_ptr,t_port* port_head,t_port* port_tail,char* port_type,signal_value port_value){
	if(port_type[0]=='i'){
		port_tail->next=entity_ptr->in_port_list_head;
		entity_ptr->in_port_list_head=port_head;
		while(port_head!=port_tail->next){
			if(trie_search(port_head->name,entity_ptr->in_port_trie_root)!=NULL || trie_search(port_head->name,entity_ptr->out_port_trie_root)!=NULL){
				printf("** ERROR: duplicate port name\n");
	                        exit(EXIT_FAILURE);
			}
			trie_insert((void*)port_head,port_head->name,entity_ptr->in_port_trie_root);
			port_head->port_value = port_value;
			port_head=port_head->next;
		}
	}
	else if(port_type[0]=='o'){
                port_tail->next=entity_ptr->out_port_list_head;
                entity_ptr->out_port_list_head=port_head;
                while(port_head!=port_tail->next){
			if(trie_search(port_head->name,entity_ptr->in_port_trie_root)!=NULL || trie_search(port_head->name,entity_ptr->out_port_trie_root)!=NULL){
                                printf("** ERROR: duplicate port name\n");
                                exit(EXIT_FAILURE);
                        }
                        trie_insert((void*)port_head,port_head->name,entity_ptr->out_port_trie_root);
			port_head->port_value = port_value;
                        port_head=port_head->next;
                }
        }
}
void insert_port_set(t_entity* entity_ptr){
	char port_name[50],temp_string_end_buffer[2];
	t_port* port_head=(t_port*)malloc(sizeof(t_port));
	t_port* port_tail=port_head;
	
	if(scanf(" %[a-zA-Z0-9_]",port_name)<1){
		printf("** ERROR:invalid port name\n");
	        exit(EXIT_FAILURE);
	}
	port_tail->name = NULL;
	set_port_name(port_tail,port_name);
	port_tail->next=NULL;
	while(scanf(" %1[,]",temp_string_end_buffer)==1){
		port_tail->next=(t_port*)malloc(sizeof(t_port));
		port_tail=port_tail->next;
		if(scanf(" %[a-zA-Z0-9_]",port_name)<1){
                	printf("** ERROR:invalid port name\n");
        	        exit(EXIT_FAILURE);
	        }
		port_tail->name = NULL;
        	set_port_name(port_tail,port_name);
	        port_tail->next=NULL;
	}
	if(scanf(" %1[:]",temp_string_end_buffer)<1){
		printf("** ERROR:invalid port name\n");
                exit(EXIT_FAILURE);
	}
	if(scanf(" %1[oO]",temp_string_end_buffer)==1){
		if(scanf("%*1[uU]%*1[tT]%1[ \n\t]",temp_string_end_buffer)<1){
			printf("** ERROR:invalid port type 1\n");
	                exit(EXIT_FAILURE);
		}
		if(scanf(" %1[sS]",temp_string_end_buffer)==1){
			if(scanf("%*1[tT]%*1[dD]%*1[_]%*1[lL]%*1[oO]%*1[gG]%*1[iI]%1[cC]",temp_string_end_buffer)<1){
				printf("** ERROR:invalid port datatype\n");
	                        exit(EXIT_FAILURE);
			}

			char char_signal_value;
			signal_value port_value;
			if(scanf(" %1[:]",temp_string_end_buffer)==1){
                                if(scanf(" %*1[=] %*1[']%c",&char_signal_value)<1){
                                        printf("** ERROR:invalid signals' value \n");
                                        exit(EXIT_FAILURE);
                                }
                                if(scanf("%1[']",temp_string_end_buffer)<1){
                                        printf("** ERROR:invalid signals' value \n");
                                        exit(EXIT_FAILURE);
                                }
                                if(char_signal_value=='0')
                                        port_value=STD_0;
                                else if(char_signal_value=='1')
                                        port_value=STD_1;
                                else if(char_signal_value=='U')
                                        port_value=STD_U;
                                else if(char_signal_value=='X')
                                        port_value=STD_X;
                                else if(char_signal_value=='W')
                                        port_value=STD_W;
                                else if(char_signal_value=='L')
                                        port_value=STD_L;
                                else if(char_signal_value=='H')
                                        port_value=STD_H;
                                else if(char_signal_value=='Z')
                                        port_value=STD_Z;
                                else if(char_signal_value=='-')
                                        port_value=STD_DCARE;
                                else{
                                        printf("** ERROR:invalid ports' value \n");
                                        exit(EXIT_FAILURE);
                                }
                        }
			else{
				port_value=STD_U;
                        }

			attach_port_list(entity_ptr,port_head,port_tail,"o",port_value);
		}
	}
	else if(scanf(" %1[iI]",temp_string_end_buffer)==1){
		if(scanf("%*1[nN]%1[ \n\t]",temp_string_end_buffer)<1){
                        printf("** ERROR:invalid port type 2\n");
                        exit(EXIT_FAILURE);
                }
                if(scanf(" %1[sS]",temp_string_end_buffer)==1){
                        if(scanf("%*1[tT]%*1[dD]%*1[_]%*1[lL]%*1[oO]%*1[gG]%*1[iI]%1[cC]",temp_string_end_buffer)<1){
                                printf("** ERROR:invalid port datatype\n");
                                exit(EXIT_FAILURE);
                        }
			
			char char_signal_value;
                        signal_value port_value;
                        if(scanf(" %1[:]",temp_string_end_buffer)==1){
                                if(scanf(" %*1[=] %*1[']%c",&char_signal_value)<1){
                                        printf("** ERROR:invalid signals' value \n");
                                        exit(EXIT_FAILURE);
                                }
                                if(scanf("%1[']",temp_string_end_buffer)<1){
                                        printf("** ERROR:invalid signals' value \n");
                                        exit(EXIT_FAILURE);
                                }
                                if(char_signal_value=='0')
                                        port_value=STD_0;
                                else if(char_signal_value=='1')
                                        port_value=STD_1;
                                else if(char_signal_value=='U')
                                        port_value=STD_U;
                                else if(char_signal_value=='X')
                                        port_value=STD_X;
                                else if(char_signal_value=='W')
                                        port_value=STD_W;
                                else if(char_signal_value=='L')
                                        port_value=STD_L;
                                else if(char_signal_value=='H')
                                        port_value=STD_H;
                                else if(char_signal_value=='Z')
                                        port_value=STD_Z;
                                else if(char_signal_value=='-')
                                        port_value=STD_DCARE;
                                else{
                                        printf("** ERROR:invalid ports' value \n");
                                        exit(EXIT_FAILURE);
                                }
                        }
                        else{
                                port_value=STD_U;
                        }
			
                        attach_port_list(entity_ptr,port_head,port_tail,"i",port_value);
                }
        }
	else{
		printf("** ERROR:invalid port type\n");
                exit(EXIT_FAILURE);
	}
}
void input_ports(t_entity* entity_ptr){
	char temp_string_end_buffer[2];
	insert_port_set(entity_ptr);
	while(scanf(" %1[;]",temp_string_end_buffer)==1)
		insert_port_set(entity_ptr);
}
#endif
