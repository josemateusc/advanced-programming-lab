#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>

void main(int argc, char **argv){
    FILE *mpeg_file = fopen(argv[1],"rb");
    unsigned int i = 1;
    char filename[20];
    sprintf(filename,"video_parte_%d.mpg",i);
    printf("Criando arquivo %s ..\n",filename);
    
    FILE *mpeg_file_out = fopen(filename,"wb");

    long int buffer_size_max = *argv[2] * 1000000;//1024 *1024;
    long int buffer_size = 0;
    char *buffer = malloc(sizeof(char)*buffer_size_max);
    char byte[4];

    long int new_file_size = 0;


    while(!feof(mpeg_file)){

        fread(byte,4,1,mpeg_file);

        if(feof(mpeg_file)){
                fclose(mpeg_file_out);
                break;
        }

        if (memcmp(byte,"\x00\x00\x01\xB3",4) != 0){
            buffer[buffer_size] = byte[0];
            buffer_size++;
            fseek(mpeg_file,-3,SEEK_CUR);
            continue;
        }
        else{
            if(new_file_size + buffer_size > buffer_size_max){
                fclose(mpeg_file_out);
                i++;
                sprintf(filename,"video_parte_%d.mpg",i);
                printf("Criando arquivo %s ..\n",filename);
                mpeg_file_out = fopen(filename,"wb");
                //fseek(mpeg_file_out, 0L, SEEK_END);
                //new_file_size = ftell(mpeg_file_out);
                //fseek(mpeg_file_out, 0L, SEEK_SET);

                fwrite(buffer, buffer_size,1,mpeg_file_out);

                new_file_size = buffer_size;
            }
            else {//if(new_file_size + buffer_size <= buffer_size_max){
                fwrite(buffer, buffer_size,1,mpeg_file_out);
                new_file_size += buffer_size;
            }
            // else{
                memcpy(buffer,byte,4);
                buffer_size = 4;
            // }
        }
    }
    fclose(mpeg_file);
}