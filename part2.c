#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_HEIGHT 100
#define MAX_WIDTH 100

// Global variables
char maze[MAX_HEIGHT][MAX_WIDTH];
int mazeHeight = 0, mazeWidth = 0;
bool visited[MAX_HEIGHT][MAX_WIDTH];
int visited_x[MAX_HEIGHT * MAX_WIDTH];
int visited_y[MAX_HEIGHT * MAX_WIDTH];
int visited_count = 0;

// Struct for position
typedef struct {
    int x, y;
} Position;

// Stack for DFS
typedef struct {
    Position *data;
    int top;
    int capacity;
} Stack;

// Function declarations
Stack *initializeStack(int capacity);
void push(Stack *stack, Position pos);
Position pop(Stack *stack);
bool isEmpty(Stack *stack);
void freeStack(Stack *stack);
void displayMaze();
void recordPosition(int x, int y);
bool isValidMove(int x, int y);
void loadMaze(const char *filename);

int main() {
    // Load maze from file
    loadMaze("maze.txt");

    // Starting and ending positions
    Position start = {1, 0};
    Position end = {7, 6};

    // Initialize visited array
    for (int i = 0; i < mazeHeight; i++) {
        for (int j = 0; j < mazeWidth; j++) {
            visited[i][j] = false;
        }
    }

    // Initialize stack for DFS
    Stack *stack = initializeStack(mazeHeight * mazeWidth);
    push(stack, start);
    visited[start.x][start.y] = true;
    recordPosition(start.x, start.y);

    while (!isEmpty(stack)) {
        Position current = pop(stack);

        // Mark current position as visited
        maze[current.x][current.y] = '+';

        // Check if end is reached
        if (current.x == end.x && current.y == end.y) {
            break;
        }

        // Explore neighbors in the order: Down, Right, Up, Left
        Position neighbors[] = {
            {current.x + 1, current.y},
            {current.x, current.y + 1},
            {current.x - 1, current.y},
            {current.x, current.y - 1}
        };

        for (int i = 0; i < 4; i++) {
            int nx = neighbors[i].x;
            int ny = neighbors[i].y;

            if (isValidMove(nx, ny) && !visited[nx][ny]) {
                push(stack, (Position){nx, ny});
                visited[nx][ny] = true;
                recordPosition(nx, ny);

                // Stop exploring once the first valid neighbor is found
                break;
            }
        }
    }

    printf("Visited Path:\n");
    for (int i = 0; i < visited_count; i++) {
        printf("[%d, %d] ", visited_x[i], visited_y[i]);
    }

    printf("\n\nFinal Maze:\n");
    displayMaze();

    freeStack(stack);
    return 0;
}

// Function to initialize a stack
Stack *initializeStack(int capacity) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->data = (Position *)malloc(sizeof(Position) * capacity);
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

void push(Stack *stack, Position pos) {
    if (stack->top < stack->capacity - 1) {
        stack->data[++stack->top] = pos;
    }
}

Position pop(Stack *stack) {
    if (stack->top >= 0) {
        return stack->data[stack->top--];
    }
    return (Position){-1, -1};  // Invalid position if stack is empty
}

bool isEmpty(Stack *stack) {
    return stack->top == -1;
}

void freeStack(Stack *stack) {
    free(stack->data);
    free(stack);
}

void displayMaze() {
    for (int i = 0; i < mazeHeight; i++) {
        for (int j = 0; j < mazeWidth; j++) {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
}

void recordPosition(int x, int y) {
    visited_x[visited_count] = x;
    visited_y[visited_count] = y;
    visited_count++;
}

bool isValidMove(int x, int y) {
    return x >= 0 && x < mazeHeight && y >= 0 && y < mazeWidth && maze[x][y] == ' ';
}

void loadMaze(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open maze file %s\n", filename);
        exit(1);
    }

    mazeHeight = 0;
    mazeWidth = 0;

    while (fgets(maze[mazeHeight], MAX_WIDTH, file)) {
        int rowLength = 0;
        while (maze[mazeHeight][rowLength] != '\0' && maze[mazeHeight][rowLength] != '\n') {
            rowLength++;
        }
        mazeWidth = rowLength > mazeWidth ? rowLength : mazeWidth;
        maze[mazeHeight][rowLength] = '\0'; // Null-terminate the row
        mazeHeight++;
    }

    fclose(file);
}
