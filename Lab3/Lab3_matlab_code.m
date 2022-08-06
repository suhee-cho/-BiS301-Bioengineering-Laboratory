%% 4.1. Spontaneous activity analysis
% import Experiment_4
path = 'C:\Users\SuheeCho\Desktop\2021_Spring\Lab1\my_finallab\Experiment_4.txt';
raw_data_all = importdata(path, '\t');
time = raw_data_all.data(:, 1);
raw_signal = raw_data_all.data(:, 2:11);

%% apply high-pass filter and plot
[B, A] = butter(2, 200/(25000/2), 'high');
filtered_signal = filter(B, A, raw_signal);

%% (4.1.2) plot raw & filtered data
figure;
for i = 1:10
    subplot(5, 2, i);
    plot(time, raw_signal(:, i), 'LineWidth', 0.1); hold on;
    plot(time, filtered_signal(:, i), 'LineWidth', 0.1);
    title('Ch ' + string(i));
    xlabel('time (msec)');
end
legend('raw', 'filtered');
sgtitle('raw & filtered data');

% find threshold
M = mean(filtered_signal);
STD = std(filtered_signal);
thres = M - 5 * STD;

%% (4.1.3) Draw 10 graphs and plot the threshold at the same time
figure;
all_peaks = zeros(571, 10);
for i = 1:10
    subplot(5, 2, i);
    plot(time, filtered_signal(:, i), 'Color', 'red'); hold on;
    y = thres(i) + 0*time;
    % plot threshold
    line(time, y, 'Color', 'black', 'LineWidth', 1); hold on;
    title('Ch ' + string(i));
    xlabel('time (msec)');
    % find peak index with find_peak function defined below
    peak_idx = find_peak(filtered_signal(:, i), thres(i));
    all_peaks(1, i) = length(peak_idx);
    for k = 1:length(peak_idx)
        all_peaks(k+1, i) = peak_idx(k);
    end
    % plot peaks
    plot(time(peak_idx), filtered_signal(peak_idx, i), 'o', 'Color', 'blue', 'MarkerSize', 2);
end
legend('filtered signal', 'threshold', 'peaks');
sgtitle('filtered signal and threshold');

%% (4.1.4) raster plot for 10 channels
figure;
for i = 1:10
    peaklen = all_peaks(1, i);
    peaks = all_peaks(2:peaklen+1, i);
    peaks = (peaks-1) * 0.04;
    % plot peaks with '|' marker,
    plot(peaks, i, '|', 'MarkerSize', 18, 'Color', 'black'); hold on;
end
ylim([0, 11]);
yticks(0:11);
yticklabels({'';'Ch10';'Ch9';'Ch8';'Ch7';'Ch6';'Ch5';'Ch4';'Ch3';'Ch2';'Ch1';''});
title('raster plot');
xlabel('time (msec)');

%% (4.1.5) spike rate histograms with bin size of 0.1, 1, 10 sec
binsize = [0.1, 1, 10];
nbins = 60 ./ binsize; % number of bin is 60/binsize

for k = 1:3
    figure;
    nbin = nbins(k);
    meanRate = [];
    % draw 10 histograms
    for i = 1:10
        subplot(5, 2, i);
        peaklen = all_peaks(1, i);
        peaks = all_peaks(2:peaklen+1, i);
        peaks = (peaks-1) * 0.04;
        histogram(peaks, nbin);
        [counts, ~] = histcounts(peaks, nbin);
        title('Ch ' + string(i));
        xlabel('time (msec)');
        % save peak number to find mean firing rate
        meanRate(i) = length(peaks);
        % calculate H value, to find optimal bin size
        H_1(i,k) = (2*mean(counts)-var(counts))/binsize(k)^2;
    end
    sgtitle('Spike rate histogram with bin size ' + string(binsize(k)) + ' sec');
end

%% (4.1.5) mean firing rate of each channel
for i = 1:10
    % mean firing rate = (number of spikes)/60
    meanRate(i) = meanRate(i)/ 60; 
    disp('Mean firing rate of Ch ' + string(i) + ' is ' + string(meanRate(i)) + ' per sec.');
end

%% (4.1.6) ISI histogram with bin size of 1, 10 , 50, 100 msec
binsize = [1, 10, 50, 100];
for k = 1:4
    figure;
    for i = 1:10
        peaklen = all_peaks(1, i);
        peaks = all_peaks(2:peaklen+1, i);
        ISI = [];
        for j = 1:peaklen-1
            % ISI(j) is time it took for j+1th spike occurs
            % from jth spike
            ISI(j) = 0.04 * (peaks(j+1) - peaks(j));
        end
        subplot(5, 2, i);
        histogram(ISI, 'BinWidth', binsize(k));
        xlim([0 1000]);
        % insert captions
        title('Ch ' + string(i));
        xlabel('time (msec)');
        figure();
        plot();
    end
    sgtitle('ISI histogram with bin size ' + string(binsize(k)) + ' ms');
