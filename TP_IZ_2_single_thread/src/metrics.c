//
// Created by ekaterina on 24.03.2020.
//

#include "../inc/library.h"

int get_metrics(FILE *input_file, Word_metrics *metrics)
{
    char *string = (char *) malloc(BUFFER_SIZE * sizeof(char));
    char *ordered_word = (char *) malloc(BUFFER_SIZE * sizeof(char));

    if ((string == NULL) || (ordered_word == NULL))
    {
        clear_string(string);
        clear_string(ordered_word);
        printf("Error with allocating memory for buffer strings\n");
        return -1;
    }

    int unique_words = 0;

    int all_scanned_words = 0;

    while(fscanf(input_file, "%s", string) != EOF)
    {
        memset(ordered_word, 0, BUFFER_SIZE);
        int flag = 0;

        //проверочные функции для каждого слова
        put_in_order(string, ordered_word);

        if (strlen(ordered_word) > 0)
        {
            //проверку на то, есть ли слово уже в структуре
            //если оно есть, то возвращается индекс в массиве и увеличивается счетчик в существующей структу
            // если нет, то возвращается -1 и создается новая запись в структуре
            flag = check_existing_word_index(metrics, ordered_word, unique_words);
            if (update_word_metrics(metrics, ordered_word, flag, &unique_words))
            {
                printf("Error with updating data in metrics structures\n");
                break;
            }
            all_scanned_words++;
        }

        memset(string, 0, BUFFER_SIZE);
    }

    update_tf_metrics(metrics, all_scanned_words, unique_words);

    free(string);
    free(ordered_word);

    return unique_words;
}



int update_word_metrics(Word_metrics *metrics, char *ordered_word, int flag, int *unique_words)
{
    if (flag == -1)
    {
        memcpy(metrics->word[*unique_words], ordered_word, BUFFER_SIZE);
        (metrics->quantity[*unique_words]) = 1;
        (*unique_words)++;

        if ((check_existing_word_index(metrics, ordered_word, *unique_words)) == -1)
            return 1;
    }

    else
        (metrics->quantity[flag])++;

    return 0;
}


int check_existing_word_index(Word_metrics *metrics, char *ordered_word, int unique_words)
{
    for (int i = 0; i < unique_words; i++)
    {
        if (strcmp(metrics->word[i], ordered_word) == 0)
            return i;
    }
    return -1;
}



void update_tf_metrics(Word_metrics *metrics, int all_scanned_words, int unique_words)
{
    for (int i = 0; i < unique_words; i++)
    {
        if (all_scanned_words)
            metrics->tf_metrics[i] = (((float)metrics->quantity[i] / (float)all_scanned_words) * 100);
    }
}


int update_idf_metrics(Word_metrics *word_metrics, int count_files, int *all_unique_words)
{
    for (int i = 0; i < count_files; i++) ////проходим по массиву структур, количество которых соответствует обработанным файлам
    {
        int references = 0;
        for (int j = 0; j < all_unique_words[i]; j++) ////проходим по массиву, соответствующему количеству уникальных слов в каждом массиве внутри структур
        {
            for (int k = 0; k < all_unique_words[i]; k++)
            {
                if (strcmp(word_metrics[i].word[k], word_metrics[i].word[j]) == 0)
                {
                    references++;
                    break;
                }
            }
            word_metrics[i].idf_metrics[j] = ((float) references / (float) count_files );
        }
    }
    return 0;
}


void update_tf_idf_metrics(Word_metrics *word_metrics, int count_files, int *all_unique_words)
{
    for (int i = 0; i < count_files; i++) ////проходим по массиву структур, количество которых соответствует обработанным файлам
    {
        for (int j = 0; j < all_unique_words[i]; j++) ////проходим по массиву, соответствующему количеству уникальных слов в каждом массиве внутри структур
            word_metrics[i].tf_idf_metrics[j] = word_metrics[i].tf_metrics[j] * word_metrics[i].idf_metrics[j];
    }
}


int write_results_to_files(char **output_files, Word_metrics *word_metrics, int count_files, int *all_unique_words)
{
    for (int i = 0; i < count_files; i++)
    {
        FILE *file = fopen(output_files[i], "w");
        if (file == NULL)
        {
            printf("File cannot be opened for writing up results\n");
            return 1;
        }

        write_results(file, &word_metrics[i], all_unique_words[i]);
        printf("Scanning results're updated in file %s\n\n", output_files[i]);

        fclose(file);
    }
    return 0;
}


void write_results(FILE *output_file, Word_metrics *metrics, int unique_words)
{
    for (int i = 0; i < unique_words; i++)
    {
        fprintf(output_file, "Word %s\n", metrics->word[i]);
        fprintf(output_file, "TF %.2f\n", metrics->tf_metrics[i]);
        fprintf(output_file, "IDF %.2f\n", metrics->idf_metrics[i]);
        fprintf(output_file, "TF-IDF %.2f\n\n", metrics->tf_idf_metrics[i]);
    }
}