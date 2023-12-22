#pragma once

#include "sudoku.h"

typedef struct HiddenPairs_Imple{         // Array of pointers to cells
    Cell *p_cell[2];       // 2 pairs
    int value[2];          // values
} HiddenPairs;

void is_the_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs, Cell * p_cell_1, Cell * p_cell_2, int * p_counter);

void find_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs, int *p_counter, int *p_arr);

void unset_hidden_pairs(HiddenPairs p_hidden_pairs);

int hidden_pairs(SudokuBoard *p_board);
