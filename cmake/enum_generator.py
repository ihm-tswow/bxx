from enum import Enum
import importlib.util
import sys
import os
import inspect

class enum_output:
    def __init__(self,path):
        self.text = ""
        self.path = path

    def append_line(self,line):
        self.text += line + '\n'

    def write_to_file(self):
        if os.path.exists(self.path):
            with open(self.path,'r') as file:
                old_text = file.read()
                if(self.text == old_text):
                    return

        with open(self.path,'w') as file:
            file.write(self.text)

def get_value(value):
    if value is tuple:
        return value[0]
    else:
        return value

generated_enums_ipp = enum_output(os.path.join(os.path.dirname(os.path.dirname(__file__)),'bxx','enums.generated.ipp'))
generated_enums_cpp = enum_output(os.path.join(os.path.dirname(os.path.dirname(__file__)),'bxx','enums.generated.cpp'))

generated_enums_ipp.append_line('#pragma once')
generated_enums_ipp.append_line('#include <bxx/enums.hpp>')

generated_enums_cpp.append_line('#include <bxx/enums.hpp>')
generated_enums_cpp.append_line('#include <map>')
generated_enums_cpp.append_line('')

def get_value_string(value):
    if type(value) is tuple:
        return get_value_string(value[0])
    if type(value) is int:
        return value
    if type(value) is str:
        return "'{0}'".format(value)

enums_path = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))),'enums')
for module in os.listdir(enums_path):
    if not module.endswith('.py'):
        continue

    module_name = 'enum.' + module[:-3]
    module_path = os.path.join(enums_path,module)
    spec = importlib.util.spec_from_file_location(module_name,os.path.join(enums_path,module))
    lib = importlib.util.module_from_spec(spec)
    sys.modules[module_name] = lib
    spec.loader.exec_module(lib)

    for name,obj in inspect.getmembers(lib):
        if hasattr(obj,'__bases__') and len(obj.__bases__) == 1 and obj.__bases__[0] == Enum:
            generated_enums_ipp.append_line('enum class {0} {{'.format(name))

            for entry in obj:
                generated_enums_ipp.append_line('    {0} = {1},'.format(entry.name,get_value_string(entry.value)))
            generated_enums_ipp.append_line('};')
            generated_enums_ipp.append_line('')

            generated_enums_ipp.append_line('namespace bxx::details {{ std::string get_enum_name_{0}({0} value); }}'.format(name))
            generated_enums_ipp.append_line('namespace bxx::details {{ {0} get_enum_value_{0}(std::string const& name); }}'.format(name))
            generated_enums_ipp.append_line('namespace bxx::details {{ std::vector<std::pair<{0},std::string>> get_enum_entries_{0}(); }}'.format(name))
            generated_enums_ipp.append_line('namespace bxx::details {{ enum_meta get_enum_meta_{0}({0}); }}'.format(name))
            generated_enums_ipp.append_line('namespace bxx::enums {{ template <> inline std::string get_enum_name<{0}>({0} value) {{ return bxx::details::get_enum_name_{0}(value); }} }}'.format(name))
            generated_enums_ipp.append_line('namespace bxx::enums {{ template <> inline {0} get_enum_value<{0}>(std::string const& name) {{ return bxx::details::get_enum_value_{0}(name); }} }}'.format(name))
            generated_enums_ipp.append_line('namespace bxx::enums {{ template <> inline std::vector<std::pair<{0},std::string>> get_enum_entries<{0}>() {{ return bxx::details::get_enum_entries_{0}(); }} }}'.format(name))
            generated_enums_ipp.append_line('namespace bxx::enums {{ template <> inline enum_meta get_enum_meta<{0}>({0} value) {{ return bxx::details::get_enum_meta_{0}(value); }} }}'.format(name))

            generated_enums_cpp.append_line('namespace bxx::details {')

            generated_enums_cpp.append_line('    std::string get_enum_name_{0}({0} value) {{'.format(name))
            generated_enums_cpp.append_line('        switch(value) {')
            for entry in obj:
                generated_enums_cpp.append_line('            case {0}::{1}: return "{1}";'.format(name,entry.name))
            generated_enums_cpp.append_line('        }')
            generated_enums_cpp.append_line('    }')

            generated_enums_cpp.append_line('')

            generated_enums_cpp.append_line('    {0} get_enum_value_{0}(std::string const& name) {{'.format(name))
            generated_enums_cpp.append_line('        static std::map<std::string,{0}> values = {{'.format(name))
            for entry in obj:
                generated_enums_cpp.append_line('            {{"{1}",{0}::{1}}},'.format(name,entry.name))
            generated_enums_cpp.append_line('        };')
            generated_enums_cpp.append_line('        auto itr = values.find(name);')
            generated_enums_cpp.append_line('        BXX_SCRIPT_ASSERT(itr != values.end(), enum_cast_error, "Invalid enum key: {{}}", name.c_str());')
            generated_enums_cpp.append_line('        return itr->second;')
            generated_enums_cpp.append_line('    }')

            generated_enums_cpp.append_line('    std::vector<std::pair<{0},std::string>> get_enum_entries_{0}() {{'.format(name))
            generated_enums_cpp.append_line('        static std::vector<std::pair<{0},std::string>> entries = {{'.format(name))
            for entry in obj:
                generated_enums_cpp.append_line('            std::make_pair({0}::{1},"{1}"),'.format(name,entry.name))
            generated_enums_cpp.append_line('        };')
            generated_enums_cpp.append_line('        return entries;')
            generated_enums_cpp.append_line('    }')

            generated_enums_cpp.append_line('    bxx::enum_meta get_enum_meta_{0}({0} value) {{'.format(name))
            generated_enums_cpp.append_line('        switch(value) {')
            for entry in obj:
                meta = None
                if type(entry.value) is tuple:
                    meta = (entry.value[1],entry.value[2],entry.value[3])
                else:
                    meta = (entry.name,"","")
                generated_enums_cpp.append_line('            case {0}::{1}: return bxx::enum_meta{{"{2}","{3}","{4}"}};'.format(name,entry.name,meta[0],meta[1],meta[2]))
            generated_enums_cpp.append_line('        }')
            generated_enums_cpp.append_line('    }')
            generated_enums_cpp.append_line('}')



generated_enums_ipp.write_to_file()
generated_enums_cpp.write_to_file()