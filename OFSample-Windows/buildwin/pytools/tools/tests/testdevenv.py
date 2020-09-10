# -*- coding: utf-8 -*-

import re
import unittest

from nose.tools import eq_
from nose.tools import ok_

from buildtool.tools.devenv import detect_parallel
from buildtool.tools.devenv import filter_devenvlog_serial
from buildtool.tools.devenv import filter_devenvlog_parallel

class TestDetectParallel(unittest.TestCase):

    def test_parallel(self):
        devenvlog = MockDevenvLog(u'''
Microsoft (R) Visual Studio 9.0.30729.1 版。
版权所有(C) Microsoft Corp。保留所有权利。

1>------ 已启动生成: 项目: dwutility, 配置: Release Win32 ------
''')

        actual = detect_parallel(devenvlog)
        eq_(True, actual, 'running parallel')

    def test_serial(self):
        devenvlog = MockDevenvLog(u'''
Microsoft (R) Visual Studio 9.0.30729.1 版。
版权所有(C) Microsoft Corp。保留所有权利。

------ 已启动生成: 项目: dwutility, 配置: Release Win32 ------
''')

        actual = detect_parallel(devenvlog)
        eq_(False, actual, 'running serial')

    def test_detect_fail(self):
        devenvlog = MockDevenvLog(u'''
Microsoft (R) Visual Studio 9.0.30729.1 版。
版权所有(C) Microsoft Corp。保留所有权利。
''')

        actual = detect_parallel(devenvlog)
        eq_(False, actual, 'detect_fail, return False as default')

class MockDevenvLog(object):

    def __init__(self, log_lines):
        self._log_lines = log_lines.splitlines()

    def __iter__(self):
        return iter(self._log_lines)

_PATTERN_KEYWORD = re.compile(r''':\ error\ C
    |>------
    ''', re.VERBOSE)

class TestFilterDevenvlogSerial(unittest.TestCase):

    def test_ok(self):
        devenvlog = MockDevenvLog(u'''
------ 已启动生成: 项目: dwutility, 配置: Release Win32 ------
winwmi.cpp(123) : error C2440: 'initializing' : cannot convert from 'SmileData' to 'FACE_ATTR'
dwutility - 0 个错误，0 个警告
========== 生成: 成功 1 个，失败 1 个，最新 1 个，跳过 0 个 ==========
''')

        expect = u'''------ 已启动生成: 项目: dwutility, 配置: Release Win32 ------
winwmi.cpp(123) : error C2440: 'initializing' : cannot convert from 'SmileData' to 'FACE_ATTR'
'''.splitlines(False)

        actual = filter_devenvlog_serial(devenvlog, _PATTERN_KEYWORD)
        self.assertListEqual(expect, actual)

    def test_multi_projects(self):
        devenvlog = MockDevenvLog(u'''
------ 已启动生成: 项目: dwutility, 配置: Release Win32 ------
winwmi.cpp(123) : error C2440: 'initializing' : cannot convert from 'SmileData' to 'FACE_ATTR'
dwutility - 0 个错误，0 个警告
------ 已启动生成: 项目: duifw, 配置: Release Win32 ------
winwmi.cpp
duifw - 0 个错误，0 个警告
========== 生成: 成功 1 个，失败 1 个，最新 1 个，跳过 0 个 ==========
''')

        expect = u'''------ 已启动生成: 项目: dwutility, 配置: Release Win32 ------
winwmi.cpp(123) : error C2440: 'initializing' : cannot convert from 'SmileData' to 'FACE_ATTR'
'''.splitlines(False)

        actual = filter_devenvlog_serial(devenvlog, _PATTERN_KEYWORD)
        self.assertListEqual(expect, actual)

class TestFilterDevenvlogParallel(unittest.TestCase):

    def test_ok(self):
        devenvlog = MockDevenvLog(u'''
1>------ 已启动生成: 项目: dwutility, 配置: Release Win32 ------
1>winwmi.cpp(123) : error C2440: 'initializing' : cannot convert from 'SmileData' to 'FACE_ATTR'
1>dwutility - 0 个错误，0 个警告
========== 生成: 成功 1 个，失败 1 个，最新 1 个，跳过 0 个 ==========
''')

        expect = u'''1>------ 已启动生成: 项目: dwutility, 配置: Release Win32 ------
1>winwmi.cpp(123) : error C2440: 'initializing' : cannot convert from 'SmileData' to 'FACE_ATTR'
'''.splitlines(False)

        actual = filter_devenvlog_parallel(devenvlog, _PATTERN_KEYWORD)
        self.assertListEqual(expect, actual)

    def test_multi_projects(self):
        devenvlog = MockDevenvLog(u'''
1>------ 已启动生成: 项目: dwutility, 配置: Release Win32 ------
1>winwmi.cpp(123) : error C2440: 'initializing' : cannot convert from 'SmileData' to 'FACE_ATTR'
1>dwutility - 0 个错误，0 个警告
2>------ 已启动生成: 项目: duifw, 配置: Release Win32 ------
2>winwmi.cpp
2>duifw - 0 个错误，0 个警告
========== 生成: 成功 1 个，失败 1 个，最新 1 个，跳过 0 个 ==========
''')

        expect = u'''1>------ 已启动生成: 项目: dwutility, 配置: Release Win32 ------
1>winwmi.cpp(123) : error C2440: 'initializing' : cannot convert from 'SmileData' to 'FACE_ATTR'
'''.splitlines(False)

        actual = filter_devenvlog_parallel(devenvlog, _PATTERN_KEYWORD)
        self.assertListEqual(expect, actual)

    def test_multi_projects_unorder(self):
        devenvlog = MockDevenvLog(u'''
1>------ 已启动生成: 项目: dwutility, 配置: Release Win32 ------
2>------ 已启动生成: 项目: duifw, 配置: Release Win32 ------
2>winwmi.cpp(123) : error C2440: 'initializing' : cannot convert from 'SmileData' to 'FACE_ATTR'
1>dwutility - 0 个错误，0 个警告
1>winwmi.cpp
2>duifw - 0 个错误，0 个警告
========== 生成: 成功 1 个，失败 1 个，最新 1 个，跳过 0 个 ==========
''')

        expect = u'''2>------ 已启动生成: 项目: duifw, 配置: Release Win32 ------
2>winwmi.cpp(123) : error C2440: 'initializing' : cannot convert from 'SmileData' to 'FACE_ATTR'
'''.splitlines(False)

        actual = filter_devenvlog_parallel(devenvlog, _PATTERN_KEYWORD)
        self.assertListEqual(expect, actual)

