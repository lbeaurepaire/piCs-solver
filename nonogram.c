#include "nonogram.h"

board initialize_board(int nrows, int ncolumns) {
  board b;
  b.nrows = nrows;
  b.ncolumns = ncolumns;

  b.rows = (line *)malloc(nrows * sizeof(line));
  b.columns = (line *)malloc(ncolumns * sizeof(line));

  for (int i = 0; i < nrows; i++) {
    b.rows[i].type = ROW;
    b.rows[i].size = ncolumns;
    b.rows[i].cells = (cell *)malloc(ncolumns * sizeof(cell));
    for (int j = 0; j < ncolumns; j++) {
      b.rows[i].cells[j].state = UNKNOWN;
      b.rows[i].cells[j].is_guess = 0;
    }
    // TODO: Set blocks/hints for rows
  }

  for (int i = 0; i < ncolumns; i++) {
    b.columns[i].type = COLUMN;
    b.columns[i].size = nrows;
    b.columns[i].cells = (cell *)malloc(nrows * sizeof(cell));
    for (int j = 0; j < nrows; j++) {
      b.columns[i].cells[j].state = UNKNOWN;
      b.columns[i].cells[j].is_guess = 0;
    }
    // TODO: Set blocks/hints for columns
  }

  return b;
}

// Function to display the board
void display_board(board b) {
  for (int i = 0; i < b.nrows; i++) {
    for (int j = 0; j < b.ncolumns; j++) {
      switch (b.rows[i].cells[j].state) {
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
  for (int i = 0; i < nrows; i++) {
    int count;
    char buffer[256]; // Assuming maximum of 256 hints in a line
    fgets(buffer, sizeof(buffer), file);

    // Count number of hints in the line
    char *token = strtok(buffer, " ");
    while (token != NULL) {
      count++;
      token = strtok(NULL, " ");
    }

    // Allocate blocks and read hints for the row
    b.rows[i].num_blocks = count;
    b.rows[i].blocks = (block *)malloc(count * sizeof(block));

    // Use strtok again to actually parse and store the hints
    token = strtok(buffer, " ");
    int idx = 0;
    while (token != NULL) {
      b.rows[i].blocks[idx].length = atoi(token);
      idx++;
      token = strtok(NULL, " ");
    }
  }

  // Read column hints (similar to row hints)
  for (int i = 0; i < ncolumns; i++) {
    int count = 0;
    char buffer[256];
    fgets(buffer, sizeof(buffer), file);

    char *token = strtok(buffer, " ");
    while (token != NULL) {
      count++;
      token = strtok(NULL, " ");
    }

    b.columns[i].num_blocks = count;
    b.columns[i].blocks = (block *)malloc(count * sizeof(block));

    token = strtok(buffer, " ");
    int idx = 0;
    while (token != NULL) {
      b.columns[i].blocks[idx].length = atoi(token);
      idx++;
      token = strtok(NULL, " ");
    }
  }

  fclose(file);
  return b;
}

int main() {
  //int nrows = 5, ncolumns = 5;
  //board b = initialize_board(nrows, ncolumns);
  
  board b = load_board_from_file("puzzle03.cfg");
  display_board(b);
  //display_board_with_hints(b);

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
