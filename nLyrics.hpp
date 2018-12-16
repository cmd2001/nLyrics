#include <bits/stdc++.h>
#define debug cerr
using namespace std;

class FileOutput {
private:
    FILE* file;
public:
    FileOutput(const string &name) {
        #ifdef _WIN32 // for Windows platfrom.
            //debug << "This is Windows" << endl;
            wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> cvt;
            wstring name_16 = cvt.from_bytes(name); // conver name to UTF_16.
            file = _wfopen(name_16.c_str(),L"w");
        #else
            //debug << "This is Linux" << endl;
            file = fopen(name.c_str(),"w");
        #endif
    }
    inline void printStr(const string &s) {
        fprintf(file,"%s",s.c_str());
    }
    ~FileOutput() {
        fclose(file);
    }
};

class EscapeSequenceFixer {
private:
    map<char,char> mem;
public:
    EscapeSequenceFixer() {
        mem['a'] = '\a', mem['b'] = '\b', mem['f'] = '\f', mem['n'] = '\n', mem['r'] = '\r', mem['t'] = '\t',
        mem['v'] = '\v', mem['\\'] = '\\', mem['?'] = '\?', mem['\''] = '\'', mem['\"'] = '\"', mem['0'] = '\0';
    }
    inline string fixStr(const string &s) {
        string ret;
        for(unsigned i = 0; i < s.size(); i++) {
            if(s[i] != '\\') ret.append(1, s[i]);
            else ret.append(1, mem[s[++i]]);
        }
        return ret;
    }
};

class FileNameFixer { // fix illegal file name of Windows.
private:
    map<char,string> mem;
public:
    FileNameFixer() {
        mem['\\'] = "＼", mem['/'] = "／", mem[':'] = "：", mem['*'] = "＊",
        mem['?'] = "？", mem['\"'] = "＂", mem['<'] = "<", mem['>'] = "＞",
        mem['|'] = "｜";
    }
    inline string fixStr(const string &s) {
        string ret;
        for(unsigned i = 0; i < s.size(); i++) {
            if(mem.find(s[i]) == mem.end()) ret = ret.append(1, s[i]);
            else ret.append(mem[s[i]]);
        }
        return ret;
    }
};

class SongParser {
private:
    inline string cutStruct(const string &s, int pos) {
        string ret;
        while(s[pos] != '{') ++pos;
        do ret.append(1, s[pos++]); while(s[pos] != '}');
        return ret;
    }
    inline string cutLyrics(const string &s) {
        if(~s.find("\"lyric\":null")) return "";
        int pos = s.find("\"lyric\":\"") + 9;
        string ret = s.substr(pos, s.size() - pos - 2);
        return ret;
    }
public:
    inline string getLyrics(const string &s, const string &tar) {
        return cutLyrics(cutStruct(s, s.find(tar)));
    }
};

class ListParser {
private:
    string T;
    vector<int> pos;
    inline bool cmp(int st, const string &tar) {
        for(unsigned i=0;i<tar.length();i++) if(T[st+i] != tar[i]) return 0;
        return 1;
    }
    inline int findLast(int pos, const string &tar) {
        do {
            if(cmp(pos,tar)) return pos;
        }while(pos--);
        assert(0);
    }
    inline string explainName(int pos) {
        pos = findLast(pos, "\"name\"") + 8;
        string ret;
        while(T[pos] != '\"') {
            if(T[pos] == '\\') ret.append(1, T[pos++]); // solve // \?
            ret.append(1, T[pos++]);
        }
        return ret;
    }
    inline int explainId(int pos) {
        pos = findLast(pos, "\"id\"") + 5;
        int ret = 0;
        while(T[pos] != ',') ret = ret * 10 + T[pos++] - '0';
        return ret;
    }
public:
    vector<pair<string,int> > parse(const string &ss) {
        T = ss;
        int lst = 0;
        while(~lst) {
            lst = T.find("\"position\"", lst + 1);
            if(~lst) pos.push_back(lst);
        }
        vector<pair<string,int> > ret;
        for(auto &i : pos) ret.push_back(make_pair(explainName(i), explainId(i)));
        return ret;
    }
};

class JsonGrabber {
public:
    inline void getSongJson(long long id) {
        static char ss[1010];
        sprintf(ss,"curl -o song.json \"https://music.163.com/api/song/lyric?os=pc&id=%lld&lv=-1&kv=-1&tv=-1\" -s",id);
        system(ss);
    }
    inline void getListJson(long long id) {
        static char ss[1010];
        sprintf(ss,"curl -o list.json \"http://music.163.com/api/playlist/detail?id=%lld\" -s", id);
        debug << ss << endl;
        system(ss);
    }
};



