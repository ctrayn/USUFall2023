close all

figcols = 4;
figrows = 5;

for idx=1:20
    x = mod((idx-1),figcols)/figcols;
    y = floor((idx-0.1)/figcols)/figrows;
    
    figure(idx)
    set(gcf,'Units','normalized')
    set(gcf,'Position',[x,y,1/figcols,1/figrows])
    set(gcf,'Units','pixels')
end

for idx=1:20
    figure(idx)
    clf
    
    fname=sprintf("ac%d.dat",idx);
    ac=importdata(fname);
        
    semilogx(ac(:,1),ac(:,2),'b');
    grid on
end

