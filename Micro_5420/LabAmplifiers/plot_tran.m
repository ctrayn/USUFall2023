%close all

figcols = 4;
figrows = 5;

for idx=1:20
    figure(idx)
    clf
    hold on
    fname=sprintf("tran%d.dat",idx);
    tran=importdata(fname);
        
    plot(tran(:,1),tran(:,6),'b');
    %plot(tran(:,1),tran(:,2),'r');
    %plot(tran(:,1),tran(:,4),'r');
    %plot(tran(:,1),tran(:,8),'g');
    %axis([0 max(tran(:,1)) -5 5])
    axis([0 max(tran(:,1)) 2.2 2.5])
    grid on
end

for idx=1:20
    x = mod((idx-1),figcols)/figcols;
    y = floor((idx-0.1)/figcols)/figrows;
    
    figure(idx)
    set(gcf,'Units','normalized')
    set(gcf,'Position',[x,y,1/figcols,1/figrows])
    set(gcf,'Units','pixels')
end