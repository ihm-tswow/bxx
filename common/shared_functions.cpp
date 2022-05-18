#include "shared_functions.hpp"
#include <iostream>

static shared_functions* _pointers = nullptr;
void __init_pointers_store(shared_functions* pointers)
{
    _pointers = pointers;
}

shared_functions* get_pointers()
{
    return _pointers;
}