# -*- coding: utf-8 -*-

import struct

def _tea_encrypt_impl(value, key, r):
    y, z = value[:2]
    sum = 0
    delta = 0x9E3779B9
    n = r
    a, b, c, d = key[:4]

    while n > 0:
        sum += delta
        y += ((z << 4) + a) ^ (z + sum) ^ ((z >> 5) + b);
        y &= 4294967295L
        z += ((y << 4) + c) ^ (y + sum) ^ ((y >> 5) + d);
        z &= 4294967295L
        n -= 1

    return [y, z]

def _string_to_longs(s):
    longs = []

    index = 0
    for i in xrange(0, len(s) - 3, 4):
        longs.append(struct.unpack('L', s[i: i + 4])[0])
        index = i + 4
    if index < len(s):
        l = 0
        for i in xrange(len(s) - 1, index - 1, -1):
            l *= 0x100
            l += ord(s[i])
        longs.append(l)

    return longs

def tea_encrypt(data, string_key, r):
    result = []

    if 16 != r and 32 != r and 64 != r:
        return result

    longs_key = _string_to_longs(string_key)
    for i in xrange(4 - len(longs_key)):
        longs_key.append(0)

    index = 0
    for i in xrange(0, len(data) - 7, 8):
        value = _string_to_longs(data[i: i + 8])
        w = _tea_encrypt_impl(value, longs_key, r)
        result.extend(w)
        index = i + 8

    last_result = _string_to_longs(data[index:])
    result.extend(last_result)

    return result

def tea_encrypt_file(read_file_name, write_file_name, string_key, r):
    with open(read_file_name, 'rb') as read_file:
        data = read_file.read()
    write_tea_encrypt_file(data, write_file_name, string_key, r)

def write_tea_encrypt_file(data, write_file_name, string_key, r):
    result = tea_encrypt(data, string_key, r)
    with open(write_file_name, 'wb') as write_file:
        for l in result[:-1]:
            write_file.write(struct.pack('L', l))
        last_l = result[-1]
        for i in xrange(4):
            last_l, b = divmod(last_l, 0x100)
            write_file.write(struct.pack('B', b))
            if last_l == 0:
                break;

