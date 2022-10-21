#! /usr/bin/python3

bool_list = {True, False}

for x in bool_list:
    for y in bool_list:
        for z in bool_list:
            print(f"X {x:1} Y {y:1} Z {z:1} Result {(x or y) and (not x or z) and (not y or not z):1}")