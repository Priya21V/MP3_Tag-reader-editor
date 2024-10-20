#include <stdio.h>
#include <string.h>
#include "main.h"
int main(int argc, char *argv[])
{
    Taginfo mp3Taginfo;
    Editinfo editinfo;
    if (argc < 2)
    {
        printf("-------------------------------------------------------------------------\n");
        printf("Error: ./a.out : INVALID ARGUMENTS\n");
        printf("USAGE :\n");
        printf("To View     : ./a.out -v mp3filename\n");
        printf("To Edit     : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
        printf("To get Help : ./a.out --help\n");
        printf("--------------------------------------------------------------------------\n");
    }
    else
    {
        uint res = check_operation(argv);
        if (res == help)
        {
            printf("----------------------HELP MENU -----------------\n");
            printf("1. -v -> to view mp3 file contents\n");
            printf("2. -e -> to edit mp3 file conetents\n");
            printf("\t2.1. -t -> to edit song title\n");
            printf("\t2.2. -a -> to edit artist name\n");
            printf("\t2.3. -A -> to edit album name\n");
            printf("\t2.4. -y -> to edit year\n");
            printf("\t2.5. -m -> to edit content\n");
            printf("\t2.6. -c -> to edit comment\n");
            printf("--------------------------------------------------\n");
        }
        else if (res == view)
        {
            if (argc == 3)
            {
                uint res1 = read_and_validate_mp3_file(argv, &mp3Taginfo);
                if (res1 == failure)
                {
                    printf("ERROR in validating %s file\n", argv[2]);
                    return 0;
                }
                // printf("INFO: %s file validation done successfully\n", argv[2]);
                printf("\n------------------------------SELECTED VIEW DETAILS-----------------------\n");
                printf("\n--------------------------------------------------------------------------\n");
                printf("-----------------------MP3 TAG READER AND EDITOR FOR ID3v2----------------");
                printf("\n--------------------------------------------------------------------------\n");
                res1 = view_tag(argv, &mp3Taginfo);
                if (res1 == failure)
                {
                    printf("Error in viewing %s file\n", argv[2]);
                    return 0;
                }
                printf("---------------------------------------------------------------------------\n");
                printf("\n------------------------DETAILS DISPLAYED SUCCESSFULLY---------------------\n");
            }
            else
            {
                printf("------------------------------------------------------------------------\n");
                printf("Error: ./a.out : INVALID ARGUMENTS\n");
                printf("USAGE :\n");
                printf("To View     : ./a.out -v mp3filename\n");
                printf("To Edit     : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
                printf("To get Help : ./a.out --help\n");
                printf("------------------------------------------------------------------------\n");
            }
        }
        else if (res == edit)
        {
            if (argc == 5)
            {
                printf("\n----------------------SELECTED EDIT DETAILS----------------\n");
                printf("\n--------------SELECTED EDIT OPTION----------- \n");
                if (read_and_validate_edit(argv, &editinfo) == success)
                {
                    if (edit_tag(argv, &editinfo) != success)
                    {
                        printf("Failure\n");
                    }
                    else
                    {
                        if (strcmp(argv[2], "-t") == 0)
                        {
                            printf("\nTITTLE\t: %s\n",argv[3]);
                            printf("\n-----------------TITTLE CHANGED SUCCESSFULLY------------\n");
                            
                        }
                        else if (strcmp(argv[2], "-a") == 0)
                        {
                           printf("\nARTIST NAME\t: %s\n",argv[3]);
                           printf("\n-----------------ARTIST NAME CHANGED SUCCESSFULLY------------\n");
                           
                        }
                        else if (strcmp(argv[2], "-A") == 0)
                        {
                            
                             printf("\nALBUM NAME\t: %s\n",argv[3]);
                             printf("\n-----------------ALBUM NAME CHANGED SUCCESSFULLY------------\n");
                        }
                        else if (strcmp(argv[2], "-y") == 0)
                        {
                           printf("\nYEAR\t: %s\n",argv[3]);
                           printf("\n-----------------YEAR CHANGED SUCCESSFULLY------------\n");
                        }
                        else if (strcmp(argv[2], "-m") == 0)
                        {
                            printf("\nCONTENT\t: %s\n",argv[3]);
                            printf("\n-----------------CONTENT CHANGED SUCCESSFULLY------------\n");
                        }
                        else if (strcmp(argv[2], "-c") == 0)
                        {
                             printf("\nCOMMENT\t: %s\n",argv[3]);
                              printf("\n-----------------COMMENT CHANGED SUCCESSFULLY------------\n");

                        }

                       
                    }
                }
                else
                {
                    printf("Error in validating file and arguments\n");
                }
            }
            else
            {
                printf("-------------------------------------------------------------------------\n");
                printf("Error: ./a.out : INVALID ARGUMENTS\n");
                printf("USAGE :\n");
                printf("To View     : ./a.out -v mp3filename\n");
                printf("To Edit     : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
                printf("To get Help : ./a.out --help\n");
                printf("-------------------------------------------------------------------------\n");
            }
        }
        else
        {
            printf("--------------------------------------------------------------------------\n");
            printf("Error: ./a.out : INVALID ARGUMENTS\n");
            printf("USAGE :\n");
            printf("To View     : ./a.out -v mp3filename\n");
            printf("To Edit     : ./a.out -e -t/-a/-A/-m/-y/-c changing_text mp3filename\n");
            printf("To get Help : ./a.out --help\n");
            printf("-------------------------------------------------------------------------\n");
        }
    }
}

Operationtype check_operation(char *argv[])
{
    if (strcmp(argv[1], "-v") == 0)
        return view;
    else if (strcmp(argv[1], "-e") == 0)
        return edit;
    else if (strcmp(argv[1], "--help") == 0)
        return help;
    else
        return unsupported;
}

Status read_and_validate_mp3_file(char *argv[], Taginfo *mp3Taginfo)
{
    char *ptr = strstr(argv[2], ".");
    if (ptr == NULL)
        return failure;
    if (strcmp(ptr, ".mp3") != 0)
    {

        return failure;
    }
    mp3Taginfo->fptr_mp3 = fopen(argv[2], "r");
    if (mp3Taginfo->fptr_mp3 == NULL)
    {

        return failure;
    }
    char str[3];
    fread(str, 1, 3, mp3Taginfo->fptr_mp3);
    if (strcmp(str, "ID3") != 0)
        return failure;
    fclose(mp3Taginfo->fptr_mp3);
    return success;
}