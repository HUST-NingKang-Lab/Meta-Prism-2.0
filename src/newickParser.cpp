//
// Created by LuoFan on 2019-12-19.
//

#include "newickParser.h"
using namespace std;
string AND="and",OTHER="Other",UC="Unclassified";
parser::parser(string newickTree, int mod) {
    char newickTreeC[newickTree.size()+1];
    int size,i=0;
    treeSize=1;leafNumber=0;
    size=(int)newickTree.size();
    strcpy(newickTreeC,newickTree.c_str());
    parseOne(newickTreeC,i,size,tree,mod);
    int id=0,depth=0;
    this->depth=0;
    this->table=new Table[this->treeSize];
    nameTravel(tree, id);
    genTable(tree,depth);
    this->depth++;
    
    return;
}
void parser::parseOne(char *newick, int &index, int end, TreeNode &node,int mod) {
    if (newick[index]!='(')
        cout<<"Error can't parse newick tree\n maybe you didn't delete annotation of tree?\n";
    //left child name
    node.lChild=new TreeNode;
    node.lChild->father=&node;
    node.lChild->cFlag=false;
    treeSize++;
    if (newick[++index]=='('){
        parseOne(newick,index,end,*(node.lChild),mod);
        while (newick[index]!=':') index++;//pass the number
    }
    else {
        while (newick[index] != ':'){//set name, now index to :
            if(newick[index]=='\''){
                index++;continue;
            }
            node.lChild->name += newick[index++];
        }
    }
    
    //left dist
    index++;char * startC=newick+index;
    while(newick[index]!=',') index++;
    //endC=newick+(index-1);
    node.lDist=strtod(startC,NULL);
    node.lChild->fDist=node.lDist;
    //now index to ',', and need to process right
    node.rChild=new TreeNode;
    node.rChild->father=&node;
    node.rChild->cFlag=true;
    treeSize++;
    
    //right child name
    if (newick[++index]=='('){
        parseOne(newick,index,end,*(node.rChild),mod);
        while (newick[index]!=':') index++;
    }else {
        while (newick[index] != ':'){//set , now index to :
            if(newick[index]=='\''){
                index++;continue;
            }
            node.rChild->name += newick[index++];}
    }
    //right dist
    index++;startC=newick+index;
    while(newick[index]!=')') index++;
    index++;
    while(newick[index]!=':'){
        if(newick[index]=='\'' || newick[index]=='_'){
            index++;
            continue;}
        node.name+=newick[index++];
    }
    //endC=newick+(index-1);
    node.rDist=strtod(startC,NULL);
    node.rChild->fDist=node.rDist;
    return;
}
/*For traditional newick tree
 void parser::nameTravel(TreeNode &node, int &ID) {
 if(node.lChild!=NULL){
 nameTravel(*(node.lChild), ID);
 nameTravel(*(node.rChild), ID);
 }
 else
 label[node.name]=ID;
 node.id=ID++;
 return;
 }*/
