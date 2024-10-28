#ifndef GRID_H
#define GRID_H

typedef struct {
    char **characters;
    int rows;
    int cols;
} Grid;

void create_grid(Grid *grid, int rows, int cols);
void fill_grid(Grid *grid);
void free_grid(Grid *grid);

#endif // GRID_H

