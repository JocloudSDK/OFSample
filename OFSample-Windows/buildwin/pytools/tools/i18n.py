# -*- coding: utf-8 -*-

import os
import subprocess

import layout_i18n
import osutils

class I18n(object):

    def __init__(self):
        self.project_name = 'yymixer'
        self._i18n_dir = None

    def do(self):
        self._i18n_dir = os.path.join('bin', 'i18n')
        osutils.mkdir(self._i18n_dir)

        self._show_root_menu()

    def _show_root_menu(self):
        print '''  0. Quit
  1. Update ts files
  2. Build qm file
'''
        n = raw_input('Make your choice : ')
        if n == '0':
            return

        if n == '1':
            print 'update ts file start ...'
            self._update_ts_files()
            print ('update ts file finished, result is in directory "%s"' %
                   'i18n')
            return

        if n == '2':
            print 'build .qm file start ...'
            build_qm_file()
            print 'build .qm file finished'
            return

        print 'unknown choice [%s]' % n

    def _update_ts_files(self):
        self._update_ts_from_layout()
        self._update_ts_from_pro()

    def _update_ts_from_layout(self):
        u'''
        搜集layout xml中需要翻译的字符串，生成临时的stub.h文件
        使用lupdate生成对应locale下的ts文件
        '''

        layout_dirs = self._get_layout_dirs()
        stub_h_path = os.path.join(self._i18n_dir, 'stub.h')

        osutils.delete(stub_h_path)
        layout_i18n.generate_stub_xml(layout_dirs, stub_h_path)

        if not os.path.isfile(stub_h_path):
            return

        ts_list_file_path = os.path.join(self._i18n_dir, 'layout_ts.lst')
        with open(ts_list_file_path, 'w') as f:
            for locale in _enum_all_locale():
                f.write('%s\n' %
                        os.path.join('i18n', locale, 'layout.ts'))

        p = subprocess.Popen(
            [
                'lupdate',
                stub_h_path,
                '-no-obsolete',
                '-no-ui-lines',
                '-locations',
                'none',
                '-ts',
                '@%s' % ts_list_file_path,
            ],
            stdout=open(os.devnull, "w"),
            stderr=subprocess.STDOUT,
        )
        p.communicate()
        if p.returncode != 0:
            print 'invoke "lupdate" failed, return code [%d]' % p.returncode

    def _get_layout_dirs(self):
        for root, dirs, files in os.walk('bin'):
            if os.path.basename(root) == 'layout':
                yield root

    def _update_ts_from_pro(self):
        p = subprocess.Popen(
            [
                'lupdate',
                os.path.join('packages', '%s.pro' % self.project_name),
                '-recursive',
                '-no-obsolete',
                '-no-ui-lines',
                '-locations',
                'none',
            ],
            stdout=open(os.devnull, "w"),
            stderr=subprocess.STDOUT,
        )
        p.communicate()
        if p.returncode != 0:
            print 'invoke "lupdate" failed, return code [%d]' % p.returncode


def build_qm_file():
    shadow_lang_dir = os.path.join('bin', 'release', 'lang')
    if os.path.isdir(shadow_lang_dir):
        for f in os.listdir(shadow_lang_dir):
            path = os.path.join(shadow_lang_dir, f)
            if os.path.isdir(path) or path.endswith('.qm'):
                 osutils.delete(path)
    else:
        osutils.delete(shadow_lang_dir)
        osutils.mkdir(shadow_lang_dir)

    all_locales = _enum_all_locale()
    for locale in all_locales:
        _build_qm_for_one_locale(locale, shadow_lang_dir)


def _build_qm_for_one_locale(locale, target_dir):
    ts_path_list = []
    for root, dirs, files in os.walk(os.path.join('i18n', locale)):
        for f in files:
            if f.endswith('.ts'):
                ts_path_list.append(os.path.join(root, f))
    if len(ts_path_list) == 0:
        return

    qm_path = os.path.join(target_dir, '%s.qm' % locale)

    command_list = ['lrelease', '-compress']
    command_list += ts_path_list
    command_list += ['-qm', qm_path]

    p = subprocess.Popen(
        command_list,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        print stderr
        print 'invoke "lrelease" failed, return code [%d]' % p.returncode


def _enum_all_locale():
    return [d for d in os.listdir('i18n')
            if os.path.isdir(os.path.join('i18n', d))]

if __name__ == "__main__":
    i18n = I18n()
    i18n.do()