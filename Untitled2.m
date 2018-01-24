order = 3;
framelen = 201;



d1 = sgolayfilt(op1,order,framelen);
figure;
plot(op1,':')
hold on
plot(d1,'.-')
legend('signal','smooth')

%%

order = 3;
framelen = 101;



d = sgolayfilt(RPM,order,framelen);
d = sgolayfilt(d,order,framelen);
figure;
plot(RPM,':')
hold on
plot(d,'.-')
legend('signal','smooth')
%%
ip1=input(2000:3000);
ip2=input(3001:4000);
ip3=input(4001:5000);
ip4=input(5001:6000);
ip5=input(6001:7000);

op1=d(2000:3000);
op2=d(3001:4000);
op3=d(4001:5000);
op4=d(5001:6000);
op5=d(6001:7000);
