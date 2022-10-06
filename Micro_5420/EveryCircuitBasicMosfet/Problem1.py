from math import sqrt
from constants import *

V_DBias = sqrt(2 * IB / K_N) + V_THN
print(f"V_DBias: {V_DBias}")

R_Bias = (VDD - V_DBias) / IB
print(f"R_Bias: {R_Bias}")

Vov = V_DBias - V_THN
print(f"Vov: {Vov}")

RL = 1e3

IL = 0.5 * K_N * (Vov**2)
print(f"IL: {IL}")

VDL = IL*RL+VDD
print(f"VDL: {VDL}")
