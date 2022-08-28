#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

void init_matrix(Matrix *mat, int rows, int cols)
{
    if (cols <= 0 || rows <= 0)
    {
        return;
    }
    mat->number_of_columns = cols;
    mat->number_of_rows = rows;
    if (NULL != mat->data)
    {
        free(mat->data);
        mat->data = NULL;
    }
    mat->data = malloc(cols * rows * sizeof(Point));
    for (int i = mat->number_of_rows - 1; i >= 0; i--)
    {
        for (int j = 0; j < mat->number_of_columns; j++)
        {
            mat->data[i * mat->number_of_rows + j].type = BLANK;
        }
    }
}

void print_matrix(const Matrix *mat)
{
    for (int i = mat->number_of_rows - 1; i >= 0; i--)
    {
        printf("%d ", i);
        for (int j = 0; j < mat->number_of_columns; j++)
        {
            elem_type type = mat->data[i * mat->number_of_columns + j].type;
            if (type == BLANK)
            {
                printf("* ");
            }
            else if (type == HYPHEN)
            {
                printf("- ");
            }
            else if (type == VERTICAL_BAR)
            {
                printf("| ");
            }
            else if (type == SLASH)
            {
                printf("/ ");
            }
            else if (type == BACKSLASH)
            {
                printf("\\ ");
            }
            else if (type == PLUS)
            {
                printf("+ ");
            }
            else
            {
                printf("z ");
            }
        }
        printf("\n");
    }
    printf("  ");
    for (int j = 0; j < mat->number_of_columns; j++)
    {
        printf("%d ", j);
    }
        printf("\n");
}

int in_range(int value, int left, int right)
{
    if (value < left || value >= right)
        return 0;
    return 1;
}

int get_abs(int first, int second)
{
    return (first > second) ? (first - second) : (second - first);
}

void draw_matrix(Matrix *mat, int start_row, int start_col, int end_row, int end_col)
{
    if (!in_range(start_row, 0, mat->number_of_rows) || !in_range(end_row, 0, mat->number_of_rows) || !in_range(start_col, 0, mat->number_of_columns) || !in_range(end_col, 0, mat->number_of_columns))
    {
        printf("Improper draw command.\n");
        return;
    }
    if (start_row == end_row && (start_col == end_col))
    {
        printf("Improper draw command.\n");
        return;
    }

    if (start_row == end_row)
    {
        // Horizontal lines
        int inc = (start_col < end_col) ? 1 : -1;
        for (int i = start_col;;)
        {
            if (mat->data[start_row * mat->number_of_columns + i].type == HYPHEN ||
                mat->data[start_row * mat->number_of_columns + i].type == BLANK)
            {
                mat->data[start_row * mat->number_of_columns + i].type = HYPHEN;
            }
            else
            {
                mat->data[start_row * mat->number_of_columns + i].type = PLUS;
            }
            if (i == end_col)
                break;
            i += inc;
        }
    }
    else if (start_col == end_col)
    {
        // Vertical lines
        int inc = (start_row < end_row) ? 1 : -1;
        for (int i = start_row;;)
        {
            if (mat->data[i * mat->number_of_columns + start_col].type == VERTICAL_BAR ||
                mat->data[i * mat->number_of_columns + start_col].type == BLANK)
            {
                mat->data[i * mat->number_of_columns + start_col].type = VERTICAL_BAR;
            }
            else
            {
                mat->data[i * mat->number_of_columns + start_col].type = PLUS;
            }
            if (i == end_row)
                break;
            i += inc;
        }
    }
    else if ((end_row - start_row) == (start_col - end_col))
    {
        // Right Diagonal
        int inc = (start_row < end_row) ? 1 : -1;
        for (int i = start_row, j = start_col;;)
        {
            if (mat->data[i * mat->number_of_columns + j].type == BACKSLASH ||
                mat->data[i * mat->number_of_columns + j].type == BLANK)
            {
                mat->data[i * mat->number_of_columns + j].type = BACKSLASH;
            }
            else
            {
                mat->data[i * mat->number_of_columns + j].type = PLUS;
            }
            if (i == end_row)
                break;
            i += inc;
            j -= inc;
        }
    }
    else if ((end_row - start_row) == (end_col - start_col))
    {
        // Left Diagonal
        int inc = (start_row < end_row) ? 1 : -1;
        for (int i = start_row, j = start_col;;)
        {
            if (mat->data[i * mat->number_of_columns + j].type == SLASH ||
                mat->data[i * mat->number_of_columns + j].type == BLANK)
            {
                mat->data[i * mat->number_of_columns + j].type = SLASH;
            }
            else
            {
                mat->data[i * mat->number_of_columns + j].type = PLUS;
            }
            if (i == end_row)
                break;
            i += inc;
            j += inc;
        }
    }
}