end

%% peak_idx function
function peak_idx = find_peak(filtered_signal, thres)
    under_idx = find(filtered_signal < thres);
    peak_val = zeros(size(under_idx));
    peak_idx = zeros(size(under_idx));
    j = 1;
    for i = 1:length(under_idx)
        % go through all the points whose value is less than threshold
        % set interval with certain length, and find the point which has
        % minimum value
        % save those points
        interval = filtered_signal(under_idx(i)-25 : under_idx(i)+50);
        [peak_val(j), peak_idx(j)] = min(interval);
        peak_idx(j) = peak_idx(j) + under_idx(i) - 26;
        j = j+1;
    end
    % delete overlapped points
    peak_idx = unique(peak_idx);
end

%% 4.2
% import data for 4.2
mainpath = 'C:\Users\SuheeCho\Desktop\2021_Spring\Lab1\my_finallab\txt';
Data_105 = divideTrial(filtering(loadTxt(mainpath + "\stim_10.5uA.txt")));
Data_110 = divideTrial(filtering(loadTxt(mainpath + "\stim_11.0uA.txt")));
Data_115 = divideTrial(filtering(loadTxt(mainpath + "\stim_11.5uA.txt")));
Data_120 = divideTrial(filtering(loadTxt(mainpath + "\stim_12.0uA.txt")));
Data_125 = divideTrial(filtering(loadTxt(mainpath + "\stim_12.5uA.txt")));
Data_130 = divideTrial(filtering(loadTxt(mainpath + "\stim_13.0uA.txt")));
Data_135 = divideTrial(filtering(loadTxt(mainpath + "\stim_13.5uA.txt")));
Data_140 = divideTrial(filtering(loadTxt(mainpath + "\stim_14.0uA.txt")));

%% (4.2.2) plot filtered data & raw data (11.0 microA, ch36, trial1)
dataNum = Data_110.trials_ch36(1,1);
rawSignal = Data_110.trials_ch36(4:dataNum+3, 1);
% filter data
filSignal = Data_110.fil_trials_ch36(4:dataNum+3, 1);
time = Data_110.time(1:dataNum);

figure;
plot(time, rawSignal); hold on;
plot(time, filSignal);
legend('raw', 'filtered');
xlabel('time(msec)');
xlim([800, 1000]);
title('raw & filtered data for 11.0 microA, ch 36, trial1');

%% apply fitInterval
Data_105 = fitInterval(Data_105);
Data_110 = fitInterval(Data_110);
Data_115 = fitInterval(Data_115);
Data_120 = fitInterval(Data_120);
Data_125 = fitInterval(Data_125);
Data_130 = fitInterval(Data_130);
Data_135 = fitInterval(Data_135);
Data_140 = fitInterval(Data_140);


%% #3 Display the evoked response data
plot4_2_3(Data_110.fil_trials_ch36, 4000, [-2000, 4000*60], 0, 'Ch36');
plot4_2_3(Data_110.fil_trials_ch48, 4000, [-2000, 4000*60], 0, 'Ch48');
plot4_2_3(Data_110.fil_trials_ch56, 4000, [-2000, 4000*60], 0, 'Ch56');

%% #4 Display the detected spikes point on figure of the evoked response data
plot4_2_3(Data_110.fil_trials_ch36, 4000, [-2000, 4000*60], 1, 'Ch36');
plot4_2_3(Data_110.fil_trials_ch48, 4000, [-2000, 4000*60], 1, 'Ch48');
plot4_2_3(Data_110.fil_trials_ch56, 4000, [-2000, 4000*60], 1, 'Ch56');

%% #5 Draw raster plot starting with -20 msec
plot4_2_5(Data_105, 10.5);
plot4_2_5(Data_110, 11.0);
plot4_2_5(Data_115, 11.5);
plot4_2_5(Data_120, 12.0);
plot4_2_5(Data_125, 12.5);
plot4_2_5(Data_130, 13.0);
plot4_2_5(Data_135, 13.5);
plot4_2_5(Data_140, 14.0);

%% #6 Construct PSTH with bin size of 5 msec
cnt105 = plot4_2_6(Data_105, 10.5);
cnt110 = plot4_2_6(Data_110, 11.0);
cnt115 = plot4_2_6(Data_115, 11.5);
cnt120 = plot4_2_6(Data_120, 12.0);
cnt125 = plot4_2_6(Data_125, 12.5);
cnt130 = plot4_2_6(Data_130, 13.0);
cnt135 = plot4_2_6(Data_135, 13.5);
cnt140 = plot4_2_6(Data_140, 14.0);

