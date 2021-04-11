from cbutil import Path
from cbutil.util.iterutil import TreeIter

self_path = Path(__file__)
work_path = self_path.prnt.prnt.prnt
sdml_path = work_path/'src/include/sdml'
cmake_out_path = fwp = Path(work_path/'build-util/cmake-module/sdml_var.gen.cmake')
sdml_path_tree = TreeIter(sdml_path, Path.get_son_iter)


source_file_paths = []
header_file_paths = []

for p in sdml_path_tree.breadth_iter():
    if p.ext =='cpp':
        source_file_paths.append(p)
    elif p.ext == 'h':
        header_file_paths.append(p)
