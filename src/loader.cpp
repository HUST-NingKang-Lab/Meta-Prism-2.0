//
//  loader.cpp
//  prism2
//
//  Created by 康凯 on 2019/12/23.
//  Copyright © 2019 康凯. All rights reserved.
//

#include "loader.hpp"
string Bacteria="Bacteria";
template <typename T>
inline void BinaryIO::v2s(char *buf, T *V){
    char *data=(char*)V;
    uint32_t size=sizeof(T);
    if(littleE){
        memcpy(buf,data,size);
    }
    else{
    for(int i=0;i<size;i++){
        buf[i]=data[size-i];
    }}
}
template <typename T>
inline void BinaryIO::s2v(char *buf, T *V){
    char *data=(char*)V;
    uint32_t size=sizeof(T);
    if(littleE){
        memcpy(data,buf,size);
    }
    else{
    for(int i=0;i<size;i++){
        data[i]=buf[size-i];
    }}
}
void changeRelative(vector<sampleData*> &A){
    for(auto iter=A.begin();iter!=A.end();iter++){
        sampleData &data=*(*iter);
        double sum=0;
        for(auto dIter=data.data.begin();dIter!=data.data.end();dIter++)
            sum+=dIter->data;
        for(auto dIter=data.data.begin();dIter!=data.data.end();dIter++)
            dIter->data/=sum;
    }
}
int getID(const string &A,const unordered_map<string, int> &lable,int begin=-1){
    int id=-1,i;
    string buf_sp,buf_name;bool select=false,sp=true;
    vector<string> level;
    if(begin==-1){begin=0;
    for(i=0;i<2;i++){
        
        while(A[begin]!='\t'&&A[begin]!=' '){
            begin++;
            if(begin>=A.length())
                break;
        }
        begin++;
    }}
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
}
int loader::loadMatData(ifstream &ifile){
    string readBuf,buffer;stringstream buf;float value;
    getline(ifile,readBuf);
    replace(readBuf.begin(), readBuf.end(), '\t', ' ');
    replace(readBuf.begin(), readBuf.end(), ',', ' ');
    buf.str(readBuf);
    buf>>buffer;// #OTUID
    auto label=this->p->getLabel();
    vector<sampleData*> datas;
    sampleData *x;
    while(buf.rdbuf()->in_avail()){
        x=new sampleData;
        buf>>x->name;
        datas.push_back(x);
    }
    while(getline(ifile, readBuf)){
        replace(readBuf.begin(), readBuf.end(), '\t', ' ');
        replace(readBuf.begin(), readBuf.end(), ',', ' ');
        
        buf.clear();
        buf.str(readBuf);
        buf>>buffer;//name
        int i=0;
        abdElement abdBuf;
        abdBuf.ID=getID(buffer, label,0);
        if(abdBuf.ID==-1)
            continue;
        while(buf.rdbuf()->in_avail()){
            buf>>value;
            cout<<buf.rdbuf()->in_avail()<<' ';
            if(value>1E-10){
                abdBuf.data=value;
                datas[i]->data.push_back(abdBuf);
            }
            i++;
            if(i>=datas.size())
                break;
        }
    }
    changeRelative(datas);
    for(auto iter=datas.begin();iter!=datas.end();iter++){
        Data[(*iter)->name]=(*iter);
    }
    cout<<endl;
    return 0;
}
sampleData* loader::loadTSVFile(ifstream &ifile,string name){
    //There are several types of TSV file: MGNify types and one FEAST type, this function aims at automatically detect tsv file's type and process it.
    // When loading new tsv file with different type, you may edit this function to support it, or change tsv file's format to any MGNify/ FEAST type.
    string readBuf,buffer;stringstream buf;
    int id,type=0;
    float sumABD=0,v;
    sampleData* result=new sampleData;
    abdElement abdBuf;
    const unordered_map<string, int> &label=p->getLabel();
    map<int, float> values;
    while(getline(ifile,readBuf)){
        if(readBuf[0]=='#'){// judge file type by the headlines' lenght
            type++;
            continue;}
        
        replace(readBuf.begin(),readBuf.end(),'\t',' ');
        if(type==1){
            int i=0;for(i=0;readBuf[i]!=' ';i++);
            readBuf=readBuf.substr(i);
        }
        buf.clear();
        buf.str(readBuf);
        buf>>v;
        if(type==2)// if type==2, current v is OTU ID
            buf>>v;//ABD
        //buf>>buffer;
        if(v<1e-9)//Ignore too small abundance
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
    uint64_t s=0;
    sampleData* sDBuf;
    cout<<"processed hundreds of files:";
    while(getline(ifile, nameBuffer)){
        s+=1;
        if(s%100==0)
            cout<<'*';
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
    cout<<endl;
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
                    cout<<"Error in loading pdata, type 0\n";
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
                    data->data[i]=abdBuf;
                }
                state=3;buf.clear();
                break;
            }case 3:{
                if(buffer!="}")
                    cout<<"Error in loading pdata, type 3\n";
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
int loader::loadBMultiTSV(ifstream &ifile){
    SampleHead sHead;
    Infos info;
    char *buffer;
    sampleData* sData;
    buffer=new char[50];
    ifile.read((char*)&(info.source),sizeof(info.source));
    ifile.read(buffer,sizeof(info.version));
    io.s2v(buffer, &(info.version));
    ifile.read(buffer,sizeof(info.size));
    io.s2v(buffer, &(info.size));
    string sBuf;
    int x=0;
    for(int i=0;i<info.size;i++){
        ifile.read((char*)&(sHead.Name),sizeof(sHead.Name));
        ifile.read(buffer,sizeof(sHead.sampleSize));
        io.s2v(buffer,&(sHead.sampleSize));
        abdElement ABDbuf;
        if(x==0)
            sData=new sampleData[100];
        sData[x].data.resize(sHead.sampleSize);
        for(int j=0;j<sHead.sampleSize;j++){
            ifile.read(buffer,sizeof(ABDbuf.ID));
            io.s2v(buffer, &(ABDbuf.ID));
            ifile.read(buffer,sizeof(ABDbuf.data));
            io.s2v(buffer, &(ABDbuf.data));
            sData[x].data[j]=ABDbuf;
        }
        sBuf=sHead.Name;
        sData[x].name=sBuf;
        Data[sBuf]=sData+x;
        x+=1;
        x=x==100?0:x;
    }
    delete [] buffer;
    return 0;
}
int loader::merge(loader &data){
    for(auto iter=data.Data.begin();iter!=data.Data.end();iter++){
        this->Data[iter->first]=iter->second;
    }
    this->genName();
    return 0;
}
int loader::outputBMirror(ofstream &ofile){
    Infos info;
    strcpy(info.source,"Meta-Prism 2.0");
    char *buffer;
    buffer=new char[50];
    info.version=1.01;
    info.size=Data.size();
    ofile.write((char *)&info.source,sizeof(info.source));
    io.v2s(buffer, &(info.version));
    ofile.write(buffer,sizeof(info.version));
    io.v2s(buffer, &(info.size));
    ofile.write(buffer,sizeof(info.size));
    SampleHead sHead;
    this->genName();
    for(auto iter=Data.begin();iter!=Data.end();iter++){
        strcpy(sHead.Name,iter->second->name.c_str());
        auto &sData=iter->second->data;
        sHead.sampleSize=sData.size();
        ofile.write((char *)&(sHead.Name),sizeof(sHead.Name));
        io.v2s(buffer, &(sHead.sampleSize));
        ofile.write(buffer,sizeof(sHead.sampleSize));
        for(auto dIter=sData.begin();dIter!=sData.end();dIter++){
            io.v2s(buffer, &(dIter->ID));
            ofile.write(buffer,sizeof(dIter->ID));
            io.v2s(buffer, &(dIter->data));
            ofile.write(buffer,sizeof(dIter->data));
        }
    }
    ofile.close();
    delete [] buffer;
    return 0;
}

