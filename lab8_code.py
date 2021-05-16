import os
# Before we start, for the sake of convenience...
if os.path.exists("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Gene_tagging_table_Train_literature.txt"):
    os.remove("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Gene_tagging_table_Train_literature.txt")
if os.path.exists("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Disease_tagging_table_Train_literature.txt"):
    os.remove("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Disease_tagging_table_Train_literature.txt")
if os.path.exists("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\E_Gene_tagging_table_Train_literature.txt"):
    os.remove("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\E_Gene_tagging_table_Train_literature.txt")
if os.path.exists("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\E_Disease_tagging_table_Train_literature.txt"):
    os.remove("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\E_Disease_tagging_table_Train_literature.txt")

# Open given files.
P_literature = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Parkinson_literature.txt", 'r', encoding="utf-8").readlines()
P_syno = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Parkinson_syno_dictionary.txt", 'r', encoding="utf-8")
gene_dictionary = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Gene_dictionary.txt", 'r', encoding="utf-8").readlines()

# create new file for gene tagging table, and write column names in it.
G_tagging_table = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Gene_tagging_table_Train_literature.txt","w+", encoding="utf-8")
G_tagging_table.write("GeneName\tPMID\tSentence index\tThe # of total tokens\tLocation of token\n")

# create new file for disease tagging table, and write column names in it.
D_tagging_table = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\Disease_tagging_table_Train_literature.txt","w+", encoding="utf-8")
D_tagging_table.write("DiseaseName\tPMID\tSentence index\tThe # of total tokens\tLocation of token\n")

# ...and extended versions of them.
E_G_tagging_table = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\E_Gene_tagging_table_Train_literature.txt","w+", encoding="utf-8")
E_G_tagging_table.write("Name\tGeneName\tPMID\tSentence index\tThe # of total tokens\tLocation of token\twhich abbreviation\n")
E_D_tagging_table = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\E_Disease_tagging_table_Train_literature.txt","w+", encoding="utf-8")
E_D_tagging_table.write("Name\tDiseaseName\tPMID\tSentence index\tThe # of total tokens\tLocation of token\twhich abbreviation\n")

# create class to record matched words in abstracts.
# class has name, id, PMID, s_idx, tot_tok, loc_tok attributes.
class Word:
    def __init__(self, name, id, PMID, s_idx, tot_tok, loc_tok, abb):
        self.name = name
        self.id = id
        self.PMID = PMID
        self.s_idx = s_idx
        self.tot_tok = tot_tok
        self.loc_tok = loc_tok
        self.abb = abb
    # simple method to create list of alphabets.
    def get_spelling(self):
        return list(self.name)

################# This part is for prelab #################
g_occurance = dict()

def create_tagging_table(table, E_table, target, reference, Disease): 
    # Let's start!
    print("create tagging table... ", end = '')

    # create dictionary of "target".
    # with key as a gene (or disease) id_val, and values as its synonym.
    targets = dict()
    for line in target:
        line = line.strip()
        line = line.split('\t')
        targets[line[0]] = line[1:]

    # Now, go through lines of abstracts.
    line_idx = 1 # record line number in each abstract.
    word_list = [] # it will contain found words in Word class.
    for line in reference:
        if "PMID" in line: # if line starts with PMID, it means the line is of PMID.
            PMID = line.split(": ")[1]
            PMID = PMID.strip() # save PMID.
            line_idx = 1 # reset line count.
            # print("new PMID")
        else:
            # Among the lines which are about main contents,
            # find gene names saved in dictionary as a values.
            line_idx += 1
            for id_val in targets.keys():
                # First, tidy the line.
                line = line.strip()
                line = line.replace(".", "") # remove period(.)
                if Disease: line = line.lower() # Let's convert string to lowercase for easy comparison.
                words = line.split()

                # Then, loop through the target words.
                for i in range(len(targets[id_val])):
                    if Disease: target_word = targets[id_val][i].lower()
                    else: target_word = targets[id_val][i]
                    # target word may contain space, so split it.
                    target_list = target_word.split()

                    # Then, utilize try-except statement.
                    # If the word does not match with gene name,
                    # error occurs and the data of the word will not be saved.
                    try:
                        # find the index of word if it exists.
                        word_idx = words.index(target_list[0])
                        word_match = True
                        # determine whether the whole word matches.
                        for j in range(len(target_list)):
                            if (target_list[j] != words[word_idx+j]):
                                word_match = False
                                break
                        if word_match: # if we find the right word...
                            row = str(id_val)+"\t"+str(PMID)+"\t"+str(line_idx)+"\t"+str(len(words))+'\t'+str(word_idx+1)
                            table.write(row+'\n') # record it!
                            if len(targets[id_val])==1: E_table.write(target_word+'\t'+row+'\t'+str(2)+'\n')
                            else: E_table.write(target_word+'\t'+row+'\t'+str(i)+'\n')
                            # and save it at word_list list as a Word class.
                            word_list.append(Word(words[word_idx], id_val, PMID, line_idx, len(words), word_idx+1, i))
                            if not Disease & (len(targets[id_val]!=1)):
                                try:
                                    g_occurance[id_val].append(i)
                                except: g_occurance[id_val] = [i]
                    except: pass # if the word does not exist in sentence, try next word.
    print("DONE.")
    return word_list

