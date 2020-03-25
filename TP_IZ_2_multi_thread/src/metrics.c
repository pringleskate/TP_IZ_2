//
// Created by ekaterina on 24.03.2020.
//

#include "../inc/library_static.h"


void  multi_call_get_metrics(void *th_data)
{
    thr_data * data = (thr_data*)th_data;
    for (int i = data->start_index; i < data->stop_index; i++)
    {
        FILE *input_file = fopen(data->input_files[i], "r");
        if (input_file == NULL)
            return;

        ////функция сбора из файла уникальных слов и подсчета tf-метрики для них
        data->all_unique_words[i] = multi_get_metrics(input_file, &(data->word_metrics[i]));
        if (data->all_unique_words[i] != -1)
            (data->flags[i])++;
        fclose(input_file);
    }
}


int multi_get_metrics(FILE *input_file, multi_Word_metrics *metrics)
{
    char *string = (char *) malloc(BUFFER_SIZE * sizeof(char));
    char *ordered_word = (char *) malloc(BUFFER_SIZE * sizeof(char));

    if ((string == NULL) || (ordered_word == NULL))
    {
        multi_clear_string(string);
        multi_clear_string(ordered_word);
        printf("Error with allocating memory for buffer strings\n");
        return -1;
    }

    int unique_words = 0;

    int all_scanned_words = 0;

    while(fscanf(input_file, "%19s", string) != EOF)
    {
        memset(ordered_word, 0, BUFFER_SIZE);

        //проверочные функции для каждого слова
        multi_put_in_order(string, ordered_word);

        if (strlen(ordered_word) > 0)
        {
            int flag = 0;
            //проверку на то, есть ли слово уже в структуре
            //если оно есть, то возвращается индекс в массиве и увеличивается счетчик в существующей структу
            // если нет, то возвращается -1 и создается новая запись в структуре
            flag = multi_check_existing_word_index(metrics, ordered_word, unique_words);
            if (multi_update_word_metrics(metrics, ordered_word, flag, &unique_words))
            {
                printf("Error with updating data in metrics structures\n");
                break;
            }
            all_scanned_words++;
        }

        memset(string, 0, BUFFER_SIZE);
    }

    multi_update_tf_metrics(metrics, all_scanned_words, unique_words);

    free(string);
    free(ordered_word);

    return unique_words;
}



int multi_update_word_metrics(multi_Word_metrics *metrics, char *ordered_word, int flag, int *unique_words)
{
    if (flag == -1)
    {
        memcpy(metrics->word[*unique_words], ordered_word, BUFFER_SIZE);
        (metrics->quantity[*unique_words]) = 1;
        (*unique_words)++;

        if ((multi_check_existing_word_index(metrics, ordered_word, *unique_words)) == -1)
            return 1;
    }

    else
        (metrics->quantity[flag])++;

    return 0;
}


int multi_check_existing_word_index(multi_Word_metrics *metrics, char *ordered_word, int unique_words)
{
    for (int i = 0; i < unique_words; i++)
    {
        if (strcmp(metrics->word[i], ordered_word) == 0)
            return i;
    }
    return -1;
}



void multi_update_tf_metrics(multi_Word_metrics *metrics, int all_scanned_words, int unique_words)
{
    for (int i = 0; i < unique_words; i++)
    {
        if (all_scanned_words)
            metrics->tf_metrics[i] = (((float)metrics->quantity[i] / (float)all_scanned_words) * 100);
    }
}


int multi_update_idf_metrics(multi_Word_metrics *word_metrics, int count_files, int *all_unique_words)
{
    for (int i = 0; i < count_files; i++)
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


void multi_update_tf_idf_metrics(multi_Word_metrics *word_metrics, int count_files, int *all_unique_words)
{
    for (int i = 0; i < count_files; i++) ////проходим по массиву структур, количество которых соответствует обработанным файлам
    {
        for (int j = 0; j < all_unique_words[i]; j++) ////проходим по массиву, соответствующему количеству уникальных слов в каждом массиве внутри структур
            word_metrics[i].tf_idf_metrics[j] = word_metrics[i].tf_metrics[j] * word_metrics[i].idf_metrics[j];
    }
}


int multi_write_results_to_files(char **input_files, multi_Word_metrics *word_metrics, int count_files, int *all_unique_words)
{
    for (int i = 0; i < count_files; i++)
    {
        FILE *file = fopen(input_files[i], "w");
        if (file == NULL)
        {
            printf("File cannot be opened for writing up results\n");
            return 1;
        }

        multi_write_results(file, &word_metrics[i], all_unique_words[i]);

        fclose(file);
    }
    return 0;
}


void multi_write_results(FILE *output_file, multi_Word_metrics *metrics, int unique_words)
{
    for (int i = 0; i < unique_words; i++)
    {
        fprintf(output_file, "Word %s\n", metrics->word[i]);
        fprintf(output_file, "TF %.2f\n", metrics->tf_metrics[i]);
        fprintf(output_file, "IDF %.2f\n", metrics->idf_metrics[i]);
        fprintf(output_file, "TF-IDF %.2f\n\n", metrics->tf_idf_metrics[i]);
    }
}