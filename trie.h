#ifndef TRIE_H
#define TRIE_H

typedef struct s_trie trie;
struct s_trie
{
  void *object;
  trie *next[64];
};

trie *
trie_constructor ()
{
  int i;
  trie *root_ptr = (trie *) malloc (sizeof (trie));
  root_ptr->object = NULL;
  for (i = 0; i < 64; i++)
    root_ptr->next[i] = NULL;
  return root_ptr;
}

void
trie_insert (void *object, char *name, trie * root_ptr)
{
  //base case
  if (name[0] == '\0')
    {
      if (root_ptr->object != NULL)
	free (root_ptr->object);
      root_ptr->object = object;
      return;
    }

  //recursive case
  char temp_index;
  if (name[0] >= 'a' && name[0] <= 'z')
    temp_index = name[0] - 'a';
  else if (name[0] >= '0' && name[0] <= '9')
    temp_index = 26 + (name[0] - '0');
  else if (name[0] >= 'A' && name[0] <= 'Z')
    temp_index = 36 + (name[0] - 'A');
  else if (name[0] == '_')
    temp_index = 62;
  else if (name[0] == '\\')
    temp_index = 63;
  else
    {
      printf ("ERROR::unknown_trie_symbol\n");
      exit (EXIT_FAILURE);
    }
  if (root_ptr->next[temp_index] == NULL)
    root_ptr->next[temp_index] = trie_constructor ();
  trie_insert (object, &(name[1]), root_ptr->next[temp_index]);
}

void *
trie_search (char *name, trie * root_ptr)
{
  //base case
  if (name[0] == '\0')
    return root_ptr->object;

  //recursive case
  char temp_index;
  if (name[0] >= 'a' && name[0] <= 'z')
    temp_index = name[0] - 'a';
  else if (name[0] >= '0' && name[0] <= '9')
    temp_index = 26 + (name[0] - '0');
  else if (name[0] >= 'A' && name[0] <= 'Z')
    temp_index = 36 + (name[0] - 'A');
  else if (name[0] == '_')
    temp_index = 62;
  else if (name[0] == '\\')
    temp_index = 63;
  else
    {
      printf ("ERROR::unknown_trie_symbol\n");
      exit (EXIT_FAILURE);
    }
  if (root_ptr->next[temp_index] == NULL)
    return NULL;
  return trie_search (&(name[1]), root_ptr->next[temp_index]);
}



#endif
