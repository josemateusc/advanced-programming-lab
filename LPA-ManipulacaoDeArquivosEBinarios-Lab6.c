#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

struct zip_file_hdr {
int signature;
short version;
short flags;
short compression;
short mod_time;
short mod_date;
int crc;
int compressed_size;
int uncompressed_size;
short name_length;
short extra_field_length;
} __attribute__ ((packed));

void main(int argc, char **argv){
    //
    FILE *zip_file = fopen(argv[1], "rb");
    struct zip_file_hdr *file_hdr = malloc(sizeof(struct zip_file_hdr));
    int i=0;

    while(!feof(zip_file)){
        fread(file_hdr, sizeof(struct zip_file_hdr), 1, zip_file);
        if(file_hdr->signature == 0x04034b50){
            i++; 
            printf("Arquivo %d ..\n",i);
            char *filename = malloc(file_hdr->name_length+1);

            fread(filename, file_hdr->name_length, 1, zip_file);

            strcat(filename,"\0");

            printf("\t--> Nome do Arquivo: %s\n", filename);
            printf("\t--> Tamanho Compactado: %d\n", file_hdr->compressed_size);
            printf("\t--> Tamanho Descompactado: %d\n", file_hdr->uncompressed_size);

            free(filename);
            fseek(zip_file, file_hdr->extra_field_length + file_hdr->compressed_size, SEEK_CUR);
        }
        // printf(" --> Assinatura: %.8x", file_hdr->signature); // Imprime hexadecimal
        // printf(" --> Tamanho Compactado: %d\n", file_hdr->compressed_size);
    }
    free(file_hdr);
    fclose(zip_file);
}