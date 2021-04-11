from collections import OrderedDict
from cbutil import Path
from cbutil.util.iterutil import TreeIter
import re

class CMakeCodeMaker:
    def __init__(self):
        #TODO self.stats = []
        self.vars = OrderedDict()

    def set_vars(self, d:dict):
        for k,v in d.items():
            self.set_var(k,v)

    def set_var(self, name, value):
        if type(value) == list:
            self.vars[str(name)] = [str(v) for v in value]
        else:
            self.vars[str(name)] = [str(value)]

    def write(self, fw):
        for var,val in self.vars.items():
            vals = [f'"{self.repr(v)}"' for v in val]
            svals = '\n'.join(vals)
            s = f'set({var}\n{svals})'
            fw.write(s)
            fw.write('\n\n')

    @staticmethod
    def repr(s:str):
        s = repr(s)[1:-1]
        s.replace('$','\\$')
        s.replace(';', '\\;')
        return s

def to_str_list(l):
    return [str(x) for x in l]

def paths_to_vars(ps):
    return [s.replace('\\', '/') for s in to_str_list(ps)]


cmakeMaker = CMakeCodeMaker()

self_path = Path(__file__)

work_path = self_path.prnt.prnt
sdml_path = work_path/'src/include/sdml'
cmake_out_path = fwp = Path(work_path/'build-util/cmake-module/sdml_var.gen.cmake')

sdml_path_tree = TreeIter(sdml_path, Path.get_son_iter)

src_file_paths = []
header_paths = []

for p in sdml_path_tree.breadth_iter():
    if p.ext =='cpp':
        src_file_paths.append(p)
    elif p.ext == 'h':
        header_paths.append(p)


cmakeMaker.set_var('SDML_SRC', paths_to_vars(src_file_paths))
cmakeMaker.set_var('SDML_HEARDER', paths_to_vars(header_paths))
cmakeMaker.set_var('SDML_TEST_NAMES', [re.match(r'([^\.]*)\.cpp', p.name).group(1) for p in (work_path/'src/test').glob('test_*.cpp')])

with cmake_out_path.open('w') as fw:
    cmakeMaker.write(fw)

#xxx ddd
