#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <arpa/inet.h>

int main(int agrc, char **argv){
    FILE *mpg_file = fopen(argv[1],"rb");
    unsigned char *prefix = malloc(3);
    unsigned char start_code_prefix[3] = {0x00,0x00,0x01};

    while(!feof(mpg_file)){
        fread(prefix, 3, 1, mpg_file);

        if(memcmp(start_code_prefix,prefix, 3) != 0){
            fseek(mpg_file,-2,SEEK_CUR);
        }
        else{
            unsigned char *code = malloc(1);
            fread(code,1,1, mpg_file);
            printf("--> Código: %.2x -- ", *code);

            if(*code == 0xb3){
                unsigned char byte1 = fgetc(mpg_file);
                unsigned char byte2 = fgetc(mpg_file);
                unsigned char byte3 = fgetc(mpg_file);
                unsigned char byte4 = fgetc(mpg_file);
                unsigned int largura = byte1 * 16 + (byte2 >> 4);
                unsigned int altura = ( byte2 & 0x0F) * 256 + byte3;
                unsigned int frame_rate = byte4 & 0x0f;

                float frame_rates[8] = {23.976, 24.000, 25.000, 29.970, 30.000, 50.000, 59.940, 60.000};

                switch(frame_rate){
                    case 1:
                        printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = %.3ffps\n", largura, altura, frame_rates[0]);
                        break;
                    case 2:
                        printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = %.3ffps\n", largura, altura, frame_rates[1]);
                        break;
                    case 3:
                        printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = %.3ffps\n", largura, altura, frame_rates[2]);
                        break;
                    case 4:
                        printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = %.3ffps\n", largura, altura, frame_rates[3]);
                        break;
                    case 5:
                        printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = %.3ffps\n", largura, altura, frame_rates[4]);
                        break;
                    case 6:
                        printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = %.3ffps\n", largura, altura, frame_rates[5]);
                        break;
                    case 7:
                        printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = %.3ffps\n", largura, altura, frame_rates[6]);
                        break;
                    case 8:
                        printf("Sequence Header -- Width = %d, Height = %d -- Frame rate = %.3ffps\n", largura, altura, frame_rates[7]);
                        break;
                }
            }
            else if(*code == 0xb8){
                printf("Group of Pictures\n");
            }
            else if(*code == 0x00){
                unsigned char byte1 = fgetc(mpg_file);
                unsigned char byte2 = fgetc(mpg_file);
                unsigned int type = (byte2 >> 3) & 0x07;

                if(type == 1){
                    printf("Picture -- Tipo = I\n");
                }
                else if(type == 2){
                    printf("Picture -- Tipo = P\n");
                }
                else if(type == 3){
                    printf("Picture -- Tipo = B\n");
                }
            }
            else if(*code == 0x01){
                printf("Slice\n");
            }
            else{
                printf("Tipo de stream não implementado\n");
            }
            free(code);
        }
    }
    free(prefix);
    fclose(mpg_file);
}