%% Design filters
% Define global variable for filters
global b_n a_n;
global b_n2 a_n2;
global b_a a_a;
global b_b a_b;
global b_t a_t;

% create four butterworth filters for filtering noise which is not from 
brain,
% noise from electronic devices,
% ...and differentiate theta, alpha, beta band
[b_n,a_n]=butter(3,[0.01,40]/100,'bandpass'); % Noise filter
[b_n2,a_n2] = butter(7, [59 61]/100, 'stop'); % Notch filter
[b_t,a_t]=butter(3,[4 7]/100,'bandpass');% Theta filter
[b_a,a_a]=butter(5,[8 13]/100,'bandpass'); % Alpha filter
[b_b,a_b]=butter(7,[14 30]/100,'bandpass'); % Beta filter

%% Import Data
% import data, save it as a struct, and apply filter
mainpath = 'C:\Users\SuheeCho\Desktop\2021_Spring\Lab1\my_finallab\lab4_mainlab data';
exp1 = loadEEG(mainpath + "\Exp1.txt");
exp2 = loadEEG(mainpath + "\Exp2.txt");
exp3 = loadEEG(mainpath + "\Exp3.txt");
exp1_marks = importdata(mainpath + "\Exp1_marks.txt", '\t');
exp2_marks = importdata(mainpath + "\Exp2_marks.txt", '\t');
exp3_marks = importdata(mainpath + "\Exp3_marks.txt", '\t');

%% Exp1 marks
% create markname and marktime variables
% which contain information of marker
global exp1_markname;
global exp1_marktime;

exp1_markname = exp1_marks.textdata(2:29, 1);
exp1_mark = exp1_marks.textdata(2:29, 2);
exp1_marktime = zeros(28,1);
exp1_markidx = zeros(28,1);

for i = 1:28
    exp1_marktime(i) = str2double(exp1_mark{i});
    exp1_markidx(i) = exp1_marktime(i) * 200 + 1;
end

%% Exp2 marks
global exp2_markname;
global exp2_marktime;

exp2_markname = exp2_marks.textdata(2:7, 1);
exp2_mark = exp2_marks.textdata(2:7, 2);
exp2_marktime = zeros(6,1);

for i = 1:6
    exp2_marktime(i) = str2double(exp2_mark{i});
end

%% Exp3 marks
global exp3_markname;
global exp3_marktime;

exp3_markname = exp3_marks.textdata(2:7, 1);
exp3_mark = exp3_marks.textdata(2:7, 2);
exp3_marktime = zeros(6,1);

for i = 1:6
    exp3_marktime(i) = str2double(exp3_mark{i});
end

%% plot raw vs. filtered EEG signals (4.7.1)
raw_filtered(exp1.LEEG, exp1.Time, 'Exp1 left', 1);
raw_filtered(exp1.REEG, exp1.Time, 'Exp1 right', 1);
raw_filtered(exp2.LEEG, exp2.Time, 'Exp2 left', 2);
raw_filtered(exp2.REEG, exp2.Time, 'Exp2 right', 2);
raw_filtered(exp3.LEEG, exp3.Time, 'Exp3 left', 3);
raw_filtered(exp3.REEG, exp3.Time, 'Exp3 right', 3);

%% plot all EEG signals (4_7_2)
f_block_plot(exp1.LEEG, exp1.Time, 'Exp1 left', 1);
f_block_plot(exp1.REEG, exp1.Time, 'Exp1 right', 1);
f_block_plot(exp2.LEEG, exp2.Time, 'Exp2 left', 2);
f_block_plot(exp2.REEG, exp2.Time, 'Exp2 right', 2);
f_block_plot(exp3.LEEG, exp3.Time, 'Exp3 left', 3);
f_block_plot(exp3.REEG, exp3.Time, 'Exp3 right', 3);

%% Compare own filter vs. given data (4_7_2)
own_and_given(exp1, 'left', 'alpha', 1, 400);
own_and_given(exp1, 'left', 'beta', 1, 250);
own_and_given(exp1, 'right', 'alpha', 1, 450);
own_and_given(exp1, 'right', 'beta', 1, 350);
own_and_given(exp2, 'left', 'alpha', 2, 50);
own_and_given(exp2, 'left', 'beta', 2, 50);
own_and_given(exp2, 'right', 'alpha', 2, 50);
own_and_given(exp2, 'right', 'beta', 2, 40);
own_and_given(exp3, 'left', 'alpha', 3, 30);
own_and_given(exp3, 'left', 'beta', 3, 30);
own_and_given(exp3, 'right', 'alpha', 3, 40);
own_and_given(exp3, 'right', 'beta', 3, 30);

