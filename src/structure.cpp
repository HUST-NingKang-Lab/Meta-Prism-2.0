//
//  structure.cpp
//  prism2
//
//  Created by 康凯 on 2020/2/22.
//  Copyright © 2020 康凯. All rights reserved.
//

#include "structure.h"
int compareResult::save(ofstream &ofile){
    ofile<<this->x<<endl;
    for (auto iter=nameA.begin();iter!=nameA.end();iter++){
        
        ofile<<*iter<<' ';
    }
    ofile<<endl;
    if(lowMem)
        return this->output(ofile, data16);
    else
        return this->output(ofile, data);
}
int compareResult::dataAlloc(int x,bool lowFlag){
    lowMem=lowFlag;
    if (lowFlag)
        return this->alloc(x, &(this->data16));
    else
        return this->alloc(x, &(this->data));
}
bool uFP16::check()
{
    Bits A[5];
    float datas[5]={0,1e-6,-1,1,3.1415926};
    A[0].ui=0x0;
    A[1].ui=0x358637bd;
    A[2].ui=0xbf800000;
    A[3].ui=0x3f800000;
    A[4].ui=0x40490fda;
    
    for(int i=0;i<5;i++){
        if (fabsf(A[i].f-datas[i])>1e-10)
            return 0;
        
    }
    return 1;
}
uint32_t uFP16::getFrac=(uint32_t)lroundf(powf(2, uFP16::frac))-1;// round(pow(2, uFP16::frac))-1;
uint32_t uFP16::Zero=((uint32_t)(lroundf(powf(2, uFP16::exp))-1))<<uFP16::frac;
ostream  &operator<<(ostream &out, uFP16 &c1){
    out<<(float)c1;
    return out;
}
template<typename T>
int compareResult::alloc(int x, T ***sData){
    auto genData=new T*[x];
    for (int i=0;i<x;i++){
        genData[i]=new T[x-i];
    }
    *sData=genData;
    return 0;
}
template<typename T>
int compareResult::output(ofstream &ofile,T** sData){
    int i,j;
    for(i=0;i<x;i++){
        for(j=0;j<i;j++){
            ofile<<(sData[j][i-j-1])<<' ';}
        ofile<<1<<' ';
        for(j=i+1;j<y;j++){
            ofile<<(sData[i][j-i-1])<<' ';
        }
        ofile<<endl;
    }
    return 0;
}
int compareResult::sendResult(int i,float* result){
    if(lowMem){
        for(int t=0;t<(x-i-1);t++)
            data16[i][t]=result[t];
    }
    else{
        for(int t=0;t<(x-i-1);t++)
            data[i][t]=result[t];
    }
    return 0;
}
