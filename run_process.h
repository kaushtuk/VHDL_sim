#include"utility_function_declaration.h"

#ifndef RUN_PROCESS_H
#define RUN_PROCESS_H

t_event *
run_process (t_process * process_ptr)
{
  signal_value temp_signal_value;
  temp_signal_value =
    evaluate_expression (process_ptr->parse_tree_root_ptr,
			 process_ptr->root_type);
  t_driver *driver_ptr = drive_signal (process_ptr, temp_signal_value);

  t_event *update_event_ptr = schedule_driver (driver_ptr);
  return update_event_ptr;
}

#endif
