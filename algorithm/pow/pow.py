#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import math

# X^N
X = 3
N = 50

print("1.")
def pow1(x, n):
    ans = 1
    while n > 0:
        if n & 1:
            ans *= x
        x *= x
        n >>= 1
    return ans
print(str(pow1(X, N)))

print("1'.")
print(str(math.pow(X, N)))
