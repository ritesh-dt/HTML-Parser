#include <iostream>
#include <fstream>
#include <list>

using namespace std;

#include "Stack.cpp"
#include "Node.cpp"
#include "HashTable.cpp"

#define MAX_TOKEN_LENGTH 30
#define MAX_FILENAME_LENGTH 20

void trimWhiteSpace(ifstream&);
int isWhiteSpace(char);
string getTokenTill(ifstream&, string);
int isSingleElement(string);

int getTag(ifstream&, int, list<Node>*, HashTable);
void getText(ifstream&, int, list<Node>*, HashTable);

void parseHtml(string, HashTable*);
void printTree(HashTable, int);
void indentOutput(int);

void displayMenu(string*, HashTable*);

int main() {
    string file;
    HashTable adjList = HashTable(151);
    displayMenu(&file, &adjList);

    // parseHtml("example.html", &adjList);
    // cout << "--- Tree ---\n";
    // printTree(adjList, 5);
    // cout << "--- Tree End ---\n";

    return 0;
}

void parseHtml(string file_name, HashTable* adjList) {
    ifstream html;
    html.open(file_name);
    char newChar;
    int nextId = 1;

    trimWhiteSpace(html);

    // Stack* elementStack = createStack();
    list<Node> elementStack;

    // *adjList = HashTable(151);

    while (1) {
        if (html.eof())     break;      // File end reached

        if (html.peek() == '<') {
            if (getTag(html, nextId, &elementStack, *adjList) == 0) {     // Opening tag
                nextId++;
            } 
        } else if (!isWhiteSpace(html.peek())) {
            getText(html, nextId, &elementStack, *adjList);
            nextId++;
        }
        trimWhiteSpace(html);
    }
    // adjList->displayHashTill(nextId);

    html.close();
    return;
}

int getTag(ifstream& fin, int nextId, list<Node> *stack, HashTable hash) {
    // Skips < and whitespaces
    fin.get();      trimWhiteSpace(fin);
    
    string token = getTokenTill(fin, " >\n");
    if (token == "!--") {
        cout << (char) fin.peek() << endl;
        return -1;
    }

    // Check if the tag is a closing tag and exit if so
    if (token[0] == '/') {
        getTokenTill(fin, ">");
        token = token.substr(1, token.length()-1);
        if (stack->back().name == token) {
            stack->pop_back();
        }
        fin.get();
        return 1;
    }

    string attrs[50];
    int attr_count = 0;
    // cout << token << endl;

    // Get attributes like id, class
    char newChar;
    trimWhiteSpace(fin);
    newChar = fin.peek();
    while (newChar != '>' && newChar != '/') {
        string attr = getTokenTill(fin, " =>/");
        // cout << "Attribute: " << attr << endl;
        if (attr == "/")
            break;
        getTokenTill(fin, "=/>");
        // cout << "Next: " << (char) fin.peek() << endl;
        attrs[(attr_count*2)] = attr;

        trimWhiteSpace(fin);    // Handle space after attribute name
        if (fin.peek() == '=') {    // Check if attribute has a value
            // Skip =, whitespace and "
            fin.get();      trimWhiteSpace(fin);       
            char invertedCommaType = fin.get();

            if (invertedCommaType == '\"') 
                attrs[(attr_count*2)+1] = getTokenTill(fin, "\"");
            else if (invertedCommaType == '\'') 
                attrs[(attr_count*2)+1] = getTokenTill(fin, "\'");

            fin.get();      // Skip "
        } else if (fin.peek() != '>' || fin.peek() != '/') {   // Check if attribute has no value e.g. required, hidden
            attrs[(attr_count*2)+1] = "true";
        }
        
        trimWhiteSpace(fin);
        newChar = fin.peek();
        attr_count++;
    }
    if (fin.peek() == '/')  fin.get();

    // Skips / and >
    fin.get();  trimWhiteSpace(fin);

    Node newNode = createNode(nextId, 1, token, attrs, attr_count);
    // Node* newNode = createNode(nextId, 1, token, attrs, attr_count);
    
    // hash.insertItem(nextId, *newNode);
    // if (!isEmpty(stack)) {
    //     Node parent = peekStack(stack);
    //     hash.insertItem(nextId, parent);
    //     hash.insertItem(parent.id, *newNode);
    // } else {
    //     hash.insertItem(nextId, *newNode);
    // }
    // newNode->print();
    // if (!isSingleElement(token))
    //     pushStack(stack, *newNode);

    hash.insertItem(nextId, newNode);
    if (stack->size() > 0) {
        Node parent = stack->back();
        hash.insertItem(nextId, parent);
        hash.insertItem(parent.id, newNode);
    } else {
        hash.insertItem(0, newNode);
        hash.insertItem(nextId, newNode);
    }
    // newNode.print();
    if (!isSingleElement(token))
        stack->push_back(newNode);
    
    return 0;
}

