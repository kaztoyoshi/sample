#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import math

# 最大公約数を求めるための2つの自然数
A = 48
B = 32

print("1.")
def gcd1(a, b):
    if b == 0:
        return a
    else:
        return gcd1(b, a % b)
print(str(gcd1(A, B)))


print("1'.")
print(str(math.gcd(A, B)))

