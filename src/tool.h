#ifndef TOOL_H
#define TOOL_H

#include "raylib.h"

typedef enum {
    TOOL_BRUSH,
    TOOL_ERASER
} ToolType;

typedef struct {
    ToolType type;
    Color color;
    int size;
} Tool;

#endif
