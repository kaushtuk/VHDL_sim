#include"utility_function_declaration.h"
#ifndef DISPLAY_REPORT_STATEMENT_H
#define DISPLAY_REPORT_STATEMENT_H

void
display_report_statement (t_report * report_head_ptr)
{
  if (report_head_ptr == NULL)
    {
      printf ("\n");
      return;
    }
  if (report_head_ptr->type == 0)
    {
      printf ("%s", report_head_ptr->string_ptr);
/*
		char* char_ptr = report_head_ptr->string_ptr;
		while((*char_ptr) != '\0'){
			if((*char_ptr) == ' ')
				printf(" ");
			else
				printf("%c",(*char_ptr));
			char_ptr++;
		}
*/
    }
  if (report_head_ptr->type == 1)
    printf ("\'%c\'",
	    signal_value_to_char (report_head_ptr->signal_ptr->value));
  display_report_statement (report_head_ptr->next);
}

#endif
