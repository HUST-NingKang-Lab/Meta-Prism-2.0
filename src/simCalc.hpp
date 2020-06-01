//
//  sparse_sim_calc.hpp
//  prism2
//
//  Created by 康凯 on 2019/12/23.
//  Copyright © 2019 康凯. All rights reserved.
//

#ifndef sparseSimCalc_hpp
#define sparseSimCalc_hpp
#include <pthread.h>
#include "structure.h"
#include "loader.hpp"
#include <stdio.h>
#include "booster.hpp"
using namespace std;
struct LineCompareArg{
    int id=0;
    int core=1;
    int number=0;
    bool lowMem=false;
    compareResult * result=NULL;
    loader *A=NULL,*B=NULL;
};
float sparseSimCalcPair(const sampleData &a,const sampleData &b,const Table* table);
float sparseSimCalcNew(const sampleData &a,const sampleData &b,const Table* table,int depth);
float denseSimCalcPair(const sampleData &a,const sampleData &b,const CompData* compData,int size,int depth);
float * multiSparseCompare(float ** a,float**b,const CompData* compData,int sampleSize,int orderSize);
void *lineCompare(void * args);
compareResult* matrixCompare( loader & A);
compareResult* matrixBoostCompare( loader & A,int core,bool lowMem);
compareResult* searchCompare( loader &A,  loader &B,int core=1);
searchResult* searchBoostCompare(class loader &A,class loader &B,int core,int topN);
#endif /* sparse_sim_calc_hpp */
