#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "main.h"

Status view_tag(char *argv[], Taginfo *mp3Taginfo)
{
    mp3Taginfo->fptr_mp3 = fopen(argv[2], "r");
    if (mp3Taginfo->fptr_mp3 == NULL)
        return failure;
    fseek(mp3Taginfo->fptr_mp3, 10, SEEK_SET);
    Status ret;
    ret = get_and_display_data ("TITTLE", "TIT2", mp3Taginfo->frame_id, &mp3Taginfo->title_size,mp3Taginfo->title_tag, mp3Taginfo->fptr_mp3);
    if (ret == failure)		
    {
        printf ("ERROR:Frame ID Contents cannot be displayed.\n");
        return failure;
    }
    ret= get_and_display_data ("ARTIST", "TPE1", mp3Taginfo->frame_id, &mp3Taginfo->artist_size,mp3Taginfo->artist_tag, mp3Taginfo->fptr_mp3);
    if (ret == failure)		
    {
        printf ("ERROR: Unable to read Frame ID contents\n");
        return failure;
    }
    ret= get_and_display_data ("ALBUM", "TALB", mp3Taginfo->frame_id, &mp3Taginfo->album_size,mp3Taginfo->album_tag, mp3Taginfo->fptr_mp3);
    if (ret == failure)		
    {
        printf ("ERROR: Unable to read Frame ID contents\n");
        return failure;
    }
    ret= get_and_display_data ("YEAR", "TYER", mp3Taginfo->frame_id, &mp3Taginfo->year_size,mp3Taginfo->year, mp3Taginfo->fptr_mp3);
    if (ret == failure)		
    {
        printf ("ERROR: Unable to read Frame ID contents\n");
        return failure;
    }
    ret= get_and_display_data ("MUSIC", "TCON", mp3Taginfo->frame_id, &mp3Taginfo->content_type_size,mp3Taginfo->content_type, mp3Taginfo->fptr_mp3);
    if (ret == failure)		
    {
       printf ("ERROR: Unable to read Frame ID contents\n");
        return failure;
    }
    ret= get_and_display_data ("COMMENT", "COMM", mp3Taginfo->frame_id, &mp3Taginfo->comment_size,mp3Taginfo->comments, mp3Taginfo->fptr_mp3);
    if (ret == failure)		
    {
        printf ("ERROR: Unable to read Frame ID contents\n");
        return failure;
    }
}
Status get_and_display_data(const char *str_frame, const char *str_Id, char *frame_Id, uint *tag_size, char *tag, FILE *fptr)
{
    int count;
    count = fread(frame_Id, 1, 4, fptr);
    if (count < 4)
    {
        printf("ERROR: Unable to read from the MP3 file.\n");
        return failure;
    }
    else
    {
        if ((strncmp(frame_Id, str_Id, 4)) == 0)
        {
            count = fread(tag_size, 4, 1, fptr);

            if (count < 1)
            {
                printf("ERROR: Unable to read from the MP3 file.\n");
                return failure;
            }
            else
            {
                char *pos = (char *)tag_size;
                int j = sizeof(uint) - 1;
                for (int i = 0; i < ((sizeof(uint)) / 2); i++)
                {
                    char temp = pos[i];
                    pos[i] = pos[j];
                    pos[j] = temp;
                    j--;
                }
                tag = malloc((*tag_size) * sizeof(char));

                fseek(fptr, 3, SEEK_CUR);
                count = fread(tag, 1, (*tag_size - 1), fptr);
                if (count < (*tag_size - 1))
                {
                    printf("ERROR: Unable to read from the MP3 file.\n");
                    return failure;
                }
                else
                {
                    tag[*tag_size - 1] = '\0';
                    printf("%-15s : %-25s\n", str_frame, tag);
                }
            }
        }
        else
        {
            printf("ERROR: %s Frame ID not found.\n", str_Id);
            return failure;
        }
    }

    free(tag);
    tag = NULL;
    return success;
}
