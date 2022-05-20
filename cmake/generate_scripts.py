import os
import re

def generate_dir(script_root):
    all_tests = []
    all_operators = []

    for cur_root,subdirs,filepaths in os.walk(script_root):
        for filepath in filepaths:
            filepath = os.path.join(cur_root,filepath)
            if not filepath.endswith('.cpp'):
                continue

            with open(filepath,'r', encoding = 'utf-8') as file:
                for line in file.readlines():
                    m = re.search('BXX_TEST\( *(.+?) *\)',line)
                    if m:
                        all_tests.append(m[1])

                    m = re.search('BXX_OPERATOR\( *(.+?) *\)',line)
                    if m:
                        all_operators.append(m[1])

    generated_path = os.path.join(script_root,'calls.generated.cpp')
    generated = '\n'.join([
        f'#include "tests.hpp"',
        f'#include "operator.hpp"',
        f'#include "api.hpp"',
        f'',
        f'// Generated test calls',
        f'',
    ])

    if len(all_tests) > 0:
        generated += '\n'.join([
            f'\n'.join([f'void __test_{test}();' for test in all_tests]),
            f'static bxx::test_case cases[] = {{',
            f',\n'.join([f'    {{"{test}",__test_{test}}}' for test in all_tests]),
            f'}};',
            f'',
            f'static bxx::test_collection col = {{ cases, {len(all_tests)} }};',
            f'',
            f'BXX_API bxx::test_collection* __register__tests() {{',
            f'    return &col;',
            f'}}',
            f'',
            f'',
        ])
    else:
        generated += '\n'.join([
            f'BXX_API bxx::test_collection* __register__tests() {{',
            f'    return nullptr;',
            f'}}',
            f'',
            f'',
        ])

    generated += '\n'.join([
        f'// Generated operator calls',
        f'\n'.join([f'void __register_{operator}(bxx::operator_builder &);' for operator in all_operators]),
        f'void __register_operators() {{',
        f'\n'.join([f'    bxx::operator_builder _op_{operator} = bxx::operators::create("bxx.{operator}");' for operator in all_operators]),
        f'\n'.join([f'    __register_{operator}(_op_{operator});' for operator in all_operators]),
        f'}}',
    ])

    if os.path.exists(generated_path):
        with open(generated_path,'r') as generated_file:
            read = generated_file.read()
            if read == generated:
                return

    with open(generated_path,'w') as generated_file:
        generated_file.write(generated)

for directory in os.listdir('scripts'):
    generate_dir(os.path.join('scripts',directory))