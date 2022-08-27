#pragma once

#include <string>

#include "Python.h"

typedef unsigned long long cy_ptr_ct;
typedef void(*cy_exec_ct)(char*);
typedef cy_ptr_ct (*cy_eval_ptr_ct)(char*);
typedef int (*cy_eval_int_ct)(char*);
typedef float (*cy_eval_float_ct)(char*);
typedef char* (*cy_eval_string_ct)(char*);
typedef PyObject* (*cy_eval_pyobject_ct)(char*);

typedef float* (*cy_create_image_buffer_ct)(unsigned long long, int, int);
typedef void (*cy_apply_image_buffer_ct)(unsigned long long, char*);
typedef void (*cy_delete_image_buffer_ct)(unsigned long long);

struct shared_functions
{
    cy_exec_ct cy_exec;
    cy_eval_ptr_ct cy_eval_ptr;
    cy_eval_int_ct cy_eval_int;
    cy_eval_float_ct cy_eval_float;
    cy_eval_string_ct cy_eval_string;
    cy_eval_pyobject_ct cy_eval_pyobject;
    cy_create_image_buffer_ct cy_create_image_buffer;
    cy_apply_image_buffer_ct cy_apply_image_buffer;
    cy_delete_image_buffer_ct cy_delete_image_buffer;
};

shared_functions* get_pointers();