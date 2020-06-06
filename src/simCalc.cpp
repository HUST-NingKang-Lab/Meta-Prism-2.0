//
//  sparse_sim_calc.cpp
//  prism2
//
//  Created by 康凯 on 2019/12/23.
//  Copyright © 2019 康凯. All rights reserved.
//

#include "simCalc.hpp"

struct state{
    float lSim=0,rSim=0;
    bool prepared=false,calculated=false;
};
struct twoValue{
    float lABD,rABD;
};
bool i_v_cmp(const index_value& a,const index_value& b){
    return a.value>b.value;
}
void calcOne(unordered_map<int,state> stateDic,multimap<int,int> toDo);
float sparseSimCalcPair(const sampleData &a,const sampleData &b,const Table* table){
    float sim=0,m,dis;int fid;
    unordered_map<int,state> stateDic;
    multimap<int,int,greater<int> > toDo;
    unordered_map<int, twoValue> beginData;
    for(auto iter=a.data.begin();iter!=a.data.end();iter++){
        stateDic[iter->ID].lSim=iter->data;
        stateDic[iter->ID].prepared=true;
        toDo.insert(make_pair(table[iter->ID].dep, iter->ID));
    }for(auto iter=b.data.begin();iter!=b.data.end();iter++){
        stateDic[iter->ID].rSim=iter->data;
        if(stateDic[iter->ID].prepared!=true){
            stateDic[iter->ID].prepared=true;
            toDo.insert(make_pair(table[iter->ID].dep, iter->ID));
        }
    }
    state sBuf;
    auto iter=toDo.begin();
    while(toDo.size()){
        //if(stateDic[iter->second].prepared==true){
        
        sBuf=stateDic[iter->second];
        m=min(sBuf.lSim,sBuf.rSim);
        sim+=m;
        fid=table[iter->second].fid;
        if(fid==-1)//root
            break;
        if(sBuf.lSim==sBuf.rSim||stateDic[iter->second].calculated){
            toDo.erase(iter);
            iter=toDo.begin();
            continue;
        }
        stateDic[iter->second].calculated=true;
        dis=table[iter->second].dist;
        stateDic[fid].lSim+=(1-dis)*(sBuf.lSim-m);
        stateDic[fid].rSim+=(1-dis)*(sBuf.rSim-m);
        //cout<<"c: "<<iter->second<<endl;
        if(stateDic[fid].prepared){}
        else{
            toDo.insert(make_pair(table[fid].dep, fid));
            stateDic[fid].prepared=true;
        }
        toDo.erase(iter);
        iter=toDo.begin();
        
    }
    sim=sim<1?sim:1;
    sim=sim>0?sim:0;
    return sim;
}
float sparseSimCalcNew(const sampleData &a,const sampleData &b,const Table* table,int depth){
    float sim=0,m,dis;int fid,nextID;
    if(!(a.data.size()&&b.data.size()))
        return 0;
    unordered_map<int,state> stateDic;
    auto nmp= new NewMap(depth);
    auto &nm=*nmp;
    for(auto iter=a.data.begin();iter!=a.data.end();iter++){
        stateDic[iter->ID].lSim=iter->data;
        stateDic[iter->ID].prepared=true;
        nm.send(table[iter->ID].dep,iter->ID);
    }for(auto iter=b.data.begin();iter!=b.data.end();iter++){
        stateDic[iter->ID].rSim=iter->data;
        if(stateDic[iter->ID].prepared!=true){
            stateDic[iter->ID].prepared=true;
            nm.send(table[iter->ID].dep,iter->ID);
        }
    }
    nm.calculate();
    state sBuf;
    nm.get(nextID);
    while(!nm.nEmpty){
        sBuf=stateDic[nextID];
        m=min(sBuf.lSim,sBuf.rSim);
        sim+=m;
        fid=table[nextID].fid;
        if(fid==-1)//root
            break;
        if(sBuf.lSim==sBuf.rSim||stateDic[nextID].calculated){
            nm.get(nextID);
            continue;
        }
        stateDic[nextID].calculated=true;
        dis=table[nextID].dist;
        stateDic[fid].lSim+=(1-dis)*(sBuf.lSim-m);
        stateDic[fid].rSim+=(1-dis)*(sBuf.rSim-m);
        //cout<<"c: "<<iter->second<<endl;
        if(stateDic[fid].prepared){}
        else{
            nm.add(fid);
            stateDic[fid].prepared=true;
        }
        nm.get(nextID);
    }
    sim=sim<1?sim:1;
    sim=sim>0?sim:0;
    delete nmp;
    return sim;
}
float denseSimCalcPair(const sampleData &a,const sampleData &b,const CompData* compData,int size,int depth){
    int REG_SIZE=depth+2;
    int OrderN=size-1;
    float *Abd_1,*Abd_2;
    Abd_1=new float[size]();
    Abd_2=new float[size]();
    for(auto iter=a.data.begin();iter!=a.data.end();iter++)
        Abd_1[iter->ID]=iter->data;
    for(auto iter=b.data.begin();iter!=b.data.end();iter++)
        Abd_2[iter->ID]=iter->data;
    float *Reg_1=new float[REG_SIZE+2];
    float *Reg_2=new float[REG_SIZE+2];
    const int *Order_1=compData->order_1,*Order_2=compData->order_2,*Order_d=compData->order_N;
    float *Dist_1=compData->dist_1,*Dist_2=compData->dist_2;
    float total = 0;
    int order_1,order_2,order_d=0;
    float c1_1,c1_2,c2_1,c2_2,dist_1,dist_2;
    for(int i = 0; i < OrderN; i++){
        order_1 = Order_1[i];
        order_2 = Order_2[i];
        order_d = Order_d[i] + REG_SIZE;
        dist_1 = 1- Dist_1[i];
        dist_2 = 1- Dist_2[i];
        
        if (order_1 >= 0){
            c1_1 = Abd_1[order_1];
            c1_2 = Abd_2[order_1];
        }else {
            c1_1 = Reg_1[order_1 + REG_SIZE];
            c1_2 = Reg_2[order_1 + REG_SIZE];
        }
        if (order_2 >= 0){
            c2_1 = Abd_1[order_2];
            c2_2 = Abd_2[order_2];
        }else {
            c2_1 = Reg_1[order_2 + REG_SIZE];
            c2_2 = Reg_2[order_2 + REG_SIZE];
        }
        //min
        float min_1 = (c1_1 < c1_2)?c1_1:c1_2;
        float min_2 = (c2_1 < c2_2)?c2_1:c2_2;
        total += min_1;
        total += min_2;
        Reg_1[order_d] = (c1_1 - min_1) * dist_1 + (c2_1 - min_2) * dist_2;
        Reg_2[order_d] = (c1_2 - min_1) * dist_1 + (c2_2 - min_2) * dist_2;
    }
    total+=(Reg_1[order_d]<Reg_2[order_d])?Reg_1[order_d]:Reg_2[order_d];
    total = (total > 1.0) ? 1 : total;
    total = (total < 0.0) ? 0 : total;
    delete []Abd_1;delete []Abd_2;delete []Reg_2;delete []Reg_1;
    return total;
}
float* multiSparseCompare(float ** a,float**b,const CompData* compData,int sampleSize,int orderSize){
    float *result;
    int order_1=0,order_2=0,order_N=0;float dist_1,dist_2;
    float *min_1,*min_2;// *c1_1,*c1_2,*c2_1,*c2_2,
    result=new float[sampleSize]();/*
                                    c1_1=new float[sampleSize]();
                                    c1_2=new float[sampleSize]();
                                    c2_1=new float[sampleSize]();
                                    c2_2=new float[sampleSize]();*/
    min_1=new float[sampleSize]();
    min_2=new float[sampleSize]();
    for(int i=0;i<orderSize;i++){
        order_1=compData->order_1[i];
        order_2=compData->order_2[i];
        order_N=compData->order_N[i];
        dist_1=1-compData->dist_1[i];
        dist_2=1-compData->dist_2[i];
        for(int j=0;j<sampleSize;j++){
            min_1[j]=min(a[order_1][j],b[order_1][j]);
            min_2[j]=min(a[order_2][j],b[order_2][j]);
            result[j]+=min_1[j]+min_2[j];
            a[order_N][j]+=(a[order_1][j]-min_1[j])*dist_1+(a[order_2][j]-min_2[j])*dist_2;
            b[order_N][j]+=(b[order_1][j]-min_1[j])*dist_1+(b[order_2][j]-min_2[j])*dist_2;
        }
        
        
    }
    for(int j=0;j<sampleSize;j++)
    {
        
        result[j]+=min(a[order_N][j],b[order_N][j]);
        result[j]=min(result[j],(float)1);
        result[j]=max(result[j],(float)0);
    }
    delete[] min_2;delete []min_1;
    return result;
}

