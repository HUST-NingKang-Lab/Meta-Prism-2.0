//
//  structure.cpp
//  prism2
//
//  Created by 康凯 on 2020/2/22.
//  Copyright © 2020 康凯. All rights reserved.
//

#include "structure.h"
int compareResult::save(ofstream &ofile){
    ofile<<this->x<<' '<<this->y<<' '<<symmetry<<endl;
    for (auto iter=nameA.begin();iter!=nameA.end();iter++){
        
        ofile<<*iter<<' ';
    }
    ofile<<endl;
    if(!symmetry){
        for (auto iter=nameB.begin();iter!=nameB.end();iter++){
            ofile<<*iter<<' ';
        }
        ofile<<endl;
    }
    int i,j;
    if(lowMem){
        for(i=0;i<x;i++){
            if(symmetry){
                for(j=0;j<i;j++){
                    ofile<<(float)(data16[j][i])<<' ';}
                ofile<<1<<' ';
                for(j=i+1;j<y;j++){
                    ofile<<(float)(data16[i][j])<<' ';
                }
            }
            else{
                for(j=0;j<y;j++)
                    ofile<<(float)(data16[i][j])<<' ';
            }ofile<<endl;
        }
    }
    else{
        for(i=0;i<x;i++){
            if(symmetry){
                for(j=0;j<i;j++)
                    ofile<<data[j][i]<<' ';
                ofile<<1<<' ';
                for(j=i+1;j<y;j++)
                    ofile<<data[i][j]<<' ';
            }
            else{
                for(j=0;j<y;j++)
                    ofile<<data[i][j]<<' ';
            }ofile<<endl;
        }
    }
    return 0;
}
int compareResult::load(ifstream &ifile){
    ifile>>x>>y>>symmetry;
    string buf;
    for(int i=0;i<x;i++){
        ifile>>buf;
        nameA.push_back(buf);
    }
    if(!symmetry){for(int i=0;i<y;i++){
        ifile>>buf;
        nameB.push_back(buf);
    }}
    int i,j;
    dataAlloc(x,y,symmetry);
    for(i=0;i<x;i++){
        float buf;
        if(symmetry){
            for(j=0;j<y;j++){
                if(j<i)
                    ifile>>buf;
                else
                    ifile>>data[i][j];
            }
        }
        else{
            for(j=0;j<y;j++){
                ifile>>data[i][j];
                if(symmetry)
                    data[j][i]=data[i][j];
            }}}
    if(symmetry){
        for(i=0;i<x;i++)
            data[i][i]=1;
    }
    return 0;
}
int compareResult::dataAlloc(int x,int y,bool sym,bool lowFlag){
    lowMem=lowFlag;
    symmetry=sym;
    if (lowFlag){
        data16=new uFP16*[x];
        
        if(!sym){
            if(x<10000||y<10000){
                data16[0]=new uFP16[x*y];
                for(int i=1;i<x;i++)
                    data16[i]=data16[0]+i*y;}
            else{
                big=true;
                for (int i=0;i<x;i++){
                    data16[i]=new uFP16[y];
                }}}
        else{
            big=true;
            for(int i=0;i<x;i++)
                data16[i]=new uFP16[y-i];
        }
    }
    else{
        data=new float*[x];
        if(!sym){
            if(x<10000||y<10000){
                data[0]=new float[x*y];
                for(int i=1;i<x;i++)
                    data[i]=data[0]+i*y;}
            else{
                big=true;
                for (int i=0;i<x;i++){
                    data[i]=new float[y];
                }}}
        else{
            big=true;
            for(int i=0;i<x;i++)
                data[i]=new float[y-i];
        }}
    return 0;
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
