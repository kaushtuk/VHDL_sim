#include"tokenizer.h"
#ifndef VALIDATE_EXPRESSION_H
#define VALIDATE_EXPRESSION_H
typedef struct token_dll
{
  t_token *head;
  t_token *tail;
} t_token_dll;
bool
check_parentheses (t_token_dll * token_dlist)
{
  int paren_check = 0;
  t_token *brace_stack_ptr = NULL;
  t_token *head = token_dlist->head;
  t_token *tail = token_dlist->tail;
  while (head != tail)
    {
      if (head->type == TOKEN_OPENING_BRACE)
	{
	  paren_check++;
	  t_token *temp_top = (t_token *) malloc (sizeof (t_token));
	  temp_top->previous = brace_stack_ptr;
	  (temp_top->token_node_ptr) = (void *) head;
	  brace_stack_ptr = temp_top;
	}
      else if (head->type == TOKEN_CLOSING_BRACE)
	{
	  paren_check--;
	  if (paren_check < 0)
	    return 0;
	  t_token *temp_top = brace_stack_ptr;
	  brace_stack_ptr = brace_stack_ptr->previous;
	  ((t_token *) (temp_top->token_node_ptr))->closing_brace_ptr = head;
	  head->opening_brace_ptr = ((t_token *) (temp_top->token_node_ptr));
	  free (temp_top);
	}
      head = head->next;
    }
  if (paren_check == 0)
    return 1;
  return 0;
}

void
remove_brace_signal (t_token_dll * token_dlist)
{
  t_token *head = token_dlist->head;
  t_token *tail = token_dlist->tail;
  t_token *exp_itr = head;
  while (exp_itr != tail)
    {
      if (exp_itr->type == TOKEN_SIGNAL)
	{
	  t_token *i;
	  t_token *j;
	  i = exp_itr->previous;
	  j = exp_itr->next;
	  while (i != head->previous && j != tail
		 && i->type == TOKEN_OPENING_BRACE
		 && j->type == TOKEN_CLOSING_BRACE)
	    {
	      i->next->previous = i->previous;
	      if (i->previous != head->previous)
		i->previous->next = i->next;	//edition
	      else
		head = i->next;
	      j->previous->next = j->next;
	      j->next->previous = j->previous;
	      t_token *temp = i;
	      i = i->previous;
	      free (temp);
	      temp = j;
	      j = j->next;
	      free (temp);
	    }
	}
      exp_itr = exp_itr->next;
    }
  token_dlist->head = head;
  token_dlist->tail = tail;
}

void
check_operator_LHS_RHS (t_token_dll * token_dlist)
{
  t_token *head = token_dlist->head;
  t_token *tail = token_dlist->tail;
  t_token *exp_itr = tail->previous;
  while (exp_itr != head->previous)
    {
      if (exp_itr->type == TOKEN_OPERATOR)
	{
	  //RHS
	  if (exp_itr->next == tail)
	    {
	      printf ("** ERROR:illegal operator RHS \n");
	      exit (EXIT_FAILURE);
	    }
	  if (exp_itr->next->type == TOKEN_CLOSING_BRACE)
	    {
	      printf ("** ERROR:illegal operator RHS \n");
	      exit (EXIT_FAILURE);
	    }
	  if (exp_itr->next->type == TOKEN_OPERATOR
	      && ((t_operator_node *) (exp_itr->next->token_node_ptr))->
	      type == BINARY_OPERATOR)
	    {
	      printf ("** ERROR:illegal operator RHS \n");
	      exit (EXIT_FAILURE);
	    }

	  //LHS
	  if (((t_operator_node *) (exp_itr->token_node_ptr))->type ==
	      BINARY_OPERATOR)
	    {
	      if (exp_itr == head)
		{
		  printf ("** ERROR:illegal operator LHS \n");
		  exit (EXIT_FAILURE);
		}
	      if (exp_itr->previous->type == TOKEN_OPENING_BRACE)
		{
		  printf ("** ERROR:illegal operator LHS \n");
		  exit (EXIT_FAILURE);
		}
	      if (exp_itr->previous->type == TOKEN_OPERATOR)
		{
		  printf ("** ERROR:illegal operator LHS \n");
		  exit (EXIT_FAILURE);
		}
	    }
	  if (((t_operator_node *) (exp_itr->token_node_ptr))->type ==
	      UNARY_OPERATOR)
	    {
	      if (exp_itr != head)
		{
		  if (exp_itr->previous->type == TOKEN_CLOSING_BRACE)
		    {
		      printf ("** ERROR:illegal operator LHS \n");
		      exit (EXIT_FAILURE);
		    }
		  if (exp_itr->previous->type == TOKEN_SIGNAL)
		    {
		      printf ("** ERROR:illegal operator LHS \n");
		      exit (EXIT_FAILURE);
		    }
		}
	    }
	}
      exp_itr = exp_itr->previous;
    }
  //operator LHS and RHS tokens are  free from error
}

