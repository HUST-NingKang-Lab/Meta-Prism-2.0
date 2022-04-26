//
//  sparse_sim_calc.hpp
//  prism2
//
//  Created by 康凯 on 2019/12/23.
//  Copyright © 2019 康凯. All rights reserved.
//
/*
 */
#ifndef sparseSimCalc_hpp
#define sparseSimCalc_hpp
#ifdef __AVX__
#include <immintrin.h>
#define __MP2_useAVX__ 1
#else
#define __MP2_Normal__ 1
#endif
#define empty_data 3
#include "structure.h"
#include "DataProcessor.hpp"
#include <stdio.h>
#include "BoostCalculator.hpp"
using namespace std;
/*!
@brief Basic arguments for line compare
*/
struct LineCompareArg{
    int id=0;
    int core=1;
    int number=0;
    bool lowMem=false;
    matrixModeResult * result=NULL;
    DataProcessor *A=NULL,*B=NULL;
};
float * multiSparseCompare(float ** a,float**b,const CompData* compData,int sampleSize,int orderSize);// Boost scheme calculation
void *lineCompare(void * args);
matrixModeResult* matrixBoostCompare( DataProcessor & A,int core,bool lowMem);
searchResult* searchBoostCompare(class DataProcessor &A,class DataProcessor &B,int core,int topN);
searchModeFullResult* fullSearchBoostCompare(class DataProcessor &A,class DataProcessor &B,int core,bool lowMem) ;
#endif /* sparse_sim_calc_hpp */
