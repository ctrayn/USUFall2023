data = importdata("fft.dat");

plot(data(:,1),data(:,2));
title("EX9_4 FFT")
ylabel('Hz')
xlabel('dB')
axis([0, 500000, -300, 0])

fftsnr = snr(data(:,2),data(:,1))
