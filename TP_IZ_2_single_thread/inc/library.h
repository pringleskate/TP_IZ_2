#ifndef TP_IZ_2_SINGLE_THREAD_LIBRARY_H
#define TP_IZ_2_SINGLE_THREAD_LIBRARY_H

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

#define INITIAL_STORAGE_SIZE 800
#define BUFFER_SIZE 20

int count_TF_IDF_metrics(int argc, char *argv[]);

int process_input_files(char **input_files, char **output_files, int count_files);

int initialize_word_metrics(Word_metrics *word_metrics, int count_files, int storage_size);

void clear_metrics_struct(Word_metrics *metrics, int size);

//---------------------------------------------------------------

int get_file_count(int *count_file, char *path_input);

void get_dir_names(int argc, char **argv, char *path_input, char *path_output);

int get_file_names(char **input_files, char **output_files, char *path_input, char *path_output, int count_files);

//----------------------------------------------------------------

int get_metrics(FILE *input_file, Word_metrics *metrics);

int update_word_metrics(Word_metrics *metrics, char *ordered_word, int flag, int *unique_words);

int check_existing_word_index(Word_metrics *metrics, char *ordered_word, int unique_words);

void update_tf_metrics(Word_metrics *metrics, int all_scanned_words, int unique_words);

int update_idf_metrics(Word_metrics *word_metrics, int count_files, int *all_unique_words);

void update_tf_idf_metrics(Word_metrics *word_metrics, int count_files, int *all_unique_words);

//-------------------------------------------------------------------

void clear_vector(char **vector, int size);

void clear_string(char *string);

void put_in_order(char *string, char *ordered_word);

void write_results(FILE *output_file, Word_metrics *metrics, int unique_words);

int write_results_to_files(char **input_files, Word_metrics *word_metrics, int count_files, int *all_unique_words);

#endif //TP_IZ_2_SINGLE_THREAD_LIBRARY_H
