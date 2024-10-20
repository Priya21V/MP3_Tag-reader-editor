
#ifndef MAIN_H
#define MAIN_H
#include "types.h"


typedef struct _Taginfo
{
    FILE *fptr_mp3;
    char frame_id[4];

    char *title_tag;
    uint title_size;
        
    char *artist_tag;
    uint artist_size;

    char *album_tag;
    uint album_size;

    char *year;
    uint year_size;

    char *content_type;
    uint content_type_size;

    char *comments;
    uint comment_size;
}Taginfo;

typedef struct _Editinfo
{
    FILE *fptr_src;
    FILE *fptr_temp;

    char frame_id[5];
    char frame_id_content[50];
    uint frame_id_size;
}Editinfo;


Operationtype check_operation(char *argv[]);
Status read_and_validate_mp3_file(char *argv[],Taginfo *mp3Taginfo);
Status view_tag(char *argv[],Taginfo *mp3Taginfo);
Status get_and_display_data (const char* str_frame, const char* str_Id, char* frame_Id, uint* tag_size, char* tag, FILE* fptr);
Status read_and_validate_edit(char *argv[],Editinfo *editinfo);
Status edit_tag(char *argv[], Editinfo *editinfo);
Status copy_data(Editinfo *editinfo, char flag);
Status copy_remaining_data(Editinfo *editinfo);
Status copy_back_to_src(Editinfo *editinfo);
#endif