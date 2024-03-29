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
/*!
@brief read and write binnary file
@note This class could detect memory storage mode: Big-endian or Little-endian, and change value to Little-edian storage.
Thus binary packaged data could share across Big-endian and Little-endian
*/
class BinaryIO{
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
/*!
@brief This class can read many types of sample datasets and convert them to other formats for output
*/
class DataProcessor{
private:
    vector<SampleData> data;
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
    map<string, SampleData*> Data;
    vector<string> names;
    NewickParser *p;
    DataProcessor(NewickParser *p){this->p=p;}
    int readMatData(ifstream &ifile);
    const vector<SampleData>& getData(){return data;}
    SampleData* readSingleTSV(ifstream &ifile,string name="not defined");
    
    int readMultiTSV(ifstream &ifile);
    int readBinaryPackage(ifstream &ifile);
    int readPackage(ifstream &ifile);
    
    int writeBinaryPackage(ofstream &ofile);
    int writePackage(ofstream &ofile);
    int writeTSV(ofstream &ofile);
    
    unsigned int size(){return Data.size();}
    void genName(){
        names.clear();
        for(auto iter=Data.begin();iter!=Data.end();iter++)
            names.push_back(iter->second->name);
        return;
    }
    int merge(DataProcessor &data);
    int printToTable(ofstream &ofile);
    //int out
};

#endif /* loader_hpp */
