#include <common/exec.hpp>
#include <common/script_index.hpp>
#include <common/script_error.hpp>

namespace bxx
{
    std::string join_strings(std::initializer_list<std::string> const& python)
    {
        std::string concat = "";
        for (std::string const& str : python)
        {
            concat += str + "\n";
        }
        return concat;
    }

    void exec(std::initializer_list<std::string> lines)
    {
        std::string text = join_strings(lines);
        int err = get_pointers()->cy_exec(
            get_script_index(),
            const_cast<char*>(text.c_str())
        );
        int h = get_script_index();
        BXX_SCRIPT_ASSERT(!err, python_exec_error, "Failed to execute python: {}", text.c_str());
    }

    int eval_int(std::initializer_list<std::string> lines)
    {
        std::string text = join_strings(lines);
        PyObject* obj = get_pointers()->cy_eval(
            get_script_index(),
            const_cast<char*>(text.c_str())
        );
        BXX_SCRIPT_ASSERT(obj, python_exec_error, "Failed to evaluate python int (script error): {}", text.c_str());
        BXX_SCRIPT_ASSERT(PyLong_Check(obj), python_type_error, "Failed to evaluate python int (incorrect type): {}", text.c_str());
        return PyLong_AsLong(obj);
    }

    float eval_float(std::initializer_list<std::string> lines)
    {
        std::string text = join_strings(lines);
        PyObject* obj = get_pointers()->cy_eval(
            get_script_index(),
            const_cast<char*>(text.c_str())
        );
        BXX_SCRIPT_ASSERT(obj, python_exec_error, "Failed to evaluate python float (script error): {}", text.c_str());
        BXX_SCRIPT_ASSERT(PyFloat_Check(obj), python_type_error, "Failed to evaluate python float (incorrect type): {}", text.c_str());
        return float(PyFloat_AsDouble(obj));
    }

    std::string eval_string(std::initializer_list<std::string> lines)
    {
        std::string text = join_strings(lines);
        PyObject* obj = get_pointers()->cy_eval(
            get_script_index(),
            const_cast<char*>(text.c_str())
        );
        BXX_SCRIPT_ASSERT(obj, python_exec_error, "Failed to evaluate python float (script error): {}", text.c_str());
        BXX_SCRIPT_ASSERT(PyUnicode_Check(obj), python_type_error, "Failed to evaluate python float (incorrect type): {}", text.c_str());
        return _PyUnicode_AsString(obj);
    }
}

