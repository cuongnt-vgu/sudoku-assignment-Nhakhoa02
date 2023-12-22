#include "hidden_singles.h"
#include <stdlib.h>
#include <stdio.h>

// find hidden single values (the value that is unique among all the candidates for a row, a collumn, or a box)
int find_hidden_single_values(Cell **p_cells, int *hidden_single_values)
{
    int hidden_single_count = 0;
    int value_count[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) value_count[i] = 0;
    //hidden_single_values = malloc(BOARD_SIZE * sizeof(Cell *)); //list to count number of each value in cell

    for (int i = 0; i < BOARD_SIZE; ++i) {

        int * candidates = get_candidates(p_cells[i]); //list of available candidates
        int num_candidates = p_cells[i] -> num_candidates;// number of candidates

        for (int j = 0; j < num_candidates; ++j){
            ++value_count[candidates[j] - 1]; 
        }

        //remove solved_cell in the list
        if (num_candidates == 1)
            ++value_count[(p_cells[i] -> value) - 1];
        free(candidates);
    }

    //check whether there are any hidden_single_value (value count = 1)
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        if (value_count[i] == 1) {
            ++hidden_single_count;
            hidden_single_values[hidden_single_count - 1] = i + 1;
        }
    }
    
    return hidden_single_count;
}

// find hidden single cells in a row, in a collumn or in a box
void find_hidden_single(Cell **p_cells, HiddenSingle *p_hidden_singles, int *p_counter) {
    //int hidden_single_values[BOARD_SIZE];
    //int num_hidden_singles = find_hidden_single_values(p_cells, hidden_single_values);

    int hidden_single_values [BOARD_SIZE]; //list of temp hidden_single 
    int values_count = find_hidden_single_values(p_cells, hidden_single_values); //number of hidden_single
    
    // printf("count: %d", values_count);
    // for (int i = 0; i < values_count; ++i)
    //     printf("\n %d\n", hidden_single_values[i]);


    for (int i = 0; i < BOARD_SIZE; ++i) {
        int *candidates = get_candidates(p_cells[i]); // List of candidates

        for (int j = 0; j < p_cells[i]->num_candidates; ++j) {
            for (int k = 0; k < values_count; ++k) {
                if (candidates[j] == hidden_single_values[k]) { // Found a hidden single cell

                    // Check for duplicate
                    int isDuplicate = 0;
                    for (int x = 0; x < *p_counter; ++x) {
                        if (p_cells[i] == p_hidden_singles[x].p_cell) {
                            isDuplicate = 1;
                            break;
                        }
                    }

                    if (!isDuplicate) {
                        // If not a duplicate, add it to the p_hidden_singles array
                        p_hidden_singles[*p_counter].p_cell = p_cells[i];
                        p_hidden_singles[*p_counter].value = hidden_single_values[k];
                        (*p_counter)++;
                    }
                }
            }
        }

        free(candidates);
    }
    return;
}

int hidden_singles(SudokuBoard *p_board)
{
    int res = 0;

    HiddenSingle hidden_arr[BOARD_SIZE * BOARD_SIZE];

    // Find hidden singles in rows
    for (int i = 0; i < BOARD_SIZE; ++i) {
        find_hidden_single(p_board->p_cols[i], hidden_arr, &res);
        find_hidden_single(p_board->p_boxes[i], hidden_arr, &res);
        find_hidden_single(p_board->p_rows[i], hidden_arr, &res);
    }

    for (int i = 0; i < res; ++i){
        int candidates[]= {hidden_arr[i].value};
        set_candidates(hidden_arr[i].p_cell, candidates, 1);
    }

    return res;
}
