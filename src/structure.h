//
//  structure.h
//  prism2
//
//  Created by 康凯 on 2019/12/23.
//  Copyright © 2019 康凯. All rights reserved.
//
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
struct abdElement{
    int ID;float data;
};
struct sampleData{
    string name;
    vector<abdElement> data;
};
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
struct waitElement{
    int dep,id;
};
class NewMap{
public:
    bool empty,nEmpty;
    NewMap(int depth){
        this->depth=depth;
        nums=new int[depth+1]();index=new int[depth+1]();
        sum=0;
        empty=false;
    }
    inline void send(int dep,int id){
        sum++;nums[dep]++;
        waitElement a;
        a.dep=dep;a.id=id;
        inputs.push_back(a);
    }
    bool calculate(){
        int sumUp=0;
        for(int i=depth;i>=0;i--){
            index[i]=sumUp;
            sumUp+=nums[i];
        }
        data=new int [sumUp+1]();
        for (auto m=inputs.begin();m!=inputs.end();m++){
            data[index[m->dep]++]=m->id;
        }
        iter=depth-1;ptr=index[iter]-1;
        return true;
    }
    bool get(int &id){
        nIter=iter;nEmpty=empty;
        sum--;
        if(ptr<index[iter+1]){
            if(iter==0 || sum==0)
                empty=true;
            else {
                iter--;
                while(index[iter+1]==index[iter])
                    iter--;
            }
            ptr=index[iter]-1;
        }
        id=data[ptr--];
        return true;
    }
    bool add(int id){
        sum++;
        nIter=iter;
        if(index[nIter]<ptr){
            cout<<"Type 1 ERROR\n";
            return false;
        }
        else
            data[--index[nIter]]=id;
        return true;
    }
    ~NewMap(){
        delete nums;
        delete data;
        delete index;
        return;
    }
private:
    int *nums,*data,depth,*index,ptr,sum,iter,nIter;
    vector<waitElement> inputs;
};

class uFP16{
    //Storage Result
    //Value is between 0~1
    //first 6 bit exponent
    //last 10 bit fraction
    const static uint32_t exp=4,shiftExp=23,frac=16-exp,shiftFrac=shiftExp-frac;
    static uint32_t getFrac;//0x7FF;
    static uint32_t Zero;
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
    uint16_t data;
public:
    uFP16(){
        data=Zero;
    }
    void inline operator =(const float & value);
    inline operator float();
    bool check();
};

void uFP16::operator=(const float &value)
{
    uFP16::Bits f,e;
    e.f=value;
    e.ui=e.ui>>shiftExp;
    e.ui&=getExp;
    e.ui=127-e.ui;
    if(e.ui>>exp){
        data=Zero;
        return;
    }
    e.ui<<=frac;
    f.f=value;
    f.ui>>=shiftFrac;
    f.ui &= getFrac;
    data=e.ui^f.ui;
    return;
}
uFP16::operator float(){
    uFP16::Bits e,f;
    f.ui=data&getFrac;
    f.ui=f.ui<<shiftFrac;
    e.ui=(data>>frac);
    e.ui=(127-e.ui);
    e.ui=e.ui<<shiftExp;
    e.ui=f.ui^e.ui;
    return e.f;
}
ostream  &operator<<(ostream &out, uFP16 &c1);
class progressBar{
public:
    uint64_t value,all;
    float percent;
    int print=0;
    void init(uint64_t a){
        value=0;all=a;
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

class compareResult{
private:
    template <typename T>
    int alloc(int x,T*** sData);
    template <typename T>
    int output(ofstream &ofile,T** sData);
    pthread_mutex_t showLock;
public:
    progressBar pBar;
    int x,y;
    bool symmetry=false;
    bool big=false,lowMem=false;
    vector<string> nameA,nameB;
    float **data;
    uFP16 **data16;
    int load(ifstream &ifile);
    int save(ofstream &ofile);
    ~compareResult(){
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
};

#endif /* structure_h */
