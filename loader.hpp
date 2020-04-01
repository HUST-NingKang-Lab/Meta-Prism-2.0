//
//  loader.hpp
//  prism2
//
//  Created by 康凯 on 2019/12/23.
//  Copyright © 2019 康凯. All rights reserved.
//

#ifndef loader_hpp
#define loader_hpp
#include "structure.h"
#include "newickParser.h"
#include <stdio.h>
using namespace std;
class loader{
private:
    CompData compData;
    map<string, sampleData*> Data;
    
public:
    
    vector<string> names;
    parser *p;
    loader(parser *p){this->p=p;}
    int loadMultiData(ifstream &ifile);
    int loadCompData(ifstream &ifile);
    sampleData* loadTSVFile(ifstream &ifile,string name="not defined");
    int loadMultiTSV(ifstream &ifile);
    int outputMirror(ofstream &ofile);
    int loadFromMirror(ifstream &ifile);
    int outTSVTable(ofstream &ofile);
    int size(){return (int)Data.size();}
    void genName(){
        names.clear();
        for(auto iter=Data.begin();iter!=Data.end();iter++)
            names.push_back(iter->second->name);
        return;
    }
    int printToTable(ofstream &ofile);
    const map<string,sampleData*> & getData(){return Data;}
    //int out
};
#endif /* loader_hpp */
