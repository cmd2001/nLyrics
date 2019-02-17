def readFile(name):
    fin = open(name, 'r', encoding = 'utf-8')
    return fin.read()

def getFileList(origin):
    ret = origin.split('\n')
    for cur in ret:
        if cur == '':
            ret.remove(cur)
    return ret

def countWord(string):
    word_list = string.split(' ')
    counter = {}
    for cur in word_list:
        if cur in counter.keys():
            counter[cur] = counter[cur] + 1
        else:
            counter[cur] = 1
    counter = sorted(counter.items(), key = lambda t:t[1], reverse = 1)
    return counter

def removeTimemark(string):
    import re
    return re.sub('\[[\d:.]*\]', '', string, flags = re.I)

def removeCertaionCharacter(string):
    import re
    return re.sub('[()（）…\[\]「」:：、　]', '', string, flags = re.I)

def mergeList(word_list, limit):
    ret = ''
    cnt = 0
    for cur in word_list:
        ret = ret + cur[0] + '\n'
        cnt += 1
        if cnt > limit:
            break
    return ret


file_list = getFileList(readFile('ls.txt'))
ful = ''
for cur in file_list:
    ful = ful + readFile(cur) + '\n'
ful = removeTimemark(ful)
ful = removeCertaionCharacter(ful)
# print(ful)
# exit()

import MeCab
mecab = MeCab.Tagger ("-Owakati")
mecab2 = MeCab.Tagger ("-Ochasen")

string = mecab.parse(ful)
word_list = countWord(string)

hot_words = mergeList(word_list, 50)
print(mecab2.parse(hot_words))
