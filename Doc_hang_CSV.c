#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** doc_hang(const char* filename, const char* row_name, int* count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    char buffer[1024];
    char** result = NULL;
    *count = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;
        buffer[strcspn(buffer, "\r")] = 0;

        char temp_buffer[1024];
        strcpy(temp_buffer, buffer);

        char *token = strtok(temp_buffer, ",");
        
        if (token != NULL && strcmp(token, row_name) == 0) {
            result = malloc(100 * sizeof(char*));
            
            printf("%s [", row_name);
            int is_first = 1;

            token = strtok(NULL, ",");
            while (token != NULL) {
                result[*count] = malloc(strlen(token) + 1);
                strcpy(result[*count], token);
                
                if (is_first == 0) {
                    printf(";");
                }
                printf("%s", token);
                is_first = 0;
                
                (*count)++;
                token = strtok(NULL, ",");
            }
            printf("]\n");
            break;
        }
    }
    
    fclose(file);
    return result;
}

int main() {
    int so_luong_phan_tu = 0;
    
    char** data = doc_hang("102293325202512A.csv", "1", &so_luong_phan_tu);
    
    if (data != NULL) {
        int i;
        for (i = 0; i < so_luong_phan_tu; i++) {
            free(data[i]);
        }
        free(data);
    }
    
    return 0;
}