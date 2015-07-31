%-------VERIFICATION------------------------
%Verify void migration model against analytical equation
%To get sufficient data, conditions must be changed from test:
%Test conditions should be set to dt = 1, and num_steps = 35
%Also, postprocessor_csv = true
function void_migration_test
close all
clear

fname = 'out.csv';

dmp=importdata(fname);
time=dmp.data(:,1);
void_pos=dmp.data(:,2);

center=find_center(void_pos,time);


plot(time,center,'*',time,void_pos,'.')
%Analytical model
y(1) = center(1)*1.035;
Dv0in = 3.1e-5;
Emvin = 0.71;
kb = 8.617343e-5;
lscale = 1.0e-9;
tscale = 1.0e-6;
Qstar = 4.0;
dt = time(2) - time(1);

%Calculate
Dv0s=Dv0in*tscale/(lscale*lscale); %Convert units
for i = 1:length(time) - 1
    T = 1000+.025*y(i);
    gradT = 0.025;
    Dv=Dv0s.*exp(-Emvin./(kb*T));
    ydot = Dv*Qstar*gradT/(kb*T^2);
    y(i+1) = y(i) + ydot*dt;
end

plot(time,center,'d',time,y,'k--','linewidth',1.1)
set(gca,'fontsize',18)
xlabel('Time ({\mu}s)')
ylabel('y-position (nm)')
legend('PF Results','Analytical','location','northwest')
legend boxoff
axis tight
end

function center=find_center(void_pos,time)
P = polyfit(time,void_pos,2);
fitvls = polyval(P,time);
top = void_pos(void_pos>fitvls);
ttime=time(void_pos>fitvls);
bottom = void_pos(void_pos<fitvls);
btime=time(void_pos<fitvls);
center = top;

interp_bottom = interp1(btime,bottom,time);
interp_top = interp1(ttime,top,time,'cubic','extrap');

center = (interp_bottom+interp_top)/2;

%plot(time,void_pos,'.',ttime,top,'k-',btime,bottom,'k--',time,center,'d')
end