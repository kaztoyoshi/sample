#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import math

# 最小公倍数を求めるための2つの自然数
A = 48
B = 32

print("1.")
def gcd1(a, b):
    if b == 0:
        return a
    else:
        return gcd1(b, a % b)
def lcm1(a, b):
    d = gcd1(a, b)
    # a*b では値によってはオーバーフローを起こす可能性がある
    return a // d * b
print(str(lcm1(A, B)))
