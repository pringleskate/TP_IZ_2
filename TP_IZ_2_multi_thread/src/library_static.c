#include "../inc/library_static.h"

int multi_count_TF_IDF_metrics(int argc, char **argv)
{
    char path_input[BUFFER_SIZE], path_output[BUFFER_SIZE];
    multi_get_dir_names(argc, argv, path_input, path_output);


    int count_files = 0;

    /////подсчет количества файлов
    multi_get_file_count(&count_files, path_input);

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
    int res = multi_get_file_names(input_files, output_files, path_input, path_output, count_files);
    if (res == -1)
    {
        multi_clear_vector(input_files, count_files);
        multi_clear_vector(output_files, count_files);
        return 1;
    }
    else if (res == -2)
        return 1;

    /////функция обработки файлов
    multi_process_input_files(input_files, output_files, count_files);

    multi_clear_vector(input_files, count_files);
    multi_clear_vector(output_files, count_files);
    return 0;
}


int multi_process_input_files(char **input_files, char **output_files, int count_files)
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

        if (multi_initialize_word_metrics(word_metrics, count_files, INITIAL_STORAGE_SIZE))
            return 1;


        int all_flags = 0;

        int count_threads = multi_get_threads_count();
        int files_for_thread = count_files / count_threads;
        ////если делится не поровну, то последнему потоку идет count_files - files_for_thread

        pthread_t *threads = (pthread_t*) malloc(count_threads * sizeof(pthread_t));
        if (threads == NULL)
        {
            for (int i = 0; i < count_files; i++)
                multi_clear_metrics_struct(&word_metrics[i], INITIAL_STORAGE_SIZE);
            free(word_metrics);
            free(all_unique_words);
            return 1;
        }

        thr_data *thread_data = (thr_data*) malloc(count_threads * sizeof(thr_data));
        if (thread_data == NULL)
        {
            for (int i = 0; i < count_files; i++)
                multi_clear_metrics_struct(&word_metrics[i], INITIAL_STORAGE_SIZE);
            free(word_metrics);
            free(all_unique_words);
            free(threads);
            return 1;
        }


        int *flags = (int *) malloc(count_files * sizeof(int));
        if (flags == NULL)
        {
            for (int i = 0; i < count_files; i++)
                multi_clear_metrics_struct(&word_metrics[i], INITIAL_STORAGE_SIZE);
            free(word_metrics);
            free(all_unique_words);
            free(threads);
            free(thread_data);
            return 1;
        }
        memset(flags, 0, count_files);

        for(int i = 0; i < count_threads; i++)
        {
            thread_data[i].start_index = i * files_for_thread;

            if (i == (count_threads - 1))
            {
                thread_data[i].stop_index = count_files;
                thread_data[i].count_files_thread = count_files - files_for_thread;
            }
            else
            {
                thread_data[i].stop_index = (i + 1) * files_for_thread;
                thread_data[i].count_files_thread = files_for_thread;
            }
            thread_data[i].all_unique_words = all_unique_words;
            thread_data[i].flags = flags;
            thread_data[i].word_metrics = word_metrics;
            thread_data[i].input_files = input_files;

            pthread_create(&(threads[i]), NULL, multi_call_get_metrics, &thread_data[i]);
        }

        for(int i = 0; i < count_threads; i++)
            pthread_join(threads[i], NULL);

        for (int i = 0; i < count_files; i++)
            all_flags += flags[i];


        if (all_flags == count_files)
        {
            multi_update_idf_metrics(word_metrics, count_files, all_unique_words);
            multi_update_tf_idf_metrics(word_metrics, count_files, all_unique_words);
            multi_write_results_to_files(output_files, word_metrics, count_files, all_unique_words);
        }

        for (int i = 0; i < count_files; i++)
            multi_clear_metrics_struct(&word_metrics[i], INITIAL_STORAGE_SIZE);
        free(word_metrics);
        free(all_unique_words);
        free(threads);
        free(thread_data);
        free(flags);

        return 0;
}


int multi_initialize_word_metrics(Word_metrics *word_metrics, int count_files, int storage_size)
{
    for (int i = 0; i < count_files; i++)
    {
        word_metrics[i].word = (char **) malloc(storage_size * sizeof(char *));
        if (word_metrics[i].word == NULL)
        {
            multi_clear_metrics_struct(&word_metrics[i], storage_size);
            return 1;
        }

        for (int j = 0; j < storage_size; j++)
        {
            word_metrics[i].word[j] = (char *) malloc(BUFFER_SIZE * sizeof(char));
            if (word_metrics[i].word[j] == NULL)
            {
                multi_clear_metrics_struct(&word_metrics[i], storage_size);
                return 1;
            }
        }

        word_metrics[i].quantity = (int *) malloc(storage_size * sizeof(int));
        word_metrics[i].tf_metrics = (float *) malloc(storage_size * sizeof(float));
        word_metrics[i].idf_metrics = (float *) malloc(storage_size * sizeof(float));
        word_metrics[i].tf_idf_metrics = (float *) malloc(storage_size * sizeof(float));

        if ((word_metrics[i].quantity == NULL) || (word_metrics[i].tf_metrics == NULL) || (word_metrics[i].idf_metrics == NULL) || (word_metrics[i].tf_idf_metrics == NULL))
        {
            multi_clear_metrics_struct(&word_metrics[i], storage_size);
            return 1;
        }
    }

    return 0;
}

void multi_clear_metrics_struct(Word_metrics *metrics, int size)
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

