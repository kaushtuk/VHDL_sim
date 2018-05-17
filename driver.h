#include"typedef_structs.h"

#ifndef DRIVER_H
#define DRIVER_H

struct driver
{
  signal_value driving_value;
  signal_value updated_driving_value;
  t_signal *driven_signal;
  t_driver *next;
};

t_driver *
drive_signal (t_process * driving_process_ptr, signal_value driving_value)
{
  t_driver *temp_driver = driving_process_ptr->driver_ptr;
  temp_driver->updated_driving_value = driving_value;
  return temp_driver;
}


#endif
