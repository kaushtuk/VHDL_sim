#include"utility_function_declaration.h"

#ifndef CREATE_INIT_EVENT_QUEUE_H
#define CREATE_INIT_EVENT_QUEUE_H

t_time_line* create_init_event_queue (t_signal *signal_list_head, t_process *process_list_head)
{
	//create time_line
	t_time_line *time_line_head=(t_time_line*)malloc(sizeof(t_time_line));
	time_line_head->time_stamp=0;
	time_line_head->next=NULL;
	time_line_head->scheduled_event_queue=(t_event_queue*)malloc(sizeof(t_event_queue));
	time_line_head->scheduled_event_queue->top=NULL;
	time_line_head->scheduled_event_queue->bottom=NULL;
	
	//create evauation events for each process in a queue
	while(process_list_head!=NULL)
	{
		//create evaluation event
		t_event* temp=(t_event*)malloc(sizeof(t_event));
		temp->next=NULL;
		temp->type=EVALUATION_EVENT;
		temp->iteration=0;
		temp->evaluation_process=process_list_head;
		temp->update_signal_driver=NULL;

		//add to the queue
		enqueue_event(time_line_head->scheduled_event_queue,temp);

		//iterate on process
		process_list_head=process_list_head->next;
	}
	return time_line_head;
}

#endif

