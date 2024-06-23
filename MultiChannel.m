clear;
clc;
%% 导入文件所在路径
cd 'E:\桌面\竞赛、研学、评奖评优\2024生医工国赛\data\EOGdata' % 所有m文件、函数文件所在路径
mfile_path=pwd;
cd(mfile_path)

%% 读取文件，匹配列表
fid=fopen('dyf62001.log');
%跳过前三行
textscan(fid, '%*[^\n]', 3);
data=textscan(fid,'%s %f');
fclose(fid);
%% 读取目标列数据
emg_multi=data{2};
emg_lab=data{1};
cd(mfile_path)
emg_div = zeros(1000, 2);  % 初始化存放结果的矩阵
%%
length=zeros(1,2);
lab={'00>','01>'};
%%
for i = 1:numel(emg_multi)
    if isequal(emg_lab(i),lab(1)) 
        emg_div(length(1)+1,1)= emg_multi(i);
        length(1)=length(1)+1;

    elseif isequal(emg_lab(i),lab(2))
        emg_div(length(2)+1,2)= emg_multi(i);
        length(2)=length(2)+1;
    end
end

%% 小波滤波
%waveletType = 'db4';   good
waveletType='dmey';
level = 11;  % 分解层数可以根据你的数据调整

data=emg_div(~any(isnan(emg_div), 2), :);
data_filtered = zeros(size(data));  % 初始化过滤后的数据矩阵

for k = 1:2
    [C, L] = wavedec(data(:,k), level, waveletType);  % 小波分解
    C(1:L(1)) = 0;  % 去除最低频的近似系数
    data_filtered(:,k) = waverec(C, L, waveletType);  % 重构信号
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
head=16050;
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
up=zeros(segment_length,10);
interactive_signal_clipping(clip(:,1), segment_length);


%% 
function interactive_signal_clipping(clip, segment_length)
    % 创建图形界面
    f = figure('Name', 'Interactive Signal Clipping', 'NumberTitle', 'off', 'WindowState', 'maximized');
    ax = axes(f);
    plot(ax, clip);
    title(ax, 'Select start point of the UP motion');
    xlabel(ax, 'Sample Number');
    ylabel(ax, 'Signal Amplitude');
    % 添加交互按钮
    uicontrol('Style', 'pushbutton', 'String', 'Select Segment', 'Position', [20 20 100 40], 'Callback', @select_segment);
    uicontrol('Style', 'pushbutton', 'String', 'Finish', 'Position', [140 20 100 40], 'Callback', @finish_selection);
    
    % 用于存储选择的片段
    selected_segments = {};
    
    function select_segment(~, ~)
        [x, ~] = ginput(1); % 选择起始点
        start_index = round(x); % 将起始点四舍五入到最近的整数索引
        end_index = start_index + segment_length - 1;
        
        if end_index > length(clip)
            msgbox('End index exceeds clip length. Please select again.', 'Error', 'error');
            return;
        end
        
        segment = clip(start_index:end_index);
        
        % 显示选择的片段
        figure;
        plot(segment);
        title(['Segment from index ' num2str(start_index) ' to ' num2str(end_index)]);
        xlabel('Sample Number');
        ylabel('Signal Amplitude');
        
        % 让用户决定是否保留该片段
        choice = questdlg('Keep this segment?', 'Segment Selection', 'Yes', 'No', 'No');
        
        if strcmp(choice, 'Yes')
            selected_segments{end+1} = segment; % 添加到选中的片段中
        end
        
        close(gcf); % 关闭片段显示图
    end

    function finish_selection(~, ~)
        % 将选择的片段保存到矩阵中
        if ~isempty(selected_segments)
            segments = cell2mat(selected_segments');
        else
            segments = [];
        end
        % 将选择的片段保存到工作区变量
        assignin('base', 'up', segments);
        close(f);
    end
end