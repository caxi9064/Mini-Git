#pragma once
#include <iostream>
#include <string>
using namespace std;
struct nodeSLL //store list of files in current commit
{
    string fileName; //local file
    string fileVersion; //file inside minigit folder
    nodeSLL* next;
    int numUpdates = 0;//this will keep track of the updates to file 

};
struct nodeDLL//corresponds to single commit
{
    int commitNumber;
    nodeSLL* head; 
    nodeDLL* prev;
    nodeDLL* next;
};

class miniGit
{
    private:
        nodeDLL* headNode; 
        nodeDLL* tailNode; //latest commit
        nodeDLL* currentNode; //corresponds to current commit version 
    public:
        miniGit();//constructor
        ~miniGit();//destructor
        bool isDLLEmpty(); //check if list is empty or not
        void initNewRepo(); //initialize new repository
        bool fileExists(string file_name);//check if file is already added
        void addFile(string file_name);//adding a file
        void removeFile(string file_name);//removing a file
        void commit();//committing changes
        void checkout(int commitNum);//checkout previous commits
        bool isLatestCommit(); //checks if current version is the same as last commit
};