#ifndef STROKE_H
#define STROKE_H

#include "raylib.h"
#include "stack.h"


// A single point
typedef struct Point {
    int x, y;
    struct Point* next;
} Point;

// One stroke (linked list of points)
typedef struct Stroke {
    Point* head;
    Point* tail;
    Color color;
    int size;

    struct Stroke* next;
} Stroke;

// A list of strokes (your drawing)
typedef struct {
    Stroke* head;
    Stroke* tail;
} StrokeList;

// Functions
Stroke* CreateStroke(Color color, int size);
void AddPointToStroke(Stroke* stroke, int x, int y);
void AddStroke(StrokeList* list, Stroke* stroke);
void DrawStrokes(StrokeList* list);
void DrawStrokesFromStack(Stack* stack);
void DrawSingleStroke(Stroke* s);
void FreeStrokes(StrokeList* list);
#endif
