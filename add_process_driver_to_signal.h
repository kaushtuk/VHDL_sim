#include"utility_function_declaration.h"
#ifndef ADD_PROCESS_DRIVER_TO_SIGNAL_H
#define ADD_PROCESS_DRIVER_TO_SIGNAL_H

void
add_process_driver_to_signal (t_process * process_ptr)
{
  t_driver *temp_driver = (t_driver *) malloc (sizeof (t_driver));
  temp_driver->driving_value = process_ptr->driven_signal_ptr->value;
  temp_driver->driven_signal = process_ptr->driven_signal_ptr;
  t_driver *temp_driver_head =
    process_ptr->driven_signal_ptr->driver_head_ptr;
  temp_driver->next = temp_driver_head;
  process_ptr->driven_signal_ptr->driver_head_ptr = temp_driver;
  process_ptr->driver_ptr = temp_driver;
}

#endif
