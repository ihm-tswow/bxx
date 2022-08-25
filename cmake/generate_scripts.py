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
                        all_tests.append(m.groups()[0])

                    m = re.search('BXX_OPERATOR\( *(.+?) *\)',line)
                    if m:
                        all_operators.append(m.groups()[0])

    generated_path = os.path.join(script_root,'calls.generated.cpp')
    generated = '\n'.join([
        '#include "tests.hpp"',
        '#include "builders/operator_builder.hpp"',
        '#include "api.hpp"',
        '',
        '// Generated test calls',
        '',
    ])

    if len(all_tests) > 0:
        generated += '\n'.join([
            '\n'.join(['void {0}();'.format(test) for test in all_tests]),
            'static bxx::test_case cases[] = {',
            ',\n'.join(['    {{"{0}",{0}}}'.format(test) for test in all_tests]),
            '};',
            '',
            'static bxx::test_collection col = {{ cases, {{ {0} }}}};'.format(len(all_tests)),
            '',
            'BXX_API bxx::test_collection* __register__tests() {',
            '    return &col;',
            '}',
            '',
            '',
        ])
    else:
        generated += '\n'.join([
            'BXX_API bxx::test_collection* __register__tests() {',
            '    return nullptr;',
            '}',
            '',
            '',
        ])

    generated += '\n'.join([
        '// Generated operator calls',
        '\n'.join(['void __register_{0}(bxx::operator_builder &);'.format(operator) for operator in all_operators]),
        'void __register_operators() {',
        '\n'.join(['    bxx::operator_builder _op_{0}("bxx.{0}",true);'.format(operator) for operator in all_operators]),
        '\n'.join(['    __register_{0}(_op_{0});'.format(operator) for operator in all_operators]),
        '}',
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

generate_dir('bxx/tests')