void erase_coordinate(Matrix *mat, int row, int col)
{
    if (row < 0 || row > mat->number_of_rows || col < 0 || col > mat->number_of_columns)
        return;
    mat->data[row * mat->number_of_columns + col].type = BLANK;
}

void resize_matrix(Matrix *mat, int num_rows, int num_cols)
{
    if (num_rows < 1 || num_cols < 1)
    {
        printf("Improper resize command.\n");
        return;
    }
    Point *new_data = malloc(num_rows * num_cols * sizeof(Point));

    for (int i = 0; i < mat->number_of_rows; i++)
    {
        for (int j = 0; j < mat->number_of_columns; j++)
        {
            if (i < num_rows && j < num_cols)
            {
                new_data[i * num_cols + j].type = mat->data[i * mat->number_of_columns + j].type;
            }
        }
    }

    mat->number_of_columns = num_cols;
    mat->number_of_rows = num_rows;
    free(mat->data);
    mat->data = new_data;
}

void add_row(Matrix *mat, int row)
{
    if (row < 0 || row > mat->number_of_rows)
        return;
    Point *new_data = malloc((mat->number_of_rows + 1) * mat->number_of_columns * sizeof(Point));
    mat->number_of_rows++;
    for (int i = 0; i < mat->number_of_rows; i++)
    {
        for (int j = 0; j < mat->number_of_columns; j++)
        {
            if (i == row)
            {
                new_data[i * mat->number_of_columns + j].type = BLANK;
            }
            else if (i < row)
            {
                new_data[i * mat->number_of_columns + j].type = mat->data[i * mat->number_of_columns + j].type;
            }
            else
            {
                new_data[i * mat->number_of_columns + j].type = mat->data[(i - 1) * mat->number_of_columns + j].type;
            }
        }
    }
    free(mat->data);
    mat->data = new_data;
}

void delete_row(Matrix *mat, int row)
{
    if (mat->number_of_rows <= 0 || row < 0 || row >= mat->number_of_rows)
        return;
    Point *new_data = malloc((mat->number_of_rows - 1) * mat->number_of_columns * sizeof(Point));
    mat->number_of_rows--;
    for (int i = mat->number_of_rows - 1; i >= 0; i--)
    {
        for (int j = 0; j < mat->number_of_columns; j++)
        {
            new_data[i * mat->number_of_rows + j].type = BLANK;
        }
    }
    for (int i = 0; i < mat->number_of_rows; i++)
    {
        for (int j = 0; j < mat->number_of_columns; j++)
        {
            if (i < row)
            {
                new_data[i * mat->number_of_columns + j].type = mat->data[i * mat->number_of_columns + j].type;
            }
            else
            {
                new_data[i * mat->number_of_columns + j].type = mat->data[(i + 1) * mat->number_of_columns + j].type;
            }
        }
    }
    free(mat->data);
    mat->data = new_data;
}

void add_col(Matrix *mat, int col)
{
    if (col < 0 || col > mat->number_of_columns)
        return;
    Point *new_data = malloc(mat->number_of_rows * (mat->number_of_columns + 1) * sizeof(Point));
    mat->number_of_columns++;
    for (int i = 0; i < mat->number_of_rows; i++)
    {
        for (int j = 0; j < mat->number_of_columns; j++)
        {
            if (j == col)
            {
                new_data[i * mat->number_of_columns + j].type = BLANK;
            }
            else if (j < col)
            {
                new_data[i * mat->number_of_columns + j].type = mat->data[i * (mat->number_of_columns - 1) + j].type;
            }
            else if (j > col)
            {
                new_data[i * mat->number_of_columns + j].type = mat->data[i * (mat->number_of_columns - 1) + j - 1].type;
            }
        }
    }
    free(mat->data);
    mat->data = new_data;
}

void delete_col(Matrix *mat, int col)
{
    if (mat->number_of_columns <= 0 || col < 0 || col > mat->number_of_columns)
        return;
    Point *new_data = malloc(mat->number_of_rows * (mat->number_of_columns - 1) * sizeof(Point));
    mat->number_of_columns--;
    for (int i = 0; i < mat->number_of_rows; i++)
    {
        for (int j = 0; j < mat->number_of_columns; j++)
        {
            if (j < col)
            {
                new_data[i * mat->number_of_columns + j].type = mat->data[i * (mat->number_of_columns + 1) + j].type;
            }
            else
            {
                new_data[i * mat->number_of_columns + j].type = mat->data[i * (mat->number_of_columns + 1) + j + 1].type;
            }
        }
    }
    free(mat->data);
    mat->data = new_data;
}

