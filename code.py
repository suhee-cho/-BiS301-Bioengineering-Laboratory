P_literature = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Parkinson_literature.txt", 'r', encoding="utf-8").read()
P_syno = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Parkinson_syno_dictionary.txt", 'r', encoding="utf-8")
gene_dictionary = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Gene_dictionary.txt", 'r', encoding="utf-8")

abstracts = dict()

temp = P_literature.split("PMID: ")
for item in temp:
    lines = item.split("\n")
    abstracts[lines[0]] = lines[1:]
for PMID in abstracts.keys():
    sentence = dict()
    for i in range(len(abstracts[PMID])):
        sentence[i+1] = abstracts[PMID][i].split()
    abstracts[PMID] = sentence


lines = [line.strip() for line in gene_dictionary.readlines()]
genes = dict()
for line in lines:
    line = line.split('\t')
    genes[line[0]] = line[1:]


for PMID in abstracts.keys():
    for line_idx in abstracts[PMID].keys():
#       abstracts[PMID][line_idx]

if genes.values[0] in P_literature

tagging_table = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Gene_tagging_table_Train_literature.txt","w+", encoding="utf-8")
tagging_table.write("GeneName\tPMID\tSentence index\tThe # of total tokens in sentence\tLocation of token in sentence\n")

for word in line_words:
    for gene in genes:
        if (gene[1] in word)|(gene[2] in word): tagging_table.write(gene[0]+"\t"+)