#include "miniGit.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#include <filesystem>

namespace fs = std::filesystem;

miniGit::miniGit()
{
    headNode = NULL;
    tailNode = NULL;
    currentNode = NULL;
}
miniGit::~miniGit()
{
    nodeDLL* temp = headNode;
    nodeDLL* prev = NULL;
    while(temp != NULL)
    {
        nodeSLL* temp1 = temp->head;
        nodeSLL* prev1 = NULL;
        while(temp1 != NULL)//delete SLL 
        {
            prev1 = temp1;
            temp1 = temp1->next;
            delete prev1;
        }
        prev = temp;
        temp = temp->next;
        delete prev;
    }
}
bool miniGit::isDLLEmpty()
{
    return (headNode == NULL);
}
nodeDLL* createDLLnode(int commitNum, nodeDLL* previousNode, nodeDLL* nextNode, nodeSLL* headSLL)
{
    nodeDLL* doubleNode = new nodeDLL();    //create new DLL node  
    doubleNode->commitNumber = commitNum;
    doubleNode->prev = previousNode;
    doubleNode->next = nextNode;
    doubleNode->head = headSLL;
    return doubleNode;
}
nodeSLL* createSLLnode(string name, string fileVersionName, int num)
{
    nodeSLL* singleNode = new nodeSLL();//create new SLL node
    singleNode->fileName = name; 
    singleNode->fileVersion = fileVersionName;
    singleNode->next = NULL;
    singleNode->numUpdates = num;
    return singleNode;
}
void printLatestCommit(nodeDLL* latestCommit)
{
    nodeDLL* temp = latestCommit;
    cout << "Commit Number: " << temp->commitNumber << endl;
    nodeSLL* singleNode = temp->head;
    cout << "Files: \n";
    while(singleNode != NULL)//printing all files in commit
    {
        cout << singleNode->fileName << "\nCommitted version: " << singleNode->fileVersion << endl;
        singleNode = singleNode->next;
    } 
    cout << endl;
}
void miniGit::initNewRepo() 
{
    //create new directory 
    cout << "Initializing new repository..."<< endl;
    fs::remove_all(".minigit");
    fs::create_directory(".minigit");

    nodeDLL* newNode;
    if(isDLLEmpty())//if DLL is empty
    {
        newNode = createDLLnode(0, NULL, NULL, NULL);
        headNode = newNode; //setting head DLL node
        tailNode = newNode; 
        currentNode = newNode;
    }
    printLatestCommit(newNode);
    cout << endl;
}
bool miniGit::fileExists(string name)
{
    nodeDLL* doubleNode = currentNode;
    nodeSLL* temp = doubleNode->head;//start at most recent commit and traverse SLL to find if file already exists
    while(temp != NULL && temp->fileName != name)
    {
        temp = temp->next;
    }
    if (temp == NULL)
    {
        return false;
    }
    return true;
}
void miniGit::addFile(string name)
{
    nodeDLL* doubleNode = currentNode;

    nodeSLL* newNode = createSLLnode(name, name + "_0", 0);

    cout << "Adding " << newNode->fileName << "...\n";

    if (doubleNode->head == NULL)//no files in latest commit
    {
        doubleNode->head = newNode;
    }
    else //if other files present then add to SLL 
    {
        nodeSLL* temp = doubleNode->head;
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode; 
        newNode->next = NULL;
    }
}
void miniGit::removeFile(string name)
{
    nodeDLL* doubleNode = currentNode;
    nodeSLL* curr = doubleNode->head;
    nodeSLL* prev = NULL;
    while(curr != NULL && curr->fileName != name)
    {
        prev = curr;
        curr = curr->next;
    }
    if (curr->fileName == name)
    {
        cout << "Deleting " << curr->fileName << "...\n";

        if (curr == doubleNode->head)//deleting head node
        {
            if (curr->next == NULL)//only node in the list
            {
                doubleNode->head = NULL;
                delete curr;
            }
            else
            {
                doubleNode->head = curr->next;
                delete curr;
            }
        }
        else
        {
            prev->next = curr->next;
            delete curr;
        }
    }
    else cout << "Nothing deleted\n";
}
void miniGit::commit()
{
    nodeDLL* doubleNode = currentNode;
    nodeSLL* temp = doubleNode->head;
    if (temp == NULL){
        cout << "No files to commit\n" << endl;
        return;
    }
    vector<string> v;//keep track of all the updated file versions
    vector<int>u;//keep track of update num for every SLL node 
    while(temp != NULL)//traverse entire SLL node
    {
        string line;
        string s1 = "";
        string s2 = "";
        ifstream inFile;
        ifstream minigitFile;

        inFile.open(temp->fileName); 
        while(getline(inFile, line)){
            s1 += line;//reading file contents
        }
        inFile.close();

        string file = ".minigit/" + temp->fileVersion;
        minigitFile.open(file);

        if (minigitFile.is_open())//if fileVersion file does exist, check if it has changed by comparing strings
        {
            cout << "File exists in .minigit \n";
            while(getline(minigitFile, line)){
                s2 += line;//read file into string
            }
            minigitFile.close();
            
            if(s1 == s2){ //no changes made
                cout << temp->fileName << " has not been changed\n";
                v.push_back(temp->fileVersion);
                u.push_back(temp->numUpdates);
            }
            else{ //add changed file to .minigit
                cout << "Changes have been made to: " << temp->fileName << endl;
                string fileVersion = temp->fileName + "_" + to_string(temp->numUpdates);//updating file version
                v.push_back(fileVersion);
                u.push_back(temp->numUpdates+1);
                string filePath = ".minigit/" + fileVersion;
                ofstream outFile(filePath);
                inFile.open(temp->fileName);
                while(getline(inFile, line))
                {
                    outFile << line << endl;//copying file to minigit folder
                }
                outFile.close();
            }
        }
        else //if fileVersion file does not exist, copy file into .minigit (name should be fileVersion name)
        {
            cout << "Copying file to .minigit: " << temp->fileName << endl;
            string fileVersion = temp->fileName + "_" + to_string(temp->numUpdates); //updating file version
            v.push_back(fileVersion);
            u.push_back(temp->numUpdates+1);
            string filePath = ".minigit/" + fileVersion; 
            inFile.open(temp->fileName);
            ofstream outFile(filePath);//copy file to .minigit
            while(getline(inFile,line))
            {
                outFile << line << endl;
            }
            outFile.close();
        }
        inFile.close();
        temp = temp->next;
    }
    cout << endl;

    //CREATING NEW COMMIT NODE
    nodeSLL* temp1 = doubleNode->head;//pointer to previous SLL head 
    nodeSLL* headSLL = createSLLnode(temp1->fileName, v[0], u[0]); //creating head node of new SLL
    nodeDLL* latestCommit = createDLLnode(doubleNode->commitNumber + 1, doubleNode, NULL, headSLL);//pointer to new DLL node
    doubleNode->next = latestCommit; 

    nodeSLL* curr = latestCommit->head;
    temp1 = temp1->next;
    int index = 1;
    while(temp1 != NULL)//copying the fileName to new SLL, also updating file version and update number
    {
        curr->next = createSLLnode(temp1->fileName, v[index], u[index]);
        index++;
        curr = curr->next;
        temp1 = temp1->next;
    }
    currentNode = latestCommit;//updating current node
    tailNode = latestCommit;
    printLatestCommit(latestCommit);
    v.clear();
    u.clear();
}
void miniGit::checkout(int commitNum)
{     
    cout << "Warning: You will lose any changes made after your last commit!\n";
    nodeDLL* doubleNode = headNode;

    while(doubleNode != NULL && doubleNode->commitNumber != commitNum){
        doubleNode = doubleNode->next;
    }
    if (doubleNode == NULL){
        cout << "Not a valid commit number.\n";
    }
    else if (doubleNode->commitNumber == commitNum)
    {        
        currentNode = doubleNode;
        nodeSLL* temp = doubleNode->head;
        while(temp != NULL)
        {
            cout << "Overwriting " << temp->fileName << " with " << temp->fileVersion << endl;
            ifstream inFile (".minigit/"+temp->fileVersion);
            ofstream outFile(temp->fileName);
            string line;
            while(getline(inFile, line)) //Overwrite current file with version in .minigit folder
            {
                outFile << line << endl;
            }
            inFile.close();
            outFile.close();
            temp = temp->next;
        }
    }
}
bool miniGit::isLatestCommit()
{    
    if (currentNode == tailNode)return true;
    else return false;
}
