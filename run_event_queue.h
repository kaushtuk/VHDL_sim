#include"utility_function_declaration.h"

#ifndef RUN_EVENT_QUEUE_H
#define RUN_EVENT_QUEUE_H

bool is_equivalent_update_event(t_event* event1,t_event* event2){
	if(event1->type==DRIVING_EVENT && event2->type==DRIVING_EVENT && event1->iteration==event2->iteration && event1->update_signal_driver==event2->update_signal_driver)
		return 1;
	return 0;
}

bool is_equivalent_evaluate_event(t_event* event1,t_event* event2){
	if(event1->type==EVALUATION_EVENT && event2->type==EVALUATION_EVENT && event1->iteration==event2->iteration && event1->evaluation_process == event2->evaluation_process)
		return 1;
	return 0;
}

signal_value resolution_function(t_driver* driver_head_ptr){
	signal_value resolved_value = driver_head_ptr->driving_value;
	driver_head_ptr = driver_head_ptr->next;
	while(driver_head_ptr != NULL){
		resolved_value = lookup_RES[resolved_value][driver_head_ptr->driving_value];
		driver_head_ptr = driver_head_ptr->next;
	}
	return resolved_value;
}

void remove_previous_driving_event(t_event_queue* temp_event_queue_ptr,t_event* update_event_ptr){
	if(isempty(temp_event_queue_ptr)) return;
	t_event* event_list_head=temp_event_queue_ptr->top;
	if(is_equivalent_update_event(event_list_head,update_event_ptr)){
		dequeue_event(temp_event_queue_ptr);
		return;
	}
	while(event_list_head->next!=NULL && !is_equivalent_update_event(event_list_head->next,update_event_ptr))
		event_list_head=event_list_head->next;
	if(event_list_head->next==NULL) return;
	t_event* temp_event=event_list_head->next;
	event_list_head->next=event_list_head->next->next;
	free(temp_event);
	if(event_list_head->next==NULL)
		temp_event_queue_ptr->bottom=event_list_head;
}

void remove_previous_triggering_event(t_event_queue* temp_event_queue_ptr,t_event* evaluate_event_ptr){
	if(isempty(temp_event_queue_ptr)) return;
	t_event* event_list_head = temp_event_queue_ptr->top;
	if(is_equivalent_evaluate_event(event_list_head,evaluate_event_ptr)){
		dequeue_event(temp_event_queue_ptr);
		return;
	}
	while(event_list_head->next!=NULL && !is_equivalent_evaluate_event(event_list_head->next,evaluate_event_ptr))
		event_list_head=event_list_head->next;
	if(event_list_head->next==NULL) return;
	t_event* temp_event=event_list_head->next;
	event_list_head->next=event_list_head->next->next;
	free(temp_event);
	if(event_list_head->next==NULL)
		temp_event_queue_ptr->bottom=event_list_head;
}

t_event_queue* return_event_queue_handle(int time_stamp,t_time_line* time_line_head){
	while(time_line_head->next!=NULL && time_line_head->next->time_stamp<time_stamp)
		time_line_head=time_line_head->next;
	if(time_line_head->next==NULL){
		time_line_head->next=(t_time_line*)malloc(sizeof(t_time_line));
		time_line_head->next->next=NULL;
		time_line_head->next->time_stamp=time_stamp;
		time_line_head->next->scheduled_event_queue=(t_event_queue*)malloc(sizeof(t_event_queue));
		time_line_head->next->scheduled_event_queue->top=NULL;
		time_line_head->next->scheduled_event_queue->bottom=NULL;
		return time_line_head->next->scheduled_event_queue;
	}
	if(time_line_head->next->time_stamp>time_stamp){
		t_time_line* temp_time_node=(t_time_line*)malloc(sizeof(t_time_line));
		temp_time_node->next=time_line_head->next;
		time_line_head->next=temp_time_node;
		time_line_head->next->time_stamp=time_stamp;
		time_line_head->next->scheduled_event_queue=(t_event_queue*)malloc(sizeof(t_event_queue));
		time_line_head->next->scheduled_event_queue->top=NULL;
		time_line_head->next->scheduled_event_queue->bottom=NULL;
		return time_line_head->next->scheduled_event_queue;
	}
	return time_line_head->next->scheduled_event_queue;
}

void run_event_queue(t_event_queue* event_queue_ptr,t_signal* signal_list_head,t_time_line* time_line_head){
	while(!isempty(event_queue_ptr)){
		t_event* current_running_event;
		current_running_event=event_queue_ptr->top;
		if(current_running_event->type==EVALUATION_EVENT){
			t_event* update_event_ptr;
			update_event_ptr=run_process(current_running_event->evaluation_process);
			current_running_event->evaluation_process->is_enqueued=0;
			int after_time_stamp;

			//No time advancement added for wait and after clauses
			if(current_running_event->evaluation_process->is_after==0){
				update_event_ptr->iteration=(current_running_event->iteration)+1;
				remove_previous_driving_event(event_queue_ptr,update_event_ptr);
				enqueue_event(event_queue_ptr,update_event_ptr);
			}
			else{
				update_event_ptr->iteration=0;
				after_time_stamp=current_running_event->evaluation_process->after_time_stamp;
				t_event_queue* temp_event_queue_ptr;
				temp_event_queue_ptr=return_event_queue_handle((time_line_head->time_stamp)+after_time_stamp,time_line_head);
				remove_previous_driving_event(temp_event_queue_ptr,update_event_ptr);
				enqueue_event(temp_event_queue_ptr,update_event_ptr);
			}

			if(current_running_event->evaluation_process->report_head_ptr!=NULL){
				printf("# ** NOTE: ");
				display_report_statement(current_running_event->evaluation_process->report_head_ptr);
				printf("#    Time: %d ns ",time_line_head->time_stamp);
				printf("iteration: %d process: %s\n",current_running_event->iteration,current_running_event->evaluation_process->name);
				//printf("\n");
			}	
		}
		else if(current_running_event->type==DRIVING_EVENT){
			t_signal* updating_signal;
			t_driver* updating_driver;
			updating_signal = current_running_event->update_signal_driver->driven_signal;
			updating_driver = current_running_event->update_signal_driver;
			/*
			   updating_driver->driving_value = updating_driver->updated_driving_value;
			   */
			updating_driver->driving_value = current_running_event->updated_driving_value;
			signal_value resolved_value = resolution_function(updating_signal->driver_head_ptr);
			if(updating_signal->value != resolved_value){
				updating_signal->value = resolved_value;
				t_trigger* trigger_list_head;
				trigger_list_head = updating_signal->triggered_processes;
				while(trigger_list_head!=NULL){
					t_event* triggered_event_ptr;
					triggered_event_ptr = create_triggered_event(trigger_list_head);
					if(triggered_event_ptr!=NULL){
						triggered_event_ptr->iteration = (current_running_event->iteration);
						remove_previous_triggering_event(event_queue_ptr,triggered_event_ptr);
						enqueue_event(event_queue_ptr,triggered_event_ptr);
					}
					trigger_list_head=trigger_list_head->next;
				}
			}
		}
		else exit(EXIT_FAILURE);
		dequeue_event(event_queue_ptr);
	}
}

#endif
