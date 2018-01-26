order = 3;
framelen = 41;



d1 = sgolayfilt(h_z,order,framelen);

plot(d1,'.-')
legend('Setpoint','velocity','filtered velocity')

%% FILTERING

order = 3;
<<<<<<< HEAD
framelen = 901;
=======
framelen = 31;
>>>>>>> 9c9955851e0517045a5d4483569f1e26ff7734fc



d = sgolayfilt(outp4,order,framelen);
d = sgolayfilt(d,order,framelen);
figure;
plot(outp4,':')
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
