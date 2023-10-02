#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Possible states for a cell
typedef enum {
  UNKNOWN,
  FILLED,
  UNFILLED
} CellState;
//Structure describing for a cell
typedef struct {
  CellState state;
    
  //used to identify each cell
  int idx_row;
  int idx_col;

  int is_guess; // 0 for no, 1 for yes. Useful during backtracking.
} cell;

//Structure describing a block
typedef struct {
  int length;
  // Any other metadata related to this block can be added later.
} block;

//Possible state for a line
typedef enum {
  ROW,
  COLUMN
} LineType;
//Structure describing a line (row or column)
typedef struct {
  LineType type;
  int size; // Number of cells in the line.
  cell* cells; // Dynamically allocated array of cells.
  int num_blocks; // Number of blocks/hints.
  block* blocks; // Dynamically allocated array of blocks.
} line;

//Structure describing a whole board
typedef struct {
    int nrows; // Number of rows.
    int ncolumns; // Number of columns.
    line* rows; // Dynamically allocated array of rows.
    line* columns; // Dynamically allocated array of columns.
} board;


