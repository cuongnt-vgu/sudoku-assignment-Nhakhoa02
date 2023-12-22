#pragma once

#include "sudoku.h"

typedef struct NakedPairs_imple
{
    Cell **p_cells;
    Cell *p_cell;
} NakedPairs;
bool is_the_same(Cell * p_cell_1, Cell * p_cell_2);

// find hidden single values (the value that is unique among all the candidates for a row, a collumn, or a box)
int find_naked_pairs_values(Cell **p_cells, int *NakedPairs_values);

// find hidden single cells in a row, in a collumn or in a box
void find_naked_pairs(Cell **p_cells, NakedPairs *p_hidden_singles, int *p_counter, int *p_arr);

void unset_naked_pairs(Cell **p_cells, NakedPairs *p_naked_pairs, int * order, int * p_arr);

// find hidden single cells in a board, return the number of hidden single cells
int naked_pairs(SudokuBoard *p_board);