void getText(ifstream& fin, int nextId, list<Node>* stack, HashTable hash) {
    trimWhiteSpace(fin);
    string text = getTokenTill(fin, "<");
    string attrs[0];

    // Node* newNode = createNode(nextId, 0, text, attrs, 0);

    // hash.insertItem(nextId, *newNode);
    // if (!isEmpty(stack)) {
    //     Node parent = peekStack(stack);
    //     hash.insertItem(nextId, parent);
    //     hash.insertItem(parent.id, *newNode);
    //     // cout << peekStack(stack).name << " -> ";
    // }
    // newNode->print();

    Node newNode = createNode(nextId, 0, text, attrs, 0);

    hash.insertItem(nextId, newNode);
    if (stack->size() > 0) {
        Node parent = stack->back();
        hash.insertItem(nextId, parent);
        hash.insertItem(parent.id, newNode);
        // cout << peekStack(stack).name << " -> ";
    } else {
        hash.insertItem(nextId, newNode);
        hash.insertItem(0, newNode);
    }
    // newNode.print();

    return;
}

void printTree(HashTable hash, int count) {

    // Stack* dfsStack = createStack();
    list<Node> dfsStack;
    list<int> levelStack;
    
    int level = 0;
    list<Node> roots = hash.getItem(0);
    list<Node>::iterator iter = roots.begin();
    for (; iter != roots.end(); iter++) {
        dfsStack.push_front(*iter);;
        levelStack.push_front(level);
    }
    // pushStack(dfsStack, start);
    

    // while (!isEmpty(dfsStack)) {
    while (dfsStack.size() > 0) {
        // Node node = popStack(dfsStack);
        Node node = dfsStack.back();
        dfsStack.pop_back();

        level = levelStack.back();
        levelStack.pop_back();

        indentOutput(level);
        cout << node.name << endl;
        // indentOutput(level);
        // cout << "----> ";

        if (hash.getItem(node.id).size() <= 2)
            continue;

        
        list<Node> elements = hash.getItem(node.id);
        
        list<Node>::iterator iter = elements.end();
        iter--;
        list<Node>::iterator end = elements.begin();
        end++;

        for (; iter != end; iter--) {
            levelStack.push_back(level+1);
            // pushStack(dfsStack, *iter);
            dfsStack.push_back(*iter);
            
        }
    }

    dfsStack.clear();
}

void indentOutput(int level) {
    for (int tab=0; tab < level; tab++) {
        cout << "\t";
    }
}

// Get the next token until a delimiter is found
string getTokenTill(ifstream& fin, string delimit) {
    string token;
    while (delimit.find(fin.peek()) >= delimit.length()) {
        char newChar;
        fin.get(newChar);
        token.push_back(newChar);
    }
    return token;
}

// Skip any whitespaces and stop before the next non-whitespace character
void trimWhiteSpace(ifstream& fin) {
    while (isWhiteSpace(fin.peek()))
        fin.get();
}

// Check if the given character is a white space
int isWhiteSpace(char c) {
    string whitespace =  " \n\r\t\f\v";
    return whitespace.find(c) < whitespace.length();
}

int isSingleElement(string token) {
    if (token.compare("!DOCTYPE") == 0 || token.compare("img") == 0)
        return 1;
    if (token.compare("meta") == 0 || token.compare("input") == 0)
        return 1;
    if (token.compare("br") == 0 || token.compare("hr") == 0)
        return 1;
    return 0;
}

void displayMenu(string* filename, HashTable* adjList) {
    printf("\n---- HTML Parser ----\n");
    printf("    1: Load HTML file\n");
    printf("    2: Load HTML using a url\n");
    printf("    3: Display HTML element tree (text-based)\n");
    printf("    4: Display HTML element tree (GUI-based) (WIP)\n");

    printf("\nEnter your choice: ");
    int choice;
    scanf("%d", &choice);
    while (choice < 1 || choice > 4) {
        printf("\nPlease select one of the above options\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
    }
    printf("\n");

    switch(choice) {
        case 1: {
            string tempFileName;
            printf("Enter input HTML file name\n");
            printf("NOTE:\n");
            printf(" - Only HTML files allowed\n");
            printf(" - Maximum length allowed: %d\n", MAX_FILENAME_LENGTH);
            printf("Enter . to exit current operation: ");
            fflush(stdin);
            getline(cin, tempFileName);
            if (tempFileName != ".") {
                if (tempFileName.length() > MAX_FILENAME_LENGTH) {
                    printf("Filename is too long\n");
                    break;
                }
                if (tempFileName.find('.') == -1) {
                    printf("Invalid filename\n");
                    break;
                }

                string extension;
                for (int idx=tempFileName.find('.')+1; idx < tempFileName.length(); idx++) {
                    extension.push_back(tempFileName.at(idx));
                }
                if (extension != "html") {
                    printf("Invalid file extension\n");
                    break;
                }
                *filename = tempFileName;
                parseHtml("example.html", adjList);
                break;
            }
        }
        case 3: {
            cout << "--- Tree ---\n";
            printTree(*adjList, 5);
            cout << "--- Tree End ---\n";
            break;
        }
            
    }
    displayMenu(filename, adjList);
}