#include"typedef_structs.h"

#ifndef PROCESS_LIST_DEFINITION_H
#define PROCESS_LIST_DEFINITION_H
struct process{
	char* name;
	t_process* next;
	bool is_enqueued;		//current iteration event flag

	//evaluation event data members
	void* parse_tree_root_ptr;
	node_type root_type;			//signal,opearator
	t_signal* driven_signal_ptr;
	t_driver* driver_ptr;

	//after clause fields
	bool is_after;
	int after_time_stamp;
	/*
	   t_signal* report_signal_ptr;
	   */
	t_report* report_head_ptr;
};

void set_process_name(t_process* t_process_ptr, char* name){
	int string_length;
	if(t_process_ptr->name!=NULL)
		free(t_process_ptr->name);
	for(string_length=0;name[string_length]!='\0';string_length++);
	t_process_ptr->name=(char*)malloc(sizeof(char[string_length+1]));
	for(;string_length>=0;string_length--)
		(t_process_ptr->name)[string_length]=name[string_length];
}

void display_process_names(t_process* process_list_head){
	printf("# ** NOTE: \n");
	while(process_list_head!=NULL){
		printf("%s -> %dns \n ",process_list_head->name,process_list_head->after_time_stamp);
		process_list_head=process_list_head->next;
	}
	printf("\n");
}

#endif
