#ifndef TOKENIZER_H
#define TOKENIZER_H
typedef enum
{ TOKEN_OPERATOR, TOKEN_SIGNAL, TOKEN_OPENING_BRACE, TOKEN_CLOSING_BRACE,
    TOKEN_SENTINAL } token_type;
typedef struct token t_token;
struct token
{
  token_type type;
  void *token_node_ptr;
  node_type brace_root_type;
  t_token *next;
  t_token *previous;
  t_token *closing_brace_ptr;
  t_token *opening_brace_ptr;
};

bool
strcmpci (char const *a, char const *b)
{
  int i;
  for (i = 0; a[i] != '\0' && b[i] != '\0'; i++)
    {
      if (a[i] != b[i] && a[i] != 'A' + (b[i] - 'a'))
	return 1;
    }
  if (a[i] != '\0' || b[i] != '\0')
    return 1;
  return 0;
}

t_token *
tokenizer (t_token * token_list_head, t_process * parent_process,
	   trie * signal_trie_root, trie * in_port_trie_root)
{
  t_token *token_list_tail;
  char token_name[50], temp_string_end_buffer[2];
  token_list_head->previous = NULL;
  while (scanf (" %1[;]", temp_string_end_buffer) < 1)
    {
      if (scanf (" %[a-zA-Z0-9_]", token_name) < 1)
	{
	  if (scanf (" %1[()]", temp_string_end_buffer) < 1)
	    {
	      printf ("** ERROR:illegal token name\n");
	      exit (EXIT_FAILURE);
	    }
	  if (temp_string_end_buffer[0] == '(')
	    token_list_head->type = TOKEN_OPENING_BRACE;
	  else
	    token_list_head->type = TOKEN_CLOSING_BRACE;
	  token_list_head->token_node_ptr = NULL;
	  token_list_head->brace_root_type = OPERATOR_NODE_TYPE;
	}
      else if (strcmpci (token_name, "after") == 0)
	{
	  parent_process->is_after = 1;
	  if (scanf ("%d", &(parent_process->after_time_stamp)) < 1)
	    {
	      printf ("** ERROR:illegal token name\n");
	      exit (EXIT_FAILURE);
	    }
	  if (scanf (" %*1[n]%*1[s]%1[;]", temp_string_end_buffer) < 1)
	    {
	      printf ("** ERROR: invalid after clause time unit\n");
	      exit (EXIT_FAILURE);
	    }
	  token_list_head->type = TOKEN_SENTINAL;
	  token_list_head->next = NULL;
	  token_list_tail = token_list_head;
	  return token_list_tail;
	}
      else if (strcmpci (token_name, "or") == 0)
	{
	  token_list_head->type = TOKEN_OPERATOR;
	  token_list_head->brace_root_type = OPERATOR_NODE_TYPE;
	  token_list_head->token_node_ptr = malloc (sizeof (t_operator_node));
	  ((t_operator_node *) (token_list_head->token_node_ptr))->type =
	    BINARY_OPERATOR;
	  ((t_operator_node *) (token_list_head->token_node_ptr))->
	    e_operator = STD_OR;
	}
      else if (strcmpci (token_name, "xor") == 0)
	{
	  token_list_head->type = TOKEN_OPERATOR;
	  token_list_head->brace_root_type = OPERATOR_NODE_TYPE;
	  token_list_head->token_node_ptr = malloc (sizeof (t_operator_node));
	  ((t_operator_node *) (token_list_head->token_node_ptr))->type =
	    BINARY_OPERATOR;
	  ((t_operator_node *) (token_list_head->token_node_ptr))->
	    e_operator = STD_XOR;
	}
      else if (strcmpci (token_name, "and") == 0)
	{
	  token_list_head->type = TOKEN_OPERATOR;
	  token_list_head->brace_root_type = OPERATOR_NODE_TYPE;
	  token_list_head->token_node_ptr = malloc (sizeof (t_operator_node));
	  ((t_operator_node *) (token_list_head->token_node_ptr))->type =
	    BINARY_OPERATOR;
	  ((t_operator_node *) (token_list_head->token_node_ptr))->
	    e_operator = STD_AND;
	}
      else if (strcmpci (token_name, "not") == 0)
	{
	  token_list_head->type = TOKEN_OPERATOR;
	  token_list_head->brace_root_type = OPERATOR_NODE_TYPE;
	  token_list_head->token_node_ptr = malloc (sizeof (t_operator_node));
	  ((t_operator_node *) (token_list_head->token_node_ptr))->type =
	    UNARY_OPERATOR;
	  ((t_operator_node *) (token_list_head->token_node_ptr))->
	    e_operator = STD_NEG;
	}
      else
	{
	  token_list_head->type = TOKEN_SIGNAL;
	  token_list_head->brace_root_type = SIGNAL_NODE_TYPE;
	  token_list_head->token_node_ptr =
	    trie_search (token_name, signal_trie_root);
	  if (token_list_head->token_node_ptr == NULL)
	    {
	      t_port *temp_token_port =
		(t_port *) trie_search (token_name, in_port_trie_root);
	      if (temp_token_port == NULL
		  || temp_token_port->signal_ptr == NULL)
		{
		  printf ("** ERROR: illegal token name\n");
		  exit (EXIT_FAILURE);
		}
	      token_list_head->token_node_ptr =
		(void *) (temp_token_port->signal_ptr);
	    }
	}
      token_list_head->next = (t_token *) malloc (sizeof (t_token));
      token_list_head->next->previous = token_list_head;
      token_list_head = token_list_head->next;
    }
  parent_process->is_after = 0;
  token_list_head->type = TOKEN_SENTINAL;
  token_list_head->next = NULL;
  token_list_tail = token_list_head;
  return token_list_tail;
}

void
print_token (t_token * head)
{
  printf ("** token list: ");
  while (head != NULL)
    {
      printf ("%d,", head->type);
      head = head->next;
    }
  printf ("\n");
}
#endif
