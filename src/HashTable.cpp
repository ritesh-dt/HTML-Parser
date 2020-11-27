#include <iostream>
#include <list>
using namespace std;

#include "Node.cpp"

class HashTable {
private:
    int capacity;
    list<Node> *table;

public:
    HashTable();
    HashTable(int capacity);
    void insertItem(int key, Node data);
    void deleteItem(int key, Node data);
    void increaseSize(int);
    void decreaseSize(int);
    void displayHashTill(int);
    list<Node> getItem(int key);

    // Check is a number is prime or not
    int isPrime(int num) {
        if (num == 1 || num == 0)
            return 0;
        for (int div=2; div <= num/2; div++) {
            if (num%div == 0)
                return 0;
        }
        return 1;
    }

    // Get the next prime after a number
    int getPrime(int num) {
        if (num%2 == 0)     num++;
        while (!isPrime(num)) num += 2;
        return num;
    }

    int hash(int key) {
        return key % capacity;
    }

    void displayHash();
};

HashTable::HashTable(int capacity) {
    int size = getPrime(capacity);
    this->capacity = size;
    table = new list<Node>[capacity];
}

// Node HashTable::insertNode(int key, int nextId, int type, string token, string attr[], int attr_count) {
//     // int index = hash(key);
//     int index = key;

//     Node* newNode = createNode(nextId, 1, token, attrs, attr_count);
//     table[index].push_back(*newNode);
// }

void HashTable::insertItem(int key, Node newNode) {
    // int index = hash(key);
    int index = key;
    table[index].push_back(newNode);
}

void HashTable::deleteItem(int key, Node newNode) {
    // int index = hash(key);
    int index = key;
    
    list<Node>::iterator iter;
    for (iter = table[index].begin(); iter != table[index].end() ; iter++) {
        if (iter->id == newNode.id)
            table[index].erase(iter);
    }
    // table[index].pop_back();
}

void HashTable::displayHash() {
    cout << "--- START ---" << endl;
    for (int i=0; i < capacity; i++) {
        cout << "table[" << i << "] ";

        list<Node>::iterator iter;
        for (iter = table[i].begin(); iter != table[i].end(); iter++) {
            cout << iter->name << " -> ";
        }
        cout << endl;
    }
    cout << "--- END ---" << endl;
}

void HashTable::displayHashTill(int end) {
    cout << "--- START ---" << endl;
    for (int i=0; i < end; i++) {
        cout << "table[" << i << "] ";
        
        list<Node>::iterator iter;
        for (iter = table[i].begin(); iter != table[i].end(); iter++) {
            cout << iter->name << " -> ";
        }
        cout << endl;
    }
    cout << "--- END ---" << endl;
}

list<Node> HashTable::getItem(int key) {
    return table[key]; 
}