%% Spectral analysis
plot4_7_3(exp2.Time, exp2.LEEG, 'Left EEG in Exp2');
plot4_7_3(exp2.Time, exp2.REEG, 'Right EEG in Exp2');
plot4_7_3(exp3.Time, exp3.LEEG, 'Left EEG in Exp3');
plot4_7_3(exp3.Time, exp3.REEG, 'Right EEG in Exp3');

%% Define functions
% import ExpN.txt, make struct and save time, raw L and R EEG as a vector.
% save alpha/beta wave and frequency as a struct
% that contains wave at first column and freq at second column.
% Then apply filter and save noise filtered data (fil), alpha wave (A),
% beta wave (B), and theta wave (C).
function Data = loadEEG(path)
    global b_n a_n;
    global b_n2 a_n2;
    global b_a a_a;
    global b_b a_b;
    global b_t a_t;
 
    loadData = importdata(path, '\t');
    v = loadData.data;
    lst = struct('A', v(:, 4:5), 'B', v(:, 6:7));
    rst = struct('A', v(:, 8:9), 'B', v(:, 10:11));
    Data = struct('Time', v(:,1), 'rawLEEG', v(:,2), 'rawREEG', v(:,3), ...
    'left', lst, 'right', rst);
    filtered_LEEG = filter(b_n, a_n, Data.rawLEEG);
    filtered_LEEG = filter(b_n2, a_n2, filtered_LEEG);
    disp('number of abnormal data in LEEG...: ' + 
        string(find_abnormal_signal(filtered_LEEG)));
 
    LEEG_A = filter(b_a, a_a, filtered_LEEG);
    LEEG_B = filter(b_b, a_b, filtered_LEEG);
    LEEG_T = filter(b_t, a_t, filtered_LEEG);
    LEEG = struct('raw', Data.rawLEEG, 'fil', filtered_LEEG , 'A', LEEG_A, 
    'B', LEEG_B , 'T', LEEG_T);
    
    filtered_REEG = filter(b_n, a_n, Data.rawREEG);
    filtered_REEG = filter(b_n2, a_n2, filtered_REEG);
    disp('number of abnormal data in REEG...: ' + 
    string(find_abnormal_signal(filtered_REEG)));
 
    REEG_A = filter(b_a, a_a, filtered_REEG);
    REEG_B = filter(b_b, a_b, filtered_REEG);
    REEG_T = filter(b_t, a_t, filtered_REEG);
    REEG = struct('raw', Data.rawREEG, 'fil', filtered_REEG , 'A', REEG_A, 
    'B', REEG_B , 'T', REEG_T);
    
    Data.LEEG = LEEG;
    Data.REEG = REEG;
    Data = rmfield(Data, {'rawLEEG', 'rawREEG'});
end

% plot raw EEG and noise filtered EEG
function Data = raw_filtered(Data, time, Title, num)
    fig = figure;
    ax1 = subplot(2,1,1);
    plot(time, Data.raw, 'Color', 'r');

    apply_marker(ax1, num); title('raw EEG');
    xlim([0, time(end)]);
    
    ax2 = subplot(2,1,2);
    plot(time, Data.fil, 'Color', 'black');
    apply_marker(ax2, num); title('noise filtered EEG');
    xlim([0, time(end)]);
    
    tot = axes(fig, 'visible', 'off');
    tot.Title.Visible = 'on';
    tot.XLabel.Visible = 'on';
    tot.YLabel.Visible = 'on';
    
    sgtitle(Title);
    xlabel(tot, 'Time (s)');
    ylabel(tot, 'Amplitude (microV)');
end

% find abnormal signal which has frequency which agrees normal EEG waveabnor 
pattern
% but has abnormally high amplitude
function abnormal_data = find_abnormal_signal(filEEG)
    Fourier = fft(filEEG);
    Fs = 200;
    L = length(Fourier);
    freq = Fs*(0:L-1)/L;
    alpha_interval = find((freq > 8)&(freq < 13));
    beta_interval = find((freq > 14)&(freq < 30));
    amp_a = 2*abs(Fourier(alpha_interval)/L);
    amp_b = 2*abs(Fourier(beta_interval)/L);
    
    abnormal_data = 0;
    for i = 1:length(alpha_interval)
        if amp_a(i)>100
            abnormal_data = abnormal_data+1;
        end
    end
    for i = 1:length(beta_interval)
        if amp_b(i)>25
            abnormal_data = abnormal_data+1;
        end
    end
end

