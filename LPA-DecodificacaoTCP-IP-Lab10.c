#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<arpa/inet.h>

typedef struct {
    uint8_t daddr[6]; // Endereco MAC de destino
    uint8_t saddr[6]; // Endereco MAC de origem (source)
    uint16_t protocol; // Protocolo da camada superior (IP!)
} ethernet_hdr_t;

typedef struct {
    uint8_t hdr_len:4, // Tamanho do Cabeçalho,
    version:4; // Versão do IP
    uint8_t tos; // Tipo de serviço
    uint16_t tot_len; // Tamanho total do IP
    uint16_t id; // Id do pa2 bits (4 bytescote
    uint16_t frag_off; // Fragmentado?
    uint8_t ttl; // Tempo de vida
    uint8_t protocol; // Protocolo da camada superior (TCP!)
    uint16_t check; // Checksum
    uint8_t saddr[4]; // Endereço IP de origem
    uint8_t daddr[4]; // Endereço IP de destino
} ip_hdr_t;

typedef struct {
    uint16_t sport; // Porta TCP de origem
    uint16_t dport; // Porta TCP de destino
    uint32_t seq; // Sequência
    uint32_t ack_seq; // Acknowledgement
    uint8_t reservado:4, // Não usado
    hdr_len:4; // Tamanho do cabecalho
    uint8_t flags; // Flags do TCP
    uint16_t window; // Tamanho da janela
    uint16_t check; // Checksum
    uint16_t urg_ptr; // Urgente
} tcp_hdr_t;

void main(int argc, char **argv){
    FILE *file = fopen(argv[1],"rb");
    if(file==NULL){printf("ERRO AO ABRIR O ARQUIVO\n");exit(0);}

    ethernet_hdr_t *ethernet = malloc(sizeof(ethernet_hdr_t));
    ip_hdr_t *ip = malloc(sizeof(ip_hdr_t));
    tcp_hdr_t *tcp = malloc(sizeof(tcp_hdr_t));

    printf("Lendo Ethernet ..\n");
    fread(ethernet,sizeof(ethernet_hdr_t),1,file);
    printf("\t--> MAC de Origem:\t");
    printf("%.2x",ethernet->saddr[0]);
    for(int i=1;i<6;i++){
        printf(":%.2x",ethernet->saddr[i]);
    }
    printf("\n\t--> MAC de Destino:\t");
    printf("%.2x",ethernet->daddr[0]);
    for(int i=1;i<6;i++){
        printf(":%.2x",ethernet->daddr[i]);
    }

    fread(ip,sizeof(ip_hdr_t),1,file);
    printf("\nLendo IP ..\n");
    printf("\t--> Versão do IP: %d\n",ip->version);
    printf("\t--> Tamanho do cabeçalho: %d bytes\n", ip->hdr_len*4);
    printf("\t--> Tamanho do pacote: %d bytes\n", ntohs(ip->tot_len));
    printf("\t--> Endereço IP de Origem: ");
    printf("%d",ip->saddr[0]);
    for(int i=1;i<4;i++){
        printf(".%d",ip->saddr[i]);
    }
    printf("\n\t--> Endereço IP de Destino: ");
    printf("%d",ip->daddr[0]);
    for(int i=1;i<4;i++){
        printf(".%d",ip->daddr[i]);
    }
    fseek(file,ip->hdr_len*4 - sizeof(ip_hdr_t),SEEK_CUR);

    fread(tcp,sizeof(tcp_hdr_t),1,file);
    printf("\nLendo TCP ..\n");
    printf("\t--> Porta de Origem: %d\n",ntohs(tcp->sport));
    printf("\t--> Porta de Destino: %d\n",ntohs(tcp->dport));
    printf("\t--> Tamanho dos dados: %d bytes\n",tcp->hdr_len*4);
    fseek(file,tcp->hdr_len*4 - sizeof(tcp_hdr_t),SEEK_CUR);

    int tam_dados = ntohs(ip->tot_len) - (ip->hdr_len*4) - (tcp->hdr_len*4);
    char ch;
    printf("Lendo Dados (HTTP) ..\n");
    printf("\t--> Tamanhdo dos dados: %d bytes\n\t--> Dados:\n",tam_dados);
    for(int i=0;i<tam_dados;i++){
        ch = fgetc(file);
        printf("%c",ch);
    }
}