#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 60
#define HEIGHT 25
#define MAX_SHAPES 50

typedef enum { LINE, RECTANGLE, TRIANGLE, CIRCLE } ShapeType;

typedef struct { int x; int y; } Point;

typedef struct {
    int id; ShapeType type; Point p1; Point p2; Point p3;
    int radius; char brush; int isActive;
} Shape;

char canvas[HEIGHT][WIDTH];
Shape database[MAX_SHAPES];
int shapeCount = 0;
int nextId = 1;

void clearCanvas() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) canvas[i][j] = ' ';
}

void initShapes() {
    for (int i = 0; i < MAX_SHAPES; i++) database[i].isActive = 0;
}

void displayCanvas() {
    printf("+"); for (int j = 0; j < WIDTH; j++) printf("-"); printf("+\n");
    for (int i = 0; i < HEIGHT; i++) {
        printf("|"); for (int j = 0; j < WIDTH; j++) printf("%c", canvas[i][j]); printf("|\n");
    }
    printf("+"); for (int j = 0; j < WIDTH; j++) printf("-"); printf("+\n");
}

// Core Math Algorithms for Drawing on Matrix Grid
void drawLineOnCanvas(int x0, int y0, int x1, int y1, char brush) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        if (x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT) {
            canvas[y0][x0] = brush;
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void drawRectOnCanvas(int x1, int y1, int x2, int y2, char brush) {
    drawLineOnCanvas(x1, y1, x2, y1, brush); // Top side
    drawLineOnCanvas(x2, y1, x2, y2, brush); // Right side
    drawLineOnCanvas(x2, y2, x1, y2, brush); // Bottom side
    drawLineOnCanvas(x1, y2, x1, y1, brush); // Left side
}

void drawTriangleOnCanvas(int x1, int y1, int x2, int y2, int x3, int y3, char brush) {
    drawLineOnCanvas(x1, y1, x2, y2, brush);
    drawLineOnCanvas(x2, y2, x3, y3, brush);
    drawLineOnCanvas(x3, y3, x1, y1, brush);
}

void drawCircleOnCanvas(int cx, int cy, int r, char brush) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    
    while (y >= x) {
        int px[] = {cx+x, cx+y, cx-x, cx-y, cx-x, cx-y, cx+x, cx+y};
        int py[] = {cy+y, cy+x, cy+y, cy+x, cy-y, cy-x, cy-y, cy-x};
        for (int i = 0; i < 8; i++) {
            if (px[i] >= 0 && px[i] < WIDTH && py[i] >= 0 && py[i] < HEIGHT) {
                canvas[py[i]][px[i]] = brush;
            }
        }
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

void rasterizeShapes() {
    clearCanvas();
    for (int i = 0; i < MAX_SHAPES; i++) {
        if (database[i].isActive) {
            switch (database[i].type) {
                case LINE:
                    drawLineOnCanvas(database[i].p1.x, database[i].p1.y, database[i].p2.x, database[i].p2.y, database[i].brush);
                    break;
                case RECTANGLE:
                    drawRectOnCanvas(database[i].p1.x, database[i].p1.y, database[i].p2.x, database[i].p2.y, database[i].brush);
                    break;
                case TRIANGLE:
                    drawTriangleOnCanvas(database[i].p1.x, database[i].p1.y, database[i].p2.x, database[i].p2.y, database[i].p3.x, database[i].p3.y, database[i].brush);
                    break;
                case CIRCLE:
                    drawCircleOnCanvas(database[i].p1.x, database[i].p1.y, database[i].radius, database[i].brush);
                    break;
            }
        }
    }
}