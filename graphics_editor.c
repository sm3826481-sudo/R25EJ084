#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 60
#define HEIGHT 20
#define MAX_SHAPES 100

typedef enum { LINE, RECTANGLE, CIRCLE, TRIANGLE } ShapeType;

typedef struct {
    int id;
    int is_active;
    ShapeType type;
    int x1, y1, x2, y2, x3, y3; 
    int radius;                 
} Shape;

char canvas[HEIGHT][WIDTH];
Shape shapes[MAX_SHAPES];
int current_id = 1;

void clear_canvas() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) canvas[i][j] = ' ';
    }
}

void display_canvas() {
    printf("\n--- Canvas ---\n");
    for (int i = 0; i < WIDTH + 2; i++) printf("-");
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        printf("|");
        for (int j = 0; j < WIDTH; j++) printf("%c", canvas[i][j]);
        printf("|\n");
    }
    for (int i = 0; i < WIDTH + 2; i++) printf("-");
    printf("\n");
}

void draw_point(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) canvas[y][x] = '*';
}

void draw_line(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2; 
    while (1) {
        draw_point(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void draw_rectangle(int x1, int y1, int x2, int y2) {
    draw_line(x1, y1, x2, y1); draw_line(x1, y2, x2, y2);
    draw_line(x1, y1, x1, y2); draw_line(x2, y1, x2, y2);
}

void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(x1, y1, x2, y2); draw_line(x2, y2, x3, y3); draw_line(x3, y3, x1, y1);
}

void draw_circle(int xc, int yc, int r) {
    int x = 0, y = r, d = 3 - 2 * r;
    while (y >= x) {
        draw_point(xc+x, yc+y); draw_point(xc-x, yc+y);
        draw_point(xc+x, yc-y); draw_point(xc-x, yc-y);
        draw_point(xc+y, yc+x); draw_point(xc-y, yc+x);
        draw_point(xc+y, yc-x); draw_point(xc-y, yc-x);
        x++;
        if (d > 0) { y--; d = d + 4 * (x - y) + 10; } 
        else { d = d + 4 * x + 6; }
    }
}

void render_all_shapes() {
    clear_canvas();
    for (int i = 0; i < MAX_SHAPES; i++) {
        if (shapes[i].is_active) {
            switch (shapes[i].type) {
                case LINE: draw_line(shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2); break;
                case RECTANGLE: draw_rectangle(shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2); break;
                case TRIANGLE: draw_triangle(shapes[i].x1, shapes[i].y1, shapes[i].x2, shapes[i].y2, shapes[i].x3, shapes[i].y3); break;
                case CIRCLE: draw_circle(shapes[i].x1, shapes[i].y1, shapes[i].radius); break;
            }
        }
    }
}

void add_shape() {
    int choice, idx = -1;
    printf("\nSelect Shape: 1.Line 2.Rectangle 3.Circle 4.Triangle: ");
    scanf("%d", &choice);
    
    for (int i = 0; i < MAX_SHAPES; i++) { if (!shapes[i].is_active) { idx = i; break; } }
    if (idx == -1) { printf("Canvas full!\n"); return; }
    
    shapes[idx].id = current_id++; shapes[idx].is_active = 1;
    if (choice == 1) { shapes[idx].type = LINE; printf("Enter x1 y1 x2 y2: "); scanf("%d %d %d %d", &shapes[idx].x1, &shapes[idx].y1, &shapes[idx].x2, &shapes[idx].y2); }
    else if (choice == 2) { shapes[idx].type = RECTANGLE; printf("Enter top-left x,y and bottom-right x,y: "); scanf("%d %d %d %d", &shapes[idx].x1, &shapes[idx].y1, &shapes[idx].x2, &shapes[idx].y2); }
    else if (choice == 3) { shapes[idx].type = CIRCLE; printf("Enter center x,y and radius: "); scanf("%d %d %d", &shapes[idx].x1, &shapes[idx].y1, &shapes[idx].radius); }
    else if (choice == 4) { shapes[idx].type = TRIANGLE; printf("Enter x1 y1 x2 y2 x3 y3: "); scanf("%d %d %d %d %d %d", &shapes[idx].x1, &shapes[idx].y1, &shapes[idx].x2, &shapes[idx].y2, &shapes[idx].x3, &shapes[idx].y3); }
    
    render_all_shapes();
    printf("Shape added with ID: %d\n", shapes[idx].id);
}

void delete_shape() {
    int id; printf("Enter Shape ID to delete: "); scanf("%d", &id);
    for (int i = 0; i < MAX_SHAPES; i++) {
        if (shapes[i].is_active && shapes[i].id == id) {
            shapes[i].is_active = 0; render_all_shapes();
            printf("Shape %d deleted.\n", id); return;
        }
    }
    printf("ID not found.\n");
}

void modify_shape() {
    int id; printf("Enter Shape ID to modify: "); scanf("%d", &id);
    for (int i = 0; i < MAX_SHAPES; i++) {
        if (shapes[i].is_active && shapes[i].id == id) {
            if (shapes[i].type == LINE) { printf("New x1 y1 x2 y2: "); scanf("%d %d %d %d", &shapes[i].x1, &shapes[i].y1, &shapes[i].x2, &shapes[i].y2); }
            else if (shapes[i].type == RECTANGLE) { printf("New top-left & bottom-right: "); scanf("%d %d %d %d", &shapes[i].x1, &shapes[i].y1, &shapes[i].x2, &shapes[i].y2); }
            else if (shapes[i].type == CIRCLE) { printf("New center and radius: "); scanf("%d %d %d", &shapes[i].x1, &shapes[i].y1, &shapes[i].radius); }
            else if (shapes[i].type == TRIANGLE) { printf("New x1 y1 x2 y2 x3 y3: "); scanf("%d %d %d %d %d %d", &shapes[i].x1, &shapes[i].y1, &shapes[i].x2, &shapes[i].y2, &shapes[i].x3, &shapes[i].y3); }
            render_all_shapes(); printf("Shape modified.\n"); return;
        }
    }
    printf("ID not found.\n");
}

int main() {
    for (int i = 0; i < MAX_SHAPES; i++) shapes[i].is_active = 0;
    clear_canvas();
    int choice;
    do {
        printf("\n=== 2D Graphics Editor ===\n");
        printf("1. Add Object\n2. Delete Object\n3. Modify Object\n4. Display Picture\n5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: add_shape(); break;
            case 2: delete_shape(); break;
            case 3: modify_shape(); break;
            case 4: display_canvas(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
    return 0;
}