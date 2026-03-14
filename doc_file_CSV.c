#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const Max_row = 200;
const Max_col = 20;

int main() {
    FILE *file = fopen("102293325202512A.csv", "r");
    if (file == NULL) return 1;

    char ***csv_data = malloc(Max_row * sizeof(char**));
    int row_count = 0;
    int col_counts[200] = {0};
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;
        buffer[strcspn(buffer, "\r")] = 0;

        csv_data[row_count] = malloc(Max_col * sizeof(char*));
        int col_count = 0;

        char *start = buffer;
        char *comma_pos;

        while ((comma_pos = strchr(start, ',')) != NULL) {
            *comma_pos = '\0';
            
            csv_data[row_count][col_count] = malloc(strlen(start) + 1);
            strcpy(csv_data[row_count][col_count], start);
            
            col_count++;
            start = comma_pos + 1;
        }

        csv_data[row_count][col_count] = malloc(strlen(start) + 1);
        strcpy(csv_data[row_count][col_count], start);
        col_counts[row_count] = col_count + 1;

        row_count++;
    }
    fclose(file);
    
    int i, j;
    for (i = 0; i < row_count; i++) {
        printf("[ ");
        for (j = 0; j < col_counts[i]; j++) {
            if(j==0) {
                printf("%3s ; ", csv_data[i][j]);
            }
            else if (j==2) {
                printf("%.20s ; ", csv_data[i][j]);
            }
            else {
                printf("%s ; ", csv_data[i][j]);
            }
        }
        printf(" ]\n");
    }

    for (i = 0; i < row_count; i++) {
        for (j = 0; j < col_counts[i]; j++) {
            free(csv_data[i][j]);
        }
        free(csv_data[i]);
    }
    free(csv_data);

    return 0;
}