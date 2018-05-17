#include"typedef_structs.h"

#ifndef TRIGGER_LIST_DEFINITION_H
#define TRIGGER_LIST_DEFINITION_H

struct trigger
{
  t_process *scheduled_process;
  t_trigger *next;
};

#endif
