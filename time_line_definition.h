#include"typedef_structs.h"

#ifndef TIME_LINE_DEFINITION_H
#define TIME_LINE_DEFINITION_H

struct time_line{
	t_time_line* next;
	t_event_queue* scheduled_event_queue;
	int time_stamp;				//nanoseconds
};

#endif
