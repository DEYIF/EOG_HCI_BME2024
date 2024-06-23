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





