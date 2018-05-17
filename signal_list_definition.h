#include"typedef_structs.h"

#ifndef SIGNAL_LIST_DEFINITION_H
#define SIGNAL_LIST_DEFINITION_H

struct signal
{
  char *name;
  t_signal *next;
  signal_value value;		//std_logic
  t_trigger *triggered_processes;
  t_driver *driver_head_ptr;
};

void
set_signal_name (t_signal * t_signal_ptr, char *name)
{
  int string_length;
  if (t_signal_ptr->name != NULL)
    free (t_signal_ptr->name);
  for (string_length = 0; name[string_length] != '\0'; string_length++);
  t_signal_ptr->name = (char *) malloc (sizeof (char[string_length + 1]));
  for (; string_length >= 0; --string_length)
    (t_signal_ptr->name)[string_length] = name[string_length];
}

char
signal_value_to_char (signal_value value)
{
  if (value == STD_U)
    return 'U';
  if (value == STD_0)
    return '0';

  if (value == STD_1)
    return '1';

  if (value == STD_X)
    return 'X';
  if (value == STD_Z)
    return 'Z';

  if (value == STD_W)
    return 'W';

  if (value == STD_L)
    return 'L';

  if (value == STD_H)
    return 'H';
  if (value == STD_DCARE)
    return '-';
  return '0';
}

void
display_signal_values (t_signal * signal_list_head)
{
  printf ("# ** NOTE:\n ");
  while (signal_list_head != NULL)
    {
      printf ("%s = \'%c\'  \n ", (signal_list_head->name),
	      signal_value_to_char (signal_list_head->value));
      signal_list_head = signal_list_head->next;
    }
  printf ("\n");
}

#endif
