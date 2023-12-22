#include "naked_triples.h"
#include <stdlib.h>
#include <stdio.h>

bool is_the_same_triples(Cell * p_cell_1, Cell * p_cell_2, Cell *p_cell_3){
    int num_diff = 0;

    int num_counter[BOARD_SIZE];
    for (int i = 0 ; i < BOARD_SIZE; ++i) num_counter[i] = 0;

    int *candidates_1 = get_candidates(p_cell_1);
    int *candidates_2 = get_candidates(p_cell_2);
    int *candidates_3 = get_candidates(p_cell_3);

    for (int i = 0 ; i < p_cell_1->num_candidates; ++i)
    {
        //printf("%d ", candidates_1[i-1]);
        if (num_counter[candidates_1[i] - 1] == 0) ++num_diff;
        ++num_counter[candidates_1[i] - 1];
    }
    // printf("\n");
    for (int i = 0 ; i < p_cell_2->num_candidates; ++i)
    {
        // printf("%d ", candidates_2[i]);
        if (num_counter[candidates_2[i] - 1] == 0) ++num_diff;
        ++num_counter[candidates_2[i] - 1];
    }
    // printf("\n");
    for (int i = 0 ; i < p_cell_3->num_candidates; ++i)
    {
        // printf("%d ", candidates_3[i]);
        if (num_counter[candidates_3[i] - 1] == 0) ++num_diff;
        ++num_counter[candidates_3[i] - 1];
    }
    // printf("\n ");

    free(candidates_1);
    free(candidates_2);
    free(candidates_3);

    if (num_diff < 4) return true;
    else return false;
}

bool is_duplicate_triples(Cell *p_cell_1, Cell *p_cell_2, Cell *p_cell_3, NakedTriples *p_naked_triples, int *p_arr)
{
    for (int i = 0; i < *p_arr; ++i)
    {
        Cell *existing_cell_1 = p_naked_triples[i].p_cell[0];
        Cell *existing_cell_2 = p_naked_triples[i].p_cell[1];
        Cell *existing_cell_3 = p_naked_triples[i].p_cell[2];

        // Check if the cells are the same (order-independent)
        bool same_cells =
            (p_cell_1 == existing_cell_1 || p_cell_1 == existing_cell_2 || p_cell_1 == existing_cell_3) &&
            (p_cell_2 == existing_cell_1 || p_cell_2 == existing_cell_2 || p_cell_2 == existing_cell_3) &&
            (p_cell_3 == existing_cell_1 || p_cell_3 == existing_cell_2 || p_cell_3 == existing_cell_3);

        if (same_cells)
        {
            // The triple is a duplicate
            return true;
        }
    }

    // No duplicate found
    return false;
}

// find hidden single cells in a row, in a collumn or in a box
void find_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int *p_counter, int *p_arr){

    for (int i = 0; i < BOARD_SIZE; ++i){
        if (p_cells[i]->num_candidates == 2 || p_cells[i]->num_candidates == 3)
            for (int j = i+1; j < BOARD_SIZE; ++j){
                if (p_cells[j]->num_candidates == 2 || p_cells[j]->num_candidates == 3)
                    for (int k = j + 1; k < BOARD_SIZE; ++k){
                        if (p_cells[k]->num_candidates == 2 || p_cells[k]->num_candidates ==3)
                            if (is_the_same_triples(p_cells[i], p_cells[j], p_cells[k])){
                                
                                if (!is_duplicate_triples(p_cells[i], p_cells[j], p_cells[k], p_naked_triples, p_arr)){
                                    ++(*p_counter);
                                }
                                p_naked_triples[*p_arr].p_cells = p_cells;
                                p_naked_triples[*p_arr].p_cell[0] = p_cells[i];
                                p_naked_triples[*p_arr].p_cell[1] = p_cells[j];
                                p_naked_triples[*p_arr].p_cell[2] = p_cells[k];
                                ++(*p_arr);
                            }
                    }
            }
    }
}

void unset_naked_triples(Cell **p_cells, NakedTriples *p_naked_triples, int * order, int * p_arr){

    for (int i = 0; i < BOARD_SIZE; ++i){
        bool is_in = false;
        if (p_cells[i] == p_naked_triples[*order].p_cell[0]) is_in = true;
        if (p_cells[i] == p_naked_triples[*order].p_cell[1]) is_in = true;
        if (p_cells[i] == p_naked_triples[*order].p_cell[2]) is_in = true;

        if (!is_in){
            
            for (int j = 0; j < 3; ++j) {
                int *candidates_cells = get_candidates(p_cells[i]);
                int *candidates_triples = get_candidates(p_naked_triples[*order].p_cell[j]);

                for (int k = 0; k < p_cells[i]->num_candidates; ++k) {
                    for (int l = 0; l < p_naked_triples[*order].p_cell[j]->num_candidates; ++l) {
                        if (candidates_cells[k] == candidates_triples[l]) {
                            unset_candidate(p_cells[i], candidates_cells[k]);
                        }
                    }
                }

                free(candidates_cells);
                free(candidates_triples);
            }
        }
    }

    return;
}

int naked_triples(SudokuBoard *p_board)
{
    int res = 0; 
    int num_arr = 0;

    NakedTriples naked_arr[BOARD_SIZE * BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; ++i){
        find_naked_triples(p_board -> p_cols[i], naked_arr, &res, &num_arr);
        find_naked_triples(p_board -> p_rows[i], naked_arr, &res, &num_arr);
        find_naked_triples(p_board -> p_boxes[i], naked_arr, &res, &num_arr);
    }


    for (int i = 0 ; i < num_arr; ++i){
        unset_naked_triples(naked_arr[i].p_cells, naked_arr,&i, &num_arr);
    }

    return res;
}