//
//  booster.hpp
//  prism2
//
//  Created by 康凯 on 2020/2/28.
//  Copyright © 2020 康凯. All rights reserved.
//

#ifndef booster_hpp
#define booster_hpp
#include "structure.h"
#include "loader.hpp"
#include "simCalc.hpp"
#include <stdio.h>
class booster{
protected:
    struct CompElement{
        int order_1=0,order_2=0,order_N=0;
        float dist_1=0,dist_2=0;
    };
    struct boostTable{
        int targetID=-1;
        float coef=0;
        bool select=false;
    };
    parser * p;
    sampleData *data;
    float **matrix,**source;
    vector<boostTable> table;
    int treeWalk(const TreeNode* a,int id,float coefficient);
    CompData compareData;
    int elementNumber,sampleSize,orderSize;
    int genWalk(const TreeNode* a,int &alloc);
    vector<CompElement> writeBuff;
public:
    booster(parser *p=nullptr){
        this->p=p;
        elementNumber=0;
    }
    void setParser(parser *p){
        this->p=p;
    }
    int setData(sampleData *A);
    ~booster(){
        
        delete matrix[0];delete source[0];
        delete matrix;delete source;
    }
    void genCompareData();
    int convert(loader & A,int begin=0,int end=-1);
    float *calc();
};
#endif /* booster_hpp */
