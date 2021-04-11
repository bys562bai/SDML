from cbutil import Path
import runpy

self_path = Path(__file__)
work_path = self_path.prnt.prnt
code_maker_path = work_path/'code-maker'

for f in code_maker_path.file_son_iter:
    if f.ext == 'py':
        runpy.run_path(f)


