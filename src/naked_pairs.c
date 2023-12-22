#include "naked_pairs.h"
#include <stdlib.h>
#include <stdio.h>

// find naked pairs values (the value that is unique among all the candidates for a row, a collumn, or a box)
bool is_the_same_pairs(Cell * p_cell_1, Cell * p_cell_2){
    int * candidates_1 = get_candidates(p_cell_1);
    int * candidates_2 = get_candidates(p_cell_2);

    for (int i = 0; i < 2; ++i){
        if (candidates_1[i] != candidates_2[i]) {
            free(candidates_1);
            free(candidates_2);
            return false;
        }
    }

    free(candidates_1);
    free(candidates_2);

    return true;
}

// find naked pairs cells in a row, in a collumn or in a box
void find_naked_pairs(Cell **p_cells, NakedPairs *p_naked_pairs, int *p_counter, int *p_arr)
{
    for (int i = 0; i < BOARD_SIZE; ++i){
        if (p_cells[i] -> num_candidates == 2) {
            for (int j = i+1 ; j < BOARD_SIZE; ++j){
                if ((p_cells[j] -> num_candidates == 2)
                && (is_the_same_pairs(p_cells[i], p_cells[j])))
                {
                    bool is_Duplicate = false;

                    for (int k = 0; k < *p_arr; ++k){
                        if 
                        (((p_cells[i] == p_naked_pairs[2*k].p_cell)
                        && (p_cells[j] == p_naked_pairs[2*k +1].p_cell))

                        || ((p_cells[j] == p_naked_pairs[2*k].p_cell)
                        && (p_cells[i] == p_naked_pairs[2*k +1].p_cell)))
                            is_Duplicate = true;
                    }

                    if (!is_Duplicate) ++(*p_counter);

                    p_naked_pairs[2* (*p_arr)].p_cells =p_cells;
                    p_naked_pairs[2* (*p_arr)].p_cell =p_cells[i];
                    p_naked_pairs[2* (*p_arr) + 1].p_cells =p_cells;
                    p_naked_pairs[2* (*p_arr) + 1].p_cell =p_cells[j];
                    ++(*p_arr);
                }

            }
        }
    }
        //printf("%d \n", p_cells[i] -> num_candidates);
}

void unset_naked_pairs(Cell **p_cells, NakedPairs *p_naked_pairs, int * order, int * p_arr){
    for (int i = 0; i < BOARD_SIZE; ++i){
        bool is_in = false;
        if (p_cells[i] == p_naked_pairs[2* (*order)].p_cell) is_in = true; 
        if (p_cells[i] == p_naked_pairs[2* (*order) + 1].p_cell) is_in = true;

        if (!is_in){
            int * candidates_2 = get_candidates(p_naked_pairs[2*(*order)].p_cell);
            int * candidates_1 = get_candidates(p_cells[i]);

            // for (int j = 0; j < p_naked_pairs[2*(*order)].p_cell->num_candidates; ++j){
            //     printf("%d ", candidates_2[j]);
            // }
    
            for (int j = 0; j < p_cells[i]->num_candidates; ++j){
                for (int k = 0 ; k < p_naked_pairs[2*(*order)].p_cell->num_candidates; ++k){
                    if (candidates_1[j] == candidates_2[k]){
                        //printf("candidate: %d \n", candidates_1[j]);
                        unset_candidate(p_cells[i], candidates_1[j]);
                    }
                }
            }
            free(candidates_1);
            free(candidates_2);
        }
    }
}

// find naked pairs cells in a board, return the number of hidden single cells
int naked_pairs(SudokuBoard *p_board)
{
    int res = 0; // number of naked_pairs;
    int num_arr = 0; //number of cells to check;

    NakedPairs naked_arr[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; ++i){
        find_naked_pairs(p_board -> p_cols[i], naked_arr, &res, &num_arr);
        find_naked_pairs(p_board -> p_rows[i], naked_arr, &res, &num_arr);
        find_naked_pairs(p_board -> p_boxes[i], naked_arr, &res, &num_arr);
    }
    //printf("check: %d", p_board->p_cols[7][8]->num_candidates);

    // for (int i = 0 ; i < res; ++i){
    //     printf("naked arr: %d \n", naked_arr[2*i].p_cell->num_candidates);
    // }
    // unset_candidate(p_board->p_rows[2][7], 2);

    for (int i = 0 ; i < num_arr; ++i){
        unset_naked_pairs(naked_arr[2*i].p_cells, naked_arr,&i, &num_arr);
        // int * candidates = get_candidates(naked_arr[2*i].p_cell);
        // for (int j = 0; j < naked_arr[2*i].p_cell->num_candidates; ++j){
        //     printf("%d ", candidates[j]);
        // }
        // printf("\n");
        // free(candidates);
    }

    return res;
}