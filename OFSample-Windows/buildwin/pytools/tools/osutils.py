# -*- coding: utf-8 -*-

import distutils.dir_util
import re
import glob
import logging
import pprint
import os, sys
import shutil

if sys.version_info < (3, 0):
    import pathutils
else:
    from . import pathutils

def mkdir(path):
    u'''
    新建目录dir
    如果父目录还未存在，则自动建立
    '''
    dir_created = distutils.dir_util.mkpath(path)
    if len(dir_created) != 0:
        logger = logging.getLogger(__name__)
        logger.info('mkdir [%s]' % path)


def delete(path):
    u'''
    删除目录及其子目录和文件，或者删除文件
    如果目录或文件不存在，则忽略
    '''
    if os.path.isdir(path):
        distutils.dir_util.remove_tree(path)
        logger = logging.getLogger(__name__)
        logger.info('delete dir [%s]' % path)
    elif os.path.isfile(path):
        os.remove(path)
        logger = logging.getLogger(__name__)
        logger.info('delete file [%s]' % path)

def get_filelist(todir, filesets):
    if todir == '.':
        todir = ''
    elif not todir.endswith('/'):
        todir += '/'

    fileset_objects = [_FileSet(**f) for f in filesets]

    for fileset in fileset_objects:
        for path in fileset.get_filelist():
            src = os.path.join(fileset.get_dir(), path)
            if not os.path.isdir(src):
                yield todir + path

def copy(todir, filesets=[], from_file=None):
    u'''
    两种用法：
    1、复制单个文件到指定目录下
    2、复制由filesets描述的若干个文件到指定目录下

    将filesets所描述的文件，复制到todir目录下
    使用方法：
    copy('abc', [
            {
                dir : 'foo',
                include : ['*.py', '*.cpp']
                exclude : ['*.moc',]
            }
        ])
    filesets是fileset的list
    fileset是一个dict，包括dir、include和exclude三种属性
    dir是指fileset的根目录，必须
    include和exclude都是一个pattern的list
    include为可选，如果不指定或者为空，则表示包含全部文件
    exclude为可选，不管是否指定，默认exclude的路径都不会带上，它们是**/.svn/**，**/.git/**，**/.hg/**
    pattern是一个表示路径的字符串，可以使用*作为通配符，匹配除了/之外的文本
    如果需要匹配多层目录，则可以使用**，如：
    foo/**/bar，匹配foo/bar，foo/abc/bar，foo/abc/def/bar

    请参考ant中fileset的使用方法：http://ant.apache.org/manual/Types/fileset.html
    '''

    if from_file != None:
        _copy_single_file(todir, from_file)
    else:
        _copy_filesets(todir, filesets)

def _copy_single_file(todir, from_file):
    u'''
    将单个文件复制到dst目录下，或复制为名为dst的文件
    '''
    logger = logging.getLogger(__name__)
    shutil.copy(from_file, todir)
    if os.path.isdir(todir):
        logger.info('copy file [%s] to dir [%s]' % (from_file, todir))

def _copy_filesets(todir, filesets):
    todir = pathutils.expand_env_path(todir)

    fileset_objects = [_FileSet(**f) for f in filesets]

    for fileset in fileset_objects:
        for path in fileset.get_filelist():
            src = os.path.join(fileset.get_dir(), path)
            dst = os.path.join(todir, path)
            # dst如果是目录，则必须以/结尾
            dest_dir = os.path.dirname(dst)
            if not os.path.isdir(dest_dir):
                distutils.dir_util.mkpath(dest_dir)
            if not os.path.isdir(src):
                shutil.copy(src, dst)

class _FileSet(object):
    u'''
    通过指定dir，以及include和exclude的规则，描述文件集合
    '''

    _default_includes = ['**']
    _default_excludes = [
        '**/.svn/**',
        '**/.git/**',
        '**/.hg/**',
    ]

    def __init__(self, **kwarg):
        u'''
        dir
        include
        exclude
        '''

        self._logger = logging.getLogger(__name__)

        self._dir = pathutils.normalize_path(kwarg['dir'])

        includes = kwarg.get('include', self._default_includes)
        self._include_pattern = _PathPattern(includes)

        excludes = self._default_excludes + kwarg.get('exclude', [])
        self._exclude_pattern = _PathPattern(excludes)

        self._logger.debug('dir = [%s], include = [%s], exclude = [%s]',
            self._dir,
            self._include_pattern._pattern.pattern,
            self._exclude_pattern._pattern.pattern
        )

    def get_dir(self):
        return pathutils.expand_env_path(self._dir)

    def get_filelist(self):
        base_dir = self.get_dir()
        for root, dirs, files in os.walk(base_dir):
            relpath = os.path.relpath(root, base_dir)
            self._logger.debug('walk root = [%s], relpath = [%s]', root, relpath)
            for d in dirs:
                path = os.path.join(relpath, d) + '/'
                result = self._filter(path)
                if result != None:
                    yield result

            for f in files:
                path = os.path.join(relpath, f)
                result = self._filter(path)
                if result != None:
                    yield result

    def _filter(self, path):
        path = pathutils.normalize_path(path)
        return self._match(path) and path or None

    def _match(self, path):
        included = self._include_pattern.match(path)
        excluded = False
        if included:
            excluded = self._exclude_pattern.match(path)

        self._logger.debug('path = [%s], include = [%s], exclude = [%s]', path, included, excluded)
        return included and not excluded

class _PathPattern(object):
    u'''
    将path list转化为可以用来匹配路径的pattern，用于判断path是否符合条件
    它可以是include pattern，也可以是exclude pattern
    '''

    def __init__(self, path_list):
        self._pattern = self._build_pattern(path_list)

    def _build_pattern(self, path_list):
        progs = []
        for path in path_list:
            progs.append(_build_pattern_string(path))

        return re.compile('|'.join(progs))

    def match(self, path):
        return self._pattern.match(path) is not None

def _build_pattern_string(path):
    u'''
    对path的内容进行必要的正规化处理，如转义点号(.)，转换反斜杠(\)
    再翻译成正则表达式
    '''
    return '^%s$' % _translate_path(path)

def _translate_path(path):
    u'''
    把一个path翻译成正则表达式，不包括表示开头的^和表示结尾的$
    '''
    if len(path) == 0:
        return path

    path = path.replace('\\', '/')

    # 路径中的点(.)需要转义成\.后，才能用在正则表达式
    path = path.replace('.', '\\.')

    if path.endswith('/'):
        path += '**'

    # /**/ => 匹配任意层次的目录，包括0层。
    # **出现在末尾 => 匹配目录下的所有子目录和文件，递归
    # * 匹配除了/之外的任意文本

    # 以/进行分割，分割后的每个token，要么是**，要么是普通的token
    tokens = path.split('/')
    translated_tokens = []
    # 末尾的token需要特殊的处理
    for token in tokens[:-1]:
        translated_tokens.append(_translate_token(token))
    translated_tokens.append(_translate_tail_token(tokens[-1]))
    return ''.join(translated_tokens)

def _translate_token(token):
    if token == '**':
        return _translate_double_star(token)
    else:
        return _translate_single_star(token)

def _translate_double_star(token):
    u'''
    匹配0到多层目录
    '''
    return '([^/]*/)*'

def _translate_single_star(token):
    return token.replace('*', '[^/]*') + '/'

def _translate_tail_token(tail_token):
    u'''
    翻译末尾的token，对末尾的处理较特殊。
    '''
    if tail_token == '**':
        return '.*'
    else:
        # 末尾token，允许匹配不以/结尾的路径，所以，加入'?'
        return _translate_single_star(tail_token) + '?'