void
remove_brace_unary_operator (t_token_dll * token_dlist)
{
  t_token *head = token_dlist->head;
  t_token *tail = token_dlist->tail;
  t_token *exp_itr = tail->previous;
  while (exp_itr != head->previous)
    {
      if (exp_itr->type == TOKEN_OPERATOR
	  && ((t_operator_node *) (exp_itr->token_node_ptr))->type ==
	  UNARY_OPERATOR)
	{
	  t_token *j = exp_itr->next;
	  if (j->type == TOKEN_OPENING_BRACE)
	    j = j->closing_brace_ptr;
	  t_token *i = exp_itr;
	  //opener
	  t_token *opener = (t_token *) malloc (sizeof (t_token));
	  opener->type = TOKEN_OPENING_BRACE;
	  opener->token_node_ptr = exp_itr->token_node_ptr;
	  opener->brace_root_type = OPERATOR_NODE_TYPE;
	  opener->next = i;
	  opener->previous = i->previous;
	  if (i->previous != head->previous)
	    i->previous->next = opener;
	  else
	    head = opener;
	  i->previous = opener;
	  //closer
	  t_token *closer = (t_token *) malloc (sizeof (t_token));
	  closer->type = TOKEN_CLOSING_BRACE;
	  closer->token_node_ptr = exp_itr->token_node_ptr;
	  closer->brace_root_type = OPERATOR_NODE_TYPE;
	  closer->previous = j;
	  closer->next = j->next;
	  j->next->previous = closer;
	  j->next = closer;
	  opener->closing_brace_ptr = closer;
	  closer->opening_brace_ptr = opener;
	  //now we have to remove extra braces
	  i = opener->previous;
	  j = closer->next;
	  while (i != head->previous && j != tail
		 && i->type == TOKEN_OPENING_BRACE
		 && j->type == TOKEN_CLOSING_BRACE)
	    {
	      i->next->previous = i->previous;
	      if (i->previous != head->previous)
		i->previous->next = i->next;	//edition
	      else
		head = i->next;
	      j->previous->next = j->next;
	      j->next->previous = j->previous;
	      t_token *temp = i;
	      i = i->previous;
	      free (temp);
	      temp = j;
	      j = j->next;
	      free (temp);
	    }
	}
      exp_itr = exp_itr->previous;
    }
  token_dlist->head = head;
  token_dlist->tail = tail;
}

