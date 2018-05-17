#include"utility_function_declaration.h"

#ifndef EVALUATE_EXPRESSION_H
#define EVALUATE_EXPRESSION_H

signal_value lookup_NEG[9];
signal_value lookup_OR[9][9] = {
  {STD_U, STD_U, STD_U, STD_1, STD_U, STD_U, STD_U, STD_1, STD_U},
  {STD_U, STD_X, STD_X, STD_1, STD_X, STD_X, STD_X, STD_1, STD_X},
  {STD_U, STD_X, STD_0, STD_1, STD_X, STD_X, STD_0, STD_1, STD_X},
  {STD_1, STD_1, STD_1, STD_1, STD_1, STD_1, STD_1, STD_1, STD_1},
  {STD_U, STD_X, STD_X, STD_1, STD_X, STD_X, STD_X, STD_1, STD_X},
  {STD_U, STD_X, STD_X, STD_1, STD_X, STD_X, STD_X, STD_1, STD_X},
  {STD_U, STD_X, STD_0, STD_1, STD_X, STD_X, STD_0, STD_1, STD_X},
  {STD_1, STD_1, STD_1, STD_1, STD_1, STD_1, STD_1, STD_1, STD_1},
  {STD_U, STD_X, STD_X, STD_1, STD_X, STD_X, STD_X, STD_1, STD_X}
};

signal_value lookup_AND[9][9] = {
  {STD_U, STD_U, STD_0, STD_U, STD_U, STD_U, STD_0, STD_U, STD_U},
  {STD_U, STD_X, STD_0, STD_X, STD_X, STD_X, STD_0, STD_X, STD_X},
  {STD_0, STD_0, STD_0, STD_0, STD_0, STD_0, STD_0, STD_0, STD_0},
  {STD_U, STD_X, STD_0, STD_1, STD_X, STD_X, STD_0, STD_1, STD_X},
  {STD_U, STD_X, STD_0, STD_X, STD_X, STD_X, STD_0, STD_X, STD_X},
  {STD_U, STD_X, STD_0, STD_X, STD_X, STD_X, STD_0, STD_X, STD_X},
  {STD_0, STD_0, STD_0, STD_0, STD_0, STD_0, STD_0, STD_0, STD_0},
  {STD_U, STD_X, STD_0, STD_1, STD_X, STD_X, STD_0, STD_1, STD_X},
  {STD_U, STD_X, STD_0, STD_X, STD_X, STD_X, STD_0, STD_X, STD_X}
};

signal_value lookup_XOR[9][9] = {
  {STD_U, STD_U, STD_U, STD_U, STD_U, STD_U, STD_U, STD_U, STD_U},
  {STD_U, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X},
  {STD_U, STD_X, STD_0, STD_1, STD_X, STD_X, STD_0, STD_1, STD_X},
  {STD_U, STD_X, STD_1, STD_0, STD_X, STD_X, STD_1, STD_1, STD_X},
  {STD_U, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X},
  {STD_U, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X},
  {STD_U, STD_X, STD_0, STD_1, STD_X, STD_X, STD_0, STD_1, STD_X},
  {STD_U, STD_X, STD_1, STD_0, STD_X, STD_X, STD_1, STD_0, STD_X},
  {STD_U, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X}
};

signal_value lookup_RES[9][9] = {
  {STD_U, STD_U, STD_U, STD_U, STD_U, STD_U, STD_U, STD_U, STD_U},
  {STD_U, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X},
  {STD_U, STD_X, STD_0, STD_X, STD_0, STD_0, STD_0, STD_0, STD_X},
  {STD_U, STD_X, STD_X, STD_1, STD_1, STD_1, STD_1, STD_1, STD_X},
  {STD_U, STD_X, STD_0, STD_1, STD_Z, STD_W, STD_L, STD_H, STD_X},
  {STD_U, STD_X, STD_0, STD_1, STD_W, STD_W, STD_W, STD_W, STD_X},
  {STD_U, STD_X, STD_0, STD_1, STD_L, STD_W, STD_L, STD_W, STD_X},
  {STD_U, STD_X, STD_0, STD_1, STD_H, STD_W, STD_W, STD_H, STD_X},
  {STD_U, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X, STD_X}
};

void
initialize_lookup_table ()
{
  lookup_NEG[STD_U] = STD_U;
  lookup_NEG[STD_0] = STD_1;
  lookup_NEG[STD_1] = STD_0;
  lookup_NEG[STD_X] = STD_X;
  lookup_NEG[STD_Z] = STD_X;
  lookup_NEG[STD_W] = STD_X;
  lookup_NEG[STD_H] = STD_0;
  lookup_NEG[STD_L] = STD_1;
  lookup_NEG[STD_DCARE] = STD_X;

  lookup_OR[STD_0][STD_0] = STD_0;
  lookup_OR[STD_0][STD_1] = STD_1;
  lookup_OR[STD_1][STD_0] = STD_1;
  lookup_OR[STD_1][STD_1] = STD_1;

  lookup_XOR[STD_0][STD_0] = STD_0;
  lookup_XOR[STD_0][STD_1] = STD_1;
  lookup_XOR[STD_1][STD_0] = STD_1;
  lookup_XOR[STD_1][STD_1] = STD_0;

}

signal_value
evaluate_expression (void *root_ptr, node_type root_type)
{
  //Base case
  if (root_type == SIGNAL_NODE_TYPE)
    return ((t_signal *) (root_ptr))->value;
  //Recursive case
  t_operator_node *temp_optr = (t_operator_node *) root_ptr;
  if (temp_optr->type == UNARY_OPERATOR)
    {
      signal_value RHS_value;
      RHS_value = evaluate_expression (temp_optr->RHS, temp_optr->RHS_type);
      if (temp_optr->e_operator == STD_NEG)
	return lookup_NEG[RHS_value];
      else
	exit (EXIT_FAILURE);
    }
  else if (temp_optr->type == BINARY_OPERATOR)
    {
      signal_value LHS_value, RHS_value;
      LHS_value = evaluate_expression (temp_optr->LHS, temp_optr->LHS_type);
      RHS_value = evaluate_expression (temp_optr->RHS, temp_optr->RHS_type);
      if (temp_optr->e_operator == STD_OR)
	return lookup_OR[LHS_value][RHS_value];
      else if (temp_optr->e_operator == STD_AND)
	return lookup_AND[LHS_value][RHS_value];
      else if (temp_optr->e_operator == STD_XOR)
	return lookup_XOR[LHS_value][RHS_value];
      else
	exit (EXIT_FAILURE);
    }
  else
    exit (EXIT_FAILURE);
}

#endif
