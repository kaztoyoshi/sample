#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import math

n = 2600        # ある自然数
print("１.")
result = []
init = 2
if n == 1:
    result.append(1)
while n != 1:
    i = init
    while i < 0xFFFFFF:
        if n % i == 0:
            result.append(i)
            n /= i
            break
        i += 1
    init = i
for val in result:
    print(str(val) + " ", end="")
print()

n = 2600        # ある自然数
print("２.")
# 素数テーブル作成
array = []
# 探索リストに 0 から n までの数値を入れる
for i in range(n+1):
    array.append(i)
# 偶数の最小素因数は2
for i in range(2, n+1, 2):
    array[i] = 2
# 3 以上の奇数は最小素因数を記録
max = math.ceil(math.sqrt(n+1)) + 1
for i in range(3, max, 2):
    # 奇数 i が i 未満の数の倍数であれば無視
    if array[i] < i:
        continue
    # 最小素因数を記録
    for j in range(i, n+1, i):
        array[j] = i
result = []
while array[n] != 1:
    result.insert(0, array[n])
    n //= array[n]
for val in result:
    print(str(val) + " ", end="")
print()
