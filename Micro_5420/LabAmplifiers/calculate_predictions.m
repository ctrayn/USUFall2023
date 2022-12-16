Kn = 700e-6;
Kp = 190e-6;
lambda_n = .042;
lambda_p = .017;

% Predict  gmn, gmp, ron, rop, and gain
% based on the bias current

IB = iref/2;

predicted_gmn = sqrt(2 * IB * Kn);
predicted_gmp = sqrt(2 * IB * Kp);
predicted_ron = 1 / (lambda_n * IB);
predicted_rop = 1 / (lambda_p * IB);

predicted_gain = 20;


