#include"signal_list_definition.h"
#include"process_list_definition.h"
#include"operator_node_definition.h"
#include"event_queue_definition.h"
#include"trigger_list_definition.h"
#include"time_line_definition.h"
#include"driver.h"

#include"entity_struct_definition.h"
#include"architecture_struct_definition.h"
#include"testcase_handle_definition.h"
#include"port_list_definition.h"

#include"report_list_definition.h"

#ifndef UTILITY_FUNCTION_DECLARATION_H
#define UTILITY_FUNCTION_DECLARATION_H

t_time_line *create_init_event_queue (t_signal * signal_list_head,
				      t_process * process_list_head);
signal_value evaluate_expression (void *root_ptr, node_type root_type);
void run_simulation (t_time_line * time_line_head,
		     t_signal * signal_list_head);
t_event *run_process (t_process * process_ptr);
t_event *schedule_driver (t_driver * driver_ptr);
void run_event_queue (t_event_queue * event_queue_ptr, t_signal *,
		      t_time_line * time_line_head);
t_event *create_triggered_event (t_trigger * trigger_ptr);

t_entity *entity_constructor (char *name);
t_architecture *architecture_constructor (char *name, t_entity * entity_ptr);

void add_process_driver_to_signal (t_process * process_ptr);

void display_report_statement (t_report * report_head_ptr);
#endif
