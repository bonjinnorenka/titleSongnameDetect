#include "titleSongnameDetect.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <codecvt>
#include <boost/algorithm/string.hpp>

namespace titleSongnameDetect{
    std::vector<std::string> artistNames = {"八王子P","BTS","syudou","supercell","ChroNoiR","加賀美ハヤト","瀬名航","HACHI","きゃりーぱみゅぱみゅ","Michael Jackson","\\*Luna","シグナルP","緑仙","伊藤由奈","ナユタン星人","Kanaria","柊マグネタイト","DECO\\*27","あいみょん","甲斐田晴","Albemuth","Ado","VΔLZ","にじさんじ","千石撫子","ヨルシカ","和ぬか","YuNi","名取さな","Official髭男dism","official髭男dism","ryo","YOASOBI","かいりきベア","LiSA","Yunomi","Nornis","ROF-MAO","HoneyWorks","みきとP","Blue Journey","ユイカ"};
    std::vector<std::string> ignore_keyword = {"#Shorts","#short","#shorts","#vtuber"};
    #ifdef _WIN32
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring string_to_wstring(std::string input){
        std::wstring output = converter.from_bytes(input);
        return output;
    }
    std::vector<std::wstring> ignore_keyword_wstring;
    void load_ignore_keyword_wstring(){
        for(auto&&keyword:ignore_keyword){
            ignore_keyword_wstring.push_back(string_to_wstring(keyword));
        }
    }
    std::vector<std::wregex> artist_regexs;
    void load_artist_regexs(){
        for(auto&&artistName:artistNames){
            artist_regexs.push_back(std::wregex(string_to_wstring(artistName)+L"(-| -|/| /|)"));
        }
    }
    std::wstring get_song_title_wstring(std::wstring raw_title) {//for windows
        std::wstring empty_wstring = L"";
        std::wstring title;
        //アーティスト名を消す
        if(artist_regexs.empty()){
            load_artist_regexs();
        }
        for(auto&&artist_regex:artist_regexs){
            raw_title = std::regex_replace(raw_title,artist_regex,empty_wstring);
        }
        if(ignore_keyword_wstring.empty()){
            load_ignore_keyword_wstring();
        }
        for(auto ignore_keyword:ignore_keyword_wstring){
            boost::algorithm::replace_all(raw_title,ignore_keyword,empty_wstring);
        }
        //raw_title = std::regex_replace(raw_title,std::wregex(L"にじさんじ(-| -|/| /|)"),empty_wstring);
        // 「作品名」より【楽曲タイトル】 というパターンがあるので、その場合は【】の中身をタイトルとする
        if (raw_title.find(L"より【") != std::wstring::npos) {
            title = raw_title.substr(raw_title.find(L"【") + 1, raw_title.find(L"】") - raw_title.find(L"【") - 1);
        } else {
            title = raw_title;
        }
        // ヘッダー的に記号がついていたら削除する
        if (title[0] == L'★') {
            title = title.substr(1);
        }
        // ()（）[]【】〈〉<>を除外する。左が半角で右が全角だったりすることもある
        title = std::regex_replace(title, std::wregex(L"[₍［<【（《〈\\(\\[].+?[】）》〉\\)\\]>］₎]"), empty_wstring);
        // 「作品名」主題歌 などのパターンの場合は、その部分を消す
        std::wstring keywords[] = {L"主題歌", L"OP", L"CMソング",L"オリジナル曲"};
        for (int i = 0; i < keywords->size(); i++) {
            if (title.find(L"」" + keywords[i]) != std::wstring::npos) {
                int end_index = title.find(L"」" + keywords[i]);
                for (int start_index = end_index; start_index >= 0; start_index--) {
                    if (title[start_index] == L'「') {
                        title = title.substr(0, start_index) + title.substr(end_index + keywords[i].length() + 1);
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < keywords->size(); i++) {
            if (title.find(L"』" + keywords[i]) != std::wstring::npos) {
                int end_index = title.find(L"』" + keywords[i]);
                for (int start_index = end_index; start_index >= 0; start_index--) {
                    if (title[start_index] == L'『') {
                        title = title.substr(0, start_index) + title.substr(end_index + keywords[i].length() + 1);
                        break;
                }
                }
            }
        }
        // 「」と『』がある場合、その中の文字列を取り出す
        // ただし、稀に「」の中に自分の名前を入れている場合がある。その場合は無視する
        if (title.find(L"「") != std::wstring::npos && title.find(L"」") != std::wstring::npos) {
            std::wstring temp_title = title.substr(title.find(L"「") + 1, title.find(L"」") - title.find(L"「") - 1);
            bool keyword_status = true;
            for(int x=0;x<keywords->size();x++){
                if(temp_title.find(keywords[x])!=std::wstring::npos){
                    keyword_status = false;
                }
            }
            if (keyword_status&&temp_title.find(L"cover") == std::wstring::npos) {
                title = temp_title;
            }
        }
        if (title.find(L"『") != std::wstring::npos && title.find(L"』") != std::wstring::npos) {
            std::wstring temp_title = title.substr(title.find(L"『") + 1, title.find(L"』") - title.find(L"『") - 1);
            bool keyword_status = true;
            for(int x=0;x<keywords->size();x++){
                if(temp_title.find(keywords[x])!=std::wstring::npos){
                    keyword_status = false;
                }
            }
            if (keyword_status&&temp_title.find(L"cover") == std::wstring::npos) {
                title = temp_title;
            }
        }
        // 歌ってみた以降の文字列を消す
        title = std::regex_replace(title, std::wregex(L"(@)?多重人格で歌ってみた.*"),L" ");
        title = std::regex_replace(title, std::wregex(L"(@)?コラボ(で)?歌ってみた.*"), L" ");
        title = std::regex_replace(title, std::wregex(L"(@)?歌って踊ってみた.*"),L" ");
        title = std::regex_replace(title, std::wregex(L"(@)?を歌ってみた.*"), L" ");
        title = std::regex_replace(title, std::wregex(L"(@)?歌ってみた.*"), L" ");
        title = std::regex_replace(title, std::wregex(L"(@)?歌わせて.*"), L" ");
        title = std::regex_replace(title, std::wregex(L"(@)?うたった.*"), L" ");
        // cover, covered, covered by 以降の文字列を消す
        title = std::regex_replace(title, std::wregex(L"[cC]over(ed)?( by)?.*"), empty_wstring);
        title = std::regex_replace(title, std::wregex(L"COVER.*"), empty_wstring);
        title = std::regex_replace(title, std::wregex(L"[Bb]y(?!e).*"), empty_wstring);
        title = std::regex_replace(title, std::wregex(L"[fF]eat.*"), empty_wstring);//featがある場合がある
        title = std::regex_replace(title, std::wregex(L"ft\\..*"), empty_wstring);//featがある場合がある
        //描いて歌ってみた系対策
        title = std::regex_replace(title,std::wregex(L"(を)?描いて.*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"(を)?喋って.*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"(を)?踊って.*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"(を)?演奏して.*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"(を)?弾いて.*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"(を)?作って.*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"(を)?アカペラで.*"),empty_wstring);
        if(!std::regex_search(title,std::wregex(L"ハロ[/／]ハワユ"))){//ハロ/ハワユ以外は基本的に曲名に/は入らない
            title = std::regex_replace(title,std::wregex(L"[/／].*"),empty_wstring);
        }
        title = std::regex_replace(title,std::wregex(L"[｜|┊¦|\\|￤┋┃│┆╎║].*"),empty_wstring);//|以降は削除する
        //title = std::regex_replace(title,std::wregex(L"｜.*"),empty_wstring);//｜以降は削除する
        title = std::regex_replace(title,std::wregex(L"[ 　]{2,}.*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"[ 　]/.*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"[Rr]emix.*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"[?？]{3,}.*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"[ 　]-.*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"-[ 　].*"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"[ 　]*\\.ver.*"),empty_wstring);
        //title = std::regex_replace(title,std::wregex(L"\\.\\..*"),empty_wstring);
        
        //title = std::regex_replace(title,std::wregex(L":.*"),empty_wstring);//:以降は削除する
        title = std::regex_replace(title,std::wregex(L".*による"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L".*が歌う"),empty_wstring);
        //title = std::regex_replace(title,std::wregex(L"$_"),empty_wstring);
        
        // /以降は削除する
        //boost::algorithm::replace_all(title, L"/", empty_wstring);
        //boost::algorithm::replace_all(title, L"／", empty_wstring);
        //boost::replace_all(title, L"\\", empty_wstring);
        //std::vector<std::wstring> remove_tag_list = {L"#Shorts",L"#short",L"#vtuber",L"-",L"‐",L"【",L"】",L"–",L"3D"};
        std::vector<std::wstring> remove_tag_list = {L"‐",L"【",L"】",L"–",L"3D"};
        for(auto remove_tag:remove_tag_list){
            boost::algorithm::replace_all(title, remove_tag, empty_wstring);
        }
        if(title.find(L"MV")!=std::wstring::npos&&title.substr(0,2)==(std::wstring)L"MV"){
            title = title.substr(2);
        }
        else{
            //MV平置き対策
            title = std::regex_replace(title, std::wregex(L"MV.*"),empty_wstring);
        }
        if(title.find(L"#")==title.size()-1){
            title = title.substr(0,title.size()-1);
        }
        title = std::regex_replace(title,std::wregex(L"music\\s*video",std::regex::icase),empty_wstring);
        boost::trim(title);
        title = std::regex_replace(title,std::wregex(L"$[#-:・_]"),empty_wstring);
        title = std::regex_replace(title,std::wregex(L"^[♪✧✦⋆]"),empty_wstring);
        boost::trim(title);//前後空白対策
        if(title.empty()&&raw_title.size()<=10){//曲名の中央値は8 対してタイトルの中央値は35この文字数でポカする確率は約10/7000で0.14%と非常に低いので一般的に曲名と同じであると考えられる
            title = raw_title;
        }
        return title;
    }
    std::string get_song_title_string(std::string raw_title){
        std::string title;
        try{
            std::wstring raw_title_wstring = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(raw_title);
            std::wstring title_wstring = get_song_title_wstring(raw_title_wstring);
            title = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(title_wstring);
        }
        catch(...){
            title = "";
        }
        return title;
    }
    #else
    std::string get_song_title_string(std::string raw_title_string) {//for linux
        std::string title;
        // 「作品名」より【楽曲タイトル】 というパターンがあるので、その場合は【】の中身をタイトルとする
        if (raw_title_string.find("より【") != std::string::npos) {
            title = raw_title_string.substr(raw_title_string.find("【") + 1, raw_title_string.find("】") - raw_title_string.find("【") - 1);
        } else {
            title = raw_title_string;
        }
        // ヘッダー的に記号がついていたら削除する
        if (title[0] == '★') {
            title = title.substr(1);
        }
        // ()（）[]【】〈〉<>を除外する。左が半角で右が全角だったりすることもある
        title = std::regex_replace(title, std::regex("[<【（《〈\\(\\[].+?[】）》〉\\)\\]>]"), "");
        // 「作品名」主題歌 などのパターンの場合は、その部分を消す
        std::string keywords[] = {"主題歌", "OP", "CMソング","オリジナル曲"};
        for (int i = 0; i < keywords->size(); i++) {
            if (title.find("」" + keywords[i]) != std::string::npos) {
                int end_index = title.find("」" + keywords[i]);
                for (int start_index = end_index; start_index >= 0; start_index--) {
                    if (title[start_index] == '「') {
                        title = title.substr(0, start_index) + title.substr(end_index + keywords[i].length() + 1);
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < keywords->size(); i++) {
            if (title.find("』" + keywords[i]) != std::string::npos) {
                int end_index = title.find("』" + keywords[i]);
                for (int start_index = end_index; start_index >= 0; start_index--) {
                    if (title[start_index] == '『') {
                        title = title.substr(0, start_index) + title.substr(end_index + keywords[i].length() + 1);
                        break;
                    }
                }
            }
        }
        // 「」と『』がある場合、その中の文字列を取り出す
        // ただし、稀に「」の中に自分の名前を入れている場合がある。その場合は無視する
        if (title.find("「") != std::string::npos && title.find("」") != std::string::npos) {
            std::string temp_title = title.substr(title.find("「") + 1, title.find("」") - title.find("「") - 1);
            bool keyword_status = true;
            for(int x=0;x<keywords->size();x++){
                if(temp_title.find(keywords[x])!=std::string::npos){
                    keyword_status = false;
                }
            }
            if (keyword_status&&temp_title.find("cover") == std::string::npos) {
                title = temp_title;
            }
        }
        if (title.find("『") != std::string::npos && title.find("』") != std::string::npos) {
            std::string temp_title = title.substr(title.find("『") + 1, title.find("』") - title.find("『") - 1);
            bool keyword_status = true;
            for(int x=0;x<keywords->size();x++){
                if(temp_title.find(keywords[x])!=std::string::npos){
                    keyword_status = false;
                }
            }
            if (keyword_status&&temp_title.find("cover") == std::string::npos) {
                title = temp_title;
            }
        }
        // 歌ってみた以降の文字列を消す
        title = std::regex_replace(title, std::regex("多重人格で歌ってみた.*"), "");
        title = std::regex_replace(title, std::regex("コラボ歌ってみた.*"), "");
        title = std::regex_replace(title, std::regex("歌って踊ってみた.*"), "");
        title = std::regex_replace(title, std::regex("を歌ってみた.*"), "");
        title = std::regex_replace(title, std::regex("歌ってみた.*"), "");
        // cover, covered, covered by 以降の文字列を消す
        title = std::regex_replace(title, std::regex("[cC]over(ed)?( by)?.*"), "");
        title = std::regex_replace(title, std::regex("[fF]eat.*"), "");//featがある場合がある
        //描いて歌ってみた系対策
        title = std::regex_replace(title,std::regex("描いて.*"),"");
        if(!std::regex_search(title,std::regex("ハロ[/／]ハワユ"))){//ハロ/ハワユ以外は基本的に曲名に/は入らない
            title = std::regex_replace(title,std::regex("[/／].*"),"");
        }
        title = std::regex_replace(title,std::regex("[｜|┊¦|\\|￤┋┃│┆╎║].*"),"");//｜以降は削除する
        title = std::regex_replace(title,std::regex("[ 　]{2,}.*"),"");//以降は削除する
        title = std::regex_replace(title,std::regex("[ 　]/.*"),"");//以降は削除する
        title = std::regex_replace(title,std::regex("[Rr]emix.*"),"");//以降は削除する
        title = std::regex_replace(title,std::regex("[?？]{3,}.*"),"");//以降は削除する
        title = std::regex_replace(title,std::regex("[ 　]-.*"),"");//以降は削除する
        title = std::regex_replace(title,std::regex("-[ 　].*"),"");//以降は削除する
        title = std::regex_replace(title,std::regex("[ 　]*\\.ver.*"),"");//以降は削除する

        title = std::regex_replace(title,std::regex(".*による"),"");
        title = std::regex_replace(title,std::regex(".*が歌う"),"");

        // /以降は削除する
        boost::algorithm::replace_all(title, "/", "");
        boost::algorithm::replace_all(title, "／", "");
        boost::algorithm::replace_all(title, "#short", "");
        boost::algorithm::replace_all(title, "-", "");
        if(title.find("MV")!=std::string::npos&&title.substr(0,2)=="MV"){
            title = title.substr(2);
        }
        else{
            //MV平置き対策
            title = std::regex_replace(title, std::regex("MV.*"),"");
        }
        title = std::regex_replace(title,std::regex("music\\s*video",std::regex::icase),"");
        boost::trim(title);//前後空白対策
        if(title.empty()&&raw_title_string.size()<=10){//曲名の中央値は8 対してタイトルの中央値は35この文字数でポカする確率は約10/7000で0.14%と非常に低いので一般的に曲名と同じであると考えられる
            title = raw_title_string;
        }
        return title;
    }
    #endif
}

