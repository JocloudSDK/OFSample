# -*- coding: utf-8 -*-

import unittest

from nose.tools import eq_
from nose.tools import ok_

from buildtool.tools.encrypt import _tea_encrypt_impl
from buildtool.tools.encrypt import _string_to_longs
from buildtool.tools.encrypt import tea_encrypt

class TestTeaEncryptImpl(unittest.TestCase):

    def test_zero(self):
        actual = _tea_encrypt_impl([0x0, 0x0], [0x0, 0x0, 0x0, 0x0], 32)
        expect = [0x41ea3a0a, 0x94baa940]
        eq_(expect, actual)

    def test_normal(self):
        actual = _tea_encrypt_impl([0x25bc1d33, 0x69841],
                                   [0xdbae, 0x0, 0xd5b21a, 0x958741],
                                   16)
        expect = [0x98ee03f4, 0x6072c372]
        eq_(expect, actual)

    def test_negative(self):
        actual = _tea_encrypt_impl([0x25bc1d33, -0x69841],
                                   [-0xdbae, 0x0, 0xd5b21a, -0x958741],
                                   64)
        expect = [0x706dbe29, 0x4260aa3a]
        eq_(expect, actual)

class TestStringToLongs(unittest.TestCase):

    def test_empty(self):
        actual = _string_to_longs('')
        expect = []
        eq_(expect, actual)

    def test_nomal(self):
        actual = _string_to_longs('abcdefg')
        expect = [0x64636261, 0x676665]
        eq_(expect, actual)

class TestTeaEncrypt(unittest.TestCase):

    def test_empty(self):
        actual = tea_encrypt('', '', 16)
        expect = []
        eq_(expect, actual)

    def test_normal(self):
        actual = tea_encrypt('abcdefgh', 'key', 64)
        expect = [0xcfee8a3d, 0xaf8b1016]
        eq_(expect, actual)

    def test_normal_other(self):
        actual = tea_encrypt('''AaBbcdfja0123456789!@#$%^*()_+~`=|\:";;'?/>.:''',
                             '''!@#$%^*()_+~`=|\:";;'?/>.:''',
                             32)
        expect = [0xeb9ececf, 0x40701d52, 0xf20bb308, 0xb251c4db, 0x8c3343d9, 0xfcb3811e\
                , 0xb65a307, 0xb8f2bc2f, 0x356824b3, 0xc739ef7, 0x2e3e2f3f, 0x3a]
        eq_(expect, actual)

    def test_less_than_eight(self):
        actual = tea_encrypt('player', 'key', 64)
        expect = _string_to_longs('player')
        eq_(expect, actual)

    def test_wrong_round(self):
        actual = tea_encrypt('abcdefgh', 'key', 60)
        expect = []
        eq_(expect, actual)
