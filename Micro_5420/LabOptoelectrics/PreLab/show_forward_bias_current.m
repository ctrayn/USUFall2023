function show_forward_bias_current(VA)
    VF = 0;    % Offset voltage
%     VA = 0.01;   % Zero-to-peak amplitude
    f  = 1e3;   % 1kHz signal frequency
    T  = 1/f;    % Period in seconds
    
    fs  = 1e6;   % Sample frequency
    Np  = 10;    % Number of periods to plot
    
    tstep = 1/fs; % Time step
    tmax  = Np*T; % Time limit
    
    t = 0:tstep:tmax; % Time vector
    
    U_T = 26e-3;
    
    % Input Signal:
    vd = VF + VA*sin(2*pi*f*t);
    I_f = 1e-3*exp((vd - 1.2)/U_T);
    plot(t, I_f);
    xlabel("time[s]")
    ylabel("current[A]")
end