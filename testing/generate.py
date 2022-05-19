import os
import re


def generate_dir(script_root):
    all_tests = []
    for cur_root,subdirs,filepaths in os.walk(script_root):
        for filepath in filepaths:
            filepath = os.path.join(cur_root,filepath)
            if not filepath.endswith('.cpp'):
                continue

            with open(filepath,'r', encoding = 'utf-8') as file:
                for line in file.readlines():
                    m = re.search('TEST\( *(.+?) *\)',line)
                    if m:
                        all_tests.append(m[1])

    generated_path = os.path.join(script_root,'tests.generated.cpp')
    if len(all_tests) > 0:
        generated = '\n'.join([
            f'#include "tests.hpp"',
            '\n'.join([f'void {test}();' for test in all_tests]),
            f'static bxx::test_case cases[] = {{',
            ',\n'.join([f'    {{"{test}",{test}}}' for test in all_tests]),
            f'}};',
            f'static bxx::test_collection col = {{ cases, {len(all_tests)} }};',
            f'extern "C" {{',
            f'    __declspec(dllexport) bxx::test_collection* __register__tests() {{',
            f'        return &col;',
            f'    }}',
            f'}}'
        ])
    else:
        generated = '\n'.join([
            f'#include "tests.hpp"',
            f'extern "C" {{',
            f'    __declspec(dllexport) bxx::test_collection* __register__tests() {{',
            f'        return nullptr;',
            f'    }}',
            f'}}'
        ])
    with open(os.path.join(script_root,'tests.generated.cpp'),'w') as generated_file:
        generated_file.write(generated)

for directory in os.listdir('scripts'):
    generate_dir(os.path.join('scripts',directory))