# complete tagging table and get list of detected word objects.
g_list = create_tagging_table(G_tagging_table, E_G_tagging_table, gene_dictionary, P_literature, False)
d_list = create_tagging_table(D_tagging_table, E_D_tagging_table, P_syno, P_literature, True)

# finally, close files.
G_tagging_table.close()
D_tagging_table.close()



################# This part is for mainlab #################

###########################################################
########## for those who start from the middle ############
###### If you already have extended tagging tables, #######
######### You can just launch from the part below! ########
############ Making table takes too much time! ############
######## However, if you don't have tagging table, ########
############# skip the code block below! ##################
###########################################################
class Word:
    def __init__(self, name, id, PMID, s_idx, tot_tok, loc_tok, abb):
        self.name = name
        self.id = id
        self.PMID = PMID
        self.s_idx = s_idx
        self.tot_tok = tot_tok
        self.loc_tok = loc_tok
        self.abb = abb
    # simple method to create list of alphabets.
    def get_spelling(self):
        return list(self.name)

E_G_tagging_table = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\E_Gene_tagging_table_Train_literature.txt","r", encoding="utf-8").readlines()
E_D_tagging_table = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\E_Disease_tagging_table_Train_literature.txt","r", encoding="utf-8").readlines()

g_list = []
d_list = []
g_occurance = dict()

def make_Word(target, result, gene):
    for line in target:
        if 'Name' in line: continue
        line = line.strip()
        words_list = line.split('\t')
        result.append(Word(words_list[0], words_list[1], words_list[2], int(words_list[3]), int(words_list[4]), int(words_list[5]), int(words_list[6])))
        if gene:
            try: g_occurance[words_list[1]].append(int(words_list[6]))
            except: g_occurance[words_list[1]] = [int(words_list[6])]
    return result

g_list = make_Word(E_G_tagging_table, g_list, True)
d_list = make_Word(E_D_tagging_table, d_list, False)

#########################################################

def score_function(word_1, word_2):
    # if words are not in same abstract, score is 0.
    if word_1.PMID != word_2.PMID: return 0
    # tok_num is an additional score for when words are in same sentence.
    # it represents how near the word_1 and word_2 are.
    # sen_var represents who many sentences are b/w two words.
    if (word_1.s_idx == word_2.s_idx): # when two words are three sentences far..
        tok_num = 5*(word_1.tot_tok)/abs(word_1.loc_tok - word_2.loc_tok)
        sen_var = 200
    elif (abs(word_1.s_idx - word_2.s_idx) == 1):
        sen_var = 100
        tok_num = 0
    else:
        sen_var = 10
        tok_num = 0
    # sp_sim represents spelling similarity.
    spell_1 = word_1.get_spelling() # get list of characters of word_1
    spell_2 = word_2.get_spelling() # get list of characters of word_2
    # calculate the ratio of the common characters between two words.
    if (len(spell_1) > len(spell_2)):
        sp_sim = float(50*len(list(set(spell_1).intersection(spell_2)))/len(spell_1))
    else: sp_sim = float(50*len(list(set(spell_2).intersection(spell_1)))/len(spell_2))
    return sen_var + tok_num + sp_sim

# Second checkpoint
# if you only modify scoring functinon,
# start from here 
import os
if os.path.exists("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\gene_and_score.txt"):
    os.remove("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\gene_and_score.txt")
if os.path.exists("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\confirmed_gene_list.txt"):
    os.remove("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\confirmed_gene_list.txt")

# calculate the scores of each and save them in dictionary.
gene_score_dict = dict()
for gene in g_list:
    for dis in d_list:
        # if the frequency of two synonyms of gene differ too much, exclude the gene.
        abb_1 = g_occurance[gene.id].count(0)/float(len(g_occurance[gene.id]))
        abb_2 = g_occurance[gene.id].count(1)/float(len(g_occurance[gene.id]))
        if (len(g_occurance[gene.id]) >= 50)&(abs(abb_1-abb_2)>0.98):
            continue
        if gene.id in gene_score_dict.keys():
            gene_score_dict[gene.id].append(score_function(dis, gene))
        else: gene_score_dict[gene.id] = [score_function(dis, gene)]

gene_and_score = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\gene_and_score.txt","w+", encoding="utf-8")
gene_and_score.write("Gene\tscore\n")

confirmed = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\confirmed_gene_list.txt","w+", encoding="utf-8")

for gene in gene_score_dict.keys():
    final_score = len(gene_score_dict[gene]) + (sum(gene_score_dict[gene])/len(gene_score_dict[gene]))
    gene_score_dict[gene] = final_score
    gene_and_score.write(str(gene) + "\t" + str(final_score) + "\n")
    if final_score > 15000: confirmed.write(str(gene) + "\n")

#scores = list(gene_score_dict.values()).sort()

# 
    

gene_and_score.close()
confirmed.close()
