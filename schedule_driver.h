#include"utility_function_declaration.h"

#ifndef SCHEDULE_DRIVER_H
#define SCHEDULE_DRIVER_H

t_event* schedule_driver(t_driver* driver_ptr){
	t_event* temp_update_event;
	temp_update_event=(t_event*)malloc(sizeof(t_event));
	temp_update_event->next=NULL;
	temp_update_event->type=DRIVING_EVENT;
	temp_update_event->evaluation_process=NULL;
	temp_update_event->update_signal_driver=driver_ptr;
	temp_update_event->updated_driving_value = driver_ptr->updated_driving_value;
	return temp_update_event;
}

#endif
