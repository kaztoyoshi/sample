#!/usr/bin/env python3
# -*- coding: utf-8 -*-

print("要素数から考える")
n = 5
size = 1<<n
for bits in range(size):
    out = "{"
    for i in range(n):
        if(bits & (1 << i)):
            out += (str(i) + ",")
    out += "}"
    print(out)

print("")
print("配列から考える")
arr = [5, 8, 2, 7, 4]
size = 1<<len(arr)
for bits in range(size):
    out = "{"
    for i in range(n):
        if(bits & (1 << i)):
            out += (str(arr[i]) + ",")
    out += "}"
    print(out)
