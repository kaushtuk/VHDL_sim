#include"utility_function_declaration.h"

#ifndef RUN_SIMULATION_H
#define RUN_SIMULATION_H

void run_simulation(t_time_line* time_line_head,t_signal* signal_list_head){
	t_event_queue* current_event_queue;
	t_time_line* temp_time_node;
	do{
		//printf("----------Time = %d ns------------\n",time_line_head->time_stamp);
		current_event_queue=time_line_head->scheduled_event_queue;
		run_event_queue(current_event_queue,signal_list_head,time_line_head);

		temp_time_node=time_line_head;
		time_line_head=time_line_head->next;
		free(temp_time_node->scheduled_event_queue);
		free(temp_time_node);
	}while(time_line_head!=NULL);
}

#endif