void save_to_file(const Matrix *mat, char *file_name)
{
    FILE *f = fopen(file_name, "w");
    if (f == NULL)
    {
        printf("Improper save command or file could not be created.\n");
        return;
    }
    for (int i = mat->number_of_rows - 1; i >= 0; i--)
    {
        for (int j = 0; j < mat->number_of_columns; j++)
        {
            elem_type type = mat->data[i * mat->number_of_columns + j].type;
            if (type == BLANK)
            {
                fprintf(f, "*");
            }
            else if (type == HYPHEN)
            {
                fprintf(f, "-");
            }
            else if (type == VERTICAL_BAR)
            {
                fprintf(f, "|");
            }
            else if (type == SLASH)
            {
                fprintf(f, "/");
            }
            else if (type == BACKSLASH)
            {
                fprintf(f, "\\");
            }
            else if (type == PLUS)
            {
                fprintf(f, "+");
            }
            else
            {
                fprintf(f, "z");
            }
            if (j != mat->number_of_columns - 1)
                fprintf(f, " ");
        }
        if (i != 0)
            fprintf(f, "\n");
    }

    fclose(f);
}

void load_from_file(Matrix *mat, char *file_name)
{
    FILE *f = fopen(file_name, "r");
    if (f == NULL)
    {
        printf("Improper load command or file could not be opened.\n");
        return;
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int row_count = 0;
    int col_count = 0;

    // First read is to get the total numbers of elements
    while ((read = getline(&line, &len, f)) != -1)
    {
        row_count++;
    }
    // We count total number of columns from last line
    char *tmp = strtok(line, " ");
    while (tmp != NULL)
    {
        tmp = strtok(NULL, " ");
        col_count++;
    }

    // Allocate memory to use
    Point *new_data = malloc(row_count * col_count * sizeof(Point));

    // Read from beginning again
    rewind(f);

    // First read is to get the total numbers of elements
    int row_idx = row_count - 1;
    int col_idx = 0;
    while ((read = getline(&line, &len, f)) != -1)
    {
        char *tmp = strtok(line, " ");
        if (tmp == NULL)
            continue;
        if (tmp[0] == '*')
        {
            new_data[row_idx * col_count + col_idx].type = BLANK;
        }
        else if (tmp[0] == '-')
        {
            new_data[row_idx * col_count + col_idx].type = HYPHEN;
        }
        else if (tmp[0] == '|')
        {
            new_data[row_idx * col_count + col_idx].type = VERTICAL_BAR;
        }
        else if (tmp[0] == '/')
        {
            new_data[row_idx * col_count + col_idx].type = SLASH;
        }
        else if (tmp[0] == '\\')
        {
            new_data[row_idx * col_count + col_idx].type = BACKSLASH;
        }
        else if (tmp[0] == '+')
        {
            new_data[row_idx * col_count + col_idx].type = PLUS;
        }
        else
        {
            new_data[row_idx * col_count + col_idx].type = BLANK;
        }
        col_idx++;
        while (tmp != NULL)
        {
            tmp = strtok(NULL, " ");
            if (tmp != NULL)
            {
                if (tmp[0] == '*')
                {
                    new_data[row_idx * col_count + col_idx].type = BLANK;
                }
                else if (tmp[0] == '-')
                {
                    new_data[row_idx * col_count + col_idx].type = HYPHEN;
                }
                else if (tmp[0] == '|')
                {
                    new_data[row_idx * col_count + col_idx].type = VERTICAL_BAR;
                }
                else if (tmp[0] == '/')
                {
                    new_data[row_idx * col_count + col_idx].type = SLASH;
                }
                else if (tmp[0] == '\\')
                {
                    new_data[row_idx * col_count + col_idx].type = BACKSLASH;
                }
                else if (tmp[0] == '+')
                {
                    new_data[row_idx * col_count + col_idx].type = PLUS;
                }
                else
                {
                    new_data[row_idx * col_count + col_idx].type = BLANK;
                }
                col_idx++;
            }
        }
        col_idx = 0;
        row_idx--;
    }

    fclose(f);

    free(mat->data);
    mat->data = new_data;
    mat->number_of_rows = row_count;
    mat->number_of_columns = col_count;
}
