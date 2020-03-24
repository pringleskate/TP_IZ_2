//
// Created by ekaterina on 24.03.2020.
//

#include "gtest/gtest.h"
#include <iostream>

using namespace std;

extern "C"
{
    #include "../TP_IZ_2_single_thread/inc/library.h"
}

TEST(testMainFunc, test1)
{
    char *test1_dir = (char *)"../../test1_1/";
    char *test2_dir = (char *)"../../test1_2/";

    int count_file_1 = 0;
    int count_file_2 = 0;

    int count_file_1_real = 10;
    int count_file_2_real = 5;

    get_file_count(&count_file_1, test1_dir);
    get_file_count(&count_file_2, test2_dir);

    ASSERT_EQ(count_file_1, count_file_1_real);
    ASSERT_EQ(count_file_2, count_file_2_real);
}

TEST(testMainFunc, test2)
{
    FILE *input_file = fopen("../../test1_1/file_1.txt", "r");
    if (input_file == nullptr)
        return;

    int unique_words = 0;
    int unique_words_real = 57;
    int count_files = 1;
    int storage_size = 500;

    Word_metrics *word_metrics = (Word_metrics *) malloc((count_files ) * sizeof(Word_metrics));

    if (word_metrics == nullptr)
        return;
    if (initialize_word_metrics(&word_metrics[0], count_files, storage_size))
        return;
    unique_words = get_metrics(input_file, &word_metrics[0]);

    clear_metrics_struct(&word_metrics[0], storage_size);
    free(word_metrics);
    fclose(input_file);

    ASSERT_EQ(unique_words, unique_words_real);
}

