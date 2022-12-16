d = importdata("results.txt");

vicm   = d(:,2);
iref   = d(:,3);
gain   = d(:,4);
gaindb = d(:,5);
bw     = d(:,6);
gmn    = d(:,7);
gmp    = d(:,8);
ron    = d(:,9);
rop    = d(:,10);

calculate_predictions


% Plot all the data:

fig_idx=1;
figure(fig_idx)
clf
plot(vicm,iref,'LineWidth',1.1)
grid on
title('Tail Current vs Vicm')
xlabel('Vicm')
ylabel('Iref')



fig_idx=fig_idx+1;
figure(fig_idx)
clf
semilogy(vicm,bw,'LineWidth',1.1)
title('Bandwidth vs Vicm')
grid on
xlabel('Vicm')
ylabel('BW (Hz)')

fig_idx=fig_idx+1;
figure(fig_idx)
clf
predicted_gmn = sqrt(2*Kn*.5*iref);
plot(vicm,gmn,'b',vicm,predicted_gmn,'r','LineWidth',1.1)
legend('Measured','Predicted','Location','SouthEast')
grid on
title('Transconductance, N-Type')
xlabel('Vicm')
ylabel('gmn (A/V)')

fig_idx=fig_idx+1;
figure(fig_idx)
clf
predicted_gmp = sqrt(2*Kp*.5*iref);
plot(vicm,gmp,'b',vicm,predicted_gmp,'r','LineWidth',1.1)
legend('Measured','Predicted','Location','SouthEast')
grid on
title('Transconductance, P-Type')
xlabel('Vicm')
ylabel('gmp (A/V)')

fig_idx=fig_idx+1;
figure(fig_idx)
clf
predicted_ron = 1./(lambda_n*0.5*iref);
semilogy(vicm,ron,'b',vicm,predicted_ron,'r','LineWidth',1.1)
axis([-5 5 1e4 1e7])
grid on
title('Output Resistance, N-type')
xlabel('Vicm')
ylabel('ron (Ohm)')

fig_idx=fig_idx+1;
figure(fig_idx)
clf
predicted_rop = 1./(lambda_p*0.5*iref);
semilogy(vicm,rop,'b',vicm,predicted_rop,'r','LineWidth',1.1)
axis([-5 5 1e5 1e7])
title('Output Resistance, P-type')
grid on
xlabel('Vicm')
ylabel('rop (Ohm)')


fig_idx=fig_idx+1;
figure(fig_idx)
clf
plot(vicm,gain,'b',vicm,predicted_gain,'r','LineWidth',1.1)
grid on
legend('Measured Gain','Predicted Gain')
title('Gain in Volts/Volt')
axis([-5 5 15 125])
xlabel('Vicm (V)')
ylabel('Gain (V/V)')


fig_idx=fig_idx+1;
figure(fig_idx)
clf
plot(vicm,gaindb,'b',vicm,20*log10(predicted_gain),'r','LineWidth',1.1)
legend('Measured Gain','Predicted Gain')
axis([-5 5 20 50])
grid on
title('Gain in dB')
xlabel('Vicm')
ylabel('Gain (dB)')


fig_idx=fig_idx+1;
figure(fig_idx)
clf
predicted_gain = sqrt(2*Kn./((lambda_n^2+lambda_p^2).*0.5*iref));
calculated_gain = gmn.*ron.*rop./(ron+rop);
plot(vicm,(gain-predicted_gain)./gain,'b',...
    'LineWidth',1.1)
%vicm,(gain-calculated_gain)./gain,'r:',...
axis([-5 5 -0.5 0.5])
grid on
xlabel('Vicm (V)')
ylabel('Relative Error')
title('Predicted vs Measured Gain, Relative Error')



fig_idx=fig_idx+1;
figure(fig_idx)
clf
plot(gain,bw,'LineWidth',1.5)
grid on
xlabel('Gain (V/V)')
ylabel('BW (Hz)')
title('Gain/Bandwidth Tradeoff')

fig_idx=fig_idx+1;
figure(fig_idx)
clf
plot(iref,gain,'b',iref,predicted_gain,'r','LineWidth',1.1)
legend('Measured Gain','Predicted Gain')
axis([min(iref) max(iref) 10 125])
grid on
xlabel('Iref (A)')
ylabel('Gain (V/V)')

fig_idx=fig_idx+1;
figure(fig_idx)
clf
loglog(iref,bw,'LineWidth',1.1)
grid on
axis([min(iref) max(iref) 1 5e6])
title('Bandwidth vs Tail Current')
xlabel('Iref (A)')
ylabel('BW (Hz)')

