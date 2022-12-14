function plot_signal_spectrum(VA)
    VF = 1.2;    % Offset voltage
    % VA = 0.01;   % Zero-to-peak amplitude
    f  = 10e3;   % 10kHz signal frequency
    T  = 1/f;    % Period in seconds
    
    fs  = 1e6;   % Sample frequency
    Np  = 10;    % Number of periods to plot
    
    tstep = 1/fs; % Time step
    tmax  = Np*T; % Time limit
    
    t = 0:tstep:tmax; % Time vector
    
    % Input Signal:
    vsig = VF + VA*sin(2*pi*f*t);
    
    IF = (1e-3)*exp((vsig-1.2)/0.026);
    
    % Compute Fourier Spectrum
    s = fft(IF);
    
    % Magnitude Spectrum:
    smag = abs(s);
    
    % Steps in Hz
    L         = numel(vsig);
    fstep     = fs/L;
    freq_axis = 0:fstep:(L-1)*fstep;
    
    % Harmonics
    harmonic_step = round(f/fstep);
    harmonic_idx  = (1+harmonic_step):harmonic_step:(1+10*harmonic_step);
    harmonic_mag  = smag(harmonic_idx);
    
    % Spectrum Plot
    figure
    plot(freq_axis,              20*log10(smag),               ...
         freq_axis(harmonic_idx),20*log10(smag(harmonic_idx)), 'ro')
         
    % Zoom in on the harmonics:
    axis([0.5*f,                         ... 
          10*f,                          ... 
          min(20*log10(harmonic_mag)),   ... 
          max(20*log10(harmonic_mag))]);
    
    % Label axes:
    xlabel('Frequency (Hz)')
    ylabel('Magnitude (dB)')
    title(sprintf('Distorted Signal Spectrum %0.1s', VA))
    
    % THD Distortion
    THD = sum(harmonic_mag(2:end))/sum(harmonic_mag);
    fprintf('THD: %f%%\n',THD*100)
    
    % SNDR
    SNDR = 20*log10(harmonic_mag(1)) - 20*log10(harmonic_mag(2));
    fprintf('SNDR: %f dB\n',SNDR)
end
