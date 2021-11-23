#include "miniGit.hpp"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

int main()
{
    miniGit g;
    bool quit = false;
    while(quit == false)
    {
        string input;
        cout << "PLEASE CHOOSE AN OPTION\n" << "1. Initialize new repo\n" <<  "2. Add new file\n" << "3. Remove file\n"<< "4. Commit changes\n" << "5. Checkout\n" << "6. QUIT\n" << endl;
        cin >> input;
        switch(stoi(input))
        {
            case 1: g.initNewRepo(); break;
            case 2: {
                if (g.isDLLEmpty())//if repo not initialized return to main menu
                {
                    cout << "Repo not initialized.\n" << endl;
                }
                else if (g.isLatestCommit())//checking if current version is the same as last commit
                {
                    string fileName;
                    bool found = false;
                    while(found == false)
                    {
                        cout << "Enter file name: \n";
                        cin >> fileName;
                        ifstream myfile;
                        myfile.open(fileName);
                        if (myfile.is_open()) //check if file is in directory
                        {
                            cout << "File exists in directory.\n";
                            found = true;
                            myfile.close();
                        }
                        else 
                        {
                            cout << "File doesn't exist.\n";
                            found = false;
                        }
                    }
                    if (g.fileExists(fileName)) //check if file exists in current commit
                    {
                        cout << "File has already been added.\n";
                    }
                    else
                    {
                        cout << "File has not been added yet.\n";
                        g.addFile(fileName);
                    }
                    cout << endl;
                }
                else cout << "Current version is different from last commit. No changes can be made.\n" << endl;
            }break;
            case 3: {
                if (g.isLatestCommit())
                {
                    string fileName;
                    cout << "Enter file name: \n";
                    cin >> fileName;
                    
                    if (g.fileExists(fileName))//check if exists in current commit
                    {
                        g.removeFile(fileName);
                    }
                    else
                    {
                        cout << "File doesn't exist. Nothing deleted.\n";
                    }
                    cout << endl;
                }
                else cout << "Current version is different from last commit. No changes can be made.\n" << endl;
                
            }break;
            case 4:{
                if (g.isLatestCommit()) g.commit();
                else cout << "Current version is different from last commit. No changes can be made.\n" << endl;
            }break;
            case 5: {
                if (g.isDLLEmpty()){
                    cout << "No commits yet.\n";
                }
                else{
                    int commitNum;
                    cout << "Enter a commit number: \n";
                    cin >> commitNum;
                    g.checkout(commitNum);
                }
                cout << endl;
            }break;
            case 6: cout << "Goodbye!" << endl; quit = true; break;
            default: cout << "Not a valid option" << endl;break;
        }
    }
    return -1;
}