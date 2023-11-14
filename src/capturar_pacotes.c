#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


// Cria uma animação curta para a execução
void animacao_de_execucao(){
    char animacaoDeCaracteres[] = {'|', '/', '-', '\\'};

    int i;
    for (i = 0; i < 200; i++) {
        printf("\r%c Voce tem 30 segundos caso queira cancelar antes de iniciar... (Ctrl + c para cancelar!) %c", animacaoDeCaracteres[i % 4], animacaoDeCaracteres[i % 4]);
        fflush(stdout);
        usleep(100000);  // pausa por 100 milissegundos (100000 microssegundos)
    }
    printf("\n");
}

int main(){
  
  pcap_t *secaoDeCaptura;
  char cacheDeErros[PCAP_ERRBUF_SIZE];
  struct pcap_pkthdr cabecalhoInfo;
  const uint8_t *pacotes;

  //abre a interface de rede wireless
  
  int tamanho_maximo_do_pacote;
  printf("QUAL O TAMANHO MAXIMO DO PACOTE(em bytes)?\n");
  scanf("%i", &tamanho_maximo_do_pacote);

  if (tamanho_maximo_do_pacote == NULL){
    int tamanho_maximo_do_pacote = 1000000;
  }else if(tamanho_maximo_do_pacote == 0){
   int tamanho_maximo_do_pacote = 1000000;
  }

  int tempo_limite;
  printf("QUAL O TEMPO LIMITE PARA CAPTURAR CADA PACOTE? (em ms)\n");
  scanf("%i", &tempo_limite);

  if (tempo_limite == NULL){
    int tempo_limite = 1000 ;
  }else if(tempo_limite == 0){
    int tempo_limite = 1000;
  }
  
  secaoDeCaptura = pcap_open_live("wlan0", tamanho_maximo_do_pacote, 1, tempo_limite, cacheDeErros); 
  // secaoDeCaptura = pcap_open_live("interface de rede", tamanho do pacote em bytes, 1 para ativar o modo promiscuo, tempo limite para capturar o pacote, eliminaar cache e erros)
 
  /*
    wlan0 é a interface de rede que sera usada,tambem pode ser substituida por eth0 para uma interface de rede cabeada
    logo em seguida o tamanho maximo do pacote 1000000(definida em bytes) igual a 1mb
    O valor 1 define o modo promiscuo se for 1, a interface captura todos os pacotes(inclusive os que são destinados a outras maquinas)
    o valor 1000 define o tempo limite para a captura do pacote em ms
    errbuf n permite que mensagens de erro sejam armazenadas
  */

  if (secaoDeCaptura == NULL) {
    printf("Erro ao abrir a interface de rede: %s\n", cacheDeErros);
    return 1;
  }

  //cria a pasta para guardar os pacotes
  char *pasta = "Pacotes";

  int status = mkdir(pasta, 0777);

  //verifica se a pasta foi criada com sucesso
  if (status == 0) {
    printf("Pasta '%s' criada com sucesso.\n", pasta);
  } else {
    printf("Erro ao criar a pasta '%s'.\n", pasta);
  }

  //captura os pacotes
  printf("A captura de pacotes sera iniciada iniciada em 20 segundos!\n\n");
  animacao_de_execucao();
  printf("CAPTURA EM EXECUÇÃO!\n");

  // Loop que captura os pacotes continuamete
  while(1){
    pacotes = pcap_next(secaoDeCaptura, &cabecalhoInfo);
    if (pacotes == NULL){
      break;
    }
  
    //grava o pacote em um arquivo

    /*
    Um arquivo .pcap contém essencialmente uma coleção de pacotes de rede que foram interceptados por meio de técnicas de detecção de pacotes. 
    Esses pacotes encapsulam dados transmitidos por uma rede. 
    A importância de um arquivo pcap reside na sua capacidade de facilitar a identificação de diversas anomalias nas operações da rede.
    */

    // Cria um nome de arquivo com base no carimbo de data/hora
    char filename[255];
    sprintf(filename, "Pacotes/%d.pcap", cabecalhoInfo.ts.tv_sec);

    // Abre um arquivo de despejo para escrever
    pcap_dumper_t *pcapDumper = pcap_dump_open(secaoDeCaptura, filename);

    if (pcapDumper == NULL){
      fprintf(stderr, "Erro ao abrir o arquivo de despejo: %s\n", filename);
      return 1;
    }

    // Grava o pacote no arquivo de despejo
    pcap_dump((unsigned char *) pcapDumper, &cabecalhoInfo, pacotes);
  
    // Fecha o arquivo de despejo
    pcap_dump_close(pcapDumper);
  }

  //encerra interface de rede
  pcap_close(secaoDeCaptura);
  

  return 0;
}