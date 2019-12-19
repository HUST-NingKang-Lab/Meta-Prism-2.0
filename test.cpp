//
// Created by 康凯 on 2019-12-19.
//
#include <iostream>
#include <fstream>
#include<sstream>
#include "newickParser.h"
using namespace std;
int main(){
    //char a[]="((E:0.1,(C:0.6,D:0.7):0.2):0.3,(A:0.9,B:1.0):0.8)";
    ifstream ifile;
    ifile.open("/Volumes/RamDisk/Download/metaphlan2.tree/tree.newick");
    stringstream buf;
    buf<<ifile.rdbuf();
    string as(buf.str());
    parser p(as);
    return 0;
}
