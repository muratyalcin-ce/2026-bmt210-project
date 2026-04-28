#include "stack.h"
#include <stdlib.h>

void InitStack(Stack* stack)
{
    stack->top = NULL;
}

void Push(Stack* stack, Stroke* stroke)
{
    StackNode* node = (StackNode*) malloc(sizeof(StackNode));
	if(!node) return;
    node->stroke = stroke;
    node->next = stack->top;
    stack->top = node;
}

Stroke* Pop(Stack* stack)
{
    if (stack->top == NULL)
        return NULL;

    StackNode* temp = stack->top;
    Stroke* stroke = temp->stroke;

    stack->top = temp->next;
    free(temp);

    return stroke;
}

int IsEmpty(Stack* stack)
{
    return stack->top == NULL;
}

void ClearStack(Stack* stack)
{
    while (!IsEmpty(stack))
    {
        Pop(stack);
    }
}
