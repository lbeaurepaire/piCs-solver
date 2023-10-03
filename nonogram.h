#ifndef NONOGRAM_H
#define NONOGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct line line;

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
  line* row; //pointer to the row containing the cell
  line* col; //pointer to the column containing the cell
  int is_guess; // 0 for no, 1 for yes. Useful during backtracking.
} cell;

//Structure describing a block
typedef struct {
  int length; //length of the block
  int min; //lowest cell index possible for this block
  int max; //highest cell index possible for this block
} block;

//Possible state for a line
typedef enum {
  ROW,
  COLUMN
} LineType;
//Structure describing a line (row or column)
typedef struct line {
  LineType type;
  cell** cells; // Dynamically allocated array of cells.
  int num_blocks; // Number of blocks/hints.
  block* blocks; // Dynamically allocated array of blocks.
} line;

//Structure describing a whole board
typedef struct {
    int nrows; // Number of rows.
    int ncolumns; // Number of columns.
    cell** full_board; //Dynamically allocated array of array of cells.
    line* rows; // Dynamically allocated array of rows.
    line* columns; // Dynamically allocated array of columns.
} board;

//Functions used

/*  Create and initialize the full board
 *  Input: size of the board n*m (rows*columns)
 *  Output: instanced & partially initialized board
 */
board initialize_board(int nrows, int ncolumns);

/*  Display all cells in the console
 *  Input: board
 *  Output: /
 */
void display_board(board b);
void display_board_with_hints(board b);

/*  Open file and read the puzzle contained
 *  Input: puzzle file path
 *  Output: instanced & initialized board
 *  Puzzle file format description:
 *    - l1: n m
 *    - l2->l(n+1): rows blocks
 *    - l(n+2)->l(n+m+1): columns blocks
 */
board load_board_from_file(const char *filename);

#endif
