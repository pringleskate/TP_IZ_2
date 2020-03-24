#include "../inc/library.h"

int count_TF_IDF_metrics(int argc, char *argv[])
{
    char path_input[BUFFER_SIZE], path_output[BUFFER_SIZE];
    get_dir_names(argc, argv, path_input, path_output);


    int count_files = 0;

    /////подсчет количества файлов
    get_file_count(&count_files, path_input);

    if (count_files == -1)
        return 1;

    char **input_files = (char **) malloc(count_files * sizeof(char *));
    if (input_files == NULL)
        return 1;

    char **output_files = (char **) malloc(count_files * sizeof(char *));;
    if (output_files == NULL)
    {
        free(input_files);
        return 1;
    }


    /////функция, в которой будет открываться и проверяться директория, а также буду записываться в массив имена файлов
    int res = get_file_names(input_files, output_files, path_input, path_output, count_files);
    if (res == -1)
    {
        clear_vector(input_files, count_files);
        clear_vector(output_files, count_files);
        return 1;
    }
    else if (res == -2)
        return 1;

    /////функция обработки файлов
    process_input_files(input_files, output_files, count_files);

    clear_vector(input_files, count_files);
    clear_vector(output_files, count_files);
    return 0;
}


int process_input_files(char **input_files, char **output_files, int count_files)
{
    Word_metrics *word_metrics = (Word_metrics *) malloc((count_files ) * sizeof(Word_metrics));

    if (word_metrics == NULL)
        return 1;

    int *all_unique_words = (int *) malloc(count_files * sizeof(int));
    if (all_unique_words == NULL)
    {
        free(word_metrics);
        return 1;
    }

    if (initialize_word_metrics(word_metrics, count_files, INITIAL_STORAGE_SIZE))
        return 1;

    int flag = 0;
    for (int i = 0; i < count_files; i++)
    {
        FILE *input_file = fopen(input_files[i], "r");
        if (input_file == NULL)
            break;

        ////функция сбора из файла уникальных слов и подсчета tf-метрики для них
        all_unique_words[i] = get_metrics(input_file, &word_metrics[i]);
        if (all_unique_words[i] != -1)
            flag++;
        fclose(input_file);
    }

    if (flag == count_files)
    {
        update_idf_metrics(word_metrics, count_files, all_unique_words);
        update_tf_idf_metrics(word_metrics, count_files, all_unique_words);
        write_results_to_files(output_files, word_metrics, count_files, all_unique_words);
    }

    for (int i = 0; i < count_files; i++)
        clear_metrics_struct(&word_metrics[i], INITIAL_STORAGE_SIZE);
    free(word_metrics);
    free(all_unique_words);

    return 0;
}


int initialize_word_metrics(Word_metrics *word_metrics, int count_files, int storage_size)
{
    for (int i = 0; i < count_files; i++)
    {
        word_metrics[i].word = (char **) malloc(storage_size * sizeof(char *));
        if (word_metrics[i].word == NULL)
        {
            clear_metrics_struct(&word_metrics[i], storage_size);
            return 1;
        }

        for (int j = 0; j < storage_size; j++)
        {
            word_metrics[i].word[j] = (char *) malloc(BUFFER_SIZE * sizeof(char));
            if (word_metrics[i].word[j] == NULL)
            {
                clear_metrics_struct(&word_metrics[i], storage_size);
                return 1;
            }
        }

        word_metrics[i].quantity = (int *) malloc(storage_size * sizeof(int));
        word_metrics[i].tf_metrics = (float *) malloc(storage_size * sizeof(float));
        word_metrics[i].idf_metrics = (float *) malloc(storage_size * sizeof(float));
        word_metrics[i].tf_idf_metrics = (float *) malloc(storage_size * sizeof(float));

        if ((word_metrics[i].quantity == NULL) || (word_metrics[i].tf_metrics == NULL) || (word_metrics[i].idf_metrics == NULL) || (word_metrics[i].tf_idf_metrics == NULL))
        {
            clear_metrics_struct(&word_metrics[i], storage_size);
            return 1;
        }
    }

    return 0;
}

void clear_metrics_struct(Word_metrics *metrics, int size)
{
    if (metrics->word != NULL) {
        for (int i = 0; i < size; i++) {
            if (metrics->word[i] != NULL)
                free(metrics->word[i]);
        }
        free(metrics->word);
    }


    if(metrics->quantity != NULL)
        free(metrics->quantity);

    if(metrics->tf_metrics != NULL)
        free(metrics->tf_metrics);

    if(metrics->idf_metrics != NULL)
        free(metrics->idf_metrics);

    if(metrics->tf_idf_metrics != NULL)
        free(metrics->tf_idf_metrics);
}

