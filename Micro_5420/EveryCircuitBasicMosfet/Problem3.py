from cmath import sqrt
from cmd import IDENTCHARS
from constants import *

VG = V_THN + sqrt(2 * IB / K_N)

gm = sqrt(2 * K_N * IB)
r0 = 1 / (LAMBDA_N * IB)

Av = -gm * r0

print(f"Av: {Av}")
print(f"gm: {gm}")
print(f"r0: {r0}")
