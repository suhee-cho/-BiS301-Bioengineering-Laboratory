%% Prelab part
% diff eq: -(p*y)+(1-p)*i;
tspan = [0 10];
p0 = 0; % initially, concentration of X-P is 0.
[t1,p1] = ode15s(@p_MMeq, tspan, p0, [], [1 1]);
[t2,p2] = ode15s(@p_MMeq, tspan, p0, [], [1 10]);
[t3,p3] = ode15s(@p_MMeq, tspan, p0, [], [10 1]);
[t4,p4] = ode15s(@p_MMeq, tspan, p0, [], [10 10]);

fig_1 = figure;
plot(t1,p1,'-o')
xlabel("time (sec)");
ylabel("concentration of X-P");
ylim([0 1]);
title("When i0 = 1 and y0 = 1");

fig_2 = figure;
plot(t2,p2,'-o')
xlabel("time (sec)");
ylabel("concentration of X-P");
ylim([0 1]);
title("When i0 = 1 and y0 = 10");

fig_3 = figure;
plot(t3,p3,'-o')
xlabel("time (sec)");
ylabel("concentration of X-P (");
ylim([0 1]);
title("When i0 = 10 and y0 = 1");

fig_4 = figure;
plot(t4,p4,'-o')
xlabel("time (sec)");
ylabel("concentration of X-P");
ylim([0 1]);
title("When i0 = 10 and y0 = 10");

%% Mainlab part
%% 4.1.2
tspan = [0 10];
p0 = [0 0 0]; % initially, concentration of phosphorylated forms are 0.
E0 = [1 1 1]; % initial conc. of i, Ex, Ey
k0 = [1 1 1 1 1 1]; % kx, kxp, ky, kyp, kz, kzp
[t1,p1] = ode15s(@m_MMeq, tspan, p0, 0, E0, k0);

main_fig_1 = figure;
plot(t1,p1,'-o')
xlabel("time (sec)");
ylabel("concentration");
legend("X-P", "Y-P", "Z-P");
ylim([0 1]);
title("Simulation of i-FFL");

%% 4.1.3
tspan = [0 10];
p0 = [0 0 0];
k0 = [1 1 1 1 1 0.1]; % kx, kxp, ky, kyp, kz, kzp
[t2,p2] = ode15s(@m_MMeq, tspan, p0, 0, E0, k0);
main_fig_2 = figure;
subplot(1,3,1);
plot(t2,p2)
xlabel("time");
ylabel("concentration");
legend("X-P", "Y-P", "Z-P");
ylim([0 1]);
title("When kzp is 0.1");

k0 = [1 1 1 1 1 5]; % kx, kxp, ky, kyp, kz, kzp
[t3,p3] = ode15s(@m_MMeq, tspan, p0, 0, E0, k0);
subplot(1,3,2);
plot(t3,p3)
xlabel("time");
ylabel("concentration");
legend("X-P", "Y-P", "Z-P");
ylim([0 1]);
title("When kzp is 5");

k0 = [1 1 1 1 1 10]; % kx, kxp, ky, kyp, kz, kzp
[t4,p4] = ode15s(@m_MMeq, tspan, p0, 0, E0, k0);
subplot(1,3,3);
plot(t4,p4)
xlabel("time");
ylabel("concentration");
legend("X-P", "Y-P", "Z-P");
ylim([0 1]);
title("When kzp is 10");

%% 4.2. Modeling MAPK signaling pathway
V1 = 2.5; n = 1; Ki = 9; K1 = 10; V2 = 0.25; K2 = 8; k3 = 0.025; K3 = 15;
k4 = 0.025; K4 = 15; V5 = 0.75; K5 = 15; V6 = 0.75; K6 = 15; k7 = 0.025; K7 = 15;
k8 = 0.025; K8 = 15; V9 = 0.5; K9 = 15; V10 = 0.5; K10 = 15;
params = [V1 n Ki K1 V2 K2 k3 K3 k4 K4 V5 K5 V6 K6 k7 K7 k8 K8 V9 K9 V10 K10];

tspan = [0 5000];
p0 = [0 0 0 0 0];

[t5,p5] = ode15s(@MAPK_model, tspan, p0, 0, params);
main_fig_3 = figure;
plot(t5,p5(:,5),t5,p5(:,3),t5,p5(:,1));
xlabel("time (sec)");
ylabel("concentration");
legend("MAPK-PP", "MKK-PP", "MKKK-P");
% ylim([0 1]);
title("MAPK pathway stimulation 1");

