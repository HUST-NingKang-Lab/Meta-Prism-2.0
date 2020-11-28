//
// Created by LuoFan  on 2019-12-19.
//

/*
 newickParser.cpp: parsing newick format phylogenic tree and output informations of tree
 
 */





#ifndef SRC_NEWICKPARSER_H
#define SRC_NEWICKPARSER_H
#include "structure.h"
using namespace std;
class parser{// newick format phylogenic tree parser
protected:
    TreeNode tree;// evolutional tree
    int treeSize,leafNumber,depth;
    unordered_map<string,int> label;// node name to node id
    unordered_map<int, TreeNode*> index;//node id to node ptr
    void parseOne(char* newick,int &index,int end,TreeNode &node,int mod);
    void nameTravel(TreeNode& node,int &ID);
    void genTable(TreeNode &node, int depth);
    int orderTravel(TreeNode &node,int& step,unordered_map<int, int>& REG);
    CompData compData;
    Table *table;//int fid,bid,dep;float dist;
    void printWalk(TreeNode* A,ofstream &ofile);
public:
    parser(string newickTree,int mod=0);//parse a newick tree
    const Table* getTable(){return table;}
    int getDepth(){return depth;}//get the max depth of tree
    void genCompData();
    const CompData& getCompData(){return compData;}
    void printCompData(ofstream& ofile);
    vector<abdElement> transfer(ifstream &in);
    const unordered_map<string,int>& getLabel(){return label;}// node name to node id
    const unordered_map<int, TreeNode*>& getIndex(){return index;}//node id to node ptr
    int getLeafSize(){return leafNumber;}//number of leaf nodes
    int getTreeSize(){return treeSize;}//number of all nodes
    int printTree(ofstream &ofile);
    const TreeNode * getTree(){return &tree;}
};
#endif //SRC_NEWICKPARSER_H
