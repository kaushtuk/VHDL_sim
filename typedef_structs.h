#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include"trie.h"
#ifndef TYPEDEF_STRUCTS_H
#define TYPEDEF_STRUCTS_H

//struct typedefs
typedef struct signal t_signal;
typedef struct process t_process;
typedef struct event t_event;
typedef struct event_queue t_event_queue;
typedef struct trigger t_trigger;
typedef struct time_line t_time_line;
typedef struct operator_node t_operator_node;
typedef struct driver t_driver;

typedef struct architecture t_architecture;
typedef struct entity t_entity;
typedef struct testcase_handle t_testcase_handle;
typedef struct port t_port;

typedef struct report t_report;

//enum typedefs
typedef enum
{ STD_U, STD_X, STD_0, STD_1, STD_Z, STD_W, STD_L, STD_H,
    STD_DCARE } signal_value;
typedef enum
{ SIGNAL_NODE_TYPE, OPERATOR_NODE_TYPE } node_type;
typedef enum
{ UNARY_OPERATOR, BINARY_OPERATOR } operator_type;
typedef enum
{ STD_OR, STD_AND, STD_XOR, STD_NEG } gate;
typedef enum
{ EVALUATION_EVENT, DRIVING_EVENT } event_type;
#endif
