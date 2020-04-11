//
//  loader.cpp
//  prism2
//
//  Created by 康凯 on 2019/12/23.
//  Copyright © 2019 康凯. All rights reserved.
//

#include "loader.hpp"
string Bacteria="Bacteria";
int getID(const string &A,const unordered_map<string, int> &lable){
    int id=-1,begin=0,i;
    string buf_sp,buf_name;bool select=false,sp=true;
    vector<string> level;
    for(i=0;i<2;i++){
        while(A[begin]!='\t'&&A[begin]!=' '){
            begin++;
            if(begin>=A.length())
                break;
        }
        begin++;
    }
    if(A.substr(begin,4)=="Root"){
        while(A[begin]!=';'){
            begin++;
            if (begin>=A.size()){begin--;
                break;}
        }
        begin++;
    }
    for(i=begin;i<A.length();i++){//0:sk 1:k 2:p 3:c 4:
        if(A[i]=='\''||A[i]=='['||A[i]==']'||A[i]==' ')
            continue;
        if(A[i]=='\t'){
            level.push_back(buf_name);
            break;}
        if(sp){
            if(A[i]=='_'){
                i++;
                select=true;
                sp=false;
            }
            else
                buf_sp+=A[i];
        }
        else{
            
            if(A[i]==';'){
                level.push_back(buf_name);
                buf_name.clear();
                buf_sp.clear();
                sp=true;
            }
            else{
                if(A[i]!='_')
                    buf_name+=A[i];
                else
                    buf_name+=' ';
            }}
    }
    if(!select)
        return -1;
    level.push_back(buf_name);
    if(level[0]==Bacteria){
        if(level.size()==8){
            auto Latin=level[6]+' '+level[7];
            auto finder=lable.find(Latin);
            if(finder!=lable.end())
                return finder->second;
        }
        for(i=level.size()-1;i>=0;i--){
            if(level[i].size()<1)
                continue;
            auto finder=lable.find(level[i]);
            if(finder!=lable.end())
                return finder->second;
        }
    }
    return id;
}/*
int loader::loadMultiData(ifstream &ifile){
    string readBuf,buffer;stringstream buf;float value;
    getline(ifile,readBuf);
    buf.str(readBuf);
    buf>>buffer;//ID
    auto label=this->p->getLabel();
    while(buf.rdbuf()->in_avail()){
        sampleData x;
        buf>>x.name;
        this->Data.push_back(x);
    }
    int m=0;
    cout<<"Hundred Files:";
    while(getline(ifile, readBuf)){
        if(m%100==0){
            cout<<'*';
        }
        buf.clear();
        buf.str(readBuf);
        buf>>buffer;//name
        int i=0;
        abdElement abdBuf;
        auto iter=label.find(buffer);
        if(iter==label.end()){
            continue;
        }
        abdBuf.ID=iter->second;
        while(buf.rdbuf()->in_avail()){
            buf>>value;
            if(value>1E-10){
                abdBuf.data=value;
                data[i].data.push_back(abdBuf);
            }
            i++;
        }
    }
    cout<<endl;
    return 0;
}*/
int loader::loadCompData(ifstream &ifile){
    int leafSize=p->getLeafSize();
    compData.order_1=new int[leafSize];
    compData.order_2=new int[leafSize];
    compData.order_N=new int[leafSize];
    compData.dist_1=new float[leafSize];
    compData.dist_2=new float[leafSize];
    string readBuf,buffer;stringstream buf;
    int i=0;
    while(getline(ifile, readBuf)){
        buf.clear();buf.str(readBuf);
        buf>>compData.order_1[i]>>compData.dist_1[i]>>compData.order_2[i]>>compData.dist_2[i]>>compData.order_N[i];
        i++;
    }
    return 0;
}
sampleData* loader::loadTSVFile(ifstream &ifile,string name){
    string readBuf,buffer;stringstream buf;
    //getline(ifile,readBuf);getline(ifile,readBuf);//pass begining two lines
    int id,type=0;
    float sumABD=0,v;
    sampleData* result=new sampleData;
    abdElement abdBuf;
    const unordered_map<string, int> &label=p->getLabel();
    map<int, float> values;
    while(getline(ifile,readBuf)){
        if(readBuf[0]=='#'){
            type++;
            continue;}
        
        replace(readBuf.begin(),readBuf.end(),'\t',' ');
        if(type==1){
            int i=0;for(i=0;readBuf[i]!=' ';i++);
            readBuf=readBuf.substr(i);
        }
        buf.clear();
        buf.str(readBuf);
        buf>>v;//pass the OTU ID
        if(type==2)
            buf>>v;//ABD
        //buf>>buffer;
        if(v<1e-9)
            continue;
        id=getID(readBuf, label);
        buffer.clear();
        if(id<0)
            continue;
        else{
            sumABD+=v;
            values[id]+=v;
        }
        //sumABD+=v;
    }
    result->name=name;
    if(sumABD==0)
        return result;
    for(auto iter=values.begin();iter!=values.end();iter++){
        abdBuf.ID=iter->first;
        abdBuf.data=iter->second/sumABD;
        result->data.push_back(abdBuf);
    }
    return result;
}
int loader::loadMultiTSV(ifstream &ifile){
    string nameBuffer,buf;ifstream TSVFile;
    int i,j=0;
    sampleData* sDBuf;
    while(getline(ifile, nameBuffer)){
        TSVFile.open(nameBuffer);
        for(i=0;i<nameBuffer.length();i++){
            if(nameBuffer[i]=='/')
                j=i;
        }
        buf=nameBuffer.substr(j+1);
        sDBuf=loadTSVFile(TSVFile,buf);
        Data[buf]=sDBuf;
        TSVFile.close();
    }
    return 0;
}
int loader::outputMirror(ofstream &ofile){
    auto iter=Data.begin();
    ofile<<"#Generated By Prism2.0,Data number: "<<Data.size()<<endl;
    for(;iter!=Data.end();iter++){
        ofile<<"{\n"<<iter->second->name<<"\n";
        ofile<<iter->second->data.size()<<':';
        for(auto vIter=iter->second->data.begin();vIter!=iter->second->data.end();vIter++){
            if(isnan(vIter->data) || isinf(vIter->data))
                vIter->data=0;
            ofile<<vIter->ID<<' '<<vIter->data<<',';
        }
        ofile<<"\n}\n";
    }
    return 0;
    
}
int loader::loadFromMirror(ifstream &ifile){
    string buffer,nameBuffer;stringstream buf;
    int state=0,num,sum=0,i,iter=0;
    abdElement abdBuf;
    getline(ifile,buffer);//first line
    sampleData* datas;
    sampleData* data;
    int zeroSum=0;
    while(getline(ifile,buffer)){
        switch (state) {
            case 0:
            {
                if(buffer!="{")
                    cout<<"0Error\n";
                else
                    state=1;
                break;
            }case 1:{
                nameBuffer=buffer;
                state=2;
                break;
            }case 2:{
                if(iter%100==0){
                    iter=0;
                    datas=new sampleData[100];
                }
                data=datas+(iter++);
                data->name=nameBuffer;
                replace(buffer.begin(),buffer.end(),':',' ');
                replace(buffer.begin(),buffer.end(),',',' ');buf.clear();
                buf.str(buffer);
                buf>>num;
                data->data.resize(num);
                for(i=0;i<num;i++){
                    buf>>abdBuf.ID;
                    buf>>abdBuf.data;
                    data->data.push_back(abdBuf);
                }
                state=3;buf.clear();
                break;
            }case 3:{
                if(buffer!="}")
                    cout<<"3Error";
                else{
                    state=0;
                    Data[nameBuffer]=data;
                    sum++;
                    //if (data->data.size()<5){
                    //    cout<<data->data.size()<<' '<<data->name<<endl;zeroSum++;}
                }
                break;
            }
            default:
                break;
        }
    }
    cout<<zeroSum;
    return 0;
}
int loader::outTSVTable(ofstream &ofile){
    map<int,string> usedID;
    
    return 0;
}
int loader::printToTable(ofstream &ofile){
    unordered_map<int,int> nameMap;
    vector<int> names;
    int id=0;
    auto pIndex=p->getIndex();
    for(auto iter=this->Data.begin();iter!=Data.end();iter++){
        for(auto dataIter=iter->second->data.begin();dataIter!=iter->second->data.end();dataIter++){
            if (nameMap.find(dataIter->ID)==nameMap.end()) {
                nameMap[dataIter->ID]=id++;
                names.push_back(dataIter->ID);
            }
        }
    }
    ofile<<"#OTUID";
    float **matrix;
    auto label=p->getLabel();
    matrix=new float*[Data.size()];
    matrix[0]=new float[Data.size()*nameMap.size()]();
    for(int i=1;i<Data.size();i++)
        matrix[i]=matrix[0]+i*nameMap.size();
    int i=0;
    for(auto iter=this->Data.begin();iter!=Data.end();iter++){
        for(auto dataIter=iter->second->data.begin();dataIter!=iter->second->data.end();dataIter++){
            matrix[i][nameMap[dataIter->ID]]=dataIter->data;
        }
        ofile<<'\t'<<iter->second->name;
        i++;
    }
    progressBar A;
    A.init(names.size());
    for(i=0;i<names.size();i++){
        if(i%10==0)
            A.show(i);
        //int id=names[i];
        //if(pIndex[id]->lChild!=NULL)
        //  continue;
        ofile<<'\n'<<"OTU_"<<names[i];
        for(int j=0;j<Data.size();j++)
            ofile<<'\t'<<matrix[j][i];
    }
    delete []matrix[0];
    delete []matrix;
    return 0;
}
