from math import sqrt

KN = 261e-6
KP = 331e-6
VTHN = 0.7086
VTHP = 0.918
VDD = 3.3

Vc = ((sqrt(KN) * VTHP) + (sqrt(KP) * (VDD - VTHP))) / (sqrt(KN) + sqrt(KP))

print(Vc)