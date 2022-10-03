% Problem 1
load constants.mat

VD_Bias = sqrt(2 * IB / Kn) + Vthn;
disp("VD_Bias: " + VD_Bias);

R_Bias = (VDD - VD_Bias)/IB;
disp("R_Bias: " + R_Bias);

RL = 1e3;
VDL = sqrt(2*VDD/(Kn * RL));
disp("VDL: " + VDL);

IL = (VDD - VDL) / RL;
disp("IL: " + IL);