//
//  sparse_sim_calc.cpp
//  prism2
//
//  Created by 康凯 on 2019/12/23.
//  Copyright © 2019 康凯. All rights reserved.
//

#include "simCalc.hpp"

bool i_v_cmp(const index_value& a,const index_value& b){
    return a.value>b.value;
}

float* multiSparseCompare(float ** a,float**b,const CompData* compData,int sampleSize,int orderSize){
    float *result;
    int order_1=0,order_2=0,order_N=0;float dist_1,dist_2;
    //float *min_1,*min_2;// *c1_1,*c1_2,*c2_1,*c2_2,
    result=new float[sampleSize]();/*
                                    c1_1=new float[sampleSize]();
                                    c1_2=new float[sampleSize]();
                                    c2_1=new float[sampleSize]();
                                    c2_2=new float[sampleSize]();*/
    //min_1=new float[sampleSize]();
    //min_2=new float[sampleSize]();
    for(int i=0;i<orderSize;i++){
        order_1=compData->order_1[i];
        order_2=compData->order_2[i];
        order_N=compData->order_N[i];
        dist_1=1-compData->dist_1[i];
        dist_2=1-compData->dist_2[i];
        int j,step,AVXStep=0;
#ifdef __MP2_useAVX__
        AVXStep=sampleSize/8;
        for(step=0;step<AVXStep;step++){
            j=step*8;
            __m256 _min1,_min2,_a_1,_a_2,_b_1,_b_2,_result;
            _a_1=_mm256_loadu_ps(a[order_1]+j);
            _a_2=_mm256_loadu_ps(a[order_2]+j);
            _b_1=_mm256_loadu_ps(b[order_1]+j);
            _b_2=_mm256_loadu_ps(b[order_2]+j);
            _min1=_mm256_min_ps(_a_1, _b_1);
            _min2=_mm256_min_ps(_a_2, _b_2);
            _result=_mm256_loadu_ps(result+j);
            //_result=_mm256_add_ps(_min1, _min2);
            _mm256_storeu_ps(result+j, _mm256_add_ps(_result, _mm256_add_ps(_min1, _min2)));
            _a_2=_mm256_add_ps((_mm256_sub_ps(_a_1, _min1)*dist_1), (_mm256_sub_ps(_a_2, _min2)*dist_2));
            _a_1=_mm256_loadu_ps(a[order_N]+j);
            _mm256_storeu_ps(a[order_N]+j, _mm256_add_ps(_a_2, _a_1));
            _b_2=_mm256_add_ps((_mm256_sub_ps(_b_1, _min1)*dist_1), (_mm256_sub_ps(_b_2, _min2)*dist_2));
            _b_1=_mm256_loadu_ps(b[order_N]+j);
            _mm256_storeu_ps(b[order_N]+j,  _mm256_add_ps(_b_2, _b_1));
        }
#endif
        for(j=AVXStep*8;j<sampleSize;j++)
        {
            auto min_1=min(a[order_1][j],b[order_1][j]);
            auto min_2=min(a[order_2][j],b[order_2][j]);
            result[j]+=min_1+min_2;
            a[order_N][j]+=(a[order_1][j]-min_1)*dist_1+(a[order_2][j]-min_2)*dist_2;
            b[order_N][j]+=(b[order_1][j]-min_1)*dist_1+(b[order_2][j]-min_2)*dist_2;
        }
    }
    for(int j=0;j<sampleSize;j++)
    {
        
        result[j]+=min(a[order_N][j],b[order_N][j]);
        result[j]=min(result[j],(float)1);
        result[j]=max(result[j],(float)0);
    }
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
            
            auto boost=new BoostCalculator(arg->A->p);
            boost->setData(iter->second);
            boost->convert(*(arg->A),i+1);
            //boost->genCompareData();
            auto bResult=boost->calc();
            arg->result->sendResult(i, bResult);
            delete bResult;
            delete boost;
        }
        else if(arg->number<0) {
            int x=arg->A->size();
            auto boost=new BoostCalculator(arg->A->p);
            searchModeFullResult* result=(searchModeFullResult*)arg->result;
            if(iter->second->data.size()==0){
                delete boost;
                continue;}
            boost->setData(iter->second);
            boost->convert(*(arg->A));
            auto bResult=boost->calc();
            result->sendResult(i,bResult);
            delete bResult;delete boost;
        }else{
            int j,topN=arg->number;
            int x=arg->A->size();
            auto boost=new BoostCalculator(arg->A->p);
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
matrixModeResult* matrixBoostCompare( class DataProcessor & A,int core,bool lowMem){
    if (A.size() ==0 )
        throw empty_data;
    matrixModeResult* result;
    int x,i,j;
    if(core <1){
        cout<<"Warning, CPU cores must >=1, we except you select 1\n";
        core=1;}
    result=new matrixModeResult;
    x=result->x=result->y=A.size();
    result->dataAlloc(x,lowMem);
    A.genName();
    cout<<"calculating "<<x<<'*'<<x<<" similarity matrix\n";
    
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
    
    result->nameA=A.names;
    cout<<endl;
    return result;
}

searchModeFullResult* fullSearchBoostCompare(class DataProcessor &A,class DataProcessor &B,int core,bool lowMem) {
    if (A.size() ==0 or B.size()==0)
        throw empty_data;
    searchModeFullResult*result;
    int x,y,i,j;
    if(core <1){
        cout<<"Warning, CPU cores must >=1, we except you select 1\n";
        core=1;}
    result=new searchModeFullResult;
    x=B.size();y=A.size();
    result->dataAlloc(x,y,lowMem);
    A.genName();B.genName();
    result->nameA=A.names;result->nameB=B.names;
    cout<<"searching "<<x<<" from "<<y<<" samples\n";
        pthread_t * tids;
        LineCompareArg * args;
        tids=new pthread_t[core];
        args=new LineCompareArg[core];
        for(int i=0;i<core;i++){
            args[i].A=&A;args[i].B=&B;args[i].core=core;
            args[i].id=i;args[i].result=(matrixModeResult*)result;args[i].number=-1;
            pthread_create(&(tids[i]), NULL, lineCompare, (void*)&(args[i]));
        }
        for(int i=0;i<core;i++)
            pthread_join(tids[i], NULL);
        delete []tids;delete []args;
    
    return result;
}

searchResult* searchBoostCompare(class DataProcessor &A,class DataProcessor &B,int core,int topN){
    if (A.size() ==0 or B.size()==0)
        throw empty_data;
    searchResult*result;
    int x,y,i,j;
    if(core <1){
        cout<<"Warning, CPU cores must >=1, we except you select 1\n";
        core=1;}
    result=new searchResult;
    x=A.size();y=B.size();
    result->dataAlloc(B.size(),topN);
    A.genName();
    cout<<"searching "<<y<<" from "<<x<<" samples\n";
    auto pBar=new ProgressBar;
    pBar->init(x*y);
        pthread_t * tids;
        LineCompareArg * args;
        tids=new pthread_t[core];
        args=new LineCompareArg[core];
        for(int i=0;i<core;i++){
            args[i].A=&A;args[i].B=&B;args[i].core=core;
            args[i].id=i;args[i].result=(matrixModeResult*)result;args[i].number=topN;
            pthread_create(&(tids[i]), NULL, lineCompare, (void*)&(args[i]));
        }
        for(int i=0;i<core;i++)
            pthread_join(tids[i], NULL);
        delete []tids;delete []args;
    
    delete pBar;
    //result->nameA=A.names;
    return result;
}
