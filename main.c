#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 60
#define HEIGHT 25
#define MAX_SHAPES 50

typedef enum { LINE, RECTANGLE, TRIANGLE, CIRCLE } ShapeType;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int id;
    ShapeType type;
    Point p1;
    Point p2;
    Point p3;
    int radius;
    char brush;
    int isActive;
} Shape;

char canvas[HEIGHT][WIDTH];
Shape database[MAX_SHAPES];
int nextId = 1;

void clearCanvas() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            canvas[i][j] = ' ';
        }
    }
}

void initShapes() {
    for (int i = 0; i < MAX_SHAPES; i++) {
        database[i].isActive = 0;
    }
}

void displayCanvas() {
    printf("\n");
    printf("+");
    for (int j = 0; j < WIDTH; j++) printf("-");
    printf("+\n");

    for (int i = 0; i < HEIGHT; i++) {
        printf("|");
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("|\n");
    }

    printf("+");
    for (int j = 0; j < WIDTH; j++) printf("-");
    printf("+\n");
}

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
    drawLineOnCanvas(x1, y1, x2, y1, brush);
    drawLineOnCanvas(x2, y1, x2, y2, brush);
    drawLineOnCanvas(x2, y2, x1, y2, brush);
    drawLineOnCanvas(x1, y2, x1, y1, brush);
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

void listShapes() {
    printf("\n--- Active Objects in Vector List ---\n");
    int empty = 1;
    for (int i = 0; i < MAX_SHAPES; i++) {
        if (database[i].isActive) {
            empty = 0;
            printf("ID [%d] - Type: ", database[i].id);
            if (database[i].type == LINE) printf("Line | Point1: (%d,%d), Point2: (%d,%d)", database[i].p1.x, database[i].p1.y, database[i].p2.x, database[i].p2.y);
            else if (database[i].type == RECTANGLE) printf("Rectangle | Top-Left: (%d,%d), Bottom-Right: (%d,%d)", database[i].p1.x, database[i].p1.y, database[i].p2.x, database[i].p2.y);
            else if (database[i].type == TRIANGLE) printf("Triangle | V1: (%d,%d), V2: (%d,%d), V3: (%d,%d)", database[i].p1.x, database[i].p1.y, database[i].p2.x, database[i].p2.y, database[i].p3.x, database[i].p3.y);
            else if (database[i].type == CIRCLE) printf("Circle | Center: (%d,%d), Radius: %d", database[i].p1.x, database[i].p1.y, database[i].radius);
            printf(" | Brush Character: '%c'\n", database[i].brush);
        }
    }
    if (empty) {
        printf("No objects found in active database registry.\n");
    }
}

void addShape() {
    int choice;
    printf("\n--- Select Vector Structure to Add ---\n");
    printf("1. Line Segment\n2. Rectangle Frame\n3. Triangle Outlining\n4. Circle Boundary\nSelect choice: ");
    scanf("%d", &choice);

    int slot = -1;
    for (int i = 0; i < MAX_SHAPES; i++) {
        if (!database[i].isActive) {
            slot = i;
            break;
        }
    }

    if (slot == -1) {
        printf("Error: Object registry database is full!\n");
        return;
    }

    Shape newShape;
    newShape.id = nextId++;
    newShape.isActive = 1;
    
    printf("Enter drawing brush element (e.g., * or _): ");
    scanf(" %c", &newShape.brush);

    if (choice == 1) {
        newShape.type = LINE;
        printf("Enter Start Coordinates X (0-%d) and Y (0-%d): ", WIDTH-1, HEIGHT-1);
        scanf("%d %d", &newShape.p1.x, &newShape.p1.y);
        printf("Enter End Coordinates X (0-%d) and Y (0-%d): ", WIDTH-1, HEIGHT-1);
        scanf("%d %d", &newShape.p2.x, &newShape.p2.y);
    } else if (choice == 2) {
        newShape.type = RECTANGLE;
        printf("Enter Top-Left X and Y coords: ");
        scanf("%d %d", &newShape.p1.x, &newShape.p1.y);
        printf("Enter Bottom-Right X and Y coords: ");
        scanf("%d %d", &newShape.p2.x, &newShape.p2.y);
    } else if (choice == 3) {
        newShape.type = TRIANGLE;
        printf("Enter Vertex 1 (X Y): "); scanf("%d %d", &newShape.p1.x, &newShape.p1.y);
        printf("Enter Vertex 2 (X Y): "); scanf("%d %d", &newShape.p2.x, &newShape.p2.y);
        printf("Enter Vertex 3 (X Y): "); scanf("%d %d", &newShape.p3.x, &newShape.p3.y);
    } else if (choice == 4) {
        newShape.type = CIRCLE;
        printf("Enter Center X and Y coordinates: ");
        scanf("%d %d", &newShape.p1.x, &newShape.p1.y);
        printf("Enter Radius length: ");
        scanf("%d", &newShape.radius);
    } else {
        printf("Invalid option parsing selection aborted.\n");
        return;
    }

    database[slot] = newShape;
    printf("Object successfully written with assigned ID: %d\n", newShape.id);
}