%% (4.2.7) Obtain strength-response curve
cnt = [cnt105 cnt110 cnt115 cnt120 cnt125 cnt130 cnt135 cnt140];
plot4_2_7(cnt);

%% Define functions
function Data = loadTxt(path)
    % create struct
    loadData = importdata(path, '\t');
    f1 = 'nSweep';  v1 = loadData.data(:, 1);
    f2 = 'nTr';     v2 = loadData.data(:, 2);
    f3 = 'time';    v3 = loadData.data(:, 3);
    f4 = 'ch36';    v4 = loadData.data(:, 4);
    f5 = 'ch48';    v5 = loadData.data(:, 5);
    f6 = 'ch56';    v6 = loadData.data(:, 6);
    f7 = 'fil_ch36';v7 = zeros(size(v4));
    f8 = 'fil_ch48';v8 = zeros(size(v5));
    f9 = 'fil_ch56';v9 = zeros(size(v6));
    
    v2 = unique(v2);
    numEachData = zeros(1, 60);
    for i = 1:60
        numEachData(1, i) = nnz(v1==i);
    end
    f10 = 'trials_ch36'; v10 = zeros(max(numEachData)+3, 60); 
    f11 = 'trials_ch48'; v11 = zeros(size(v10));
    f12 = 'trials_ch56'; v12 = zeros(size(v10));
    f13 = 'fil_trials_ch36'; v13 = zeros(size(v10));
    f14 = 'fil_trials_ch48'; v14 = zeros(size(v10));
    f15 = 'fil_trials_ch56'; v15 = zeros(size(v10));
    v10(1, :) = numEachData; v10(2, :) = v2;
    v11(1, :) = numEachData; v11(2, :) = v2;
    v12(1, :) = numEachData; v12(2, :) = v2;
    v13(1, :) = numEachData; v13(2, :) = v2;
    v14(1, :) = numEachData; v14(2, :) = v2;
    v15(1, :) = numEachData; v15(2, :) = v2;
    Data = struct(f1, v1, f2, v2, f3, v3, f4, v4, f5, v5, f6, v6, f7, v7, f8, v8, f9, v9, f10, v10, f11, v11, f12, v12, f13, v13, f14, v14, f15, v15);
end

function Data = filtering(Data)
    % filter data with butterworth high-pass filter
    [B, A] = butter(2, 200/(50000/2), 'high');
    Data.fil_ch36 = filter(B, A, Data.ch36);
    Data.fil_ch48 = filter(B, A, Data.ch48);
    Data.fil_ch56 = filter(B, A, Data.ch56);
end

function Data = divideTrial(Data)
    % divide each trials
    Data.trials_ch36 = dividing(Data.ch36, Data.trials_ch36, Data.time);
    Data.trials_ch48 = dividing(Data.ch48, Data.trials_ch48, Data.time);
    Data.trials_ch56 = dividing(Data.ch56, Data.trials_ch56, Data.time);
    Data.fil_trials_ch36 = dividing(Data.fil_ch36, Data.fil_trials_ch36, Data.time);
    Data.fil_trials_ch48 = dividing(Data.fil_ch48, Data.fil_trials_ch48, Data.time);
    Data.fil_trials_ch56 = dividing(Data.fil_ch56, Data.fil_trials_ch56, Data.time);
    function divided = dividing(all, divided, time)
        k = 1;
        for i = 1:60
            divided(3, i) = time(k) - divided(2, i);
            for j = 4 : divided(1, i)+3
                divided(j,i) = all(k);
                k = k+1;
            end
        end
    end
end

function Data = fitInterval(Data)

    Data.trials_ch36 = fit(Data.trials_ch36);
    Data.trials_ch48 = fit(Data.trials_ch48);
    Data.trials_ch56 = fit(Data.trials_ch56);
    Data.fil_trials_ch36 = fit(Data.fil_trials_ch36);
    Data.fil_trials_ch48 = fit(Data.fil_trials_ch48);
    Data.fil_trials_ch56 = fit(Data.fil_trials_ch56);
    function trial = fit(trial)
        for i = 1:60
            if trial(3, i) == -100
                trial(4:6003, i) = trial(4004:10003, i);
                trial(6004:10003, i) = zeros(4000,1);
            else
                if trial(3, i) < -50
                    trial(4:6002, i) = trial(4005:10003, i);
                    trial(6003:10003, i) = zeros(4001,1);
                else
                    trial(496:6003, i) = trial(4:5511, i);
                    trial(4:495, i) = zeros(492, 1);
                end
            end
        end
    end
