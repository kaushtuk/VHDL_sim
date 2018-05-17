#include"typedef_structs.h"
#ifndef TESTCASE_HANDLE_DEFINITION_H
#define TESTCASE_HANDLE_DEFINITION_H
struct testcase_handle{
	t_signal* signal_list_head;
	t_process* process_list_head;
	trie* signal_trie_root;
};
#endif
