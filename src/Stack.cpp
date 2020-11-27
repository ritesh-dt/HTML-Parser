#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include "Node.cpp"

typedef struct node {
    Node node;
    struct node* next;
} StackNode;

typedef struct stackStruct {
    StackNode* top;  
    int length;
} Stack; 

Stack* createStack() {
    Stack* newStack = (Stack*) malloc(sizeof(Stack));
    newStack->top = NULL;
    newStack->length = 0;
    return newStack;
}

int isEmpty(Stack* s) {
    return s->length == 0;
}

void pushStack(Stack* s, Node data) {
    StackNode* newNode = (StackNode*) malloc(sizeof(StackNode));
    newNode->node = data;
    newNode->next = s->top;
    s->top = newNode;
    s->length++;
}

Node peekStack(Stack *s) {
    if (isEmpty(s)) {
        cout << "Stack is empty!\n";
        exit(1);
    }
    return s->top->node;
}

Node popStack(Stack *s) {
   if (isEmpty(s)) {
        cout << "Stack is empty!\n";
        exit(1);
    }
    StackNode* toFree = s->top;
    Node data = s->top->node;
    s->top = s->top->next;
    s->length--;
    free(toFree);
    return data;
}


void printStack(Stack *s) {
    StackNode* node = s->top;
    cout << "\nTOP -> ";
    while (node != NULL) {
        cout << node->node.name << " -> ";
        node = node->next;
    }
    cout << "BOTTOM\n";
}

int lengthStack(Stack *s) {
    return s->length;
}