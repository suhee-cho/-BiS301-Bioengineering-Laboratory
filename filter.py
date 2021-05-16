f = open("C:\\Users\\suhee\\Documents\\2021_Spring\\BiS301 Bioeng Lab1\\git_code\\lab8\\Lab8_dataset\\Dataset A\\gene_and_score.txt", 'r').readlines()

for line in f:
    l = line.strip()
    li = l.split("\t")
    # print(li)
    try:
        if float(li[1])>=1000: print(li[0])
    except: pass
