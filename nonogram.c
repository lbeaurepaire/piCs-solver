#include "nonogram.h"

board initialize_board(int nrows, int ncolumns) {
  board b;
  b.nrows = nrows;
  b.ncolumns = ncolumns;

  //allocate memory for full board & lines
  b.full_board = (cell **)malloc(nrows * sizeof(cell*));
  b.rows = (line *)malloc(nrows * sizeof(line));
  b.columns = (line *)malloc(ncolumns * sizeof(line));

  //initialize full board
  for (int i=0; i<nrows; i++) {
    b.full_board[i] = (cell *)malloc(ncolumns * sizeof(cell));
    for (int j=0; j<ncolumns; j++) {
      b.full_board[i][j].state = UNKNOWN;
      b.full_board[i][j].is_guess = 0;
      b.full_board[i][j].row = &b.rows[i]; //reference to the row
      b.full_board[i][j].col = &b.columns[j];  //reference to the col
    }
  }

  //initialize rows
  for (int i=0; i<nrows; i++) {
    b.rows[i].type = ROW;
    b.rows[i].cells = (cell **)malloc(ncolumns * sizeof(cell *));
    for (int j=0; j<ncolumns; j++) {
      b.rows[i].cells[j] = &b.full_board[i][j];
    }
  }

  //initialize columns (same logic as rows)
  for (int j=0; j<ncolumns; j++) {
    b.columns[j].type = COLUMN;
    b.columns[j].cells = (cell **)malloc(nrows * sizeof(cell *));
    for (int i=0; i<nrows; i++) {
      b.columns[j].cells[i] = &b.full_board[i][j];
    }
  }

  return b;
}

// Function to display the board
void display_board(board b) {
  for (int i = 0; i < b.nrows; i++) {
    for (int j = 0; j < b.ncolumns; j++) {
      switch (b.rows[i].cells[j]->state) {
        case UNKNOWN:
          printf("? ");
          break;
        case FILLED:
          printf("X ");
          break;
        case UNFILLED:
          printf(". ");
          break;
      }
    }
    printf("\n");
  }
}

void display_board_with_hints(board b) {
    // Find the maximum number of column hints
    int max_col_hints = 0;
    for (int i = 0; i < b.ncolumns; i++) {
        if (b.columns[i].num_blocks > max_col_hints) {
            max_col_hints = b.columns[i].num_blocks;
        }
    }

    // Display column hints
    for (int i = 0; i < max_col_hints; i++) {
        for (int space = 0; space < b.nrows; space++) {
            printf("  ");
        }

        for (int j = 0; j < b.ncolumns; j++) {
            if (i < max_col_hints - b.columns[j].num_blocks) {
                printf("  ");  // Display 0 if no hint for this level
            } else {
                // Display the hint, taking into account the offset
                printf("%d ", b.columns[j].blocks[i - (max_col_hints - b.columns[j].num_blocks)].length);
            }
        }
        printf("\n");
    }

    // Display board rows with row hints
    for (int i = 0; i < b.nrows; i++) {
        // Display row hints
        int remaining_zeros = b.nrows - b.rows[i].num_blocks;  // Calculate spaces to print before actual hints
        for (int j = 0; j < remaining_zeros; j++) {
            printf("  ");
        }
        for (int j = 0; j < b.rows[i].num_blocks; j++) {
            printf("%d ", b.rows[i].blocks[j].length);
        }

        // Display row cells
        for (int j = 0; j < b.ncolumns; j++) {
            switch (b.rows[i].cells[j]->state) {
                case UNKNOWN:
                    printf("? ");
                    break;
                case FILLED:
                    printf("X ");
                    break;
                case UNFILLED:
                    printf(". ");
                    break;
            }
        }
        printf("\n");
    }
}


board load_board_from_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    exit(1);
  }

  int nrows, ncolumns;
  fscanf(file, "%d %d", &nrows, &ncolumns);

  board b = initialize_board(nrows, ncolumns);

  // Read row hints
  for (int i=0; i<nrows; i++) {
    int nb_blocks; //number of blocks for the current row
    
    //read number of blocks for the row
    fscanf(file, "%d", &nb_blocks);

    //set the number of blocks and allocate memory for them
    b.rows[i].num_blocks = nb_blocks;
    b.rows[i].blocks = (block *)malloc(nb_blocks * sizeof(block));

    // Loop through each block and read its length (hint)
    for (int blk=0; blk<nb_blocks; blk++) {
      fscanf(file, "%d", &b.rows[i].blocks[blk].length);
    }
  }

  // Read column hints (similar to row hints)
  for (int j=0; j<ncolumns; j++) {
    int nb_blocks; //number of blocks for the current column

    //read number of blocks for the column
    fscanf(file, "%d", &nb_blocks);

    //set the number of blocks and allocate memory for them
    b.columns[j].num_blocks = nb_blocks;
    b.columns[j].blocks = (block *)malloc(nb_blocks * sizeof(block));

    // Loop through each block and read its length (hint)
    for (int blk=0; blk<nb_blocks; blk++) {
      fscanf(file, "%d", &b.columns[j].blocks[blk].length);
    }
  }

  fclose(file);
  return b;
}

int main() {
  //int nrows = 5, ncolumns = 5;
  //board b = initialize_board(nrows, ncolumns);
  
  board b = load_board_from_file("examples/puzzle03.cfg");
  display_board(b);
  printf("> DISPLAY WITH HINTS <\n");
  display_board_with_hints(b);

  int nrows = b.nrows, ncolumns = b.ncolumns;

  // Free allocated memory before exit
  for (int i = 0; i < nrows; i++) {
    free(b.rows[i].cells);
  }
  for (int i = 0; i < ncolumns; i++) {
    free(b.columns[i].cells);
  }
  free(b.rows);
  free(b.columns);

  return 0;
}
