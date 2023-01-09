#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <arpa/inet.h>

struct png_chunk{
    int length;
    char type[4];
} __attribute__ ((packed));

struct png_chunk_IHDR{
    int width;
    int height;
    char bit_depth;
    char colour_type;
    char compression_method;
    char filter_method;
    char interlace_method;
} __attribute__ ((packed));

void main(int argc, char **argv){
    struct png_chunk *chunk = malloc(sizeof(struct png_chunk)); 
    struct png_chunk_IHDR *IHDR = malloc(sizeof(struct png_chunk_IHDR));
    FILE *png_file = fopen(argv[1],"rb");
    fseek(png_file, 8, SEEK_CUR);
    int i=0;
    while(!feof(png_file)){
        i++;
        fread(chunk, sizeof(struct png_chunk), 1, png_file);
        printf("\nLendo chunk %d:\n",i);
        printf("\t--> Tamanho: %d\n", ntohl(chunk->length));
        printf("\t--> Tipo: %.4s\n",chunk->type);

        if(strcmp(chunk->type,"IHDR") == 0){
            fread(IHDR, sizeof(struct png_chunk_IHDR), 1, png_file);
            printf("\t\t--> Largura: %d\n",ntohl(IHDR->width));
            printf("\t\t--> Altura: %d\n",ntohl(IHDR->height));
            fseek(png_file, 4, SEEK_CUR);
        }
        else if(strcmp(chunk->type,"IEND") == 0){
            break;
        }
        else{
            fseek(png_file, ntohl(chunk->length)+4, SEEK_CUR);
        }
    }
    free(chunk);
    free(IHDR);
    fclose(png_file);

}