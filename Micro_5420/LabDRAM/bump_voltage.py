CD = 30e-15
CBL = 3e-12
VDD = 3.3
VTH = 0.7086

V1 = (((VDD/2) - VTH) * CD) / (CBL + CD)
V0 = (-CD * VDD / 2) / (CBL + CD)

print(f"Vbump 1 = {V1}")
print(f"Vbump 0 = {V0}")