#!/usr/bin/env python


def ackermann(a, b, c):
    stack = [(a, b)]
    res = 0
    while stack:
        a, b = stack.pop()
        if a == 0:
            res = b + 1
        elif a == 1:
            res = b + c + 1
        elif a == 2:
            res = (b + 2) * c + (b + 1)
        elif b == 0:
            stack.append((a - 1, c))
        else:
            stack.append((a, b - 1))
            stack.append((a - 1, b))
    return res


def check_ack():
    for i in range(32768):
        print("checking:", i)
        res = ackermann(4, 1, i) % 32768
        if res == 6:
            return i
    return -1


if __name__ == "__main__":
    res = check_ack()
    print("magic number:", res)
