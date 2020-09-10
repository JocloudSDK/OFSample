# -*- coding: utf-8 -*-

import codecs
import collections
import logging
import os
import re
import subprocess
import sys

import buildtool.butil
import buildtool.buildexp

def detect_parallel(devenvlog):
    u'''通过是否存在 >------，就可判定是否并行编译'''

    logger = logging.getLogger(__name__)

    for line in devenvlog:
        index = line.find('------')
        if index != -1:
            # 0: VC 并行项目数为 1，也即串行
            # 非0: VC 并行项目数不为 1，也即并行
            is_parallel = index != 0
            if is_parallel:
                logger.debug('devenv is running parallel')
            else:
                logger.debug('devenv is NOT running parallel')
            return is_parallel

    logger.debug('detect fail')
    return False

class DevenvLog(object):

    def __init__(self, log_file):
        self._log_file = log_file

    def __iter__(self):
        return self._log_file

class Devenv(object):

    def __init__(self, build_prop, **kwarg):
        self._build_prop = build_prop
        self._logger = logging.getLogger(__name__)

        self._solution_filename = kwarg['solution_filename']
        self._devenv_action = kwarg['devenv_action']
        self._devenv_config = kwarg['devenv_config']
        self._working_dir = kwarg['working_dir']
        self._show_warning = kwarg.get('show_warning', True)

    def __call__(self):
        buildtool.butil.mkdir('dist/compile')

        devenv = self._build_prop.share.build_exe

        self._logger.info('%s [%s:%s] [%s] running',
            os.path.split(devenv)[1],
            self._devenv_action,
            self._devenv_config,
            self._solution_filename)

        output_file_path = 'dist/compile/%s-%s-%s.txt' % (
            self._solution_filename,
            self._devenv_action,
            self._devenv_config.replace('|', '-')
        )

        returncode = 0
        with open(output_file_path, 'w') as f:
            returncode = subprocess.call([devenv,
                self._solution_filename,
                '/' + self._devenv_action,
                self._devenv_config],
                stdout=f,
                stderr=subprocess.STDOUT,
                cwd=self._working_dir)

        # 如果指定强制输出警告（即使编译通过也要输出警告）
        # 或编译失败
        if returncode != 0:
            error_warning_lines = self._output_compile_result(output_file_path)
            error_warning = '\n'.join(error_warning_lines)
            print error_warning.encode(self._build_prop.os_encoding)
            self._logger.debug(error_warning)

        if returncode != 0:
            raise buildtool.buildexp.BuildException('%s [%s:%s] [%s] failed, returncode: %d' % (
                os.path.split(devenv)[1],
                self._devenv_action,
                self._devenv_config,
                self._solution_filename,
                returncode))
        else:
            self._logger.info('%s [%s:%s] [%s] successed',
                os.path.split(devenv)[1],
                self._devenv_action,
                self._devenv_config,
                self._solution_filename)

    def _output_compile_result(self, output_file_path):
        if self._build_prop.share.devenv_parallel is None:
            self._logger.debug('detect by %s', output_file_path)
            with codecs.open(output_file_path, 'r', 'GBK') as f:
                devenvlog = DevenvLog(f)
                self._build_prop.share.devenv_parallel = detect_parallel(devenvlog)

        # 并行编译时，使用">"前的数字作为项目的标识，输出的第一行可以和错误信息统一在一起
        pattern = '''>------
            |:\ general\ error
            |:\ fatal\ error
            |:\ error\ C
            |:\ error\ RC
            |:\ error\ LNK
            |:\ error\ PRJ
            '''

        if self._show_warning:
            pattern += '|:\ warning\ C'

        self._pattern_keyword = re.compile(pattern, re.VERBOSE)

        with codecs.open(output_file_path, 'r', 'GBK') as f:
            devenvlog = DevenvLog(f)
            if self._build_prop.share.devenv_parallel:
                return filter_devenvlog_parallel(devenvlog, self._pattern_keyword)
            else:
                return filter_devenvlog_serial(devenvlog, self._pattern_keyword)

def filter_devenvlog_parallel(devenvlog, pattern_keyword):
    result = {}
    for line in devenvlog:
        index = line.find('>')
        # 只处理带有 > 符号的行
        if index == -1:
            continue

        key = line[:index]

        # 确保entry一定存在
        if not result.has_key(key):
            result[key] = []

        # 属于同一个项目内容，归在同一个entry
        if pattern_keyword.search(line) != None:
            result[key].append(line.strip())

    output = []
    # 按照 k 的数字大小，升序输出
    for k, v in sorted(result.items(), key=lambda x: int(x[0])):
        # 只有title，或者连title都不存在的项目，不输出
        if len(v) > 1:
            output += v
    return output

def filter_devenvlog_serial(devenvlog, pattern_keyword):
    output = []
    title = None
    content = []
    for line in devenvlog:
        if line.startswith('------'):
            _flush_output_serial(output, title, content)
            title = line.strip()
            content = []
        if pattern_keyword.search(line) != None:
            content.append(line.strip())
    _flush_output_serial(output, title, content)
    return output

def _flush_output_serial(output, title, content):
    if title is not None and len(content) != 0:
        output.append(title)
        output += content

