#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

int playingGame(int argc, char**argv) {
    Matrix mat;
    mat.data = NULL;
    if (argc == 1)
    {
        init_matrix(&mat, 10, 10);
    }
    else if (argc == 3)
    {
        int rows = strtol(argv[1], NULL, 10);
        int cols = strtol(argv[2], NULL, 10);
        if (cols <= 0)
        {   printf("The number of columns is less than 1.\n");
            printf("Making default board of 10 X 10.\n");
            init_matrix(&mat, 10, 10);
            //  return -1;
        }
        if (rows <= 0)
        {   printf("The number of rows is less than 1.\n");
            printf("Making default board of 10 X 10.\n");
            init_matrix(&mat, 10, 10);
            //  return -1;
        }
        init_matrix(&mat, rows, cols);
    }
    else
    {
         printf("Wrong number of command line arguements entered.\n");
         printf("Usage: ./paint.out [num_rows num_cols]\n");
         printf("Making default board of 10 X 10.\n");
         init_matrix(&mat, 10, 10);
        
        return -1;
    }
    print_matrix(&mat);
    char command[50];
    while (1)
    {

        printf("Enter your command: ");
        fgets(command, 50, stdin);
        // printf("Hi there, command: %s", command);

        // Parse command:
        char *pch; // Use to get tokens from command
        pch = strtok(command, " ");
        if (pch == NULL) {
            printf("Unrecognized command. Type h for help.\n");
            printf("Enter your command: ");
            continue;
        }
        if (pch[0] == 'q')
        {
            if (mat.data != NULL)
            {
                free(mat.data);
                mat.data = NULL;
            }
            break;
        } else if (pch[0] == 'h') {
        printf("Commands:\n");
        printf("Help: h\n");
        printf("Quit: q\n");
        printf("Draw line: w row_start col_start row_end col_end\n");
        printf("Resize: r num_rows num_cols\n");
        printf("Add row or column: a [r | c] pos\n");
        printf("Delete row or column: d [r | c] pos\n");
        printf("Erase: e row col\n");
        printf("Save: s file_name\n");
        printf("Load: l file_name\n");

        }
        else if (pch[0] == 'w')
        {
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper draw command.\n");
                continue;
            }
            int start_row = strtol(pch, NULL, 10);
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper draw command.\n");
                continue;
            }
            int start_col = strtol(pch, NULL, 10);
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper draw command.\n");
                continue;
            }
            int end_row = strtol(pch, NULL, 10);
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper draw command.\n");
                continue;
            }
            int end_col = strtol(pch, NULL, 10);
            draw_matrix(&mat, start_row, start_col, end_row, end_col);
        }
        else if (pch[0] == 'e')
        {
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper erase command.\n");
                continue;
            }
            int row = strtol(pch, NULL, 10);
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper erase command.\n");
                continue;
            }
            int col = strtol(pch, NULL, 10);
            erase_coordinate(&mat, row, col);
        }
        else if (pch[0] == 'r')
        {
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper resize command.\n");
                continue;
            }
            int num_rows = strtol(pch, NULL, 10);
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper resize command.\n");
                continue;
            }
            int num_cols = strtol(pch, NULL, 10);
            resize_matrix(&mat, num_rows, num_cols);
        }
        else if (pch[0] == 'a')
        {
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper add command.\n");
                continue;
            }
            if (pch[0] == 'c')
            {
                pch = strtok(NULL, " ");
                if (pch == NULL)
                {
                    printf("Improper add command.\n");
                    continue;
                }
                int col = strtol(pch, NULL, 10);
                add_col(&mat, col);
            }
            else if (pch[0] == 'r')
            {
                pch = strtok(NULL, " ");
                if (pch == NULL)
                {
                    printf("Improper resize command.\n");
                    continue;
                }
                int row = strtol(pch, NULL, 10);
                add_row(&mat, row);
            }
        }
        else if (pch[0] == 'd')
        {
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper delete command.\n");
                continue;
            }
            if (pch[0] == 'c')
            {
                pch = strtok(NULL, " ");
                if (pch == NULL)
                {
                    printf("Improper delete command.\n");
                    continue;
                }
                int col = strtol(pch, NULL, 10);
                delete_col(&mat, col);
            }
            else if (pch[0] == 'r')
            {
                pch = strtok(NULL, " ");
                if (pch == NULL)
                {
                    printf("Improper delete command.\n");
                    continue;
                }
                int row = strtol(pch, NULL, 10);
                delete_row(&mat, row);
            }
        }
        else if (pch[0] == 's')
        {
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper save command or file could not be created.\n");
                continue;
            }
            pch[strcspn(pch, "\r\n")] = 0; // remove unwanted end-of-line character
            save_to_file(&mat, pch);
        }
        else if (pch[0] == 'l')
        {
            pch = strtok(NULL, " ");
            if (pch == NULL)
            {
                printf("Improper load command or file could not be opened.\n");
                continue;
            }
            pch[strcspn(pch, "\r\n")] = 0; // remove unwanted end-of-line character
            load_from_file(&mat, pch);
        }
        print_matrix(&mat);
    }
    return 0;
}

int main(int argc, char **argv)
{
    playingGame(argc, argv);
    return 0;
}