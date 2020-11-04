//
// Created by 康凯 on 2019-12-19.
//
/*
 main.cpp parsing the command line arguments and execute loading, calculating, saving functions
 */
#include <iostream>
#include <fstream>
#include<sstream>
#include "newickParser.h"
#include "loader.hpp"
#include "simCalc.hpp"
#include <time.h>
using namespace std;

class ArgParser{// parsing the command line arguments
public:
    void printHelp();
    unordered_map<string, vector<string>> args;
    int parse(int argc,const char*argv[]);
    inline const vector<string>* operator[](const string a);
    inline const vector<string>* get(const string a);
    inline const vector<string>* get(const string a,const string b);
    inline int exist(const string &a);
};
const vector<string>* ArgParser::operator[](const string a){
    if(args.count(a))
        return &args[a];
    else
        return nullptr;
}
const vector<string>* ArgParser::get(const string a){
    return this->operator[](a);
}
const vector<string>* ArgParser::get(const string a,const string b){
    if(args.count(a))
        return &args[a];
    else if(args.count(b))
        return &args[b];
    else
        return nullptr;
}
int ArgParser::exist(const string &a){
    return args.count(a);
}
void ArgParser::printHelp(){
    cout<<"--tree(-t) [newick tree path]: path of silva newick tree\n"
    <<"--load(-l) + [list|OTU|ascii|binary] + [path]: load datas from different types\n\n"
    
    <<"--matrix(-m) : computing similarity matrix\n"
    <<"--search(-s) + [single|list|OTU|ascii|binary] +[search sample path] + [select number|f]: Load and search samples then return top N (default=5, input char 'f' will return full result as matrix) similar samples\n\n"
    
    <<"--output(-o) [path]: path to output calculation result\n"
    <<"--package(-p) [ascii|binary] [path]: save package of loaded file\n"
    // <<"--convertOTU [path]: save as OTU format\n"
    // <<"--outTree [path]: save package of loaded file\n"
    <<"--threads(-T)+ [number of threads]:default single thread\n--help(-h) help\n";
}
int ArgParser::parse(int argc,const char *argv[]){
    int i=1,t=0;
    string buffer,head;
    vector<string> *v_buffer=nullptr;
    if(argc<2)
        return -1;
    for(i=1;i<argc;i++){
        //buffer=argv[i];
        if (argv[i][0]=='-'){
            if(i!=1){
                args[head]=*v_buffer;
                t++;
            }
            v_buffer=new vector<string>;
            head=argv[i];
        }
        else{
            if(i==1){
                cout<<"Args wrong at "<<i<<' '<<argv[i]<<"\n";
                return -1;
            }
            v_buffer->push_back(argv[i]);
        }}
    args[head]=*v_buffer;
    t++;
    return t;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    ArgParser aP;
    vector<string> const *arg_buf;
    int threads=1,topN=5;
    ifstream ifile1,ifile2;ofstream ofile1,ofile2;
    bool testFlag=false,lowMemory=true;
    string pathBuffer,buffer;
    //string pathTree,pathLoad,pathSample,pathOut,pathSave,pathConvert,pathOutTree,pathOutOrder;
    parser *p = nullptr;loader *database,*sample;
    cout << "Welcome to using meta-prism 2.0";
#ifdef __MP2_Normal__
    cout<<" normal model\n";
#endif
#ifdef __MP2_useAVX__
    cout<<" AVX model\n";
#endif
    if(aP.parse(argc, argv)<0){
        cout<<"Command line args error\n";
        aP.printHelp();
        return 0;}
    clock_t startTime,finishTime;
    if(aP.exist("-h")||aP.exist("--help")){
        aP.printHelp();return 0;}
    
    if(aP.exist("--test")){
        startTime=clock();
        testFlag=true;
    }
    if((arg_buf=aP.get("-t","--tree"))){
        ifile1.open((*arg_buf)[0]);
        stringstream buf;
        string as,line;
        while(getline(ifile1, line)){
            buf<<line;
        }
        ifile1.close();
        as=buf.str();
        p=new parser(as);
    }
    else{
        cout<<"Error, not defined newick tree path\n";
        return 0;
    }
    
    if((arg_buf=aP.get("-l","--load"))){// loading basic sample set
        database=new loader(p);
        pathBuffer=(*arg_buf)[1];
        cout<<"loading from "<<pathBuffer<<endl;
        buffer=(*arg_buf)[0];
        if(buffer=="list"){
            ifile1.open(pathBuffer);
            database->loadMultiTSV(ifile1);
        }
        else if (buffer=="OTU"){
            ifile1.open(pathBuffer);
            database->loadOTUData(ifile1);
        }
        else if (buffer=="ascii"){
            ifile1.open(pathBuffer);
            database->loadFromMirror(ifile1);
        }else if (buffer=="binary"){
            ifile1.open(pathBuffer,ios::binary);
            database->loadBMultiTSV(ifile1);
        }
        else{
            cout<<"Error, undefined load type: "<<buffer<<endl;
            return 0;
        }
        ifile1.close();
    }
    else{
        cout<<"Error, not defined load database path\n";
        return 0;
    }
    
    if(testFlag){
        finishTime=clock();
        cout<<"load use"<<(double)(finishTime-startTime)/CLOCKS_PER_SEC<<endl;
    }
    
    if((arg_buf=aP.get("-p","--package"))){// package basic dataset as ascii format or binary format
        pathBuffer=(*arg_buf)[1];
        buffer=(*arg_buf)[0];
        cout<<"\npackaging data to "<<pathBuffer<<" as "<<buffer<<endl;
        if(buffer=="ascii"){
            ofile1.open(pathBuffer);
            database->outputMirror(ofile1);
        }else if(buffer=="binary"){
            ofile1.open(pathBuffer,ios::binary);
            database->outputBMirror(ofile1);
        }
        else {
            cout<<"\nError, undefined package type:"<<buffer<<endl;
            return 0;
        }
        ofile1.close();
    }
    
    if((arg_buf=aP.get("--threads","-T"))){
        buffer=(*arg_buf)[0];
        threads=atoi(buffer.c_str());
    }
    
    uFP16 d;
    lowMemory=d.check();
    
    if(!lowMemory)
        cout<<"\nCompiler or device don't follow IEEE854, can't use low memory mode"<<endl;
    
    if(aP.get("-m","--matrix")){// matrix mode: calculate and output the similarity matrix
        if(!(arg_buf=aP.get("-o","--output"))){
            if(!testFlag)
                cout<<"\nWarning, execute matrix comparison but didn't give a output path\nMeta-Prism 2.0 will skip save step\n";
        }
        auto result=matrixBoostCompare(*database,threads,lowMemory);
        if(testFlag)
            cout<<"skip save step"<<endl;
        else{
        pathBuffer=(*arg_buf)[0];
            ofile1.open(pathBuffer);
        cout<<"\nSaving calculation result to: "<<pathBuffer<<endl;
            result->save(ofile1);
        }
    }
    
    if((arg_buf=aP.get("-s","--search"))){// search mode: load another sample set and calcualte between two sets, then output TopN match or similarity matrix based on user
        pathBuffer=(*arg_buf)[1];
        buffer=(*arg_buf)[0];
        
        sample=new loader(p);
        if(buffer=="list"){
            ifile2.open(pathBuffer);
            sample->loadMultiTSV(ifile2);
        }else if(buffer=="single"){
            ifile2.open(pathBuffer);
            sample->loadTSVFile(ifile2);
        }
        else if (buffer=="OTU"){
            ifile2.open(pathBuffer);
            sample->loadOTUData(ifile2);
        }
        else if (buffer=="ascii"){
            ifile2.open(pathBuffer);
            sample->loadFromMirror(ifile2);
        }else if (buffer=="binary"){
            ifile2.open(pathBuffer,ios::binary);
            sample->loadBMultiTSV(ifile2);
        }else{
            cout<<"Error, undefined load type"<<buffer<<endl;
            return 0;
        }
        ifile2.close();
        
        if(arg_buf->size()>2){
            buffer=(*arg_buf)[2];
            if(buffer[0]=='f')// if user send 'f', return full similarity matrix
                topN=-1;
                else
            topN=atoi((*arg_buf)[2].c_str());}
        else
            topN=5;
        if((arg_buf=aP.get("-o","--output"))){
            ;
        }else{
            if(!testFlag)
                cout<<"\nWarning, execute matrix comparison but didn't give a output path\nMeta-Prism 2.0 will skip save step\n";
        }
        if(topN>=0){
            auto result = searchBoostCompare(*database, *sample, threads, topN);// search and get Top N best match as result
            if (testFlag)
                cout << "skip save step" << endl;
            else {
                pathBuffer = (*arg_buf)[0];
                ofile1.open(pathBuffer);
                cout << "\nSaving calculation result to: " << pathBuffer << endl;
                sample->genName();
                database->genName();
                result->save(ofile1,database->names,sample->names);
                ofile1.close();
            }
        }else{
                auto result=fullSearchBoostCompare(*database, *sample, threads, lowMemory);// search and output similarity matrix
                if (testFlag)
                    cout << "skip save step" << endl;
                else {
                    pathBuffer = (*arg_buf)[0];
                    ofile1.open(pathBuffer);
                    cout << "\nSaving calculation result to: " << pathBuffer << endl;
                    result->save(ofile1);
                }
            }
    }
    
    //following functions are not shown in --help, because we think these are not frequently used
    if((arg_buf=aP.get("--convertOTU"))){
        pathBuffer=(*arg_buf)[0];
        ofile1.open(pathBuffer);
        cout<<"Converting data to: "<<pathBuffer<<endl;
        database->printToTable(ofile1);
        ofile1.close();
    }
    if((arg_buf=aP.get("--printTree"))){
        pathBuffer=(*arg_buf)[0];
        ofile1.open(pathBuffer);
        p->printTree(ofile1);
        ofile1.close();
    }
    if((arg_buf=aP.get("--printOrder"))){
        pathBuffer=(*arg_buf)[0];
        ofile1.open(pathBuffer);
        p->genCompData();
        p->printCompData(ofile1);
        ofile1.close();
    }
    return 0;
}
