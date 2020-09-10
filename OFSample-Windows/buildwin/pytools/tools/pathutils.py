# -*- coding: utf-8 -*-

import os
import sys

def normalize_path(path):
    u'''
    确保路径无冗余，无开头的'./'，目录分隔符统一为'/'
    我们需要使用 / 的路径，而python在windows下的实现却是正规化为全部使用 \\
    '''

    # 调用os.path.normpath后，末尾的/会丢失，因此还需要再补上一次
    is_dir = path.endswith('/')
    return os.path.normpath(path).replace('\\', '/') + (is_dir and '/' or '')

def expand_env_path(path):
    u'''
    展开path中的环境变量。环境变量必须按照WINDOWS的格式，如 %PATH%
    整个目录名必须都由环境变量构成，不支持：%PATH%123/456 这种形式，只能%PATH%/456


    os.path.expandvars的虽然实现更加完整
    但是os.environ的返回值仍旧保持操作系统自身的编码，会导致unicode异常
    '''
    if '%' not in path:
        return path

    path = normalize_path(path)
    result = [
        _convert_expand_token(token)
        for token in path.split('/')
    ]

    return '/'.join(result)

def _convert_expand_token(token):
    if token == '%%':
        return '%'

    if len(token) > 3 and token.startswith('%') and token.endswith('%'):
        env_key = token[1:-1]
        if os.environ.has_key(env_key):
            return os.environ[env_key].decode(sys.getfilesystemencoding())

    return token

