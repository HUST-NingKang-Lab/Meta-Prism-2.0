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
class BinaryIO{// this class could detect memory storage mode: Big-endian or Little-endian, and change value to Little-edian storage
            // thus binary packaged data could share across Big-endian and Little-endian
private:
    bool littleE;
public:
    BinaryIO(){
        union
        {
            unsigned int  a;
            unsigned char b;
        }c;
        c.a = 1;
        littleE=(1==c.b);
    }
    template <typename T>
    inline void v2s(char *buf,T* V);//change value to Little-endian
    template <typename T>
    inline void s2v(char *buf,T* V);//change Little-endian to value
};

class loader{// this class could load sample data set and package
private:
    vector<sampleData> data;
    CompData compData;
    BinaryIO io;
public:
    struct Infos{
        char source[20];
        float version;
        uint64_t size;
    };
    struct SampleHead{
        char Name[38];
        uint64_t sampleSize;
    };
    map<string, sampleData*> Data;
    vector<string> names;
    parser *p;
    loader(parser *p){this->p=p;}
    int loadOTUData(ifstream &ifile);
    const vector<sampleData>& getData(){return data;}
    sampleData* loadTSVFile(ifstream &ifile,string name="not defined");
    
    int loadMultiTSV(ifstream &ifile);
    int loadBMultiTSV(ifstream &ifile);
    int loadFromMirror(ifstream &ifile);
    
    int outputBMirror(ofstream &ofile);
    int outputMirror(ofstream &ofile);
    int outTSVTable(ofstream &ofile);
    
    int size(){return Data.size();}
    void genName(){
        names.clear();
        for(auto iter=Data.begin();iter!=Data.end();iter++)
            names.push_back(iter->second->name);
        return;
    }
    int printToTable(ofstream &ofile);
    //int out
};

#endif /* loader_hpp */
