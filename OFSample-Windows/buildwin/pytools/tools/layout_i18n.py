# -*- coding: utf-8 -*-

import codecs
import os
import re
import StringIO
import sys
import xml.etree.ElementTree

# 需要被翻译的属性
_i18n_attrs = [
    'text', 'adjustText', 'plainText', 'icontext',
    'toolTip', 'tip', 'icontooltip',
    'title', 'windowTitle',
    'html',
]


def generate_stub_xml(src_dirs, stub_h_path):
    untranslates = set()

    for src_dir in src_dirs:
        for xml_file in _get_xml_files(src_dir):
            _extract_i18n_key(xml_file, untranslates)

    if len(untranslates) == 0:
        # 不输出 stub_h_path，表示不需要翻译
        return

    with codecs.open(stub_h_path, 'w', encoding='gbk') as f:
        content = StringIO.StringIO()
        for text in sorted(untranslates):
            content.write('    str = QObject::tr("%s");\n' % text)

        f.write('''#pragma once

#include <QObject>

void _dummyfunc()
{
    QString str;
%s
}
''' % content.getvalue())


def _get_xml_files(dir_):
    for root, dirs, files in os.walk(dir_):
        for f in files:
            if f.endswith('.xml'):
                yield os.path.join(root, f)


def _extract_i18n_key(xml_path, untranslates):
    u'''
    从xml文件中，提取所有需要被翻译的属性值
    '''
    try:
        tree = xml.etree.ElementTree.parse(xml_path)
    except (xml.parsers.expat.ExpatError,
            xml.etree.ElementTree.ParseError) as e:
        print '''file [%s] : %s
  file encoding must be ascii or utf-8
  <, >, &, \' must be escape''' % (xml_path, e)
        sys.exit(1)

    for element in tree.iter():
        for attr_name, attr_value in element.attrib.iteritems():
            if attr_name in _i18n_attrs:
                if _contain_english_char(attr_value):
                    untranslates.add(attr_value)


def _contain_english_char(text):
    return re.compile(r'[A-Za-z]').search(text) is not None
