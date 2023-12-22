#pragma once

#include "sudoku.h"

typedef struct NakedPairs_imple
{
    Cell **p_cells;
    Cell *p_cell;
} NakedPairs;
bool is_the_same_pairs(Cell * p_cell_1, Cell * p_cell_2);

// find naked_pairs cells in a row, in a collumn or in a box
void find_naked_pairs(Cell **p_cells, NakedPairs *p_naked_pairs, int *p_counter, int *p_arr);

void unset_naked_pairs(Cell **p_cells, NakedPairs *p_naked_pairs, int * order, int * p_arr);

// find naked_pairs cells in a board, return the number of hidden single cells
int naked_pairs(SudokuBoard *p_board);