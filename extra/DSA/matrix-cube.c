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

void fatal(const char * msg);

int rows;
int cols;

// Config settings.
config_t configuration;
config_setting_t * cfg_setting;
unsigned ** testM;

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

void print_matrix(unsigned ** testM) {
	for (unsigned i = 0; i < rows; ++i) {
		for (unsigned j = 0; j < cols; ++j) {
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
	if (mat == NULL) {
		fatal("Not enough memory to allocate the matrix.");
	}
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

void fatal(const char * msg) {
	fputs(msg, stderr);
	fprintf(stderr, "\n");
	exit(1);
}

int main(void) {
	/* Variables */
	int start_row = 0, start_col = 0;
	int end_row   = 0, end_col   = 0;

	/* Initialize the configuration structure */
	config_init(&configuration);

	if ( config_read_file(&configuration, "data.cfg") != CONFIG_TRUE ) {
		fprintf(stderr, "Configuration Error: %s\n", config_error_text(&configuration));
		fprintf(stderr, "File: %s on line %d\n", config_error_file(&configuration),
 			config_error_line(&configuration));
		return EXIT_FAILURE;
	}

	/* Get the matrix settings. */
	cfg_setting = config_lookup(&configuration, "application.matrix");
	if ( cfg_setting != NULL ) {
		/* Matrix rows */
		if ( config_setting_lookup_int(cfg_setting, "rows", &rows)
		!= CONFIG_TRUE ) {
			fatal("Error finding the length of the rows.");
		}
		/* Matrix cols */
		if ( config_setting_lookup_int(cfg_setting, "columns", &cols)
		!= CONFIG_TRUE ) {
			fatal("Error finding the length of the cols.");
		}
		/* Engineer start row */
		if ( config_setting_lookup_int(cfg_setting, "start_row", &start_row)
			!= CONFIG_TRUE ) {
			fatal("Error finding the start row.");
		}
		/* Engineer start col */
		if ( config_setting_lookup_int(cfg_setting, "start_col", &start_col)
		!= CONFIG_TRUE ) {
			fatal("Error finding the start col.");
		}
		/* Exit row */
		if ( config_setting_lookup_int(cfg_setting, "end_row", &end_row)
		!= CONFIG_TRUE ) {
			fatal("Error finding the exit row.");
		}
		/* Exit col */
		if ( config_setting_lookup_int(cfg_setting, "end_col", &end_col)
		!= CONFIG_TRUE ) {
			fatal("Error finding the exit col.");
		}

		/* Create the matrix and the exit */
		testM = new_matrix(rows, cols);
		make_exit_cell(&testM[end_row][end_col]);
	} else {
		fatal("Matrix settings not found!");
	}

	/* Get the wall settings. */
	cfg_setting = config_lookup(&configuration, "application.walls");
	if ( cfg_setting != NULL ) {
		int wall_count = config_setting_length(cfg_setting);

		for (int i = 0; i < wall_count; ++i) {
			config_setting_t * wall_setting = config_setting_get_elem(cfg_setting, i);
			int wall_row = 0;
			int wall_col = 0;

			if (!config_setting_lookup_int(wall_setting, "row", &wall_row)) {
				fatal("Invalid row found in wall settings.");
			}
			if (!config_setting_lookup_int(wall_setting, "column", &wall_col)) {
				fatal("Invalid row found in wall settings.");
			}

			make_wall_cell(&testM[wall_row][wall_col]);
		}
	} else {
		fatal("Wall settings not found!");
	}

	cfg_setting = config_lookup(&configuration, "application.timers");
	if ( cfg_setting != NULL ) {
		int timer_count = config_setting_length(cfg_setting);
		int timer_idx = 0;
		if ( timer_count != rows * cols) {
			fatal("Invalid number of timers provided!");
		}

		int val = 0;
		for (unsigned i = 0; i < rows; ++i) {
			for (unsigned j = 0; j < cols; ++j) {
				val = config_setting_get_int_elem(cfg_setting, timer_idx);
				set_timmer(i, j, val);
				++timer_idx;
			}
		}
	} else {
		fatal("Timer settings not found!");
	}

	print_matrix(testM);
	int solution_found = solve_mat(start_col, start_row);
	printf("Matrix solved? %s\n", solution_found ? "yes" : "no");
	if (solution_found) {
		print_matrix(testM);
	}

	config_destroy(&configuration);
	return EXIT_SUCCESS;
}
