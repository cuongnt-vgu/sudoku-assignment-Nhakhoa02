#include "hidden_triples.h"
#include <stdlib.h>
#include <stdio.h>

int is_hidden_triples(Cell *p_cell, int * value_count, int * hidden_arr){
    //initial create arr to store the appearance of values
    int * candidates = get_candidates(p_cell);
    int res = 0;

    for (int i = 0 ; i < p_cell->num_candidates; ++i){
        if (value_count[candidates[i] - 1] == 0) {
            hidden_arr[res] = candidates[i];
            ++res;
        }
    }

    free(candidates);

    return res;
}

bool is_duplicate_hidden_triples(Cell *p_cell_1, Cell *p_cell_2, Cell *p_cell_3, HiddenTriples *p_hidden_triples, int *p_counter)
{
    for (int i = 0; i < *p_counter; ++i)
    {
        Cell *existing_cell_1 = p_hidden_triples[i].p_cell[0];
        Cell *existing_cell_2 = p_hidden_triples[i].p_cell[1];
        Cell *existing_cell_3 = p_hidden_triples[i].p_cell[2];

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

void is_the_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples, Cell *p_cell_1, Cell *p_cell_2, Cell *p_cell_3, int *p_counter) {
    bool is_naked = true;
    if (p_cell_1 -> num_candidates > 3) is_naked = false;
    if (p_cell_2 -> num_candidates > 3) is_naked = false;
    if (p_cell_3 -> num_candidates > 3) is_naked = false;

    if (is_naked) return;

    int value_count[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) value_count[i] = 0;

    for (int i = 0 ; i < BOARD_SIZE; ++i){
        if (
            (p_cells[i] != p_cell_1)
            && (p_cells[i] != p_cell_2)
            && (p_cells[i] != p_cell_3)
        ) {
            int * candidates = get_candidates(p_cells[i]);
            for (int j = 0 ; j < p_cells[i]->num_candidates; ++j){
                ++value_count[candidates[j] - 1];
            }
            free(candidates);
        }        
    }

    //get hidden values for p_cell_1
    int hidden_arr_1[BOARD_SIZE];
    int num_hidden_1 = is_hidden_triples(p_cell_1, value_count, hidden_arr_1);

    //get hidden values for p_cell_2
    int hidden_arr_2[BOARD_SIZE];
    int num_hidden_2 = is_hidden_triples(p_cell_2, value_count, hidden_arr_2);

    //get hidden values for p_cell_3
    int hidden_arr_3[BOARD_SIZE];
    int num_hidden_3 = is_hidden_triples(p_cell_3, value_count, hidden_arr_3);

    if (num_hidden_1 < 2 || num_hidden_1 > 3) return;
    if (num_hidden_2 < 2 || num_hidden_2 > 3) return;
    if (num_hidden_3 < 2 || num_hidden_3 > 3) return;

    

    //findind triples
    int num_diff = 0;

    for (int i = 0 ; i < BOARD_SIZE; ++i) value_count[i] = 0;

    for (int i = 0 ; i < num_hidden_1; ++i)
    {
        //printf("%d ", candidates_1[i-1]);
        if (num_diff == 4) break;
        if (value_count[hidden_arr_1[i] - 1] == 0) {
            p_hidden_triples[*p_counter].value[num_diff] = hidden_arr_1[i];
            ++num_diff;
        }
        ++value_count[hidden_arr_1[i] - 1];
    }
    // printf("\n");
    for (int i = 0 ; i < num_hidden_2; ++i)
    {
        // printf("%d ", candidates_2[i]);
        if (num_diff == 4) break;
        if (value_count[hidden_arr_2[i] - 1] == 0) {
            p_hidden_triples[*p_counter].value[num_diff] = hidden_arr_2[i];
            ++num_diff;
        }
        ++value_count[hidden_arr_2[i] - 1];
    }
    // printf("\n");
    for (int i = 0 ; i < num_hidden_3; ++i)
    {
        // printf("%d ", candidates_3[i]);
        if (num_diff == 4) break;
        if (value_count[hidden_arr_3[i] - 1] == 0){
            p_hidden_triples[*p_counter].value[num_diff] = hidden_arr_3[i];
            ++num_diff;
        } 
        ++value_count[hidden_arr_3[i] - 1];
    }

    if (num_diff == 3) {
        if (!is_duplicate_hidden_triples(p_cell_1, p_cell_2, p_cell_3, p_hidden_triples, p_counter)){
            p_hidden_triples[*p_counter].p_cell[0] = p_cell_1;
            p_hidden_triples[*p_counter].p_cell[1] = p_cell_2;
            p_hidden_triples[*p_counter].p_cell[2] = p_cell_3;
            ++(*p_counter);
        }
    }
}



void find_hidden_triples(Cell **p_cells, HiddenTriples *p_hidden_triples, int *p_counter) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (p_cells[i]->num_candidates >= 2) {
            for (int j = i + 1; j < BOARD_SIZE; ++j) {
                if (p_cells[j]->num_candidates >= 2) {
                    for (int k = j + 1; k < BOARD_SIZE; ++k) {
                        if (p_cells[k]->num_candidates >= 2) {
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

    for (int i = 0; i < BOARD_SIZE; ++i) {
        find_hidden_triples(p_board->p_cols[i], hidden_triples_arr, &res);
        find_hidden_triples(p_board->p_rows[i], hidden_triples_arr, &res);
        find_hidden_triples(p_board->p_boxes[i], hidden_triples_arr, &res);
    }

    for (int i = 0; i < res; ++i) {
        unset_hidden_triples(hidden_triples_arr[i]);
    }

    return res;
}
