from math import sqrt
from constants import *

R = 89.88e3

AV = -sqrt(2 * K_N * (VDD - V_THN) * R)
print(f"AV: {AV}")

ID = (VDD - V_THN)/R
print(f"ID: {ID}")

gm = sqrt(2 * K_N * ID)
r0 = 1/(LAMBDA_N * ID)
print(f"gm: {gm}")
print(f"r0: {r0}")