#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import math

# f(2)=x^5-12*x^4+3*x^3-8*x^2+x-5
X = 2
# 次数が小さい順に係数を並べる
A = [-5, 1, -8, 3, -12, 1]
N = len(A)

print("1.")
def polynomial1(x, a, n):
    ans = 0
    for i in range(n):
        ans += a[i] * int(math.pow(x, i))
    return ans
print(str(polynomial1(X, A, N)))

print("2'.")
def polynomial2(x, a, n):
    ans = 0
    for i in range(n-1, -1, -1):
        ans = ans * x + a[i]
    return ans
print(str(polynomial2(X, A, N)))
