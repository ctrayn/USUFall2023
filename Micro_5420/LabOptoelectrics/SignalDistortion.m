%% Global Vars
V_DD = 5;
Rd = 3.3e3;
%% DC Operating Point: Transmit Side
I_LED = (5 - 1.2) / Rd;
disp("--------------")
disp("LED Current " + I_LED)
disp("I_LED = 1.2 mA; V_F = 1.2 V")
%% DC Operating Point: Receive Side
disp("------------------")
K = 0.011;
I_PH = K * I_LED;
Ry = 100e3;
V = V_DD - Ry * I_PH;
disp("I_PH " + I_PH)
disp("V_PH" + V)

%% Low-Frequency Cutoff
C_L = 1e-6;
R_sig = 100e3;
rd = 25;
f_L = 1/(2 * pi * C_L * (R_sig + rd));
disp("Low Frequency: " + f_L)

%% High-Frequency Cutoff
R3 = 3.3e3;
C_H = 26e-6;
f_H = 1/(2 * pi * C_H * (R3 + rd));
disp("High Frequency " + f_H)