#include "script.hpp"

#include "python_object.hpp"

static std::string script_name = "";
static size_t script_index;
std::vector<std::function<bxx::python_object(bxx::python_tuple const&)>> events;

void setup_script_data(std::string const& name, size_t index)
{
    script_name = name;
    script_index = index;
}

size_t bxx::lib_register_event(std::function<python_object(bxx::python_tuple const&)> evt)
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