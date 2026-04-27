#include <stdlib.h>
#include "stroke.h"

// Create new stroke
Stroke* CreateStroke(Color color, int size)
{
    Stroke* s = (Stroke*) malloc(sizeof(Stroke));
    s->head = NULL;
    s->tail = NULL;
    s->color = color;
    s->size = size;
    s->next = NULL;
    return s;
}

// Add point to stroke
void AddPointToStroke(Stroke* stroke, int x, int y)
{
    Point* p = (Point*) malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    p->next = NULL;

    if (stroke->tail == NULL)
    {
        stroke->head = stroke->tail = p;
    }
    else
    {
        stroke->tail->next = p;
        stroke->tail = p;
    }
}

// Add stroke to list
void AddStroke(StrokeList* list, Stroke* stroke)
{
    if (list->tail == NULL)
    {
        list->head = list->tail = stroke;
    }
    else
    {
        list->tail->next = stroke;
        list->tail = stroke;
    }
}

// Draw all strokes
void DrawStrokes(StrokeList* list)
{
    for (Stroke* s = list->head; s != NULL; s = s->next)
    {
        for (Point* p = s->head; p != NULL; p = p->next)
        {
            DrawCircle(p->x, p->y, s->size, s->color);
        }
    }
}


/* void DrawStrokesFromStack(Stack* stack) */
/* { */
/*     // Reverse stack (so oldest draws first) */
/*     StackNode* current = stack->top; */

/*     // Temporary array (simple solution) */
/*     Stroke* temp[10000]; */
/*     int count = 0; */

/*     while (current) */
/*     { */
/*         temp[count++] = current->stroke; */
/*         current = current->next; */
/*     } */

/*     for (int i = count - 1; i >= 0; i--) */
/*     { */
/*         Stroke* s = temp[i]; */

/* 		Point* prev = s->head; */
/* 		Point* p = s->head; */
/* 		while (p != NULL) { */
/*             //DrawCircle(p->x, p->y, s->size, s->color); */
/* 			DrawLine(prev->x, prev->y, p->x, p->y, s->color); */
/* 			prev = p; */
/* 			p = p->next; */
/*         } */
/*     } */
/* } */

void DrawStrokesFromStack(Stack* stack)
{
    int count = 0;
    StackNode* cur = stack->top;

    // Count strokes
    while (cur)
    {
        count++;
        cur = cur->next;
    }

    if (count == 0) return;

    Stroke** arr = (Stroke**) malloc(sizeof(Stroke*) * count);

    cur = stack->top;
    for (int i = 0; i < count; i++)
    {
        arr[i] = cur->stroke;
        cur = cur->next;
    }

    // Draw oldest first
    for (int i = count - 1; i >= 0; i--)
    {
        DrawSingleStroke(arr[i]);
    }

    free(arr);
}


void DrawSingleStroke(Stroke* s)
{
    Point* prev = NULL;

    for (Point* p = s->head; p != NULL; p = p->next)
    {
        if (prev)
        {
            DrawLine(prev->x, prev->y, p->x, p->y, s->color);
        }
        prev = p;
    }
}


// Free memory
void FreeStrokes(StrokeList* list)
{
    Stroke* s = list->head;
    while (s)
    {
        Point* p = s->head;
        while (p)
        {
            Point* temp = p;
            p = p->next;
            free(temp);
        }

        Stroke* tempS = s;
        s = s->next;
        free(tempS);
    }
}
