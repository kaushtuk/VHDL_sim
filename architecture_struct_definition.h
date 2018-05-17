#include"typedef_structs.h"
#ifndef ARCHITECTURE_STRUCT_DEFINITION_H
#define ARCHITECTURE_STRUCT_DEFINITION_H
struct architecture
{
  char *name;
  t_testcase_handle *architecture_testcase_handle;
  t_entity *parent_entity_ptr;
};
#endif
