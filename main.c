#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 60
#define HEIGHT 25
#define MAX_SHAPES 50

// Shape types enum
typedef enum { LINE, RECTANGLE, TRIANGLE, CIRCLE } ShapeType;

// Coordinate structure
typedef struct {
    int x;
    int y;
} Point;

// Core Shape structure holding metadata
typedef struct {
    int id;
    ShapeType type;
    Point p1; // Used for Line start, Rect top-left, Tri v1, Circle center
    Point p2; // Used for Line end, Rect bottom-right, Tri v2
    Point p3; // Used for Tri v3
    int radius; // Used for Circle only
    char brush; // Custom symbol like '*' or '_'
    int isActive;
} Shape;

// Global canvas matrix and shape database
char canvas[HEIGHT][WIDTH];
Shape database[MAX_SHAPES];
int shapeCount = 0;
int nextId = 1;

// Function declarations
void clearCanvas();
void displayCanvas();
void initShapes();
void rasterizeShapes();
void addShape();
void deleteShape();
void modifyShape();

void drawLineOnCanvas(int x0, int y0, int x1, int y1, char brush);
void drawRectOnCanvas(int x1, int y1, int x2, int y2, char brush);
void drawTriangleOnCanvas(int x1, int y1, int x2, int y2, int x3, int y3, char brush);
void drawCircleOnCanvas(int cx, int cy, int r, char brush);

void clearCanvas() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            canvas[i][j] = ' '; // Empty background space
        }
    }
}

void initShapes() {
    for (int i = 0; i < MAX_SHAPES; i++) {
        database[i].isActive = 0;
    }
}

void displayCanvas() {
    // Top Border
    printf("+");
    for (int j = 0; j < WIDTH; j++) printf("-");
    printf("+\n");

    // Matrix content
    for (int i = 0; i < HEIGHT; i++) {
        printf("|");
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("|\n");
    }

    // Bottom Border
    printf("+");
    for (int j = 0; j < WIDTH; j++) printf("-");
    printf("+\n");
}