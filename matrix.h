#ifndef MATRIX_H
  #define MATRIX_H 
  
  typedef enum element_type {
    BLANK = 0,    //   '*'
    HYPHEN,       //   '-'
    VERTICAL_BAR, //   '|'
    SLASH,        //   '/'
    BACKSLASH,    //   '\'
    PLUS,         //   '+'
} elem_type;

typedef struct point {
    elem_type type;
} Point;

typedef struct matrix {
    int number_of_rows;
    int number_of_columns;
    Point *data;
} Matrix;

  void init_matrix(Matrix *mat, int rows, int cols);
  void print_matrix(const Matrix *mat);
  int in_range(int value, int left, int right);
  int get_abs(int first, int second);
  void draw_matrix(Matrix *mat, int start_row, int start_col, int end_row, int end_col);
  void erase_coordinate(Matrix *mat, int row, int col);
  void resize_matrix(Matrix *mat, int num_rows, int num_cols);
  void add_row(Matrix *mat, int row);
  void delete_row(Matrix *mat, int row);
  void add_col(Matrix *mat, int col);
  void delete_col(Matrix *mat, int col);
  void save_to_file(const Matrix *mat, char *file_name);
  void load_from_file(Matrix *mat, char *file_name);
  

#endif