//
// Created by LuoFan  on 2019-12-19.
//
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
using namespace std;

#ifndef SRC_NEWICKPARSER_H
#define SRC_NEWICKPARSER_H

struct TreeNode{
    string name;
    int id=-1;
    TreeNode *lChild= nullptr,*rChild= nullptr,*father=nullptr;
    float lDist=0,rDist=0,fDist=0;
    bool cFlag;
};
struct Table{
    int fid,bid;float dist;
};
class parser{
private:
    TreeNode tree;
    int treeSize;
    unordered_map<string,int> label;
    void parseOne(char* newick,int &index,int end,TreeNode &node,int mod);
    void nameTravel(TreeNode& node,int &ID);
    void genTable(TreeNode &node);
    Table *table;
public:
    parser(string newickTree,int mod=0);
    Table* getTable();
    vector<Table> transfer();
};

#endif //SRC_NEWICKPARSER_H
