//
// Created by ekaterina on 24.03.2020.
//

#include "../inc/library.h"

void get_dir_names(int argc, char **argv, char *path_input, char *path_output)
{
    if (argc == 1)
    {
        printf("Enter path to dir with input files:\n");
        scanf("%19s", path_input);

        printf("Enter path to dir with output files:\n");
        scanf("%19s", path_output);
    }
    else {
        memcpy(path_input, argv[1], BUFFER_SIZE);
        memcpy(path_output, argv[2], BUFFER_SIZE);
    }
}


int get_file_count(int *count_file, char *path_input)
{
    DIR *dir;
    dir = opendir(path_input);
    struct dirent *directory;

    if (!dir)
    {
        closedir(dir);
        return -1;
    }

    while ((directory = readdir(dir)) != NULL)
    {
        if (directory->d_name[0] != '.')
            (*count_file)++;

    }
    closedir(dir);
    return 0;
}


int get_file_names(char **input_files, char **output_files, char *path_input, char *path_output, int count_files)
{
    DIR *dir;
    dir = opendir(path_input);
    struct dirent *directory;

    if (!dir)
        return -1;

    unsigned long input_dir_len = strlen(path_input);
    unsigned long output_dir_len = strlen(path_output);

    int i = 0;
    while ((directory = readdir(dir)) != NULL)
    {
        if (directory->d_name[0] != '.')
        {
            unsigned long file_len = strlen(directory->d_name);
            char buf[file_len + 1];
            memcpy(buf, directory->d_name, file_len + 1);

            unsigned long size_in = file_len + input_dir_len + 1;
            unsigned long size_out = file_len + output_dir_len + 2;

            char file_name[size_in];
            char file_name2[size_out];

            memset(file_name, 0, size_in);
            memset(file_name2, 0,size_out);

            snprintf(file_name, size_in + 2, "%s/%s", path_input, directory->d_name);
            snprintf(file_name2, size_out + 2, "%s/%s", path_output, buf);

            input_files[i] = (char *) malloc ((size_in + 2) * sizeof(char));
            memcpy(input_files[i], file_name, size_in + 1);

            output_files[i] = (char *) malloc ((size_out + 2) * sizeof(char));
            memcpy(output_files[i], file_name2, size_out + 1);

            if ((input_files[i] == NULL)  || (output_files[i] == NULL))
            {
                clear_vector(input_files, i);
                clear_vector(output_files, i);
                closedir(dir);
                return -2;
            }

            i++;
        }
    }

    closedir(dir);
    return 0;
}