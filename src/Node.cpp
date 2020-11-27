#ifndef NODE_PRESENT
#define NODE_PRESENT

#include <iostream>
#include <list>

using namespace std;
// #include "Node.h"

class Node {
    public:
        int id;
        string name;

        // type (int) - 0 - text
        //              1 - tag
        int type;

        string* attrs;
        // list<string> attrs;
        int attr_count;
    
        Node (int type, string name) {
            type = type;
            name = name;
        }
        void print();
};

// Print a node's contents
void Node::print() {
    if (type == 0)
        cout << "Text ";
    else
        cout << "Tag ";
    cout << id << ": " << name << endl;

    // list<string>::iterator iter;
    // for (iter = attrs.begin(); iter != attrs.end(); iter++) {
    //     cout << "\t" << &iter <<  ": ";
    //     iter++;
    //     cout << &iter << endl;
    // }
    for (int i=0; i < attr_count; i++) {
        cout << "\t" << attrs[(i*2)] <<  ": " << attrs[(i*2)+1] << endl;
    }
}

Node createNode(int id, int type, string name, string attrs[], int attr_count) {
    Node newNode = Node(type, name);
    newNode.id = id;
    newNode.name = name;
    newNode.type = type;
    // cout << name << " Creating string attr" << endl;
    
    // newNode.attrs = (string*) malloc(sizeof(string) * attr_count);
    newNode.attrs = new string[attr_count*2];
    // cout << sizeof(string) * attr_count << endl;
    newNode.attr_count = attr_count;
    for (int i=0; i < attr_count*2; i++) {
        newNode.attrs[i] = attrs[i];
        // newNode.attrs.push_back(attrs[i]);
    }
    // newNode.print();
    return newNode;
}

// Node* createNode(int id, int type, string name, string attrs[], int attr_count) {
//     Node* newNode = (Node*) malloc(sizeof(Node));
//     newNode->id = id;
//     newNode->type = type;
//     newNode->name = name;
//     // cout << name << " Creating string attr" << endl;
    
//     // newNode->attrs = (string*) malloc(sizeof(string) * attr_count);
//     newNode->attrs = new string[attr_count];
//     // cout << sizeof(string) * attr_count << endl;
//     newNode->attr_count = attr_count;
//     for (int i=0; i < attr_count; i++) {
//         newNode->attrs[i] = attrs[i];
//         // newNode->attrs.push_back(attrs[i]);
//     }
//     return newNode;
// }

#endif