Core functionality:
In my main function, I implemented a menu for the user to choose from using a while loop and switch statement. 
I implemented all other parts of the project in the miniGit class.
The DLL keeps track of all the commits and has a pointer to the head of the SLL which contains the files the user adds. 
I used a struct to implement each node of the linked lists. 
The pointers headNode, tailNode, and currentNode are private variables in my miniGit class and are pointers so I can keep track of the commits.
Additionately, the variable numOfUpdates in nodeSLL struct will keep track of the version number. 

For the initialize function, I used the filesystem library to create a new repository called ".minigit". 
If the repository already exists, the program will remove it first and everything inside, then create a new one. 

For the add file function, the name of file that the user wants to add is taken as a parameter and a new SLL node is create.

For the remove file function, the SLL node is traversed until the file to be deleted is found. 

For the commit function, the SLL node is traversed and for each node I used the fstream library to first open the file 
in the current repository then the version in .minigit. If file exists in .minigit and there are changes or if file doesn't exist, 
then I will use a ofstream variable to read to file in .minigit. Then a new DLL node is created for the new commit and all the files are copied to a new SLL.

The checkout function just traverses the DLL until commit number is found then copies it into the file in the repository.

The destructor will deallocate the memory by traversing through the DLL and SLL.

Helper functions: 
createDLLnode() and createSLLnode() create a new node and return a pointer to it.
isDLLempty() will check if there are any commits. If there are none then the user must choose to initialize new repo before the other options.
fileExists() checks if the file already exists when the user decides to add or remove a file. 
After each commit, the printLatestCommit() function will print out all the files in the commit for user.
isLatestCommit() function returns a boolean value. If currentNode equals tailNode, then the function is true.



