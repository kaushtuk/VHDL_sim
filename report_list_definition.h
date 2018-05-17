#include"typedef_structs.h"
#ifndef REPORT_LIST_DEFINITION_H
#define REPORT_LIST_DEFINITION_H

struct report{
	int type;		//0-string, 1-signal
	char* string_ptr;
	t_signal* signal_ptr;
	t_report* next;
};
void set_report_string(t_report* report_ptr, char* name){
        int string_length;
        if(report_ptr->string_ptr!=NULL)
                free(report_ptr->string_ptr);
        for(string_length=0;name[string_length]!='\0';string_length++);
        report_ptr->string_ptr=(char*)malloc(sizeof(char[string_length+1]));
        for(;string_length>=0;--string_length)
                (report_ptr->string_ptr)[string_length]=name[string_length];
}

#endif
