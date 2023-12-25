#include "../../src/cpp/titleSongnameDetect.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unf/normalizer.hh>
#include <boost/algorithm/string.hpp>

struct testdata_body{
    std::string raw_title;
    std::string songname;
    std::string detected_songname;
    std::string videoid;
    bool is_correct;
    testdata_body();
};

testdata_body::testdata_body(){
    is_correct = false;
}

std::string normalizeString(std::string input){
    UNF::Normalizer norm;
    UNF::Normalizer::Form form = UNF::Normalizer::FORM_NFKC;
    std::string output = norm.normalize(input.c_str(), form);
    return output;
}

int main(){
    std::ifstream ifs("testdata.csv");
    //parse csv
    std::vector<testdata_body> testdata;
    std::string str_buf;
    std::string str_conma_buf;
    while (getline(ifs, str_buf)) {    
        // 「,」区切りごとにデータを読み込むためにistringstream型にする
        std::istringstream i_stream(str_buf);
        testdata_body testdata_buf;
        getline(i_stream, str_conma_buf, ',');
        testdata_buf.videoid = str_conma_buf;
        getline(i_stream, str_conma_buf, ',');
        testdata_buf.raw_title = str_conma_buf;
        getline(i_stream, str_conma_buf, ',');
        testdata_buf.songname = normalizeString(str_conma_buf);
        testdata.push_back(testdata_buf);
    }
    //test
    int correct_count = 0;
    int test_count = 0;
    std::ofstream ofs("result.tsv");
    std::ofstream ofs_correct("correct.csv");
    for(auto testdata_buf:testdata){
        test_count++;
        std::string detected_songname = titleSongnameDetect::get_song_title_string(testdata_buf.raw_title);
        detected_songname = normalizeString(detected_songname);
        //this replace because of test data was normalized by NFKC
        //boost::replace_all(detected_songname,"！","!");
        //boost::replace_all(detected_songname,"？","?");
        //boost::replace_all(detected_songname,"～","~");
        std::string detected_lower = boost::to_lower_copy(detected_songname);
        boost::to_lower(testdata_buf.songname);
        boost::trim(testdata_buf.songname);
        if(detected_lower == testdata_buf.songname){
            correct_count++;
            testdata_buf.is_correct = true;
            ofs_correct << testdata_buf.videoid << "," << normalizeString(testdata_buf.raw_title) << "," << detected_songname << std::endl;
        }
        else{
            //std::cout << "videoid:" << testdata_buf.videoid << "\ttitle:" << testdata_buf.raw_title << "\tdetected:" << detected_songname << std::endl; 
            ofs << testdata_buf.videoid << "\t" << testdata_buf.songname << "\t" << testdata_buf.raw_title << "\t" << detected_songname << std::endl;
        }
        testdata_buf.detected_songname = detected_songname;
    }
    ofs.close();
    ofs_correct.close();
    //output
    std::cout << "test count:" << test_count << std::endl;
    std::cout << "correct count:" << correct_count << std::endl;
    std::cout << "correct rate:" << (float)correct_count / (float)test_count * 100 << "%" << std::endl;
}
