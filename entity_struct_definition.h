#include"typedef_structs.h"
#ifndef ENTITY_STRUCT_DEFINITION_H
#define ENTITY_STRUCT_DEFINITION_H
struct entity{
        char* name;
        trie* in_port_trie_root;
        trie* out_port_trie_root;
	t_port* in_port_list_head;
	t_port* out_port_list_head;
        t_architecture* default_architecture_ptr;
};
#endif
