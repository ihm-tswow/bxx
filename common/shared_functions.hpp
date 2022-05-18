#pragma once

#include <string>

typedef unsigned long long cy_ptr_ct;
typedef void(*cy_exec_ct)(char*);
typedef cy_ptr_ct (*cy_eval_ptr_ct)(char*);
typedef int (*cy_eval_int_ct)(char*);
typedef float (*cy_eval_float_ct)(char*);
typedef char* (*cy_eval_string_ct)(char*);

struct shared_functions
{
    cy_exec_ct cy_exec;
    cy_eval_ptr_ct cy_eval_ptr;
    cy_eval_int_ct cy_eval_int;
    cy_eval_float_ct cy_eval_float;
    cy_eval_string_ct cy_eval_string;
};

shared_functions* get_pointers();