void deleteShape() {
    listShapes();
    int targetId;
    printf("\nEnter target entity ID code to delete from structure: ");
    scanf("%d", &targetId);

    for (int i = 0; i < MAX_SHAPES; i++) {
        if (database[i].isActive && database[i].id == targetId) {
            database[i].isActive = 0;
            printf("Object ID [%d] cleared and removed from active vector pipeline.\n", targetId);
            return;
        }
    }
    printf("Object ID target value index not tracked.\n");
}

void modifyShape() {
    listShapes();
    int targetId;
    printf("\nEnter structural vector reference ID to modify: ");
    scanf("%d", &targetId);

    for (int i = 0; i < MAX_SHAPES; i++) {
        if (database[i].isActive && database[i].id == targetId) {
            printf("Updating fields for matching record entry [%d]:\n", targetId);
            printf("Provide alternate brush element character asset (e.g. * or _): ");
            scanf(" %c", &database[i].brush);
            
            if (database[i].type == LINE || database[i].type == RECTANGLE) {
                printf("Enter updated coordinates (P1_X P1_Y P2_X P2_Y): ");
                scanf("%d %d %d %d", &database[i].p1.x, &database[i].p1.y, &database[i].p2.x, &database[i].p2.y);
            } else if (database[i].type == TRIANGLE) {
                printf("Enter updated coordinates (V1_X V1_Y V2_X V2_Y V3_X V3_Y): ");
                scanf("%d %d %d %d %d %d", &database[i].p1.x, &database[i].p1.y, &database[i].p2.x, &database[i].p2.y, &database[i].p3.x, &database[i].p3.y);
            } else if (database[i].type == CIRCLE) {
                printf("Enter structural adjustment parameter values (Center_X Center_Y Radius): ");
                scanf("%d %d %d", &database[i].p1.x, &database[i].p1.y, &database[i].radius);
            }
            printf("Object adjustments modification successfully executed.\n");
            return;
        }
    }
    printf("Reference code ID not detected in runtime database array structural elements.\n");
}

int main() {
    initShapes();
    int menuOption = 0;

    printf("===========================================\n");
    printf("   Vector-Based 2D Character Engine GUI    \n");
    printf("===========================================\n");

    while (1) {
        rasterizeShapes();
        displayCanvas();

        printf("\n>>> WORKSPACE ENGINE CONTROL CONTROL CONSOLE <<<\n");
        printf("1. Allocate New Shape Entry\n");
        printf("2. Purge and Remove Active Shape Vector\n");
        printf("3. Mutate and Modify Existing Geometric Entry\n");
        printf("4. Print Raw List Registry Specifications\n");
        printf("5. Shut down engine processing pipeline\n");
        printf("Enter processing code selection choice: ");
        
        if (scanf("%d", &menuOption) != 1) {
            printf("Invalid programmatic choice. Aborting execution loops safely.\n");
            break;
        }

        if (menuOption == 1) addShape();
        else if (menuOption == 2) deleteShape();
        else if (menuOption == 3) modifyShape();
        else if (menuOption == 4) listShapes();
        else if (menuOption == 5) {
            printf("Dumping buffers. Shutting down cleanly.\n");
            break;
        } else {
            printf("Command entry not parsed index array mapping out of boundaries.\n");
        }
    }
    return 0;
}