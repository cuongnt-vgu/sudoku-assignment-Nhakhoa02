#pragma once

#include "sudoku.h"

typedef struct NakedTriples_imple
{
    Cell **p_cells;
    Cell *p_cell[3];
} NakedTriples;

bool is_the_same_triples(Cell * p_cell_1, Cell * p_cell_2, Cell *p_cell_3);

// find hidden single cells in a row, in a collumn or in a box
void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter, int *p_arr);

void unset_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int * order, int * p_arr);

int naked_triples(SudokuBoard *p_board);