#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>

void main(int argc, char **argv){
    FILE *mpeg_file = fopen(argv[1],"rb");
    unsigned int i = 1;
    char filename[20];
    sprintf(filename,"video_parte_%d.mpg",i);
    printf("Criando arquivo %s ..\n",filename);
    
    FILE *mpeg_file_out = fopen(filename,"wb");

    long int buffer_size = *argv[2] * 1000000;
    unsigned char buffer = malloc(buffer_size);
    unsigned char byte[4];

    fseek(mpeg_file_out, 0L, SEEK_END);
    long int new_file_size = ftell(mpeg_file_out);
    fseek(mpeg_file_out, 0L, SEEK_SET);


    while(!feof(mpeg_file)){

        fread(byte,sizeof(unsigned char),4,mpeg_file);

        if (memcmp(byte,"\x00\x00\x01\xB3") != 0 && !feof(mpeg_file)){
            buffer = byte[1];
            //retorna 3 bytes?
            continue;
        }
        else{
            if(new_file_size + sizeof(buffer) > buffer_size){
                fclose(mpeg_file_out);
                i++;
                sprintf(filename,"video_parte_%d.mpg",i);
                printf("Criando arquivo %s ..\n",filename);
                *mpeg_file_out = fopen(filename,"wb");
                fseek(mpeg_file_out, 0L, SEEK_END);
                new_file_size = ftell(mpeg_file_out);
                fseek(mpeg_file_out, 0L, SEEK_SET);

                fwrite(buffer, sizeof(buffer),1,mpeg_file_out);

                new_file_size = sizeof(buffer);
            }
            else if(new_file_size + sizeof(buffer) <= buffer_size){
                fwrite(buffer, sizeof(buffer),1,mpeg_file_out);
                new_file_size += sizeof(buffer);
            }
            else if(feof(mpeg_file)){
                fclose(mpeg_file_out);
                break;
            }
            else{
                memcpy(buffer,byte,4);
                //Sete o tamanho do buffer para 4, de forma que o buffer comece já com o código Sequence lido.
            }
        }
    }
    free(buffer);
    fclose(mpeg_file);
}