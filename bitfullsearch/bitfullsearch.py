#!/usr/bin/env python3
# -*- coding: utf-8 -*-

print("要素数から考える")
n = 5
for bits in range(1<<n):
    out = "{"
    for i in range(n):
        if(bits & (1 << i)):
            out += (str(i) + ",")
    out += "}"
    print(out)

print("")
print("配列から考える")
arr = [5, 8, 2, 7, 4]
for bits in range(1<<len(arr)):
    out = "{"
    for i in range(n):
        if(bits & (1 << i)):
            out += (str(arr[i]) + ",")
    out += "}"
    print(out)
