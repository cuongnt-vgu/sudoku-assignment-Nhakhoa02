#include "hidden_pairs.h"
#include <stdlib.h>
#include <stdio.h>

void is_the_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs, Cell *p_cell_1, Cell *p_cell_2, int *p_counter){
    int value_count[BOARD_SIZE];
    int num_count = 0; //number of value appear 2 times;

    for (int i = 0; i < BOARD_SIZE; ++i) value_count[i] = 0;

    int * candidates_1 = get_candidates(p_cell_1);
    int * candidates_2 = get_candidates(p_cell_2);

    for (int i = 0; i < p_cell_1 -> num_candidates; ++i){
        for (int j = 0; j < p_cell_2 -> num_candidates; ++j){
            if (candidates_1[i] == candidates_2[j]) {
                
                bool is_unique = true;
                //loop through all cells to check if it unique on 2 cell
                for (int k = 0; k < BOARD_SIZE; ++k) {
                    if ((p_cells[k] != p_cell_1)
                    && (p_cells[k] != p_cell_2)) {

                        int * candidates_3 = get_candidates(p_cells[k]);
                        for (int x = 0; x < p_cells[k] -> num_candidates; ++x){
                            if (candidates_3[x] == candidates_2[j]) {
                                is_unique = false;
                                break;
                            }
                        }
                        free(candidates_3);
                    }
                }

                if (is_unique){
                    //printf("unique: %d \n", candidates_1[i]);
                    p_hidden_pairs[*p_counter].value[num_count] = candidates_1[i];
                    ++num_count;
                }
            }
        }
    }

    free(candidates_1);
    free(candidates_2);

    if (num_count == 2){
        p_hidden_pairs[*p_counter].p_cell[0] = p_cell_1;
        p_hidden_pairs[*p_counter].p_cell[1] = p_cell_2;
        ++(*p_counter);
    }
}

void find_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs, int *p_counter, int *p_arr){
    for (int i = 0; i < BOARD_SIZE; ++i){
        if (p_cells[i] -> num_candidates > 2) {
            for (int j = i + 1; j < BOARD_SIZE; ++j) {
                if (p_cells[j] -> num_candidates > 2) {
                    is_the_hidden_pairs(p_cells, p_hidden_pairs, p_cells[i], p_cells[j], p_counter);
                }
            }
        }
    }
}

void unset_hidden_pairs(HiddenPairs p_hidden_pairs){
    for (int i = 0; i < 2; ++i) {
        int *candidates = get_candidates(p_hidden_pairs.p_cell[i]);
        int num_candidates = p_hidden_pairs.p_cell[i]->num_candidates;


        for (int j = 0; j < num_candidates; ++j){
            bool is_value = false;
            //printf("candidate can access: %d \n", candidates[j]);
            for (int k = 0; k < 2; ++k) {
                if (candidates[j] == p_hidden_pairs.value[k]) {
                    is_value = true;
                    break;
                }
            }

            if (!is_value) {
                printf("candidate: %d \n", candidates[j]);
                unset_candidate(p_hidden_pairs.p_cell[i], candidates[j]);
            }
        }

        // for (int j = 0; j < p_hidden_pairs.p_cell[i]->num_candidates; ++j){
        //     printf("%d ", candidates[j]);
        // }

        free(candidates);
    }
}


int hidden_pairs(SudokuBoard *p_board)
{
    int res = 0;
    int num_arr = 0 ;

    HiddenPairs hidden_pairs_arr[BOARD_SIZE * BOARD_SIZE];
    
    for (int i = 0 ; i < BOARD_SIZE; ++i){
        find_hidden_pairs(p_board -> p_cols[i], hidden_pairs_arr, &res, &num_arr);
        find_hidden_pairs(p_board -> p_rows[i], hidden_pairs_arr, &res, &num_arr);
        find_hidden_pairs(p_board -> p_boxes[i], hidden_pairs_arr, &res, &num_arr);
    }

    for (int i = 0; i < res; ++i){
        unset_hidden_pairs(hidden_pairs_arr[i]);
    }
    
    //printf("candidate: %d ",hidden_pairs_arr[1].value[1]);
    return res;
}
