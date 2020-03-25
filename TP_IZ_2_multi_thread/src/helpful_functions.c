//
// Created by ekaterina on 24.03.2020.
//

#include "../inc/library_static.h"

int multi_get_threads_count()
{
#ifdef IS_MACOS
    int threadsCount = sysconf( _SC_NPROCESSORS_CONF );
 printf("\nThreads count: %i\n", threadsCount);
 return threadsCount;
#elif IS_LINUX
    int threadsCount = get_nprocs();
    printf("\nThreads count: %i\n", threadsCount);
    return threadsCount;
#endif
}

void multi_clear_vector(char **vector, int size)
{
    if (vector != NULL) {
        for (int i = 0; i < size; i++) {
            if (vector[i] != NULL)
                free(vector[i]);
        }
        free(vector);
    }
}

void multi_clear_string(char *string)
{
    if (string != NULL)
        free(string);
}



void multi_put_in_order(char *string, char *ordered_word)
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

