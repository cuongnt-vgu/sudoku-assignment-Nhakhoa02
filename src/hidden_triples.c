#include "hidden_triples.h"
#include <stdlib.h>
#include <stdio.h>

void is_the_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples, Cell *p_cell_1, Cell *p_cell_2, Cell *p_cell_3, int *p_counter) {
    int value_count[BOARD_SIZE];
    int num_count = 0; // Number of values that appear 3 times
    int num_diff = 0; 

    for (int i = 0; i < BOARD_SIZE; ++i)
        value_count[i] = 0;

    int *candidates_1 = get_candidates(p_cell_1);
    int *candidates_2 = get_candidates(p_cell_2);
    int *candidates_3 = get_candidates(p_cell_3);

    for (int i = 0; i < BOARD_SIZE; ++i) { 
        int * candidates_4 = get_candidates(p_cells[i]);
        for (int j = 0 ; j < p_cells[i] -> num_candidates; ++j) {
            ++value_count[candidates_4[j] - 1];
        }
        free(candidates_4);
    }

    // for (int i =0; i < BOARD_SIZE; ++i)
    //     printf("%d ",value_count[i]);
    // printf("\n");
    
    bool is_triples = true;


    //first-----------------------------------------------------------------------
    for (int i = 0; i < p_cell_1 -> num_candidates; ++i){
        if (value_count[candidates_1[i] - 1] == 2 
        || value_count[candidates_1[i] - 1] == 3)
        {
            p_hidden_triples[*p_counter].value[num_diff] = candidates_1[i];
            ++num_diff;
            ++num_count;
            value_count[candidates_1[i] - 1] = 10;
        }
        else if (value_count[candidates_1[i] - 1] == 10) ++num_count;
    }

    if (num_count != 2 && num_count != 3) is_triples=false;
    num_count = 0;

    // printf("num_diff_1: %d \n", num_diff);

    //second-------------------------------------------------------------------------
    for (int i = 0; i < p_cell_2 -> num_candidates; ++i){
        if (value_count[candidates_2[i] - 1] == 2 
        || value_count[candidates_2[i] - 1] == 3)
        {
            p_hidden_triples[*p_counter].value[num_diff] = candidates_2[i];
            ++num_diff;
            ++num_count;
            value_count[candidates_2[i] - 1] = 10;
        }
        else if (value_count[candidates_2[i] - 1] == 10) ++num_count;
    }
    // printf("num_diff_2: %d \n", num_diff);

    if (num_count != 2 && num_count != 3) is_triples=false;
    num_count = 0;

    //third -----------------------------------------------------------------------
    for (int i = 0; i < p_cell_3 -> num_candidates; ++i){
        if (value_count[candidates_3[i] - 1] == 2 
        || value_count[candidates_3[i] - 1] == 3)
        {
            p_hidden_triples[*p_counter].value[num_diff] = candidates_3[i];
            ++num_diff;
            value_count[candidates_3[i] - 1] = 10;
        }
        else if (value_count[candidates_3[i] - 1] == 10) ++num_count;
    }
    // printf("num_diff_3: %d \n", num_diff);

    if (num_count != 2 && num_count != 3) is_triples=false;
    num_count = 0;

    free(candidates_1);
    free(candidates_2);
    free(candidates_3);
//----------------------------------------------------------------------------------
    if (num_diff < 4 && is_triples) {
        p_hidden_triples[*p_counter].p_cell[0] = p_cell_1;
        p_hidden_triples[*p_counter].p_cell[1] = p_cell_2;
        p_hidden_triples[*p_counter].p_cell[2] = p_cell_3;
        ++(*p_counter);
    }
}

void find_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples, int *p_counter) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (p_cells[i]->num_candidates >= 3) {
            for (int j = i + 1; j < BOARD_SIZE; ++j) {
                if (p_cells[j]->num_candidates >= 3) {
                    for (int k = j + 1; k < BOARD_SIZE; ++k) {
                        if (p_cells[k]->num_candidates >= 3) {
                            is_the_hidden_triples(p_cells, p_hidden_triples, p_cells[i], p_cells[j], p_cells[k], p_counter);
                        }
                    }
                }
            }
        }
    }

    //is_the_hidden_triples(p_cells, p_hidden_triples, p_cells[2], p_cells[6], p_cells[7], p_counter);
}

void unset_hidden_triples(HiddenTriples p_hidden_triples) {
    for (int i = 0; i < 3; ++i) {
        int *candidates = get_candidates(p_hidden_triples.p_cell[i]);
        int num_candidates = p_hidden_triples.p_cell[i]->num_candidates;

        for (int j = 0; j < num_candidates; ++j) {
            bool is_value = false;
            for (int k = 0; k < 3; ++k) {
                if (candidates[j] == p_hidden_triples.value[k]) {
                    is_value = true;
                    break;
                }
            }

            if (!is_value) {
                //printf("candidate: %d \n", candidates[j]);
                unset_candidate(p_hidden_triples.p_cell[i], candidates[j]);
            }
        }

        free(candidates);
    }
}

int hidden_triples(SudokuBoard *p_board) {
    int res = 0;
    int num_arr = 0;

    HiddenTriples hidden_triples_arr[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < 1; ++i) {
        //find_hidden_triples(p_board->p_cols[i], hidden_triples_arr, &res);
        find_hidden_triples(p_board->p_rows[i], hidden_triples_arr, &res);
        //find_hidden_triples(p_board->p_boxes[i], hidden_triples_arr, &res);
    }

    for (int i = 0; i < res; ++i) {
        unset_hidden_triples(hidden_triples_arr[i]);
    }

    return res;
}
