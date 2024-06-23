clear;
clc;
%% 导入文件所在路径
cd 'E:\桌面\竞赛、研学、评奖评优\2024生医工国赛\data\EOGdata' % 所有m文件、函数文件所在路径
mfile_path=pwd;
cd(mfile_path)

%% 读取文件，匹配列表
fid=fopen('dyf60703.log');
%跳过前三行
textscan(fid, '%*[^\n]', 3);
data=textscan(fid,'%s %f');
fclose(fid);
%% 读取目标列数据
emg_multi=data{2};
emg_lab=data{1};
cd(mfile_path)
emg_div = zeros(1000, 2);  % 初始化存放结果的矩阵
length=zeros(1,2);
lab={'00>','01>'};

for i = 1:numel(emg_multi)
    if isequal(emg_lab(i),lab(1)) 
        emg_div(length(1)+1,1)= emg_multi(i);
        length(1)=length(1)+1;

    elseif isequal(emg_lab(i),lab(2))
        emg_div(length(2)+1,2)= emg_multi(i);
        length(2)=length(2)+1;
    end
end

%% 滤波
%小波滤波
%waveletType = 'db4';   good
waveletType='dmey';
level = 11;  % 分解层数可以根据你的数据调整

data=emg_div(~any(isnan(emg_div), 2), :);
data_filtered = zeros(size(data));  % 初始化过滤后的数据矩阵
temp1 = zeros(size(data));
temp2 = zeros(size(data));

for k = 1:2
    [C, L] = wavedec(data(:,k), level, waveletType);  % 小波分解
    C(1:L(1)) = 0;  % 去除最低频的近似系数
    temp1(:,k) = waverec(C, L, waveletType);  % 重构信号
end

%带阻滤波
fs=250;
% 设计FIR带阻滤波器
freq_band = [40 60]; % 带阻频率范围
filter_order = 500; % 滤波器阶数，具体阶数可根据需要调整
nyquist = fs / 2; % 奈奎斯特频率
bandstop_freq = freq_band / nyquist; % 归一化频率

% 设计带阻FIR滤波器
b = fir1(filter_order, bandstop_freq, 'stop');

% 对每一列信号进行带阻滤波
for i = 1:2
    temp2(:, i) = filtfilt(b, 1, temp1(:, i));
end

% 设计FIR带阻滤波器
freq_band = [90 110]; % 带阻频率范围
filter_order = 500; % 滤波器阶数，具体阶数可根据需要调整
nyquist = fs / 2; % 奈奎斯特频率
bandstop_freq = freq_band / nyquist; % 归一化频率

% 设计带阻FIR滤波器
b = fir1(filter_order, bandstop_freq, 'stop');

% 对每一列信号进行带阻滤波
for i = 1:2
    data_filtered(:, i) = filtfilt(b, 1, temp2(:, i));
end

% 滤波前后对比
figure(1);
subplot(2,2,1)
plot(emg_div(1:end,1));
xlabel('t/s');
ylabel('V/μV');
title('channel-1滤波前');

subplot(2,2,3)
plot(data_filtered(1:end,1));
xlabel('t/s');
ylabel('V/μV');
title('channel-1滤波后');

subplot(2,2,2)
plot(emg_div(:,2));
xlabel('t');
ylabel('μV');
title('channel-2滤波前');

subplot(2,2,4)
plot(data_filtered(:,2));
xlabel('t');
ylabel('μV');
title('channel-2滤波后');

%% 去头去尾
head=10000;
tail=102300;
clip=data_filtered(head:tail,:);
figure('Name','去头去尾');
subplot(2,1,1)
plot(clip(1:end,1));
xlabel('t/s');
ylabel('V/μV');
title('channel-1');

subplot(2,1,2)
plot(clip(1:end,2));
xlabel('t/s');
ylabel('V/μV');
title('channel-2');
%% 交互式分段截取
segment_length = 700; % 每个片段的长度
motion='down';    %选择次次分割的动作名称
interactive_signal_clipping(clip, segment_length,motion);
%% 
function segClip = interactive_signal_clipping(clip, segment_length,motion)
    % 创建图形界面
    f = figure('Name', 'Interactive Signal Clipping', 'NumberTitle', 'off', 'WindowState', 'maximized');
    ax1 = subplot(2, 1, 1, 'Parent', f);
    plot(ax1, clip(:,1));
    title(ax1, 'Channel 1: Select start point of the segment');
    xlabel(ax1, 'Sample Number');
    ylabel(ax1, 'Signal Amplitude');
    
    ax2 = subplot(2, 1, 2, 'Parent', f);
    plot(ax2, clip(:,2));
    title(ax2, 'Channel 2: Select start point of the segment');
    xlabel(ax2, 'Sample Number');
    ylabel(ax2, 'Signal Amplitude');
    
    % 用于存储选择的片段
    segClip = [];
    
    % 添加交互按钮
    uicontrol('Style', 'pushbutton', 'String', 'Select Segment', 'Position', [20 20 100 40], 'Callback', @select_segment);
    uicontrol('Style', 'pushbutton', 'String', 'Finish', 'Position', [140 20 100 40], 'Callback', @finish_selection);
    
    function select_segment(~, ~)
        [x, ~] = ginput(1); % 选择起始点
        start_index = round(x); % 将起始点四舍五入到最近的整数索引
        end_index = start_index + segment_length - 1;
        
        if end_index > size(clip, 1)
            msgbox('End index exceeds data length. Please select again.', 'Error', 'error');
            return;
        end
        
        segment1 = clip(start_index:end_index, 1);
        segment2 = clip(start_index:end_index, 2);
        
        % 显示选择的片段
        figure;
        subplot(2, 1, 1);
        plot(segment1);
        title(['Channel 1: Segment from index ' num2str(start_index) ' to ' num2str(end_index)]);
        xlabel('Sample Number');
        ylabel('Signal Amplitude');
        
        subplot(2, 1, 2);
        plot(segment2);
        title(['Channel 2: Segment from index ' num2str(start_index) ' to ' num2str(end_index)]);
        xlabel('Sample Number');
        ylabel('Signal Amplitude');
        
        % 让用户决定是否保留该片段
        choice = questdlg('Keep this segment?', 'Segment Selection', 'Yes', 'No', 'No');
        
        if strcmp(choice, 'Yes')
            new_segment = [segment1, segment2]; % 将两个通道的片段合并为2行矩阵
            if isempty(segClip)
                segClip = new_segment; % 初始化矩阵
            else
                segClip = [segClip, new_segment]; % 将新片段添加为新列
            end
        end
        
        close(gcf); % 关闭片段显示图
    end

    function finish_selection(~, ~)
        % 关闭图形界面
        close(f);
        disp('Selected segments stored in the variable "segClip".');
        assignin('base', motion, segClip); % 将结果保存到工作区变量
    end
end