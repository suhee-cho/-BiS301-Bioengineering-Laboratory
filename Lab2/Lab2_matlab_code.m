%import ECG and EMG files
ECG = importdata('C:\Users\SuheeCho\Desktop\2021 Spring\계측1\my finallab\ECG.txt');
EMG = importdata('C:\Users\SuheeCho\Desktop\2021 Spring\계측1\my finallab\EMG.txt');
%% Plot ECG vs time
%extract each column of ECG file
ECG_time = ECG.data(:,1);
ECG_ECG = ECG.data(:,2);
ECG_Pulse = ECG.data(:,3);
ECG_Pulse_Integral = ECG.data(:,4);

plot(ECG_time, ECG_ECG)
title('ECG vs Time') %set title
xlabel('Time (sec)') %set x label
ylabel('Voltage (mV)') %set y label
xlim([0,65]) %limit x-axis for whole graph
xlim([0,10]) %limit x-axis for t=0 to t=10
xlim([0.2, 2]) %limit x-axis for only one phase
%% Plot EMG vs time
%extract each column of EMG file
EMG_time = EMG.data(:,1);
EMG_EMG = EMG.data(:,2);
EMG_MF = EMG.data(:,3);

plot(EMG_time, EMG_EMG)
title('EMG vs Time') %set title
xlabel('Time (sec)') %set x label
ylabel('Voltage (V)') %set y label
xlim([0,30]) %limit x-axis
%% Plot Muscle force vs time
plot(EMG_time, EMG_MF)
title('Muscle Force vs Time') %set title
xlabel('Time (sec)') %set x label
xlim([0,30]) %limit x-axis
%% Plot Force vs time
subplot(2,1,1) %we will plot two graphs vertically
plot(EMG_time, EMG_EMG)
title('EMG') %set title
xlabel('Time (sec)') %set x label
xlim([0,30]) %limit x-axis
subplot(2,1,2)
plot(EMG_time, EMG_MF)
title('Muscle Force') %set title
xlabel('Time (sec)') %set x label
xlim([0,30]) %limit x-axis
%% Overlay plot
%normalize data
norm_EMG = normalize(EMG_EMG, 'norm', Inf);
norm_MF = normalize(EMG_MF - min(EMG_MF), 'norm', Inf);

subplot(1,2,1); %plot two graph horizontally
plotyy(EMG_time,EMG_EMG,EMG_time,EMG_MF); %left plot
title('EMG & Force plotyy'); %set title
xlabel('Time(sec)'); %set x label
xlim([0,30]) %set x limit
subplot(1,2,2); %right plot
plot(EMG_time, norm_EMG); %normalized EMG
hold on;
plot(EMG_time, norm_MF); %normalized muscle force
title('EMG & Force normalization');
xlabel('Time(sec)');
xlim([0,30])
%% R peak timing
%After drawing time vs ECG line plot, hold the plot on
%to add 'o' mark on R peak of the plot
hold on

%save peak value at pks and peak location at locs
%Set minimum peak height as 0.3 and minimum peak distance as 0.5
%to filter out noise
[pks, locs] = findpeaks(ECG_ECG, 'minpeakheight', 0.3, 'minpeakdistance', 0.5);

%The data is collected at every 0.005 second
%So nth data is a data at t = (n-1)*0.005
timing = (locs-1)*0.005;

%plot 'o' mark
plot(timing, pks, 'o')
%% Average QRS waveform
%make matrix named block, which contains
%ECG value at (R peak-50ms) to (Rpeak+100ms)
%I wrote a for loop iterate through i=-10 to 20,
%since the time interval between adjacent two points is 0.005sec
for (i = [-10:20])
    blocks(:,i+11) = ECG_ECG(locs+i)
end

%draw plot of each QRS complex with blue line
for(i = [1:71])
    plot([-50:5:100], blocks(i,:), 'Color', [0,0.7,0.9])
    hold on
end

%find mean values of QRS complex at each time point,
%and plot them with red line
mean_QRS = mean(blocks);
plot([-50:5:100], mean_QRS, 'Color', 'r')
grid on %turn on the grid
title('QRS complex waveform') %set title
xlabel('Time (msec)') %set x label
ylabel('Voltage (mV)') %set y label

%find R interval by just subtracting timing of two R peak
R_interval = []
for(i = [1:length(timing)-1])
    R_interval(end+1) = timing(i+1)-timing(i);
end
%% Heart rave bse time
%calculate heartrate by 60/(R-R interval)
%Note that last 6 R peaks occur later than 60second
%so omit 6 data.
heart_rate = 60./R_interval(1:end-5);
plot(timing(1:end-6), heart_rate)
xlim([0,60])
title('Heart rate vs Time') %set title
xlabel('Time (sec)') %set x label
ylabel('Heart beat (bpm)') %set y label
%% Heart rate histogram
%draw histogram with bin width = 1
histogram(heart_rate, 'BinWidth',1)
ylim([0,12]) %set y limit
%% Plot EMG envelope vs time
%Find EMG envelope with window size = 10msec
%let i be the starting point of the window
for i = 1:length(norm_EMG)
    j = i+10; %j is the end point of window
    if j > length(EMG_time)
        j = length(EMG_time);
        env1(i) = rms(norm_EMG(i:j));
        break %when window is out of range, terminate loop
    end
    %ith value of env vector is the rms value
    %of ith to jth elements in EMG_EMG vector
    env1(i) = rms(norm_EMG(i:j));
end

%Similarly, find EMG envelope with window size = 100msec
for i = 1:length(norm_EMG)
    j = i+100;
    if j > length(EMG_time)
        j = length(EMG_time);
        env2(i) = rms(norm_EMG(i:j));
        break
    end
    env2(i) = rms(norm_EMG(i:j));
end

%Similarly, find EMG envelope with window size = 1000msec
for i = 1:length(norm_EMG)
    j = i+1000;
    if j > length(EMG_time)
        j = length(EMG_time);
        env3(i) = rms(norm_EMG(i:j));
        break
    end
    env3(i) = rms(norm_EMG(i:j));
end

%plot EMG envelope with window size = 10ms vs. time
plot(EMG_time, norm_EMG); %first plot EMG vs. time
hold on;
plot(EMG_time(1:length(env1)), env1); %plot envelope vs. time
title('RMS envelope (window size: 10 ms)'); %set title
xlabel('Time (sec)'); %set x label
xlim([0, 30.335]); %set x limit
legend('EMG', 'envelope');%set title
grid on %turn grid on

%repeat the process for window size = 100ms
plot(EMG_time, norm_EMG);
hold on;
plot(EMG_time(1:length(env2)), env2, 'linewidth', 2);
title('RMS envelope (window size: 100 ms)');
xlabel('Time (sec)');
xlim([0, 30.335]);
legend('EMG', 'envelope');
grid on

%repeat the process for window size = 1000ms
plot(EMG_time, norm_EMG);
hold on;
plot(EMG_time(1:length(env3)), env3, 'linewidth', 2);
title('RMS envelope (window size: 1 s)');
xlabel('Time (sec)');
xlim([0, 30.335]);
legend('EMG', 'envelope');
grid on
%% Plot Force vs EMG envelope
%This time, plot muscle force vs. EMG envelope
%I used rms value with window size = 10ms
plot(EMG_time, norm_MF);
hold on;
plot(EMG_time(1:length(env1)), env1);
title('Force and EMG envelope (window size: 10 ms)');
xlabel('Time (sec)');
xlim([0, 30.335]);
legend('force', 'EMG');
grid on

