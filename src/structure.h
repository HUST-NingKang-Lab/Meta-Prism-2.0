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
class compareResult{
public:
    int x,y;
    bool symmetry=false;
    bool big=false;
    vector<string> nameA,nameB;
    float **data;
    int load(ifstream &ifile);
    int save(ofstream &ofile);
    ~compareResult(){
        if(big){
            for(int i=0;i<x;i++)
                delete[] data[i];
        }
        else
        delete []data[0];
        delete [] data;
        return;
    }
    int dataAlloc(int x,int y,bool sym=false);
};
class progressBar{
public:
    float value,all,percent;
    int print=0;
    void init(int a){
        value=0;all=a;
        percent=100*value/all;
        cout<<endl;
        cout<<"percents:"<<flush;
    }
    void show(int a){
        value=a;
        percent=100*value/all;
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

#endif /* structure_h */
