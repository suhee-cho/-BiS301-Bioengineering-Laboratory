%% load data and save it
[data,varnames,casenames] = tblread('data.txt', '\t'); % read raw data
class = string;
for i = 1:length(varnames)
    class(i) = string(varnames(i,:));
    class(i) = strip(class(i));
end
genes = string;
for i = 1:length(casenames)
    genes(i) = string(casenames(i,:));
    genes(i) = strip(genes(i));
end

clear varnames
clear casenames

%% normalize data and draw boxplots
norm_data = quantilenorm(data); % quantile normalization
rand_sample = randsample(91,10);
original_boxplot = figure;
boxplot(data(:,rand_sample)), title("Raw data"), xticklabels(class(rand_sample))
norm_boxplot = figure;
boxplot(norm_data(:,rand_sample)), title("Quantile normalized data"), xticklabels(class(rand_sample))

%% select discovery/validation dataset
idx_control = find(class == "control");
idx_tumor = find(class == "tumor");
dis_control = randsample(idx_control, 35);
dis_tumor = randsample(idx_tumor, 35);
val_control = randsample(setdiff(idx_control, dis_control), 10);
val_tumor = randsample(setdiff(idx_tumor, dis_tumor), 11);

%% calculate p-values and t-scores, and plot the graphs
%  Then and find DEGs which has p-values < 0.05
[PValues, TScores] = mattest(norm_data(:,dis_tumor),norm_data(:,dis_control), ...
    'Showhist',true,'Showplot',true);
sig_idx = find(PValues < 0.05);

%% create string arrays that contain column namee
d_tumor_var_names = string();
d_control_var_names = string();
for i = 1:length(dis_tumor)
    d_tumor_var_names(i) = strcat('dis_tumor_', num2str(i));
end
for i = 1:length(dis_control)
    d_control_var_names(i) = strcat('dis_control_', num2str(i));
end

v_tumor_var_names = string();
v_control_var_names = string();
for i = 1:length(val_tumor)
    v_tumor_var_names(i) = strcat('val_tumor_', num2str(i));
end
for i = 1:length(val_control)
    v_control_var_names(i) = strcat('val_control_', num2str(i));
end

%% create tables and merge them
d_tumor_table = array2table(norm_data(sig_idx,dis_tumor),'VariableNames',d_tumor_var_names);
d_control_table = array2table(norm_data(sig_idx,dis_control),'VariableNames',d_control_var_names);
v_tumor_table = array2table(norm_data(sig_idx,val_tumor),'VariableNames',v_tumor_var_names);
v_control_table = array2table(norm_data(sig_idx,val_control),'VariableNames',v_control_var_names);
pvalue_table = table(genes(sig_idx)', PValues(sig_idx),'VariableNames',["Probe","pvalue"]);

sig_gene_table = sortrows([pvalue_table,d_tumor_table,d_control_table,...
    v_tumor_table,v_control_table],'pvalue');

%% save tables as txt files
writetable(sig_gene_table(1:100,["Probe", "pvalue"]),'top 100 DEGs and p-value.txt','Delimiter', '\t')
writetable(sig_gene_table(1:100,startsWith(sig_gene_table.Properties.VariableNames,["Probe","dis"])), ...
    'top 100 DEGs expression data for discovery.txt','Delimiter', '\t')
writetable(sig_gene_table(1:100,startsWith(sig_gene_table.Properties.VariableNames,["Probe","val"])), ...
    'top 100 DEGs expression data for validation.txt','Delimiter', '\t')

%% draw qqplots for each dataset
qqplot_d_tumor = figure;
qqplot(mean(norm_data(:,dis_tumor))), title("QQ plot for average gene expression of discovery tumor samples")
qqplot_d_control = figure;
qqplot(mean(norm_data(:,dis_control))), title("QQ plot for average gene expression of discovery control samples")
qqplot_v_tumor = figure;
qqplot(mean(norm_data(:,dis_tumor))), title("QQ plot for average gene expression of validation tumor samples")
qqplot_v_control = figure;
qqplot(mean(norm_data(:,dis_tumor))), title("QQ plot for average gene expression of validation control samples")

qqplot_d_compare = figure;
qqplot(mean(norm_data(:,dis_tumor)), norm_data(:,dis_control)) ...
    , title("QQ plot for average gene expression of discovery samples")
xlabel("discovery tumor"), ylabel("discovery control")

qqplot_v_compare = figure;
qqplot(mean(norm_data(:,val_tumor)), norm_data(:,val_control)) ...
    , title("QQ plot for average gene expression of validation samples")
xlabel("validation tumor"), ylabel("validation control")

%% draw qqplots for significant DEGs
qqplot_DEG_1 = figure;
qqplot(norm_data(find(genes == sig_gene_table{1,["Probe"]}),:))
title("QQ plot for gene expression with smallest p-value")
ylabel(strcat("Quantiles of ", sig_gene_table{1,["Probe"]}, " expression"))

qqplot_DEG_2 = figure;
qqplot(norm_data(find(genes == sig_gene_table{2,["Probe"]}),:))
title("QQ plot for gene expression with smallest p-value")
ylabel(strcat("Quantiles of ", sig_gene_table{2,["Probe"]}, " expression"))

qqplot_DEG_3 = figure;
qqplot(norm_data(find(genes == sig_gene_table{3,["Probe"]}),:))
title("QQ plot for gene expression with smallest p-value")
ylabel(strcat("Quantiles of ", sig_gene_table{3,["Probe"]}, " expression"))

qqplot_DEG_4 = figure;
qqplot(norm_data(find(genes == sig_gene_table{4,["Probe"]}),:))
title("QQ plot for gene expression with smallest p-value")
ylabel(strcat("Quantiles of ", sig_gene_table{4,["Probe"]}, " expression"))