void
remove_brace_binary_operator (t_token_dll * token_dlist)
{
  t_token *head = token_dlist->head;
  t_token *tail = token_dlist->tail;
  t_token *exp_itr = head;
  while (exp_itr != tail)
    {
      if (exp_itr->type == TOKEN_OPERATOR
	  && ((t_operator_node *) (exp_itr->token_node_ptr))->type ==
	  BINARY_OPERATOR)
	{
	  t_token *j = exp_itr->next;
	  if (j->type == TOKEN_OPENING_BRACE)
	    j = j->closing_brace_ptr;
	  t_token *i = exp_itr->previous;
	  if (i->type == TOKEN_CLOSING_BRACE)
	    i = i->opening_brace_ptr;
	  //opener
	  t_token *opener = (t_token *) malloc (sizeof (t_token));
	  opener->type = TOKEN_OPENING_BRACE;
	  opener->token_node_ptr = exp_itr->token_node_ptr;
	  opener->brace_root_type = OPERATOR_NODE_TYPE;
	  opener->next = i;
	  opener->previous = i->previous;
	  if (i->previous != head->previous)
	    i->previous->next = opener;
	  else
	    head = opener;
	  i->previous = opener;
	  //closer
	  t_token *closer = (t_token *) malloc (sizeof (t_token));
	  closer->type = TOKEN_CLOSING_BRACE;
	  closer->token_node_ptr = exp_itr->token_node_ptr;
	  closer->brace_root_type = OPERATOR_NODE_TYPE;
	  closer->previous = j;
	  closer->next = j->next;
	  j->next->previous = closer;
	  j->next = closer;
	  opener->closing_brace_ptr = closer;
	  closer->opening_brace_ptr = opener;
	  //now we have to remove extra braces
	  i = opener->previous;
	  j = closer->next;
	  while (i != head->previous && j != tail
		 && i->type == TOKEN_OPENING_BRACE
		 && j->type == TOKEN_CLOSING_BRACE)
	    {
	      i->next->previous = i->previous;
	      if (i->previous != head->previous)
		i->previous->next = i->next;	//edition
	      else
		head = i->next;
	      j->previous->next = j->next;
	      j->next->previous = j->previous;
	      t_token *temp = i;
	      i = i->previous;
	      free (temp);
	      temp = j;
	      j = j->next;
	      free (temp);
	    }
	}
      exp_itr = exp_itr->next;
    }
  token_dlist->head = head;
  token_dlist->tail = tail;
}

void
validate_expression (t_token_dll * token_dlist)
{
  //print_token(token_dlist->head);
  if (check_parentheses (token_dlist) == 0)
    {
      printf ("** ERROR: improper parentheses  \n");
      exit (EXIT_FAILURE);
    }
  remove_brace_signal (token_dlist);
  check_operator_LHS_RHS (token_dlist);
  remove_brace_unary_operator (token_dlist);
  remove_brace_binary_operator (token_dlist);
  //validate expressions {EE},{)(}
  t_token *head = token_dlist->head;
  t_token *tail = token_dlist->tail;
  t_token *exp_itr = head;
  while (exp_itr != tail)
    {
      if (exp_itr->type == TOKEN_SIGNAL)
	{
	  if (exp_itr->next->type == TOKEN_SIGNAL
	      || exp_itr->next->type == TOKEN_OPENING_BRACE)
	    {
	      printf ("** ERROR: missing operator between expressions\n");
	      exit (EXIT_FAILURE);
	    }
	}
      if (exp_itr->type == TOKEN_CLOSING_BRACE)
	{
	  if (exp_itr->next->type == TOKEN_SIGNAL
	      || exp_itr->next->type == TOKEN_OPENING_BRACE)
	    {
	      printf ("** ERROR: missing operator between expressions\n");
	      exit (EXIT_FAILURE);
	    }
	}
      if (exp_itr->type == TOKEN_OPERATOR)
	{
	  //set LHS RHS and their types for each operator
	  t_operator_node *temp_optr =
	    ((t_operator_node *) (exp_itr->token_node_ptr));
	  temp_optr->RHS = exp_itr->next->token_node_ptr;
	  temp_optr->RHS_type = exp_itr->next->brace_root_type;
	  if (temp_optr->type == BINARY_OPERATOR)
	    {
	      temp_optr->LHS = exp_itr->previous->token_node_ptr;
	      temp_optr->LHS_type = exp_itr->previous->brace_root_type;
	    }
	}
      exp_itr = exp_itr->next;
    }
  //print_token(token_dlist->head);
}
#endif
