#!/usr/bin/python3

from cmath import pi


V_DD = 5

Rd = 3.3e3
I_LED = (5 - 1.2) / Rd
# DC Operating Point: Transmit Side
print("-------------------------")
print(f"Current Value: {I_LED}")
print(f": I_LED = 1.2mA; V_F = 1.2V")

# DC Operating Point: Receive Side
print("-------------------------")
K = 0.011
I_PH = K * I_LED
print(f"Current Value: {I_PH}")
Ry = 100e3
V = V_DD - Ry * I_PH
print(f"Voltage: {V}")

# Low-Frequency Cutoff
print("-------------------------")
C = 1e-6
Rsig = 100e3
rd = 25

f_L = 1/(2 * pi * C * (Rsig + rd))
print(f"Low Frequency: {f_L:1f} Hz")

R3 = 3.3e3
f_H = 1/(2 * pi * C * (R3 + rd))
print(f"High Frequency: {f_H:1f} Hz")


