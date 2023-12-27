#pragma once

#include "sudoku.h"

typedef struct HiddenTriples_Imple{         // Array of pointers to cells
    Cell *p_cell[3];       // 2 pairs
    int value[9];          // values
} HiddenTriples;

int is_hidden_triples(Cell *p_cell, int * value_count, int * hidden_arr);

bool is_duplicate_hidden_triples(Cell *p_cell_1, Cell *p_cell_2, Cell *p_cell_3, HiddenTriples *p_hidden_triples, int *p_counter);

void is_the_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples, Cell * p_cell_1, Cell * p_cell_2, Cell * p_cell_3, int * p_counter);

void find_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples, int *p_counter);

void unset_hidden_triples(HiddenTriples p_hidden_triples);

int hidden_triples(SudokuBoard *p_board);