compareResult* matrixCompare( class loader & A){
    compareResult* result;
    int x,i,j;
    result=new compareResult;
    x=result->x=result->y=A.size();
    result->dataAlloc(x,x);result->symmetry=true;
    A.genName();
    cout<<"calculating "<<x<<'*'<<x<<" similarity matrix\n";
    auto pBar=new progressBar;
    pBar->init(x*x/2);
    int m=0;auto iterI=A.Data.begin();
    for(i=0;i<x;i++){
        result->data[i][i]=1;
        auto iterJ=iterI;
        for(j=i+1;j<x;j++){
            iterJ++;
            m+=1;
            if(m%20==0)
                pBar->show(m);
            //cout<<i<<' '<<j<<endl;
            result->data[i][j]=result->data[j][i]=sparseSimCalcNew(*(iterI->second),*(iterJ->second), A.p->getTable(), A.p->getDepth());
            //result->data[i][j]=result->data[j][i]=sparseSimCalcPair(*(iterI->second),*(iterJ->second), A.p->getTable());
            
        }
        iterI++;
    }
    delete pBar;
    result->nameA=A.names;
    return result;
}
void *lineCompare(void * args){
    LineCompareArg * arg=(LineCompareArg*)args;
    if(arg->B==NULL)
        arg->B=arg->A;
    int size=arg->B->size();
    auto iter=arg->B->Data.begin();
    for(int i=0;i<arg->id;i++){
        iter++;
        if(iter==arg->B->Data.end())
            return NULL;
    }
    for(int i=arg->id;i<size;i+=arg->core){
        if(arg->B==arg->A){//matrix
            
            auto boost=new booster(arg->A->p);
            boost->setData(iter->second);
            boost->convert(*(arg->A),i+1);
            //boost->genCompareData();
            auto bResult=boost->calc();
            arg->result->sendResult(i, bResult);
            delete bResult;
            delete boost;
        }
        else{
            int j,topN=arg->number;
            int x=arg->A->size();
            auto boost=new booster(arg->A->p);
            searchResult* result=(searchResult*)arg->result;
            if(iter->second->data.size()==0){
                delete boost;
                continue;}
            boost->setData(iter->second);
            boost->convert(*(arg->A));
            auto bResult=boost->calc();
            vector<struct index_value> datas;
            datas.resize(x);
            for(j=0;j<x;j++){
                datas[j].index=j;
                datas[j].value=bResult[j];
            }
            partial_sort(datas.begin(), datas.begin()+topN, datas.end(),i_v_cmp);
            for(j=0;j<topN;j++)
                result->data[i][j]=datas[j];
            delete boost;
            delete bResult;
        }
        for(int j=0;j<arg->core;j++){
            if(iter!=arg->B->Data.end())
                iter++;
            else
                return NULL;
        }
    }
    return NULL;
}
compareResult* matrixBoostCompare( class loader & A,int core,bool lowMem){
    compareResult* result;
    
    booster * boost;
    int x,i,j;
    if(core <1){
        cout<<"Warning, CPU cores must >=1, we except you select 1\n";
        core=1;}
    result=new compareResult;
    x=result->x=result->y=A.size();
    result->dataAlloc(x,lowMem);
    A.genName();
    cout<<"calculating "<<x<<'*'<<x<<" similarity matrix\n";
    
    if(core >1){
        pthread_t * tids;
        LineCompareArg * args;
        tids=new pthread_t[core];
        args=new LineCompareArg[core];
        for(int i=0;i<core;i++){
            args[i].A=&A;args[i].B=NULL;args[i].core=core;
            args[i].id=i;args[i].result=result;args[i].number=x;
            pthread_create(&(tids[i]), NULL, lineCompare, (void*)&(args[i]));
        }
        for(int i=0;i<core;i++)
            pthread_join(tids[i], NULL);
        delete []tids;delete []args;
    }
    else{
        auto iterI=A.Data.begin();
        for(i=0;i<x;i++,iterI++){
            boost=new booster(A.p);
            if(iterI->second->data.size()==0)
                continue;
            boost->setData(iterI->second);
            boost->convert(A,i+1);
            auto bResult=boost->calc();
            result->sendResult(i, bResult);
            delete bResult;
            delete boost;
            
        }}
    result->nameA=A.names;
    cout<<endl;
    return result;
}
compareResult* searchCompare( class loader &A,  class loader &B, int core){
    compareResult* result;
    progressBar pb;
    int x,y,i,j;
    result=new compareResult;
    x=result->x=A.size();
    y=result->y=B.size();
    cout<<"calculating "<<x<<'*'<<y<<" searching\n";
    result->dataAlloc(x, y);
    A.genName();B.genName();
    pb.init(x*y/2);
    int m=0;
    auto iterI=A.Data.begin();
    for(i=0;i<x;i++){
        auto iterJ=B.Data.begin();
        for(j=0;j<y;j++){
            m+=1;
            if(m%20==0)
                pb.show(m);
            result->data[i][j]=result->data[j][i]=sparseSimCalcNew(*(iterI->second),*(iterJ->second), A.p->getTable(), A.p->getDepth());
            iterJ++;
        }
        iterI++;
    }
    result->nameA=A.names;
    result->nameB=B.names;
    
    return result;
}

