//
// Created by LuoFan on 2019-12-19.
//

#include "newickParser.h"
using namespace std;

parser::parser(string newickTree, int mod) {
    char newickTreeC[newickTree.size()+1];
    int size,i=0;
    treeSize=1;
    size=newickTree.size();
    strcpy(newickTreeC,newickTree.c_str());
    parseOne(newickTreeC,i,size,tree,mod);
    int id=0;
    this->table=new Table[this->treeSize];
    nameTravel(tree, id);
    genTable(tree);
    return;
}
void parser::parseOne(char *newick, int &index, int end, TreeNode &node,int mod) {
    if (newick[index]!='(')
        cout<<"Error\n";
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
        while (newick[index] != ':')//set name, now index to :
            node.lChild->name += newick[index++];
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
        while (newick[index] != ':')//set name, now index to :
            node.rChild->name += newick[index++];
    }
    //right dist
    index++;startC=newick+index;
    while(newick[index]!=')') index++;
    //endC=newick+(index-1);
    node.rDist=strtod(startC,NULL);
    node.rChild->fDist=node.rDist;
    return;
}

void parser::nameTravel(TreeNode &node, int &ID) {
    if(node.lChild!=NULL){
        nameTravel(*(node.lChild), ID);
        nameTravel(*(node.rChild), ID);
    }
    else{
        label[node.name]=ID;
    }
    node.id=ID++;
    return;
}
void parser::genTable(TreeNode &node){
    if(node.lChild!=NULL){
        genTable(*(node.lChild));
        genTable(*(node.rChild));
    }
    else{
        table[node.id].fid=node.father->id;
        table[node.id].dist=node.fDist;
        if(node.cFlag)
            table[node.id].bid=node.father->lChild->id;
        else
            table[node.id].bid=node.father->rChild->id;
    }
}