void parser::nameTravel(TreeNode &node, int &ID) {//SILVA newick tree
    if(node.lChild!=NULL){
        nameTravel(*(node.lChild), ID);
        nameTravel(*(node.rChild), ID);
    }
    if(node.name.length()>0){
        
        auto name = node.name;
        auto andF=name.find(" and ");
        auto OtherF=name.find("Other");
        bool flagAnd=(andF==string::npos),flagOther=(OtherF==string::npos);
        int i=0,begin=0,end=0;string nameBuf;
        bool flag=false;
        if(name[0]=='\'')// incase there is \' at begin or end of name
            begin=1;
        if(name[name.length()-1]=='\'')
            end=name.length()-1;
        else
            end=name.length();
         node.name=name.substr(begin,end-begin);
       
        
        index[ID]=&node;
        if(flagAnd&&flagOther)
            for(i=0;i<name.length();i++){
                if (name[i]=='_'){
                    if (!flag){
                        flag=true;
                        begin=i+2;
                    }
                    else{
                        end=i;
                        break;
                    }
                }
            }
        if(flag)
            label[name.substr(begin,end-begin)]=ID;
        else{
            
            nameBuf.clear();
            for(i=0;i<name.length();i++){
                if(name[i]==' '){
                    if (nameBuf==AND ||nameBuf ==OTHER||nameBuf==UC){
                        nameBuf.clear();
                    }
                    else{
                        label[nameBuf]=ID;
                        nameBuf.clear();}
                }
                else
                    nameBuf+=name[i];
            }
            label[nameBuf]=ID;
            /*
            if(!andF){
                label[name.substr(0,andF)]=ID;//before " and"
                label[name.substr(andF+5)]=ID;//after "and "
            }
            else if(!OtherF){
                label[name.substr(6)]=ID;
            }
            else
                label[name]=ID;*/
        }
    }node.id=ID++;
    return;
}
void parser::genTable(TreeNode &node,int const depth){
    node.dep=depth;
    if(node.lChild!=NULL){
        genTable(*(node.lChild),depth+1);
        genTable(*(node.rChild),depth+1);
        
    }
    else{
        this->leafNumber++;
    }
    this->depth=this->depth<depth?depth:this->depth;
    
    if(node.father==NULL){
        table[node.id].fid=-1;
        table[node.id].dep=node.dep;
        return;
    }
    table[node.id].fid=node.father->id;
    table[node.id].dist=node.fDist;
    table[node.id].dep=node.dep;
    if(node.cFlag)
        table[node.id].bid=node.father->lChild->id;
    else
        table[node.id].bid=node.father->rChild->id;
    
}
int parser::orderTravel(TreeNode& node,int &step,unordered_map<int, int>& REG){
    int leftID,rightID;
    if(node.lChild->lChild!=NULL) leftID=orderTravel(*(node.lChild),step,REG);
    else leftID=node.lChild->id;
    if(node.rChild->lChild!=NULL) rightID=orderTravel(*(node.rChild),step,REG);
    else rightID=node.rChild->id;
    // because only leaf node id is added
    compData.order_1[step]=leftID;
    compData.order_2[step]=rightID;
    int i;
    for(i=0;i<depth;i++){
        if(REG.find(i)!=REG.end())
            continue;
        else{
            REG[i]=1;
            break;
        }
    }
    compData.order_N[step]=node.id;
    compData.dist_1[step]=node.lDist;
    compData.dist_2[step]=node.rDist;
    step++;
    if (leftID<0)
        REG.erase(REG.find(leftID+depth));
    if (rightID<0)
        REG.erase(REG.find(rightID+depth));
    return node.id;
}
void parser::genCompData(){
    compData.order_1=new int[leafNumber];
    compData.order_2=new int[leafNumber];
    compData.order_N=new int[leafNumber];
    compData.dist_1=new float[leafNumber];
    compData.dist_2=new float[leafNumber];
    int i=0;
    unordered_map<int, int> REG;
    orderTravel(tree,i,REG);
}
void parser::printCompData(ofstream &ofile){
    for (int i=0;i<leafNumber-1;i++){
        ofile<<compData.order_1[i]<<'\t';
        ofile<<compData.dist_1[i]<<'\t';
        ofile<<compData.order_2[i]<<'\t';
        ofile<<compData.dist_2[i]<<'\t';
        ofile<<compData.order_N[i]<<'\n';
    }
}
void parser::printWalk(TreeNode *A, ofstream &ofile){
    bool flag=false;
    if(A->lChild!=NULL){
        if(A->name.length()>2){
            flag=true;
            ofile<<'(';
        }
            
        ofile<<'(';
        printWalk(A->lChild, ofile);
        ofile<<',';
        printWalk(A->rChild, ofile);
        ofile<<')';
        if(flag){
            ofile<<":0,OTU_"<<A->id<<":0)";
        }
    }
    else
        ofile<<"OTU_"<<A->id;
    ofile<<':'<<A->fDist;
    return;
}
int parser::printTree(ofstream &ofile){
    auto pointer=tree;
    printWalk(&tree, ofile);
    return 0;
}