searchResult* searchBoostCompare(class loader &A,class loader &B,int core,int topN){
    searchResult*result;
    booster * boost;
    int x,y,i,j;
    if(core <1){
        cout<<"Warning, CPU cores must >=1, we except you select 1\n";
        core=1;}
    result=new searchResult;
    x=A.size();y=B.size();
    result->dataAlloc(B.size(),topN);
    A.genName();
    cout<<"calculating "<<x<<'*'<<x<<" similarity matrix\n";
    auto pBar=new progressBar;
    pBar->init(x*x);
    if(core >1){
        pthread_t * tids;
        LineCompareArg * args;
        tids=new pthread_t[core];
        args=new LineCompareArg[core];
        for(int i=0;i<core;i++){
            args[i].A=&A;args[i].B=&B;args[i].core=core;
            args[i].id=i;args[i].result=(compareResult*)result;args[i].number=topN;
            pthread_create(&(tids[i]), NULL, lineCompare, (void*)&(args[i]));
        }
        for(int i=0;i<core;i++)
            pthread_join(tids[i], NULL);
        delete []tids;delete []args;
    }
    else{
        int m=0;auto iterI=B.Data.begin();
        for(i=0;i<y;i++,iterI++){
            
            if(iterI->second->data.size()==0)
                continue;
            boost=new booster(A.p);
            boost->setData(iterI->second);
            boost->convert(A);
            auto bResult=boost->calc();
            vector<struct index_value> datas;
            datas.resize(x);
            for(j=0;j<x;j++){
                //cout<<i<<' '<<j<<endl;
                datas[j].index=j;
                datas[j].value=bResult[j];
            }
            pBar->show(x);
            partial_sort(datas.begin(), datas.begin()+topN, datas.end(),i_v_cmp);
            for(j=0;j<topN;j++)
                result->data[i][j]=datas[j];
            delete bResult;
            delete boost;
            
        }}
    delete pBar;
    //result->nameA=A.names;
    return result;
}
