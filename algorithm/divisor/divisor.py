#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import math

N = 36      # 整数 N

print("1.")
out = "{"
for i in range(1, N+1):
    if N % i == 0:
        out += (str(i)) + ","
out += "}"
print(out)

print("2.")
arr = []
max = math.ceil(math.sqrt(N+1)) + 1
for i in range(1, max):
    if N % i == 0:
        arr.append(i)
        if i**2 != N:
            arr.append(N // i)
arr.sort()
out = "{"
for v in arr:
    out += (str(v)) + ","
out += "}"
print(out)

print("2'.")
asc = []        # 昇順
desc = []       # 降順
max = math.ceil(math.sqrt(N+1)) + 1
for i in range(1, max):
    if N % i == 0:
        asc.append(i)
        if i**2 != N:
            desc.insert(0, N // i)
out = "{"
for v in asc:
    out += (str(v)) + ","
for v in desc:
    out += (str(v)) + ","
out += "}"
print(out)
