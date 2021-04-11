from cbutil import Path
from pathinfo import *

license = r'''/*
Copyright 2020 happyxianyu.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/'''


for p in source_file_paths + header_file_paths:
    with p.open('r') as fr:
        content = fr.read()
    content_w = license + '\n'*2 + content
    if license not in content:
        print(p)
        with p.open('w', encoding = 'UTF-8') as fw:
            fw.write(content_w)