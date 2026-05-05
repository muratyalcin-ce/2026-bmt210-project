#include "stroke.h"

#include <stdlib.h>
#include <math.h>
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
    Point* prev = s->head;

    for (Point* p = s->head; p != NULL; p = p->next)
    {
        if (prev)
        {
			Vector2 start = {(float)prev->x, (float)prev->y};
			Vector2 end = {(float)p->x, (float)p->y};
			DrawCircleV(start, (float)s->size/2.0f, s->color);
			DrawCircleV(end, (float)s->size/2.0f, s->color);
			DrawLineEx(end, start, (float)s->size, s->color);
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

