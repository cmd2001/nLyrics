#!/usr/bin/python
# -*- coding: UTF-8 -*-

def LCS(a, b):
    if len(a) == 0 or len(b) == 0:
        return 0
    f = [0 for _ in range(len(b) + 1)]
    for i in range(1, len(a) + 1):
        lst = 0
        for j in range(1, len(b) + 1):
            upd = 0
            if a[i - 1] == b[j - 1]:
                upd = lst + 1
            else:
                upd = lst
            lst = f[j]
            f[j] = max({f[j], f[j - 1], upd})
    return f[len(b)]

def genRealName(s):
    if s.lower()[len(s) - 4: ] == ".lrc":
        return s[: -4]
    elif s.lower()[len(s) - 4: ] == ".mp3":
        return s[: -4]
    elif s.lower()[len(s) - 5: ] == ".flac":
        return s[: -5]
    elif s.lower()[len(s) - 4: ] == ".wav":
        return s[: -4]

def fixStr(s):
    ret = ""
    s = s.lower()
    for ch in s:
        if ch != ' ' and ch != '-':
            ret = ret + ch
    return genRealName(ret)

def findBest(lft, rit):
    ALPHA = 0.8
    if len(rit) == 0:
        return -1
    ret = 0
    mxp = LCS(lft, rit[0]) / min({len(lft), len(fixStr(rit[0]))})
    lft = fixStr(lft)
    for i in range(1, len(rit) - 1):
        pp = LCS(lft, fixStr(rit[i])) / min({len(lft), len(fixStr(rit[i]))})
        if pp > mxp:
            mxp = pp
            ret = i
    if mxp < ALPHA:
        return -1
    return ret

import os

lrcs = []
songs = []

file_dir = os.getcwd()
for _, __, _files in os.walk(file_dir):
    for files in _files:
        if files.lower()[len(files) - 4: ] == ".lrc":
            lrcs.append(files)
        elif files.lower()[len(files) - 4: ] == ".mp3":
            songs.append(files)
        elif files.lower()[len(files) - 5: ] == ".flac":
            songs.append(files)
        elif files.lower()[len(files) - 4: ] == ".wav":
            songs.append(files)

# print(songs)
# print(lrcs)

# exit()

for lft in songs:
    fnd = findBest(lft, lrcs)
    if fnd == -1:
        continue
    rit = lrcs[fnd]
    del lrcs[fnd]
    print("Renaming", rit, "to", genRealName(lft) + ".lrc")
    os.rename(rit, genRealName(lft) + ".lrc")
