/*
	Created by Fry.
	Date: 10.12.2016
	https://www.ryoko-rpg.ro
	https://github.com/Metonimie
	Dependencies:
		- libconfig
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <libconfig.h>

#define VISITED_MASK   ( (unsigned) 1 << 4)
#define DEATH_MASK     ( (unsigned) 1 << 5)
#define EXIT_MASK      ( (unsigned) 1 << 6)
#define WALL_MASK      ( (unsigned) 1 << 7)
#define NUMBER_MASK   ~( (unsigned) ~0 << 4)

#define ISVISITED(x) ((x) & VISITED_MASK)
#define ISDEATH(x)   ((x) & DEATH_MASK)
#define ISEXIT(x)    ((x) & EXIT_MASK)
#define ISWALL(x)    ((x) & WALL_MASK)
#define GETNUMBER(x) ((x) & NUMBER_MASK)

unsigned rows = 4;
unsigned cols = 4;

// Config settings.
config_t configuration;

unsigned testM[4][4] = {
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
};

// The number will represent the death time, when it reaches 0, the cell
// becomes a death wall.

void make_visited_cell(unsigned * x) { *x |= VISITED_MASK; }
void unmake_visited_cell(unsigned * x) { *x &= ~VISITED_MASK; }

void make_death_cell(unsigned * x) { *x |= DEATH_MASK; }
void unmake_death_cell(unsigned * x) { *x &= ~DEATH_MASK; }

void make_exit_cell(unsigned * x) { *x |= EXIT_MASK; }
void unmake_exit_cell(unsigned * x) { *x &= ~EXIT_MASK; }

void make_wall_cell(unsigned * x) { *x |= WALL_MASK; }
void unmake_wall_cell(unsigned * x) { *x &= ~WALL_MASK; }

void increment_cell(unsigned * x) {
	 // We don't want to mess with walls or the exit cell.
	 if ( ISWALL(*x) || ISEXIT(*x)) {
		 return;
	 }
	 // check the timmer is not bigget than 15, the maximum value for a timmer.
	 if ( GETNUMBER(*x) < 16 ) {
		 *x = *x + 1;
	 }
}

void decrement_cell(unsigned * x) {
	 // We don't want to mess with walls or the exit cell.
	 if ( ISWALL(*x) || ISEXIT(*x)) {
		 return;
	 }
	 // We need to make sure the cell is always greater than 0.
	 if ( GETNUMBER(*x) > 0 ) {
		 *x = *x - 1;
	 }
}

// This should be called when updating the timmers of a cell that's not a
// death cell.
void update_cell_mechanism() {
	for (unsigned i = 0; i < rows; ++i) {
		for (unsigned j = 0; j < cols; ++j) {
			// We don't update the walls or exit cells.
			if ( ISWALL(testM[i][j]) || ISEXIT(testM[i][j])) {
				continue;
			}

			// We update the cell.
			if ( ISDEATH(testM[i][j]) ) {
				increment_cell(&testM[i][j]);
			} else if ( !ISDEATH(testM[i][j]) ) {
				decrement_cell(&testM[i][j]);
			}

			// We have a cell that is going to turn.
			if ( GETNUMBER(testM[i][j] == 0) ) {
				// We turn the cell.
				testM[i][j] = testM[i][j] ^ DEATH_MASK;
			}
		}
	}
}

// This should be called when backtracking, cheater.
void reverse_update_cell_mechanism() {
	for (unsigned i = 0; i < rows; ++i) {
		for (unsigned j = 0; j < cols; ++j) {
			// We don't update the walls or exit cells.
			if ( ISWALL(testM[i][j]) || ISEXIT(testM[i][j])) {
				continue;
			}

			// We update the cell.
			if ( !ISDEATH(testM[i][j]) ) {
				increment_cell(&testM[i][j]);
			} else if ( ISDEATH(testM[i][j]) ) {
				decrement_cell(&testM[i][j]);
			}

			// We have a cell that is going to turn.
			if ( GETNUMBER(testM[i][j] == 0) ) {
				// We turn the cell. Death doesn't wait.
				testM[i][j] = testM[i][j] ^ DEATH_MASK;
			}
		}
	}
}

void set_timmer(unsigned row, unsigned col, unsigned timer) {
	// If the timer is correct, we set it. If not, thou shall get blessed.
	// We don't mess with walls or the exit.
	if ( ISWALL(testM[row][col]) || ISEXIT(testM[row][col]) ) {
		return;
	}
	if ( timer > 0 && timer < 16) {
		testM[row][col] += timer;
	} else {
		testM[row][col] += (15 - GETNUMBER(testM[row][col]));
	}
}

void print_matrix2(unsigned m[4][4], unsigned row, unsigned col) {
	for (unsigned i = 0; i < row; ++i) {
		for (unsigned j = 0; j < col; ++j) {
			printf(" ");
			if ( ISEXIT(testM[i][j])) {
				printf("E  ");
				continue;
			}

			if ( ISWALL(testM[i][j])) {
				printf("W  ");
				continue;
			}

			if ( ISVISITED(testM[i][j]) ) {
				printf("V");
			}

			if ( ISDEATH(testM[i][j]) ) {
				printf("D");
			}

			printf("%d ", GETNUMBER(testM[i][j]));
		}
		printf("\n");
	}
}

unsigned ** new_matrix(unsigned row, unsigned col) {
	unsigned ** mat = (unsigned **) calloc(row, sizeof(unsigned *));
	for (unsigned i = 0; i < col; ++i) {
		mat[i] = (unsigned *) calloc(col, sizeof(unsigned) * col);
	}
	return mat;
}

// This function will be called with the initial position from where the player
// will start the labyrinth.
unsigned solve_mat(unsigned c, unsigned r) {
	// Make sure we check that everything is safe before accessing memory.
	if ( !( (c < cols) && (r < rows)) ) {
		return 0;
	}
	// check if we're on a wall or on a visited path or a deathcell.
	else if ( (ISWALL(testM[r][c])) || ISVISITED(testM[r][c])
			|| ISDEATH(testM[r][c]) )
	{
		return 0;
	} // check for deathcell.

	// Check if we found the exit.
	if ( ISEXIT(testM[r][c]) ) {
		return 1;
	}

	update_cell_mechanism();
	make_visited_cell(&testM[r][c]);

	if ( solve_mat(c + 1, r) == 1) {
		return 1;
	}

	if ( solve_mat(c, r + 1) == 1) {
		return 1;
	}

	if ( solve_mat(c - 1, r) == 1) {
		return 1;
	}

	if ( solve_mat(c, r - 1) == 1) {
		return 1;
	}

	reverse_update_cell_mechanism();
	unmake_visited_cell(&testM[r][c]);

	return 0;
}

/* Simple test code */

// make_exit_cell(&testM[3][3]);
// make_wall_cell(&testM[0][3]);
// make_wall_cell(&testM[1][1]);
// make_wall_cell(&testM[2][1]);
// make_wall_cell(&testM[2][2]);
// make_wall_cell(&testM[2][3]);
//
// for (unsigned i = 0; i < rows; ++i) {
// 	for (unsigned j = 0; j < cols; ++j) {
// 		set_timmer(i, j, 6);
// 	}
// }
//
// print_matrix2(testM, 4, 4);
// unsigned x = solve_mat(0, 0);
// printf("Solved: %d\n", x);
// print_matrix2(testM, 4, 4);

int main(void) {
	/* Initialize the configuration structure */
	config_init(&configuration);

	if ( config_read_file(&configuration, "data.cfg") != CONFIG_TRUE ) {
		fprintf(stderr, "Configuration Error: %s\n", config_error_text(&configuration));
		fprintf(stderr, "File: %s on line %d\n", config_error_file(&configuration),
 			config_error_line(&configuration));
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
