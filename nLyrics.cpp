#include <bits/stdc++.h>
#include "nLyrics.hpp"

inline string readStr(fstream &f) {
    string ret;
    char c;
    while(f.get(c) && c != EOF) ret.append(1, c);
    return ret;
}

inline void workSingleSong(int id, string name) {
    JsonGrabber jg;
    jg.getSongJson(id);
    fstream fin("song.json",fstream::in);
    string str = readStr(fin);
    if(~str.find("\"uncollected\":true") || ~str.find("\"nolyric\":true"))
        return void(cout << "Nothing to Download" << endl);
    
    EscapeSequenceFixer esf;
    FileNameFixer fnf;
    SongParser sp;
    
    name = fnf.fixStr(esf.fixStr(name));
    
    FileOutput ori(name + ".lrc");
    ori.printStr(esf.fixStr(sp.getLyrics(str, "\"lrc\"")));
    
    string lyrics = sp.getLyrics(str, "\"tlyric\"");
    if(lyrics != "") {
        FileOutput trs(name + "_trans.lrc");
        trs.printStr(esf.fixStr(lyrics));
    }
}

inline void clearTempFile() {
    #ifdef __WIN32
        system("del /Q *.json");
    #else
        system("rm *.json");
    #endif
}

int main() {
    locale::global(locale(""));
    setlocale(LC_ALL,"");
    wcout.imbue(locale(""));
    
    long long id;
    cout << "Please input your NeteaseMusic Playlist id" << endl, cin >> id;
    
    JsonGrabber jg;
    jg.getListJson(id);
    fstream fin("list.json",fstream::in);
    string str = readStr(fin);
    fin.close();
    
    ListParser lp;
    auto v = lp.parse(str);
    
    cout << "Playlist metadata initialized" << endl;
    cout << "There are " << v.size() << " song(s) in your playlist." << endl;
    
    for(unsigned i = 0; i < v.size(); i ++) {
        cout << "Downloading lyrics for ", cout.clear();
        #ifdef _WIN32
            wstring_convert<codecvt_utf8_utf16<wchar_t>, wchar_t> cvt;
            wstring data_16 = cvt.from_bytes(v[i].first); // conver name to UTF_16.
            wcout << data_16, wcout.clear();
        #else
            cout << v[i].first, cout.clear();
        #endif
        cout << "\nProgress " << i + 1 << "/" << v.size() << endl;
        workSingleSong(v[i].second, v[i].first);
    }
    
    clearTempFile();
    
    return 0;
}

