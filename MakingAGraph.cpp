// MakingAGraph.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*

0. Need to create a list of links yes
1. Need to write a parser to split connections given, eg. DC DG GE GA GD yes
2. Need to create graph from those aforementioned links. Recursion?? no recursion used, pointers/references are a pain doe :/

-----------------

Pointers and dat init
int* ptr; -> this creates a pointer - currently it points to nothing as we havent set it to point to anything
int var = 5; -> this creates a variable called "var", placed somewhere in the stack memory with the value 5
ptr = &var; -> this sets the pointer "ptr" to point to the _address_ of the "var" variable.

cout << ptr; -> this will print the address that "ptr" is pointing to, in this case the address of the "var" variable
cout << *ptr; -> this will print the value of the variable at that address, in this case printing 5

int& ref = var; -> gives us another way of accessing that address in memory - both "var" and "ref" will lead to the same variable in memory.

-----------------
*/

#include <iostream>
#include <string>
#include <vector>

//We want to reference the stringToSplit in order to not copy it, char doesn't have to be referenced
std::vector<std::string> SplitString(std::string& strToSplit, char splitChar) {
    std::vector<std::string> vecToReturn;

    int firstOfChain = -1;

    for (size_t i = 0; i <= strToSplit.size(); i++)
    {
        if (strToSplit[i] == splitChar || i == strToSplit.size()) {
            std::string element = strToSplit.substr(firstOfChain + 1, i - firstOfChain - 1);
            //Want to allocate in heap memory as the string could contain many split strings inside it
            firstOfChain = i;
            vecToReturn.emplace_back(element);
        }
    }

    return vecToReturn;
};

struct Node {
    std::vector<Node*> connectedNodes;
    char nodeChar;

    Node(char startChar) {
        nodeChar = startChar;
    }

    void AddNode(Node* nodeToAdd) {
        connectedNodes.emplace_back(nodeToAdd);
    }
};

Node& GetNodeWithChar(char charToLookFor, std::vector<Node*>& nodeVec) {
    for (Node* n : nodeVec) {
        if (n->nodeChar == charToLookFor) {
            return *n;
        }
    }
}

int main()
{
    //Could expand into automatically creating new nodes when coming across a new letter
    //Be aware of pushing something onto a vector and the data could be moved, meaning that every pointer that points to the data is invalidated.
    
    //Hello this will split based on the spaces
    std::string links = "DC DB BE BA BD CE CA";
    std::vector<std::string> splitLinks = SplitString(links, ' ');
    
    /* This is allocated on the heap
    Node* a = new Node('A');
    Node* b = new Node('B');
    Node* c = new Node('C');
    Node* d = new Node('D');
    */

    //This is allocated on the stack
    Node a('A');
    Node b('B');
    Node c('C');
    Node d('D');

    // In the vector, this assignment/initialization is putting in new instances of a,b,c,d, which is why at the end of the program a,b,c,d are empty
    // We are modifying the nodes inside the vector and not the ones at the top. I have to put in references to the nodes inside the vector;
    // std::vector<Node> nodeVec = {a,b,c,d};
    // a and nodeVec[0] are at different addresses because the members of nodeVec are placed in the heap whilst a is placed in the stack when it is
    // declared.  

    // Here, we want a vector of pointers for Nodes, so that we can reference the correct node from the stack memory and not
    // create new Nodes in the heap. We pass in the locations of the different nodes
    std::vector<Node*> nodeVec = { &a,&b,&c,&d};

    for (std::string& str : splitLinks) {
        std::cout << "Adding " << str[1] << " to " << str[0] << "\n";

        GetNodeWithChar(str[0], nodeVec).AddNode(&GetNodeWithChar(str[1], nodeVec));
        std::cout << "There are " << GetNodeWithChar(str[0], nodeVec).connectedNodes.size() << " nodes connected to " << str[0] << "\n\n";
    }

    for (Node* base : nodeVec) {
        for (Node* n : base->connectedNodes) {
            std::cout << base->nodeChar << n->nodeChar << "\n";
        }
    }

    std::cout << "\n";
    std::cout << "Address of a at the top is " << &a << "\n";
    std::cout << "Address of a in the vector is " << &(GetNodeWithChar('A', nodeVec)) << "\n";
};