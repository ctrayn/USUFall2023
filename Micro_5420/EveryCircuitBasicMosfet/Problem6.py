from constants import *
from math import sqrt

Vov = sqrt(2 * IB / K_P)
Vg = Vov + VDD + V_THP
R = Vg / IB
gm = sqrt(2 * K_P * IB)
r0 = 1/(LAMBDA_P * IB)
Av = -1 * gm * r0


print(f"R: {R}")
print(f"gm: {gm}")
print(f"r0: {r0}")
print(f"Av: {Av}")