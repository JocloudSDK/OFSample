# -*- coding: utf-8 -*-

import unittest

from nose.tools import eq_
from nose.tools import ok_

from buildtool.tools.osutils import _build_pattern_string
from buildtool.tools.osutils import _PathPattern

class TestBuildPatternString(unittest.TestCase):

    def test_empty(self):
        actual = _build_pattern_string('')
        expect = '^$'
        eq_(expect, actual)

    def test_ok(self):
        actual = _build_pattern_string('foo')
        expect = '^foo/?$'
        eq_(expect, actual)

    def test_normalize_path(self):
        actual = _build_pattern_string('foo\\bar')
        expect = '^foo/bar/?$'
        eq_(expect, actual)

    def test_translate_dot(self):
        actual = _build_pattern_string('foo.txt')
        expect = '^foo\.txt/?$'
        eq_(expect, actual)

    def test_add_shorthand(self):
        actual = _build_pattern_string('foo/')
        expect = '^foo/.*$'
        eq_(expect, actual)

    def test_translate_double_star_at_end(self):
        actual = _build_pattern_string('foo/**')
        expect = '^foo/.*$'
        eq_(expect, actual)

    def test_translate_double_star_at_begin(self):
        actual = _build_pattern_string('**/foo')
        expect = '^([^/]*/)*foo/?$'
        eq_(expect, actual)

    def test_translate_wrong_double_star_at_begin(self):
        actual = _build_pattern_string('**foo')
        expect = '^[^/]*[^/]*foo/?$'
        eq_(expect, actual)

    def test_translate_wrong_double_star_at_begin_2(self):
        actual = _build_pattern_string('**foo/**')
        expect = '^[^/]*[^/]*foo/.*$'
        eq_(expect, actual)

    def test_translate_double_star_at_middle(self):
        actual = _build_pattern_string('foo/**/bar')
        expect = '^foo/([^/]*/)*bar/?$'
        eq_(expect, actual)

    def test_translate_single_star_at_begin(self):
        actual = _build_pattern_string('*foo')
        expect = '^[^/]*foo/?$'
        eq_(expect, actual)

    def test_translate_single_star_at_middle(self):
        actual = _build_pattern_string('f*oo')
        expect = '^f[^/]*oo/?$'
        eq_(expect, actual)

    def test_translate_single_star_at_end(self):
        actual = _build_pattern_string('foo*')
        expect = '^foo[^/]*/?$'
        eq_(expect, actual)

    def test_translate_single_star_between_slash(self):
        actual = _build_pattern_string('foo/*/bar')
        expect = '^foo/[^/]*/bar/?$'
        eq_(expect, actual)

    def test_translate_three_star_at_end(self):
        actual = _build_pattern_string('foo/***')
        expect = '^foo/[^/]*[^/]*[^/]*/?$'
        eq_(expect, actual)

    def test_double_star_without_leading_slash(self):
        actual = _build_pattern_string('foo/bar**/abc')
        expect = '^foo/bar[^/]*[^/]*/abc/?$'
        eq_(expect, actual)

    def test_many_star(self):
        actual = _build_pattern_string('**/**/foo/**/**/bar/**/**abc****def**/**')
        expect = '^([^/]*/)*([^/]*/)*foo/([^/]*/)*([^/]*/)*bar/([^/]*/)*[^/]*[^/]*abc[^/]*[^/]*[^/]*[^/]*def[^/]*[^/]*/.*$'
        self.assertSequenceEqual(expect, actual)

class PathPatternTestCase(unittest.TestCase):
    u'''
    match所传入的参数，如果是目录，则肯定以/结尾，否则，就是文件
    这一点，有match的调用者保证
    '''

    def test_normal_ending(self):
        path_list = [
            'foo',
        ]

        pattern = _PathPattern(path_list)
        ok_(pattern.match('foo'), 'match file')
        ok_(pattern.match('foo/'), 'match directory')
        ok_(not pattern.match('bar'))
        ok_(not pattern.match('foo/bar'))

    def test_ends_with_single_star(self):
        u'''
        匹配目录foo、foo下的文件、foo的子目录（但不包括子目录的内容）
        '''
        path_list = [
            'foo/*',
        ]

        pattern = _PathPattern(path_list)
        assert not pattern.match('foo')
        assert pattern.match('foo/')
        assert pattern.match('foo/bar')
        assert pattern.match('foo/bar/')
        assert not pattern.match('foo/bar/abc')

    def test_ends_with_double_star(self):
        u'''
        匹配目录foo、foo下的文件、foo的子目录（包括子目录的内容）
        '''
        path_list = [
            'foo/**',
        ]

        pattern = _PathPattern(path_list)
        assert not pattern.match('foo')
        assert pattern.match('foo/')
        assert pattern.match('foo/bar')
        assert pattern.match('foo/bar/')
        assert pattern.match('foo/bar/abc')

    def test_file_with_wildcard(self):
        path_list = [
            '*foo*bar*',
        ]

        pattern = _PathPattern(path_list)
        assert pattern.match('foobar')
        assert pattern.match('123foo456bar789')
        assert pattern.match('123foobar')
        assert pattern.match('foo456bar')
        assert pattern.match('foobar789')

    def test_starts_with_double_star(self):
        path_list = [
            '**/foo',
        ]

        pattern = _PathPattern(path_list)
        assert pattern.match('foo')
        assert pattern.match('bar/foo')
        assert pattern.match('bar/foo/')
        assert pattern.match('abc/bar/foo')
        assert not pattern.match('123foo')

    def test_double_star_at_middle(self):
        path_list = [
            'foo/**/bar',
        ]

        pattern = _PathPattern(path_list)
        assert pattern.match('foo/bar')
        assert pattern.match('foo/abc/bar')
        assert pattern.match('foo/abc/def/bar')
        assert not pattern.match('foobar')

