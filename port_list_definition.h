#include"typedef_structs.h"
#ifndef PORT_LIST_DEFINITION_H
#define PORT_LIST_DEFINITION_H
struct port{
	char* name;
	t_port* next;
	t_signal* signal_ptr;
	signal_value port_value;
};
void set_port_name(t_port* t_port_ptr, char* name){
        int string_length;
        if(t_port_ptr->name!=NULL)
                free(t_port_ptr->name);
        for(string_length=0;name[string_length]!='\0';string_length++);
        t_port_ptr->name=(char*)malloc(sizeof(char[string_length+1]));
        for(;string_length>=0;string_length--)
                (t_port_ptr->name)[string_length]=name[string_length];
}
#endif
