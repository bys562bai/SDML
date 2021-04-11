from cbutil import Path
from pathinfo import *


for p in source_file_paths + header_file_paths:
    content = None
    with p.open('r') as fr:
        content = fr.read()
    if content != None and '\r' in content:
        print(p)
        with p.open('w', encoding = 'UTF-8') as fw:
            fw.write(content)
        