%% 4.3. Ultra-sensitivity of MAPK signaling pathway
tspan = 0:5000;
p0 = [0 0 0 0 0];

n = 1; Ki = 9; K1 = 10; V2 = 0.25; K2 = 8; k3 = 0.025; K3 = 15;
k4 = 0.025; K4 = 15; V5 = 0.75; K5 = 15; V6 = 0.75; K6 = 15; k7 = 0.025; K7 = 15;
k8 = 0.025; K8 = 15; V9 = 0.5; K9 = 15; V10 = 0.5; K10 = 15;
V1 = 0:0.1:3;

val = zeros(length(V1),3); % will contains max values of MAPK_PP, MKK_PP, MKKK_P

for i = 1:length(V1)
    params = [V1(i) n Ki K1 V2 K2 k3 K3 k4 K4 V5 K5 V6 K6 k7 K7 k8 K8 V9 K9 V10 K10];
    [t_temp,p_temp] = ode15s(@MAPK_model, tspan, p0, 0, params);
    val(i,3) = max(p_temp(1000:end,1));
    val(i,2) = max(p_temp(1000:end,3));
    val(i,1) = max(p_temp(1000:end,5));
end

main_fig_4 = figure;
plot(V1,val(:,1),V1,val(:,2),V1,val(:,3));
xlabel('Input Stimulus')
ylabel('steady state concentration');
legend('MAPK-PP','MKK-PP','MKKK-P')
% ylim([0 1]);
title('MAPK pathway simulation 2 (not normalized)')

main_fig_5 = figure;
plot(V1,val(:,1)/max(val(:,1)), ... 
    V1,val(:,2)/max(val(:,2)), ...
    V1,val(:,3)/max(val(:,3)));
xlabel('Input Stimulus')
ylabel('normalized steady state concentration');
legend('MAPK-PP','MKK-PP','MKKK-P')
% ylim([0 1]);
title('MAPK pathway simulation 3 (normalized)')

%% 4.4. changing Ki
tspan = 0:5000;
p0 = [0 0 0 0 0];

% parameters
V1 = 2.5; n = 1; K1 = 10; V2 = 0.25; K2 = 8; k3 = 0.025; K3 = 15;
k4 = 0.025; K4 = 15; V5 = 0.75; K5 = 15; V6 = 0.75; K6 = 15; k7 = 0.025; K7 = 15;
k8 = 0.025; K8 = 15; V9 = 0.5; K9 = 15; V10 = 0.5; K10 = 15;
Log2Ki = -6:0.1:6; Ki = 2.^Log2Ki; % log scaled Ki
val = zeros(length(Ki),2); % maximum MAPK_PP and minimum MAPK_PP

% save max and min values in val matrix
for i = 1:length(Ki)
    params = [V1 n Ki(i) K1 V2 K2 k3 K3 k4 K4 V5 K5 V6 K6 k7 K7 k8 K8 V9 K9 V10 K10];
    [t_temp, p_temp] = ode15s(@MAPK_model, tspan, p0, 0, params);
    val(i,1) = max(p_temp(3000:end,5));
    val(i,2) = min(p_temp(3000:end,5));
end

main_fig_6 = figure;
plot(Log2Ki,val(:,1),Log2Ki,val(:,2))
title('MAPK pathway simulation 4 (changing Ki)');
xlabel('parameter Ki (log scale)');
ylabel('steady state concentration');
legend('Max','Min')

%% 4.4. Changing V1
tspan = 0:5000;
p0 = [0 0 0 0 0];

% parameters
n = 1; Ki = 9; K1 = 10; V2 = 0.25; K2 = 8; k3 = 0.025; K3 = 15;
k4 = 0.025; K4 = 15; V5 = 0.75; K5 = 15; V6 = 0.75; K6 = 15; k7 = 0.025; K7 = 15;
k8 = 0.025; K8 = 15; V9 = 0.5; K9 = 15; V10 = 0.5; K10 = 15;
Log2V1 = -3:0.1:4; V1 = 2.^Log2V1; % log scaled V1

val = zeros(length(V1),2); % maximum MAPK_PP and minimum MAPK_PP

