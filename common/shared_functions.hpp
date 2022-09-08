#pragma once

#include <string>

#include <Python.h>

typedef void(*cy_exec_ct)(char*);
typedef PyObject* (*cy_eval_ct)(char*);
typedef float* (*cy_create_float_buffer_ct)(unsigned long long, int);

struct shared_functions
{
    cy_exec_ct cy_exec;
    cy_eval_ct cy_eval;
    cy_create_float_buffer_ct cy_create_float_buffer;
};

shared_functions* get_pointers();