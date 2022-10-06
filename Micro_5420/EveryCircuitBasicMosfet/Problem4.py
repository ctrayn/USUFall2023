from math import sqrt
from constants import *

Vov = sqrt(2 * IB / K_P)
VGS = -1 * (Vov + V_THP - VDD)
VG = VDD - VGS
R = VG / IB

RL = 1e3
IL = 0.5 * K_P * (Vov**2)
VDL = IL * RL

print(f"R : {R}")
print(f"VG: {VG}")
print(f"IL: {IL}")
print(f"VDL: {VDL}")