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
    void v2s(char *buf,T* V){
        char *data=(char*)V;
        uint32_t size=sizeof(T);
        if(littleE){
            memcpy(buf,data,size);
        }
        else{
        for(int i=0;i<size;i++){
            buf[i]=data[size-i];
        }}
    }
    template <typename T>
    void s2v(char *buf,T* V){
        char *data=(char*)V;
        uint32_t size=sizeof(T);
        if(littleE){
            memcpy(data,buf,size);
        }
        else{
        for(int i=0;i<size;i++){
            data[i]=buf[size-i];
        }}
    }
};
class loader{
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
    int loadCompData(ifstream &ifile);
    sampleData* loadTSVFile(ifstream &ifile,string name="not defined");
    int loadMultiTSV(ifstream &ifile);
    int outputMirror(ofstream &ofile);
    int loadBMultiTSV(ifstream &ifile);
    int outputBMirror(ofstream &ofile);
    int loadFromMirror(ifstream &ifile);
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