% save max and min values in val matrix
for i = 1:length(V1)
    params = [V1(i) n Ki K1 V2 K2 k3 K3 k4 K4 V5 K5 V6 K6 k7 K7 k8 K8 V9 K9 V10 K10];
    [t_temp, p_temp] = ode15s(@MAPK_model, tspan, p0, 0, params);
    val(i,1) = max(p_temp(3000:end,5));
    val(i,2) = min(p_temp(3000:end,5));
end

main_fig_7 = figure;
plot(Log2V1,val(:,1),Log2V1,val(:,2)); 
title('MAPK pathway simulation 5 (changing V1)');
xlabel('parameter V1 (log scale)');
ylabel('steady state concentration');
legend('Max','Min');


%% Functions for formulating differntial equations
% diff eq: (k3*i0*x)/(x+KM) - (k3'*y0*p)/(p+KM');
function pre_diff_eq = p_MMeq(tspan, p, params)
    i0 = params(1); y0 = params(2);
    k3 = 1; k3p = 1;
    KM = 1; KMp = 1;
    pre_diff_eq = (k3*(1-p)*i0)/(1-p + KM) - (k3p*y0*p)/(p + KMp);
end

function main_diff_eq = m_MMeq(tspan, p, E, k)
    i0 = E(1); Ex0 = E(2); Ey0 = E(3);
    % var for x
    kx = k(1); kxp = k(2);
    xKM = 1; xKMp = 1;
    xp = p(1);
    %var for y
    ky = k(3); kyp = k(4);
    yKM = 1; yKMp = 1;
    yp = p(2);
    %var for z
    kz = k(5); kzp = k(6);
    zKM = 1; zKMp = 1;
    zp = p(3);
    
    main_diff_eq = zeros(3,1);
    main_diff_eq(1) = (kx*(1-xp)*i0)/(1-xp + xKM) - (kxp*Ex0*xp)/(xp + xKMp);
    main_diff_eq(2) = (ky*(1-yp)*xp)/(1-yp + yKM) - (kyp*Ey0*yp)/(yp + yKMp);
    main_diff_eq(3) = (kz*(1-zp)*xp)/(1-zp + zKM) - (kzp*zp*yp)/(zp + zKMp);
end

function dy = MAPK_model(t, y, params)

    % Parameters
    V1 = params(1); n = params(2); Ki = params(3); K1 = params(4);
    V2 = params(5); K2 = params(6);
    k3 = params(7); K3 = params(8);
    k4 = params(9); K4 = params(10);
    V5 = params(11); K5 = params(12);
    V6 = params(13); K6 = params(14);
    k7 = params(15); K7 = params(16);
    k8 = params(17); K8 = params(18);
    V9 = params(19); K9 = params(20);
    V10 = params(21); K10 = params(22);

    % Variables
    MKKK_P = y(1);
    MKK_P = y(2);
    MKK_PP = y(3);
    MAPK_P = y(4);
    MAPK_PP = y(5);

    MKKK = 100 - MKKK_P;
    MKK = 300 - MKK_P - MKK_PP;
    MAPK = 300 - MAPK_P - MAPK_PP;

    % Reaction rates
    v1 = V1*MKKK/((1+(MAPK_PP/Ki)^n)*(K1+MKKK)); %Allosteric inhibition : negative feedback
    % v1 = V1*MKKK/(K1+MKKK);
    v2 = V2*MKKK_P/(K2+MKKK_P);
    v3 = k3*MKKK_P*MKK/(K3+MKK);
    v4 = k4*MKKK_P*MKK_P/(K4+MKK_P);
    v5 = V5*MKK_PP/(K5+MKK_PP);
    v6 = V6*MKK_P/(K6 + MKK_P);
    v7 = k7*MKK_PP*MAPK/(K7+MAPK);
    v8 = k8*MKK_PP*MAPK_P/(K8+MAPK_P);
    v9 = V9*MAPK_PP/(K9+MAPK_PP);
    v10 = V10*MAPK_P/(K10+MAPK_P);

    % ODE Equations
    %dMKKK = v2 - v1;
    dMKKK_P = v1 - v2;
    %dMKK = v6 - v3;
    dMKK_P = v3 + v5 - v4 - v6;
    dMKK_PP = v4 - v5;
    %dMAPK = v10 - v7;
    dMAPK_P = v7 + v9 - v8 - v10;
    dMAPK_PP = v8 - v9;

    dy = [dMKKK_P, dMKK_P, dMKK_PP, dMAPK_P, dMAPK_PP]';
end
