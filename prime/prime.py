#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import math

N1 = 13
N2 = 15
N3 = 17

print("1.")
def isPrime1(N):
    for i in range(2, N):
        if N % i == 0:
            return False
    return True
print(str(N1) + ":" + ("OK" if isPrime1(N1) else "NG"))
print(str(N2) + ":" + ("OK" if isPrime1(N2) else "NG"))
print(str(N3) + ":" + ("OK" if isPrime1(N3) else "NG"))

print("2.")
def isPrime2(N):
    max = math.ceil(math.sqrt(N+1)) + 1
    for i in range(2, max):
        if N % i == 0:
            return False
    return True
print(str(N1) + ":" + ("OK" if isPrime2(N1) else "NG"))
print(str(N2) + ":" + ("OK" if isPrime2(N2) else "NG"))
print(str(N3) + ":" + ("OK" if isPrime2(N3) else "NG"))

print("3.")
N = 100
# 探索リストに 0 から N+1 まで true を入れる
array = [True] * (N+1)
# 0,1は素数ではないので外す
array[0] = False
array[1] = False
max = math.ceil(math.sqrt(N+1)) + 1
for i in range(2, max):
    # 素数の場合
    if array[i]:
        # 先頭の数の倍数をリストから篩い落とす
        for j in range(i**2, N+1, i):
            array[j] = False
print(str(N1) + ":" + ("OK" if array[N1] else "NG"))
print(str(N2) + ":" + ("OK" if array[N2] else "NG"))
print(str(N3) + ":" + ("OK" if array[N3] else "NG"))
