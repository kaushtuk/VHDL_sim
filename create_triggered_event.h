#include"utility_function_declaration.h"

#ifndef CREATE_EVALUATION_EVENT_H
#define CREATE_EVALUATION_EVENT_H

t_event *
create_triggered_event (t_trigger * trigger_ptr)
{
  t_process *scheduled_process;
  scheduled_process = trigger_ptr->scheduled_process;
  t_event *evaluation_event_ptr = NULL;
  if (scheduled_process->is_enqueued == 0)
    {
      evaluation_event_ptr = (t_event *) malloc (sizeof (t_event));
      evaluation_event_ptr->next = NULL;
      evaluation_event_ptr->type = EVALUATION_EVENT;
      evaluation_event_ptr->evaluation_process = scheduled_process;
      evaluation_event_ptr->update_signal_driver = NULL;
      scheduled_process->is_enqueued = 1;
    }
  return evaluation_event_ptr;
}

#endif
