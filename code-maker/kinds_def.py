from cbutil import Path

self_path = Path(__file__)
work_path = self_path.prnt.prnt
sdml_path = work_path/'src/include/sdml'
dst_path = sdml_path/'core/kind_def.gen.h'


kinds = [
    'UNKNOWN',
    'SPACE',
    'COMMENT',
    'NUMBER',
    'STRING',
    'IDENTIFIER',
    'KEYWORD',
    'OPERATOR',

    'UNEXPECTED'
]



def make_case_statement(kind):
    return \
    f'''       case sdml::{kind}:
                return "{kind}";'''


content = \
'''#pragma once
#include <string>

namespace sdml
{
    enum SDML_NODE_KIND
    {
        ''' + ',\n\t\t'.join(f'KIND_{kind}' for kind in kinds) + \
'''
    };
''' + \
'''

    inline const std::string& to_string(SDML_NODE_KIND kind) {
''' + '\n'.join(f'       static const std::string {kind}_str = "{kind}";' for kind in kinds) + \
'''

        switch (kind)
        {
'''+ '\n'.join(
f'''        case sdml::KIND_{kind}:
            return {kind}_str;''' for kind in kinds
) + '''
        }

        return "";
    }
}
'''

with dst_path.open('w') as fw:
    fw.write(content)