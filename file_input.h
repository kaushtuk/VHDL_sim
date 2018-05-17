#include"input_ports.h"
#include"input_testcases.h"

#ifndef FILE_INPUT_H
#define FILE_INPUT_H
void file_input(trie* entity_trie_root,trie* architecture_trie_root){
        char temp_string_end_buffer[2],entity_name[50],port_name[50],architecture_name[50];
        t_entity* entity_ptr;
        t_architecture* architecture_ptr;
        do{
        if(scanf(" %1[eE]",temp_string_end_buffer)==1){
                if(scanf("%*1[nN]%*1[tT]%*1[iI]%*1[tT]%1[yY]",temp_string_end_buffer)<1){
                        printf("** ERROR:invalid unit\n");
                        exit(EXIT_FAILURE);
                }
                if(scanf("%*1[ \n\t] %[a-zA-Z0-9_]",entity_name)<1){
                        printf("** ERROR:invalid unit name\n");
                        exit(EXIT_FAILURE);
                }
                entity_ptr=entity_constructor(entity_name);
                if(scanf("%*1[ \n\t] %*1[iI]%*1[sS]%1[ \n\t]",temp_string_end_buffer)<1){
                        printf("** ERROR:invalid entity name\n");
                        exit(EXIT_FAILURE);
                }
                if(scanf(" %1[pP]",temp_string_end_buffer)==1){
                        if(scanf("%*1[oO]%*1[rR]%*1[tT] %1[(]",temp_string_end_buffer)<1){
                                printf("** ERROR:invalid port list\n");
                                exit(EXIT_FAILURE);
                        }
                        //port list code
                        input_ports(entity_ptr);
                        if(scanf(" %*1[)] %1[;]",temp_string_end_buffer)<1){
                                printf("** ERROR:invalid port list\n");
                                exit(EXIT_FAILURE);
                        }
                }
                if(scanf(" %*1[eE]%*1[nN]%*1[dD]%*1[ \n\t] %*1[eE]%*1[nN]%*1[tT]%*1[iI]%*1[tT]%*1[yY] %1[;]",temp_string_end_buffer)<1){
                        printf("** ERROR:invalid end statement\n");
                        exit(EXIT_FAILURE);
                }
                trie_insert((void*)entity_ptr,entity_name,entity_trie_root);
                printf("-- compiling Entity %s\n",entity_ptr->name);
        }
        else if(scanf(" %1[aA]",temp_string_end_buffer)==1){
                if(scanf("%*1[rR]%*1[cC]%*1[hH]%*1[iI]%*1[tT]%*1[eE]%*1[cC]%*1[tT]%*1[uU]%*1[rR]%1[eE]",temp_string_end_buffer)<1){
                        printf("** ERROR:invalid unit\n");
                        exit(EXIT_FAILURE);
                }
                if(scanf("%*1[ \n\t] %[a-zA-Z0-9_]",architecture_name)<1){
                        printf("** ERROR:invalid unit name\n");
                        exit(EXIT_FAILURE);
                }
                if(scanf("%*1[ \n\t] %*1[oO]%*1[fF]%*1[ \n\t] %[a-zA-Z0-9_]",entity_name)<1){
                        printf("** ERROR:invalid architecture's entity name\n");
                        exit(EXIT_FAILURE);
                }
                entity_ptr=(t_entity*)trie_search(entity_name,entity_trie_root);
                if(entity_ptr==NULL){
                        printf("** ERROR:invalid architecture's entity name\n");
                        exit(EXIT_FAILURE);
                }
                architecture_ptr=architecture_constructor(architecture_name,entity_ptr);
                if(scanf("%*1[ \n\t] %*1[iI]%*1[sS]%1[ \n\t]",temp_string_end_buffer)<1){
                        printf("** ERROR:invalid architecture name\n");
                        exit(EXIT_FAILURE);
                }
                //Signal Declarative part **
                        *(architecture_ptr->architecture_testcase_handle)=input_testcase(entity_ptr,entity_trie_root,architecture_trie_root);
                //Process Definition part **

                trie_insert((void*)architecture_ptr,architecture_name,architecture_trie_root);
                printf("-- compiling Architecture %s of %s\n",architecture_ptr->name,entity_ptr->name);
        }
        else if(scanf(" %1[lL]",temp_string_end_buffer)==1){
                if(scanf("%*1[iI]%*1[bB]%*1[rR]%*1[aA]%*1[rR]%*1[yY] %*1[iI]%*1[eE]%*1[eE]%*1[eE] %*1[;] %*1[uU]%*1[sS]%*1[Ee] %*1[iI]%*1[eE]%*1[eE]%*1[eE]%*1[.]%*1[sS]%*1[tT]%*1[dD]%*1[_]%*1[lL]%*1[oO]%*1[gG]%*1[iI]%*1[cC]%*1[_]1164.%*1[aA]%*1[lL]%*1[lL]%1[;]",temp_string_end_buffer)<1){
                        printf("** ERROR:invalid unit\n");
                        exit(EXIT_FAILURE);
                }
        }
        else{
                printf("** ERROR:invalid unit\n");
                exit(EXIT_FAILURE);
        }
        scanf(" ");
        }while(!feof(stdin));
        printf("Errors: 0,Warnings: 0\n\n");
}

#endif
