#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "main.h"

Status read_and_validate_edit(char *argv[], Editinfo *editinfo)
{
    if (strcmp(argv[2], "-t") == 0)
    {
        printf("\n------------- CHANGE SONG TITTLE-------------\n");
        strcpy(editinfo->frame_id, "TIT2");
    }
    else if (strcmp(argv[2], "-a") == 0)
    {
        printf("\n------------- CHANGE ARTIST NAME-------------\n");
        strcpy(editinfo->frame_id, "TPE1");
    }
    else if (strcmp(argv[2], "-A") == 0)
    {
         printf("\n------------- CHANGE ALBUM NAME-------------\n");
        strcpy(editinfo->frame_id, "TALB");
    }
    else if (strcmp(argv[2], "-y") == 0)
    {
         printf("\n------------- CHANGE YEAR-------------\n");
        strcpy(editinfo->frame_id, "TYER");
    }
    else if (strcmp(argv[2], "-m") == 0)
    {
        printf("\n------------- CHANGE CONTENT-------------\n");
        strcpy(editinfo->frame_id, "TCON");
    }
    else if (strcmp(argv[2], "-c") == 0)
    {
        printf("\n------------- CHANGE COMMENT-------------\n");
        strcpy(editinfo->frame_id, "COMM");
    }
    else
    {
        return failure;
    }
    char *ptr = strstr(argv[4], ".");
    if (ptr == NULL)
        return failure;
    if (strcmp(ptr, ".mp3") != 0)
    {
        return failure;
    }
    editinfo->fptr_src = fopen(argv[4], "r");
    if (editinfo->fptr_src == NULL)
    {
        return failure;
    }
    char str[3];
    fread(str, 1, 3, editinfo->fptr_src);
    if (strcmp(str, "ID3") != 0)
        return failure;
    strcpy(editinfo->frame_id_content, argv[3]);
    editinfo->frame_id_size = sizeof(editinfo->frame_id_content);
    fclose(editinfo->fptr_src);
    return success;
}

Status edit_tag(char *argv[], Editinfo *editinfo)
{
    editinfo->fptr_src = fopen(argv[4], "r");
    editinfo->fptr_temp = fopen("temp.mp3", "w");

    fseek(editinfo->fptr_src, 0, SEEK_SET);
    fseek(editinfo->fptr_temp, 0, SEEK_SET);

    char str[10];
    int count;
    count = fread(str, 1, 10, editinfo->fptr_src);
    if (count < 10)
    {
        printf("ERROR: Unable to read from the MP3 file.\n");
        return failure;
    }
    else
    {
        count = fwrite(str, 1, 10, editinfo->fptr_temp);
        if (count < 10)
        {
            printf("ERROR: Unable to write on the MP3 file.\n");
            return failure;
        }
    }

    char *frame_tag[6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    for (int i = 0; i < 6; i++)
    {
        count = fread(str, 1, 4, editinfo->fptr_src);
        if (count < 4)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return failure;
        }
        else
        {
            count = fwrite(str, 1, 4, editinfo->fptr_temp);
            if (count < 4)
            {
                printf("ERROR: Unable to write on the MP3 file.\n");
                return failure;
            }
            else
            {
                if ((strncmp(editinfo->frame_id, frame_tag[i], 4)) == 0)
                {
                    Status ret = copy_data(editinfo, 1);
                    if (ret == failure)
                    {
                        printf("ERROR: Unable to copy the updated Data from Source to Destination '.mp3' file.\n");
                        return failure;
                    }
                }
                else
                {
                    Status ret = copy_data(editinfo, 0);
                    if (ret == failure)
                    {
                        printf("ERROR: Unable to copy the remaining Data from Source to Destination '.mp3' file.\n");
                        return failure;
                    }
                }
            }
        }
    }

    Status ret;
    ret = copy_remaining_data(editinfo);
    if (ret == failure)
    {
        printf("ERROR: Unable to copy the remaining Data from Source to Destination '.mp3' file.\n");
        return failure;
    }

    fclose(editinfo->fptr_src);
    fclose(editinfo->fptr_temp);

    editinfo->fptr_src = fopen(argv[4], "w");
    editinfo->fptr_temp = fopen("temp.mp3", "r");

    ret = copy_back_to_src(editinfo);
    if (ret == failure)
    {
        printf("ERROR: Unable to copy the remaining Data from Destination to Source '.mp3' file.\n");
        return failure;
    }
    return success;
}

