import sys, re

dictionary = dict()

for line in open(sys.argv[1], "r", encoding='big5hkscs'):
    wordlist = re.split(' ', line)
    word = wordlist[0]
    zhu = re.split('/',re.split('\\n',wordlist[1])[0])
    for x in zhu:
        if x[0] not in dictionary:
            dictionary[x[0]] = set()
        dictionary[x[0]].add(word)

with open(sys.argv[2], "w", encoding='big5hkscs') as outfile:
    for key, value in dictionary.items():
        outfile.write(key+'\t'+' '.join(str(x) for x in value)+'\n')
        for x in value:
            outfile.write(str(x)+' '+str(x)+'\n')
        

