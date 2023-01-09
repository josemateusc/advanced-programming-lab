#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

void generator(int TAM_MAX, char *seed){
    int possibilities = 0;
    int seed_size = strlen(seed);

    for(int i=1;i<=TAM_MAX;i++){
        possibilities = pow(seed_size,i); //Number of combinations
        char pwd[i];

        for(int j=0; j<possibilities; j++){
            int u = j;
            for(int k=0; k<i;k++){
                pwd[k] = seed[u%seed_size];
                u /= seed_size;
            }
            printf("%s\n", pwd);
        }
    }
}

int main(int argc, char **argv){
    generator(atoi(argv[1]), argv[2]);
    return 0;
}