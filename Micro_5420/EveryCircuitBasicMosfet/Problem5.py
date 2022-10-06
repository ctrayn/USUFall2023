from cmath import sqrt
from constants import *
from Problem4 import R

Vg = 0
Vov = Vg - VDD - V_THP
VD = R/2 * K_P * (Vov**2)
IL = 0.5 * K_P * (Vov**2)
r0 = 1 / (LAMBDA_P * IL)
gm = sqrt(2 * K_P * IL)
Av = (gm*r0)/(gm + r0)

print(f"R: {R}")
print(f"gm: {gm}")
print(f"r0: {r0}")
print(f"Av: {Av}")