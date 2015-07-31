%This file reads in .csv from tests to ensure that they match the
%analytical model.  
clear
close all

%***Verify result for circle*************
fname1='Tgrad.csv';
fname2='Tgrad_thexp.csv';
dim = [1000 1000];
Tgrad = 0.01;

Q=0.23; %eV
M0=2.5e-6; %m^4/Js
sigma=0.708; %J/m^2
kb = 8.617343e-5;
entropy_diff = 8e3;
molar_volume = 7.11e-6;
d = 0.5e-9;
JtoeV = 6.24150974e18;
lscale=1.0e-9;
tscale=1.0e-9;

%Convert constants to new units
M0 = M0*tscale/(JtoeV*(lscale^4));
entropy_diff = entropy_diff*JtoeV;
molar_volume = molar_volume/(lscale^3);
d = d/lscale;

%Read in marmot data
dmp=importdata(fname1);
time=dmp.data(:,1);
grarea=dmp.data(:,2);
xpos = grarea/dim(2);


dmp=importdata(fname2);
time2=dmp.data(:,1);
grarea2=dmp.data(:,2);
xpos2 = grarea2/dim(2);

%analytical model
anal_xpos = size(xpos);
anal_xpos(1) = xpos(1);
for i= 2:length(time)
    T = 450 + anal_xpos(i-1)*Tgrad;
    mob = M0*exp(-Q./(kb*T));
    anslope = mob*d*Tgrad*(entropy_diff/molar_volume);
    anal_xpos(i) = anal_xpos(1) + anslope.*(time(i)-time(1));
end

figure(1)
set(gcf,'units','inches','position',[1,1,6,4])
set(gca,'fontsize',18)
plot(time,xpos,'.',time,anal_xpos,'--',time2,xpos2,'.','linewidth',1.5,'markersize',22)
xlabel('Time (ns)')
ylabel('x-position (nm)')
axis tight
legend('MARMOT','Analytical','Just Th. Exp.','location','northwest')
legend boxoff
title('Bicrystal')