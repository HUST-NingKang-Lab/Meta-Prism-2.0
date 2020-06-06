//
//  booster.cpp
//  prism2
//
//  Created by 康凯 on 2020/2/28.
//  Copyright © 2020 康凯. All rights reserved.
//

#include "booster.hpp"
int booster::treeWalk(const TreeNode *a, int id, float coefficient){
    int thisId=a->id;
    bool sim=false;
    //const Table* compareTable=p->getTable();
    sim=(table[thisId].select);
    if(sim){
        id=thisId;
        coefficient=1;
    }
    else{
        table[thisId].targetID=id;
        table[thisId].coef=coefficient;
    }
    if(a->lChild==nullptr){
        ;
    }else{
        treeWalk(a->lChild, id, coefficient*(1-a->lDist));
        treeWalk(a->rChild, id, coefficient*(1-a->rDist));
    }
    return 0;
}
int booster::setData(sampleData *A){
    if(p==nullptr){
        return -1;
    }
    int id;
    data=A;
    const Table* compareTable=p->getTable();
    table=new boostTable[p->getTreeSize()];
    for(int i=0;i<p->getTreeSize();i++)
        table[i].select=false;
    for(auto iter=A->data.begin();iter!=A->data.end();iter++){
        id=iter->ID;
        while(id>=0&&table[id].select==false){
            table[id].targetID=id;
            table[id].coef=1;
            table[id].select=true;
            id=compareTable[id].fid;
            elementNumber++;
        }
    }
    elementNumber++;
    treeWalk(p->getTree(), p->getTree()->id, 1);
    genCompareData();
    return 0;
}
void booster::genCompareData(){
    //compareData.init(elementNumber);
    int alloc=1;
    genWalk(p->getTree(), alloc);
    for(int i=0;i<p->getTreeSize();i++){
        if(table[i].select==false)
            table[i].targetID=table[table[i].targetID].targetID;
        //cout<<i<<' '<<table[i].targetID<<' '<<table[i].coef<<endl;
    }
    compareData.init((int)writeBuff.size());
    for(int i=0;i<writeBuff.size();i++){
        compareData.dist_1[i]=writeBuff[i].dist_1;
        compareData.dist_2[i]=writeBuff[i].dist_2;
        compareData.order_1[i]=writeBuff[i].order_1;
        compareData.order_2[i]=writeBuff[i].order_2;
        compareData.order_N[i]=writeBuff[i].order_N;
        //cout<<writeBuff[i].order_1<<' '<<writeBuff[i].order_2<<' '<<writeBuff[i].order_N<<'\n';
    }
    orderSize=(int)writeBuff.size();
    
    return;
}
int booster::genWalk(const TreeNode *a, int &alloc){
    CompElement buf;int id;
    if(a->lChild!=nullptr){
        if(table[a->lChild->id].select)
            buf.order_1=genWalk(a->lChild, alloc);
        else buf.order_1=0;
        if(table[a->rChild->id].select)
            buf.order_2=genWalk(a->rChild, alloc);
        else buf.order_2=0;
        id=alloc++;
        if(buf.order_1||buf.order_2){
        buf.order_N=id;
        buf.dist_1=a->lDist;
        buf.dist_2=a->rDist;
        writeBuff.push_back(buf);}
    }
    else{
        id=alloc++;
    }
    table[a->id].targetID=id;
    return id;
}
int booster::convert(loader &A,int begin,int end){
    if(begin <0)
        begin=0;
    if (end<0)
        end=A.size();
    sampleSize=end-begin;
    matrix=new float*[elementNumber+1]();source=new float*[elementNumber+1]();
    matrix[0]=new float[(elementNumber+1)*sampleSize]();source[0]=new float[(elementNumber+1)*sampleSize]();
    for(int i=1;i<=elementNumber;i++){
        matrix[i]=matrix[0]+i*sampleSize;
        source[i]=source[0]+i*sampleSize;
    }
    int targetID;float val;
    int i=0,j;
    auto iter=A.Data.begin();
    for(j=0;j<begin;j++)
        iter++;
    for(;j<end;j++,iter++,i++){
        auto &data=iter->second->data;
        for(auto dataIter=data.begin();dataIter!=data.end();dataIter++){
            auto& x=table[dataIter->ID];
            targetID=x.targetID;
            val=x.coef*dataIter->data;
            matrix[targetID][i]+=val;
        }
    }
    auto &sourceData=this->data->data;
    for(auto dataIter=sourceData.begin();dataIter!=sourceData.end();dataIter++){
        targetID=table[dataIter->ID].targetID;
        val=table[dataIter->ID].coef*dataIter->data;
        for(int j=0;j<sampleSize;j++)
            source[targetID][j]+=val;
    }
    return 0;
}
float* booster::calc()
{
    return multiSparseCompare(source, matrix, &compareData, sampleSize, orderSize);
}
