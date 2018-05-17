#include"typedef_structs.h"
#ifndef OPERATOR_NODE_DEFINITION_H
#define OPERATOR_NODE_DEFINITION_H
struct operator_node
{
  operator_type type;		//unary-0,bin-1
  gate e_operator;		//or,and,xor
  void *LHS;
  void *RHS;
  node_type LHS_type;		//signal,operator
  node_type RHS_type;
};
#endif
