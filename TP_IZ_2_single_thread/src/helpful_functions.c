//
// Created by ekaterina on 24.03.2020.
//

#include "../inc/library.h"


void clear_vector(char **vector, int size)
{
    if (vector != NULL) {
        for (int i = 0; i < size; i++) {
            if (vector[i] != NULL)
                free(vector[i]);
        }
        free(vector);
    }
}

void clear_string(char *string)
{
    if (string != NULL)
        free(string);
}



void put_in_order(char *string, char *ordered_word)
{
    int k = 0;
    for (int j = 0; j < strlen(string); j++)
    {
        if (!(ispunct(string[j])))
        {
            ordered_word[k] = tolower(string[j]);
            k++;
        }
    }
    ordered_word[k] = '\0';
}

