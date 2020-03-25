//
// Created by ekaterina on 24.03.2020.
//

#include "gtest/gtest.h"

#include <iostream>

using namespace std;


extern "C"
{
    #include "library.h"
    #include "library_static.h"
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

TEST(testMainFunc, test3)
{
    char *test_1_dir = (char *)"../../test3_single/";
    char *test_1_res_dir = (char *)"../../test3_single_res/";

    char *test_2_res_dir = (char *)"../../test3_multi_res/";

    char *argv_0 = (char *)"";
    char *argv[3] = {argv_0, test_1_dir, test_1_res_dir};

    std::clock_t start_time_single_thread = std::clock();
    count_TF_IDF_metrics(3, argv);
    std::clock_t end_time_single_thread = std::clock();

    float time_single_thread = ((float)((float)end_time_single_thread - (float)start_time_single_thread) / CLOCKS_PER_SEC) * 1000;
    printf("\nSingle thread program execution time = %f\n", time_single_thread);



    char *argv_multi[3] = {argv_0, test_1_dir, test_2_res_dir};

    std::clock_t start_time_multi_thread = std::clock();
    multi_count_TF_IDF_metrics(3, argv_multi);
    std::clock_t end_time_multi_thread = std::clock();

    float time_multi_thread = ((float)((float)end_time_multi_thread - (float)start_time_multi_thread) / CLOCKS_PER_SEC) * 1000;
    printf("Multi thread program execution time = %f\n\n\n", time_multi_thread);


    int count_file_single = 0;
    get_file_count(&count_file_single, test_1_res_dir);

    int count_file_multi = 0;
    get_file_count(&count_file_multi, test_2_res_dir);

    ASSERT_EQ(count_file_multi, count_file_single);
}