Status copy_data(Editinfo *editinfo, char flag)
{
    if (flag == 1)
    {
        uint old_size, new_size;
        int count;
        count = fread(&old_size, 4, 1, editinfo->fptr_src);
        if (count < 1)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return failure;
        }
        else
        {
            char *pos = (char *)&old_size;
            int j = sizeof(uint) - 1;
            for (int i = 0; i < ((sizeof(uint)) / 2); i++)
            {
                char temp = pos[i];
                pos[i] = pos[j];
                pos[j] = temp;
                j--;
            }
        }

        new_size = editinfo->frame_id_size;
        char *pos = (char *)&editinfo->frame_id_size;

        int j = sizeof(uint) - 1;
        for (int i = 0; i < ((sizeof(uint)) / 2); i++)
        {
            char temp = pos[i];
            pos[i] = pos[j];
            pos[j] = temp;
            j--;
        }

        count = fwrite(&editinfo->frame_id_size, 4, 1, editinfo->fptr_temp);
        if (count < 1)
        {
            printf("ERROR: Unable to write on the MP3 file.\n");
            return failure;
        }

        char f[3];
        count = fread(f, 1, 3, editinfo->fptr_src);
        if (count < 3)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return failure;
        }
        else
        {
            count = fwrite(f, 1, 3, editinfo->fptr_temp);
            if (count < 3)
            {
                printf("ERROR: Unable to write on the MP3 file.\n");
                return failure;
            }
        }

        count = fwrite(editinfo->frame_id_content, 1, (new_size - 1), editinfo->fptr_temp);
        if (count < (new_size - 1))
        {
            printf("ERROR: Unable to write on the MP3 file.\n");
            return failure;
        }
        fseek(editinfo->fptr_src, (long)(old_size - 1), SEEK_CUR);
    }
    else
    {
        uint size;
        int count;
        count = fread(&size, 4, 1, editinfo->fptr_src);
        if (count < 1)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return failure;
        }
        else
        {
            count = fwrite(&size, 4, 1, editinfo->fptr_temp);
            if (count < 1)
            {
                printf("ERROR: Unable to write on the MP3 file.\n");
                return failure;
            }
        }
        char *pos = (char *)&size;
        int j = sizeof(uint) - 1;
        for (int i = 0; i < ((sizeof(uint)) / 2); i++)
        {
            char temp = pos[i];
            pos[i] = pos[j];
            pos[j] = temp;
            j--;
        }

        char f[3];
        count = fread(f, 1, 3, editinfo->fptr_src);
        if (count < 3)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return failure;
        }
        else
        {
            count = fwrite(f, 1, 3, editinfo->fptr_temp);
            if (count < 3)
            {
                printf("ERROR: Unable to write on the MP3 file.\n");
                return failure;
            }
        }

        char str[size - 1];
        count = fread(str, 1, (size - 1), editinfo->fptr_src);
        if (count < (size - 1))
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return failure;
        }
        else
        {
            str[size - 1] = '\0';
            //printf(" 1= %s.\n", str);
            count = fwrite(str, 1, (size - 1), editinfo->fptr_temp);
            if (count < (size - 1))
            {
                printf("ERROR: Unable to write on the MP3 file.\n");
                return failure;
            }
        }

    }
    return success;
}

Status copy_remaining_data(Editinfo *editinfo)
{
    //printf("Copy Remaining Data:\n");
    char ch[1024];
    int count;
    long start = ftell(editinfo->fptr_src);

    fseek(editinfo->fptr_src, 0L, SEEK_END);
    long end = ftell(editinfo->fptr_src);

    fseek(editinfo->fptr_src, start, SEEK_SET);

    while ((ftell(editinfo->fptr_src)) != end)
    {
        count = fread(ch, 1, 1024, editinfo->fptr_src);
        if (count < 0)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return failure;
        }
        else
        {
            fwrite(ch, 1, count, editinfo->fptr_temp);
        }
    }

    return success;
}

Status copy_back_to_src(Editinfo *editinfo)
{
    //printf("Copy Data Back to Source:\n");
    fseek(editinfo->fptr_src, 0L, SEEK_SET);
    fseek(editinfo->fptr_temp, 0L, SEEK_SET);
    int count;
    char ch[1024];
    long start = ftell(editinfo->fptr_temp);

    fseek(editinfo->fptr_temp, 0, SEEK_END);
    long end = ftell(editinfo->fptr_temp);

    fseek(editinfo->fptr_temp, 0, SEEK_SET);

    while ((ftell(editinfo->fptr_temp)) != end)
    {
        count = fread(ch, 1, 1024, editinfo->fptr_temp);
        if (count < 0)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return failure;
        }
        else
        {
            fwrite(ch, 1, count, editinfo->fptr_src);
        }
    }
    return success;
}