% plot EEG data of alpha, beta and theta
% time = time data on experiment
% title = desired name of plot title
function Data = f_block_plot(Data, time, Title, num)
    fig = figure;
    ax1 = subplot(3,1,1);
    plot(time, Data.A, 'Color', 'b');
    apply_marker(ax1, num);
    title('alpha wave');
    xlim([time(1) time(end)])
    
    ax2 = subplot(3,1,2);
    plot(time, Data.B, 'Color', 'b');
    apply_marker(ax2, num);
    xlim([time(1) time(end)])

    title('beta wave');
    
    ax3 = subplot(3,1,3);
    plot(time, Data.T);
    apply_marker(ax3, num);
    xlim([time(1) time(end)])
    title('theta wave');
    
    sgtitle(Title);
    tot = axes(fig, 'visible', 'off');
    tot.Title.Visible = 'on';
    tot.XLabel.Visible = 'on';
    tot.YLabel.Visible = 'on';
    xlabel(tot, 'Time (s)');
    ylabel(tot, 'Amplitude (microV)');
end

% direction = 'left' or 'right'
% wave = 'alpha' or 'beta'
% num represents exp #
% plot data filtered by own filter and data which was given by file
function Data = own_and_given(Data, direction, wave, num, lim)
    global b_a a_a;
    global b_b a_b;
    if strcmp(direction, 'left')
        orig = Data.left; own = Data.LEEG;
    else
        orig = Data.right; own = Data.REEG;
    end
    
    if strcmp(wave, 'alpha')
        orig = filter(b_a, a_a, orig.A(:,1)); own = own.A;
    else
        orig = filter(b_b, a_b, orig.B(:,1)); own = own.B;
    end
 
    fig = figure;
    subplot(3,1,1);
    plot(Data.Time, own, 'Color', 'r');
    xlim([0, max(Data.Time)])
    ylim([-lim, lim])
    title('signal from own filter');
    
    subplot(3,1,2);
    plot(Data.Time, orig, 'Color', 'black');
    xlim([0, max(Data.Time)])
    ylim([-lim, lim])
    title('signal from original array');
    
    subplot(3,1,3);
    plot(Data.Time, orig, 'Color', 'black');
    hold on
    plot(Data.Time, own, 'Color', 'r');
    xlim([0, max(Data.Time)])
    ylim([-lim, lim])
    title('overlapped signal');
    
    tot = axes(fig, 'visible', 'off');
    tot.Title.Visible = 'on';
    tot.XLabel.Visible = 'on';
    tot.YLabel.Visible = 'on';

    xlabel(tot, 'Time (s)');
    ylabel(tot, 'Amplitude (microV)');
    sgtitle(string(wave) + ' wave for ' + string(direction) + ' EEG in Exp' + 
    string(num)) ;
end

% plot marks which have previously generated
function ax = apply_marker(ax, exp_num)
    if exp_num == 1
        mark_1(ax);
    elseif exp_num == 2
        mark_2(ax);
    else
        mark_3(ax);
    end
end

% create markers
function ax = mark_1(ax)
    global exp1_markname;
    global exp1_marktime;
    for i = 1:28
        xline(exp1_marktime(i), '--b', exp1_markname(i));
    end
end

function ax = mark_2(ax)
 global exp2_markname;
 global exp2_marktime;
 for i = 1:6
 xline(exp2_marktime(i), '--b', exp2_markname(i));
 end
end
function ax = mark_3(ax)
    global exp3_markname;
    global exp3_marktime;
    for i = 1:6
        xline(exp3_marktime(i), '--b', exp3_markname(i));
    end
end

% plot 4_7_3 for spectral analysis
% draw plot on spectral domain with calculating PSD value
function Data = plot4_7_3(t_scale, Data, Title)
    Fs = 200;
    L = length(Data.raw);
    f = Fs*(0:L-1)/L;
    
    figure;
    subplot(4,2,1); plot(t_scale, Data.fil);
    xlim([0 t_scale(end)])
    title('EEG signal (t)')
    
    subplot(4,2,3);
    plot(t_scale, Data.T);
    title('theta wave (t)')
    xlim([0 t_scale(end)])
    ylim([-50 50])
    
    subplot(4,2,5);

    plot(t_scale, Data.A);
    title('alpha wave (t)')
    xlim([0 t_scale(end)])
    ylim([-50 50])
    
    subplot(4,2,7);
    plot(t_scale, Data.B);
    title('beta wave (t)')
    xlim([0 t_scale(end)])
    ylim([-50 50])
    xlabel('Time (s)');
    
    subplot(4,2,2); plot(f, log10(abs(fft(Data.fil).^2))); xlim([0 100]);
    title('EEG signal (f)');
    
    subplot(4,2,4); plot(f, log10(abs(fft(Data.T).^2))); xlim([0 100]);
    title('theta wave (f)');
    
    subplot(4,2,6); plot(f, log10(abs(fft(Data.A).^2))); xlim([0 100]);
    title('alpha wave (f)');
    
    subplot(4,2,8); plot(f, log10(abs(fft(Data.B).^2))); xlim([0 100]);
    title('beta wave (f)');
    xlabel('Frequeny (Hz)');
    sgtitle(Title);
end