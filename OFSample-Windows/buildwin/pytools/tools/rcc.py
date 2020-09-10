# -*- coding: utf-8 -*-

import codecs
import os
import subprocess

import pathutils

u'''将layout目录下的xml文件，打包成.rcc文件'''

def generate_layout_rcc(rcc_path):
    layout_dir = 'bin/release/layout'
    anchor_dir = 'bin/release'
    qrc_path = os.path.join(anchor_dir, 'layoutxml.qrc')

    QRC_HEAD = '''<!DOCTYPE RCC>
<RCC version="1.0">
  <qresource>
'''
    QRC_TAIL = '''  </qresource>
</RCC>
'''

    with codecs.open(qrc_path, 'w', encoding='utf-8') as f:
        f.write(QRC_HEAD)
        for path in _get_layout_xmls(layout_dir, anchor_dir):
            f.write('    <file>%s</file>\n' % path)
        f.write(QRC_TAIL)
    
    generate_rcc(rcc_path, 'layoutxml.qrc', anchor_dir)


def generate_rcc(rcc_path, qrc_name, anchor_dir):
    rcc_path = os.path.abspath(rcc_path)
    subprocess.call([
            'rcc.exe',
            '-binary',
            '-o',
            rcc_path,
            qrc_name,
        ],
        cwd=anchor_dir,
    )


def _get_layout_xmls(layout_dir, anchor):
    for root, dirs, files in os.walk(layout_dir):
        if '.svn' in dirs:
            dirs.remove('.svn')
        for f in files:
            if f.endswith('.xml'):
                full_path = os.path.join(root, f)
                relative_path = os.path.relpath(full_path, anchor)
                yield pathutils.normalize_path(relative_path)

