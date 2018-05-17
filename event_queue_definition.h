#include"typedef_structs.h"

#ifndef EVENT_QUEUE_DEFINITION_H
#define EVENT_QUEUE_DEFINITION_H

struct event
{
  t_event *next;
  event_type type;		//0-evalation,1-driving
  int iteration;
  t_process *evaluation_process;
  t_driver *update_signal_driver;
  signal_value updated_driving_value;
};

struct event_queue
{
  t_event *top;
  t_event *bottom;
};

void
enqueue_event (t_event_queue * t_event_queue_ptr, t_event * latest_event_ptr)
{
  if (t_event_queue_ptr->top == NULL && t_event_queue_ptr->bottom == NULL)
    t_event_queue_ptr->top = latest_event_ptr;
  else
    t_event_queue_ptr->bottom->next = latest_event_ptr;
  t_event_queue_ptr->bottom = latest_event_ptr;
}

bool isempty (t_event_queue * t_event_queue_ptr);
void
dequeue_event (t_event_queue * t_event_queue_ptr)
{
  if (t_event_queue_ptr->top != t_event_queue_ptr->bottom)
    {
      t_event *temp = t_event_queue_ptr->top;
      t_event_queue_ptr->top = t_event_queue_ptr->top->next;
      free (temp);
    }
  else if (!isempty (t_event_queue_ptr))
    {
      t_event *temp = t_event_queue_ptr->top;
      t_event_queue_ptr->top = NULL;
      t_event_queue_ptr->bottom = NULL;
      free (temp);
    }
}

bool
isempty (t_event_queue * t_event_queue_ptr)
{
  if (t_event_queue_ptr->top == NULL && t_event_queue_ptr->bottom == NULL)
    return true;
  else
    return false;
}

#endif
