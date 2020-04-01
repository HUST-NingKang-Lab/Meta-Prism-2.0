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
int compareResult::dataAlloc(int x,int y,bool sym){
    data=new float*[x];
    symmetry=sym;
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
            data[i]=new float[y];
    }
    return 0;
}
