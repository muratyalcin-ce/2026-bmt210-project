#ifndef STACK_H
#define STACK_H


typedef struct Stroke Stroke;

typedef struct StackNode {
    Stroke* stroke;
    struct StackNode* next;
} StackNode;

typedef struct {
    StackNode* top;
} Stack;

void InitStack(Stack* stack);
void Push(Stack* stack, Stroke* stroke);
Stroke* Pop(Stack* stack);
int IsEmpty(Stack* stack);
void ClearStack(Stack* stack);

#endif
