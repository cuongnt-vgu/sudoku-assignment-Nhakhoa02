#include "hidden_pairs.h"
#include <stdlib.h>
#include <stdio.h>

int is_hidden_pairs(Cell *p_cell, int *value_count, int *hidden_arr) {
    int *candidates = get_candidates(p_cell);
    int res = 0;

    for (int i = 0; i < p_cell->num_candidates; ++i) {
        if (value_count[candidates[i] - 1] == 0) {
            hidden_arr[res] = candidates[i];
            ++res;
        }
    }

    free(candidates);

    return res;
}

bool is_duplicate_hidden_pairs(Cell *p_cell_1, Cell *p_cell_2, HiddenPairs *p_hidden_pairs, int *p_counter) {
    for (int i = 0; i < *p_counter; ++i) {
        Cell *existing_cell_1 = p_hidden_pairs[i].p_cell[0];
        Cell *existing_cell_2 = p_hidden_pairs[i].p_cell[1];

        // Check if the cells are the same (order-independent)
        bool same_cells =
            (p_cell_1 == existing_cell_1 || p_cell_1 == existing_cell_2) &&
            (p_cell_2 == existing_cell_1 || p_cell_2 == existing_cell_2);

        if (same_cells) {
            // The pair is a duplicate
            return true;
        }
    }

    // No duplicate found
    return false;
}

void is_the_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs, Cell *p_cell_1, Cell *p_cell_2, int *p_counter) {
    bool is_naked = true;
    if (p_cell_1->num_candidates > 2) is_naked = false;
    if (p_cell_2->num_candidates > 2) is_naked = false;

    if (is_naked) return;

    int value_count[BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i) value_count[i] = 0;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        if ((p_cells[i] != p_cell_1) && (p_cells[i] != p_cell_2)) {
            int *candidates = get_candidates(p_cells[i]);
            for (int j = 0; j < p_cells[i]->num_candidates; ++j) {
                ++value_count[candidates[j] - 1];
            }
            free(candidates);
        }
    }

    // get hidden values for p_cell_1
    int hidden_arr_1[BOARD_SIZE];
    int num_hidden_1 = is_hidden_pairs(p_cell_1, value_count, hidden_arr_1);

    // get hidden values for p_cell_2
    int hidden_arr_2[BOARD_SIZE];
    int num_hidden_2 = is_hidden_pairs(p_cell_2, value_count, hidden_arr_2);

    if (num_hidden_1 < 2 || num_hidden_1 > 2) return;
    if (num_hidden_2 < 2 || num_hidden_2 > 2) return;

    // finding pairs
    int num_diff = 0;

    for (int i = 0; i < BOARD_SIZE; ++i) value_count[i] = 0;

    for (int i = 0; i < num_hidden_1; ++i) {
        if (num_diff == 3) break;
        if (value_count[hidden_arr_1[i] - 1] == 0) {
            p_hidden_pairs[*p_counter].value[num_diff] = hidden_arr_1[i];
            ++num_diff;
        }
        ++value_count[hidden_arr_1[i] - 1];
    }

    for (int i = 0; i < num_hidden_2; ++i) {
        if (num_diff == 3) break;
        if (value_count[hidden_arr_2[i] - 1] == 0) {
            p_hidden_pairs[*p_counter].value[num_diff] = hidden_arr_2[i];
            ++num_diff;
        }
        ++value_count[hidden_arr_2[i] - 1];
    }

    if (num_diff == 2) {
        if (!is_duplicate_hidden_pairs(p_cell_1, p_cell_2, p_hidden_pairs, p_counter)) {
            p_hidden_pairs[*p_counter].p_cell[0] = p_cell_1;
            p_hidden_pairs[*p_counter].p_cell[1] = p_cell_2;
            ++(*p_counter);
        }
    }
}

void find_hidden_pairs(Cell **p_cells, HiddenPairs *p_hidden_pairs, int *p_counter) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (p_cells[i]->num_candidates >= 2) {
            for (int j = i + 1; j < BOARD_SIZE; ++j) {
                if (p_cells[j]->num_candidates >= 2) {
                    is_the_hidden_pairs(p_cells, p_hidden_pairs, p_cells[i], p_cells[j], p_counter);
                }
            }
        }
    }
}

void unset_hidden_pairs(HiddenPairs p_hidden_pairs) {
    for (int i = 0; i < 2; ++i) {
        int *candidates = get_candidates(p_hidden_pairs.p_cell[i]);
        int num_candidates = p_hidden_pairs.p_cell[i]->num_candidates;

        for (int j = 0; j < num_candidates; ++j) {
            bool is_value = false;
            for (int k = 0; k < 2; ++k) {
                if (candidates[j] == p_hidden_pairs.value[k]) {
                    is_value = true;
                    break;
                }
            }

            if (!is_value) {
                unset_candidate(p_hidden_pairs.p_cell[i], candidates[j]);
            }
        }

        free(candidates);
    }
}

int hidden_pairs(SudokuBoard *p_board) {
    int res = 0;
    HiddenPairs hidden_pairs_arr[BOARD_SIZE * BOARD_SIZE];

    for (int i = 0; i < BOARD_SIZE; ++i) {
        find_hidden_pairs(p_board->p_cols[i], hidden_pairs_arr, &res);
        find_hidden_pairs(p_board->p_rows[i], hidden_pairs_arr, &res);
        find_hidden_pairs(p_board->p_boxes[i], hidden_pairs_arr, &res);
    }

    for (int i = 0; i < res; ++i) {
        unset_hidden_pairs(hidden_pairs_arr[i]);
    }

    return res;
}
