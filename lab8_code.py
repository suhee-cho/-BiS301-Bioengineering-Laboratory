# Open given files
P_literature = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Parkinson_literature.txt", 'r', encoding="utf-8").readlines()
P_syno = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Parkinson_syno_dictionary.txt", 'r', encoding="utf-8")
gene_dictionary = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Gene_dictionary.txt", 'r', encoding="utf-8").readlines()

# create new file for gene tagging table, and write column names in it
G_tagging_table = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Gene_tagging_table_Train_literature.txt","w+", encoding="utf-8")
G_tagging_table.write("GeneName\tPMID\tSentence index\tThe # of total tokens\tLocation of token\n")

# create new file for disease tagging table, and write column names in it
D_tagging_table = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Disease_tagging_table_Test_literature.txt","w+", encoding="utf-8")
D_tagging_table.write("DiseaseName\tPMID\tSentence index\tThe # of total tokens\tLocation of token\n")


# create class to record matched words in abstracts.
# class has name, id, PMID, s_idx, tot_tok, loc_tok attributes.
class Word:
    def __init__(self, name, id, PMID, s_idx, tot_tok, loc_tok):
        self.name = name
        self.id = id
        self.PMID = PMID
        self.s_idx = s_idx
        self.tot_tok = tot_tok
        self.loc_tok = loc_tok
    # simple method to create list of alphabets.
    def get_spelling(self):
        return list(self.name)


def create_tagging_table(table, target, reference): 
    # create dictionary of "target".
    # with key as a gene (or disease) id_val, and values as its synonym.
    targets = dict()
    for line in target:
        line = line.strip()
        line = line.split('\t')
        targets[line[0]] = line[1:]
    # go through lines of abstracts
    line_idx = 1 # record line number in each abstract
    word_list = []
    for line in reference:
        if "PMID" in line: # if line starts with PMID, it means the line if of PMID
            PMID = line.split(": ")[1]
            PMID = PMID.strip() # save PMID
            line_idx = 1 # reset line counting
            # print("new PMID")
        else:
            line_idx += 1
            line = line.strip()
            line = line.replace(".", "") # remove period(.)
            words = line.split()
            for id_val in targets.keys():
                # find gene name saved in dictionary as a values
                # utilize try-except statement. If the word does not match with gene name,
                # error occurs and the data of the word will not be saved
                for i in range(len(targets[id_val])):
                    try:
                        # find the index of word if it exists
                        word_idx = words.index(targets[id_val][i])
                        row = str(id_val)+"\t"+str(PMID)+"\t"+str(line_idx)+"\t"+str(len(words))+'\t'+str(word_idx+1)+'\n'
                        table.write(row) # if gene name is identified, record it
                        # if it does exist, save it at word_list list as a Word class
                        word_list.append(Word(words[word_idx], id_val, PMID, line_idx, len(words), word_idx+1))
                        # create text row to be saved in tagging table
                    except: pass # if the word does not exist in sentence, pass
    print("create tagging table... done.")
    return word_list

# complete tagging table and get list of detected word objects
g_list = create_tagging_table(G_tagging_table, gene_dictionary, P_literature)
d_list = create_tagging_table(D_tagging_table, P_syno, P_literature)

# finally, close file
G_tagging_table.close()
D_tagging_table.close()

def score_function(word_1, word_2):
    # if words are not in same abstract, score is 0
    if word_1.PMID != word_2.PMID: return 0
    # if words are not in same or very next sentence, score is 0
    if abs(word_1.s_idx - word_2.s_idx) > 2: return 0
    # tok_num represents number of tokens between word_1 and word_2
    # sen_var represents whether words are in same sentence or very next sentence
    if (word_1.s_idx > word_2.s_idx): # when word_2 locates in former sentence..
        tok_num = word_1.loc_tok + (word_2.tot_tok - word_2.loc_tok)
        sen_var = 1
    elif (word_1.s_idx < word_2.s_idx): # when word_1 locates in former sentence..
        tok_num = (word_1.tot_tok - word_1.loc_tok) + word_2.loc_tok
        sen_var = 1
    else:
        tok_num = abs(word_1.loc_tok - word_2.loc_tok)
        sen_var = 2
    # sp_sim represents spelling similarity
    # 1 <= sp_sim <= 2
    spell_1 = word_1.get_spelling() # get list of characters of word_1
    spell_2 = word_2.get_spelling() # get list of characters of word_2
    if (len(spell_1) > len(spell_2)):
        # calculate the ratio of the common characters between two words,
        # multiply 0.1, and add 1
        sp_sim = 1 + 0.1*len(list(set(spell_1).intersection(spell_2)))/len(spell_1)  
    return (sen_var*sp_sim)/tok_num

gene_and_score = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\gene_and_score.txt","w+", encoding="utf-8")
gene_and_score.write("Gene\tscore\n")

gene_score_dict = dict()
for gene in g_list:
    for dis in d_list:
        if gene.id in gene_score_dict.keys(): gene_score_dict[gene.id] += score_function(dis, gene)
        else: gene_score_dict[gene.id] = score_function(dis, gene)

for gene in gene_score_dict.keys():
    gene_and_score.write(str(gene) + "\t" + str(gene_score_dict[gene]) + "\n")

gene_and_score.close()
