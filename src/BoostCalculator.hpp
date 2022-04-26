//
//  booster.hpp
//  prism2
//
//  Created by 康凯 on 2020/2/28.
//  Copyright © 2020 康凯. All rights reserved.
//
//Generate reduced tree, reduced calculation order and convert method

#ifndef booster_hpp
#define booster_hpp
#include "structure.h"
#include "DataProcessor.hpp"
#include "simCalc.hpp"
#include <stdio.h>
class BoostCalculator{
protected:
    /*!
    @brief Node of comparison order data
    @note Similar to CompData, store one comparison step.
    */
    struct CompNode{
        int order_1=0,order_2=0,order_N=0;
        float dist_1=0,dist_2=0;
    };
    struct boostTable{
        int targetID=-1;
        float coef=0;
        bool select=false;
    };
    NewickParser * p;
    SampleData *data;
    float **matrix,**source;
    boostTable *table;
    int treeWalk(const TreeNode* a,int id,float coefficient);
    CompData compareData;
    int elementNumber,sampleSize,orderSize;
    int genWalk(const TreeNode* a,int &alloc);
    vector<CompNode> writeBuff;
public:
    BoostCalculator(NewickParser *p=nullptr){
        this->p=p;
        elementNumber=0;
    }
    /*!
    @brief Set a parsed parser to BoostCalculator
    @note Set a parser that has parsed an phylogeny tree to the BoostCalculator. That is, assign phylogeny tree information to the BoostCalculator.
    */
    void setParser(NewickParser *p){
        this->p=p;
    }
    int setData(SampleData *A);
    ~BoostCalculator(){
        delete table;
        delete matrix[0];delete source[0];
        delete matrix;delete source;
    }
    void genCompareData();
    int convert(DataProcessor & A,int begin=0,int end=-1);
    float *calc();
};
#endif /* booster_hpp */
