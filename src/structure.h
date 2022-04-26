//
//  structure.h
//  prism2
//
//  Created by 康凯 on 2019/12/23.
//  Copyright © 2019 康凯. All rights reserved.
//

/*
  following is main structures used in Meta-Prism 2.0
 */
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <unordered_map>
#include <stack>
#include <cmath>
#include <algorithm>
#include <pthread.h>
#include <map>
#include <fstream>
#include <string.h>
using namespace std;
#ifndef structure_h
#define structure_h
/*!
@brief  Phylogenic tree storage unit
@note Each node records the name, ID, depth, left and right child nodes and the distance from the child nodes
*/
struct TreeNode{
    string name;
    int id=-1,dep,buf;
    TreeNode *lChild= nullptr,*rChild= nullptr,*father=nullptr;
    float lDist=0,rDist=0,fDist=0;
    bool cFlag;//Left child:false,right child true
};
struct Table{
    int fid,bid,dep;float dist;//treeNode's father id is -1
};
/*!
@brief  Basic unit of sample data
@note Store each nodes' ID  corresponding to evolutionary tree and relative abundance
*/
struct abdElement{
    uint32_t ID;float data;
};
/*!
@brief Storage each sample
*/
struct SampleData{
    string name;
    vector<abdElement> data;
};
/*!
@brief Comparison order data
@note Each element store informations to operate at one node. That is to obtain the abundance from the left child node (order_1), multiply the evolutionary distance of the left child node (dist_1), and then obtain the relative abundance of the right child node (order_2) and multiply the evolutionary distance (dist_2). After the common abundance is obtained, the residual abundance is stored in the current node (order_N).
*/
struct CompData{
    int *order_1,*order_2,*order_N;
    float *dist_1,*dist_2;
    int init(int sum){
        dist_1=new float[sum];
        dist_2=new float[sum];
        order_1=new int[sum];
        order_2=new int[sum];
        order_N=new int[sum];
        return 0;
    }
    ~CompData(){
        delete []dist_2;delete []dist_1;delete []order_N;delete []order_2;delete []order_1;
    }
};

struct index_value{
    unsigned short int index;
    float value;
};
/*!
@brief unsigned 16 bit float point
@note Space efficient method of saving similarity matrix result for reducing memory usage, just take 16 bit. But this need compiler and platform follow IEEE854. Since similarity value is between 0~1, we can save two sign bits from exponent and fraction. First [exp] bit for exponent and last 16-[exp] bit for fraction.
*/
class uFP16{
    
    const static uint32_t exp=4,shiftExp=23,frac=16-exp,shiftFrac=shiftExp-frac;// user can change [exp] by their self, but it's not recommanded
    static uint32_t getFrac,Zero;//these will be calculated based on exp
    const static uint32_t getExp=0x0000FF;
    const static uint32_t Min=0;
    const static uint32_t Head=(0x1)<<29;
    union Bits
    {
        float f;
        int32_t si;
        uint32_t ui;
    };
private:
    uint16_t data;// take 16 bit for storage
public:
    uFP16(){
        data=Zero;
    }
    void inline operator =(const float & value);//take 32 bit float input and convert to 16 bit FP
    inline operator float();//convert 16 bit FP to 32 bit float
    bool check();// use some value to check wether compiler and platform follow IEEE854
};

ostream  &operator<<(ostream &out, uFP16 &c1);

class ProgressBar{
public:
    uint64_t value,all;
    float percent;
    int print=0;
    void init(uint64_t a){
        value=0;all=a;
        if(a==0)
            all=1;
        percent=100*value/all;
        cout<<endl;
    }
    void show(unsigned int a){
        value+=a;
        percent=100*(float)value/(float)all;
        while(percent>print){
            print++;
            if(print%50==0)
                cout<<'|';
            else if(print%10==0)
                cout<<'&';
            else
                cout<<'*';
            cout<<flush;
        }
    }
};

class matrixModeResult{
private:
    template <typename T>
    int alloc(int x,T*** sData);
    template <typename T>
    int write(ofstream &ofile,T** sData);
    pthread_mutex_t showLock;
public:
    ProgressBar pBar;
    int x,y;
    bool symmetry=false;
    bool big=false,lowMem=false;
    vector<string> nameA,nameB;
    float **data;
    uFP16 **data16;
    int load(ifstream &ifile);
    int save(ofstream &ofile);
    ~matrixModeResult(){
        if (lowMem){
            for(int i=0;i<x;i++)
                delete[] data16[i];
            
            delete [] data16;
        }
        else{
            for(int i=0;i<x;i++)
                delete[] data[i];
            
            delete [] data;
            
        }
        return;
    }
    int dataAlloc(int x,bool lowFlag=false);
    int sendResult(int i,float* result);
};


class searchResult{
public:
    int x,topN;
    index_value **data;
    int dataAlloc(int x,int topN){
        data=new index_value*[x];
        this->x=x;
        this->topN=topN;
        data[0]=new index_value[x*topN];
        for(int i=1;i<x;i++)
            data[i]=data[i-1]+topN;
        return 0;
    }
    ~searchResult(){
        delete [] data[0];
        delete [] data;
    }
    int save(ofstream &ofile,vector<string>& DNames,vector<string>&SNames) ;
};
class searchModeFullResult{
private:
    template <typename T>
    int alloc(int x,int y,T*** sData);
    template <typename T>
    int write(ofstream &ofile,T** sData);
    pthread_mutex_t showLock;
public:
    int x,y;
    ProgressBar pBar;
    bool big=false,lowMem=false;
    vector<string> nameA,nameB;
    float **data;
    uFP16 **data16;
    int load(ifstream &ifile);
    int save(ofstream &ofile);
    ~searchModeFullResult(){
        if (lowMem){
            for(int i=0;i<x;i++)
                delete[] data16[i];
            delete [] data16;
        }
        else{
            for(int i=0;i<x;i++)
                delete[] data[i];
            delete [] data;
        }
        return;
    }
    int dataAlloc(int x,int y,bool lowFlag=false);
    int sendResult(int i,float* result);
};
#endif /* structure_h */
