#ifndef TP_IZ_2_MULTI_THREAD_LIBRARY_H
#define TP_IZ_2_MULTI_THREAD_LIBRARY_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <sys/stat.h>

typedef struct Word_metrics {
    char **word;
    int *quantity;
    float *tf_metrics;
    float *idf_metrics;
    float *tf_idf_metrics;
} Word_metrics;

typedef struct
{
    int start_index;
    int stop_index;
    int count_files_thread;
    int *all_unique_words;
    int *flags;
    char **input_files;
    Word_metrics *word_metrics;
} thr_data;

#ifdef IS_MACOS
#include <unistd.h>
#elif IS_LINUX
#include <sys/sysinfo.h>
#endif

#define INITIAL_STORAGE_SIZE 800
#define BUFFER_SIZE 20

int multi_count_TF_IDF_metrics(int argc, char **argv);

int multi_process_input_files(char **input_files, char **output_files, int count_files);

int multi_initialize_word_metrics(Word_metrics *word_metrics, int count_files, int storage_size);

void multi_clear_metrics_struct(Word_metrics *metrics, int size);

//---------------------------------------------------------------

void  multi_call_get_metrics(void *th_data);

int multi_get_file_count(int *count_file, char *path_input);

void multi_get_dir_names(int argc, char **argv, char *path_input, char *path_output);

int multi_get_file_names(char **input_files, char **output_files, char *path_input, char *path_output, int count_files);

//----------------------------------------------------------------

int multi_get_metrics(FILE *input_file, Word_metrics *metrics);

int multi_update_word_metrics(Word_metrics *metrics, char *ordered_word, int flag, int *unique_words);

int multi_check_existing_word_index(Word_metrics *metrics, char *ordered_word, int unique_words);

void multi_update_tf_metrics(Word_metrics *metrics, int all_scanned_words, int unique_words);

int multi_update_idf_metrics(Word_metrics *word_metrics, int count_files, int *all_unique_words);

void multi_update_tf_idf_metrics(Word_metrics *word_metrics, int count_files, int *all_unique_words);

//-------------------------------------------------------------------

int multi_get_threads_count();

void multi_clear_vector(char **vector, int size);

void multi_clear_string(char *string);

void multi_put_in_order(char *string, char *ordered_word);

void multi_write_results(FILE *output_file, Word_metrics *metrics, int unique_words);

int multi_write_results_to_files(char **input_files, Word_metrics *word_metrics, int count_files, int *all_unique_words);

#endif //TP_IZ_2_MULTI_THREAD_LIBRARY_H