end

function trialData = plot4_2_3(trialData, interval, cutrange, showPeak, chName)
    figure;
    % plot from -20ms to +100ms
    x = -20: 0.02: 100-0.02;
    for i = 1:60
        y = trialData(4:6003, i);
        y = y + (60-i)*interval;
        plot(x, y, 'Color', [0 0 0]); hold on;
        if showPeak==1
            % show peak found with threshold
            [peak_val, peak_idx] = findpeaks(-trialData(4:6003, i), 'MinPeakHeight', -findThres(trialData(4:6003, i)));
            for j = 1:length(peak_idx)
                xx = x(peak_idx(j));
                plot(xx, -peak_val(j)+(60-i)*interval, 'o', 'MarkerSize', 4, 'Color', 'r'); hold on;
            end
        end
    end
    ylim(cutrange);
    xlim([-20, 100]);
    xlabel('time (msec)');
    yticks(0 : interval : 59*interval);
    for i = 1:60
        ylab(i) = {string('trial ' + string(61-i))};
    end
    yticklabels(ylab);
    if showPeak ==1
        title(string(chName) + ' detected spikes point on figure of the evoked response data');
    else
        title(string(chName) + ' the evoked response data');
    end
end

function thres = findThres(signalColumn)
    % find threshold
    M = mean(signalColumn(1501:6000));
    STD = std(signalColumn(1501:6000));
    thres = M - 4*STD;
end
    
function trialData = plot4_2_5(Data, name)
    figure;
    ch36 = Data.fil_trials_ch36;
    ch48 = Data.fil_trials_ch48;
    ch56 = Data.fil_trials_ch56;
    channels = [ch36 ch48 ch56];
    chname = ['ch36'; 'ch48'; 'ch56'];
    x = -20: 0.02: 100-0.02;
    % draw rasterplot on each trial, on each channel
    for k = 1:3
        subplot(1,3,k);
        trialData = channels(:, 60*(k-1)+1 : 60*k);
        for i = 1:60
            % find spikes
            [~, peak_idx] = findpeaks(-trialData(4:6003, i), 'MinPeakHeight', -findThres(trialData(4:6003, i)));
            for j = 1:length(peak_idx)
                xx = x(peak_idx(j));
                plot(xx, 61-i, 'marker', '|', 'MarkerSize', 5, 'Color', [0 0 0])
                hold on;
            end
        end
        for i = 1:60
            ylab(i) = {string('trial ' + string(61-i))};
        end
        yticks(1:60);
        yticklabels(ylab);
        xlim([-20,40])
        xlabel('time (msec)');
        ylim([-1,61])
        title(chname(k,:));
    end
    sgtitle('raster plot of ' + string(name) + 'microA');
end

function output = plot4_2_6(Data, name)
    figure;
    ch36 = Data.fil_trials_ch36;
    ch48 = Data.fil_trials_ch48;
    ch56 = Data.fil_trials_ch56;
    channels = [ch36 ch48 ch56];
    chname = ['ch36'; 'ch48'; 'ch56'];
    x = -20: 0.02: 100-0.02;
    xx = [];
    xxx = [];
    output = zeros(3,1);
    for k = 1:3
        subplot(1, 3, k);
        trialData = channels(:, 60*(k-1)+1 : 60*k);
        for i = 1:60
            % find peak indices
            [~, peak_idx] = findpeaks(-trialData(4:6003, i), 'MinPeakHeight', -findThres(trialData(4:6003, i)));
            xx = [xx x(peak_idx)];
            peak_idx = find(peak_idx > 1000);
            peak_idx = find(peak_idx < 3500);
            xxx = [xxx; peak_idx];
        end
        % draw PSTH
        histogram(xx, 'BinWidth', 5);
        xlim([-20, 40]);
        xlabel('time (msec)');
        title(chname(k,:));
        output(k, 1) = numel(xxx);
    end
    sgtitle('PSTH of ' + string(name) + 'microA');
end

function trialData = plot4_2_7(cnt)
    % draw strength-response data with spike count values
    figure;
    strength_36 = cnt(1, :)./60;
    strength_48 = cnt(2, :)./60;
    strength_56 = cnt(3, :)./60;
    plot(1:8, strength_36); hold on;
    plot(1:8, strength_48); hold on;
    plot(1:8, strength_56);
    xticklabels({'10.5uA', '11.0uA', '11.5uA', '12.0uA', '12.5uA', '13.0uA','13.5uA','14.0uA'});
    ylabel('total spike counts')
    title('Strength-response curves')
    legend('Ch36', 'Ch48', 'Ch56');
end

