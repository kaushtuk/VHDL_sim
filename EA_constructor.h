#include"utility_function_declaration.h"
#ifndef EA_CONSTRUCTOR_H
#define EA_CONSTRUCTOR_H
t_entity* entity_constructor(char* name){
	int i;
	t_entity* entity_ptr=(t_entity*)malloc(sizeof(t_entity));
	for(i=0;name[i]!='\0';i++);
	entity_ptr->name=(char*)malloc((i+1)*sizeof(char));
	for(;i>=0;i--)
		(entity_ptr->name)[i]=name[i];
	entity_ptr->in_port_trie_root=trie_constructor();
	entity_ptr->out_port_trie_root=trie_constructor();
	entity_ptr->in_port_list_head=NULL;
	entity_ptr->out_port_list_head=NULL;

	entity_ptr->default_architecture_ptr=NULL;
	return entity_ptr;
}
t_architecture* architecture_constructor(char* name,t_entity* entity_ptr){
	int i;
	t_architecture* architecture_ptr=(t_architecture*)malloc(sizeof(t_architecture));
	for(i=0;name[i]!='\0';i++);
	architecture_ptr->name=(char*)malloc((i+1)*sizeof(char));
	for(;i>=0;i--)
		(architecture_ptr->name)[i]=name[i];
	architecture_ptr->architecture_testcase_handle=(t_testcase_handle*)malloc(sizeof(t_testcase_handle));
	(architecture_ptr->architecture_testcase_handle)->signal_list_head=NULL;
	(architecture_ptr->architecture_testcase_handle)->process_list_head=NULL;
	architecture_ptr->parent_entity_ptr=entity_ptr;
	if(entity_ptr->default_architecture_ptr==NULL)
		entity_ptr->default_architecture_ptr=architecture_ptr;
	return architecture_ptr;
}
#endif
