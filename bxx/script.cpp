#include "script.hpp"

static std::string script_name = "";
static size_t script_index;
std::vector<std::function<void(PyObject*)>> events;

void setup_script_data(std::string const& name, size_t index)
{
    script_name = name;
    script_index = index;
}

size_t bxx::lib_register_event(std::function<void(PyObject*)> evt)
{
    events.push_back(evt);
    return events.size() - 1;
}

size_t bxx::get_script_index()
{
    return script_index;
}

std::string bxx::cur_script_name()
{
    return script_name;
}