#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


//cria uma animação curta para a execução
void animacao_de_execucao(){
    char animacaoDeCaracteres[] = {'|', '/', '-', '\\'};

    int i;
    for (i = 0; i < 100; i++) {
        printf("\r%c Captura sera iniciada... (Ctrl + c para cancelar!) %c", animacaoDeCaracteres[i % 4], animacaoDeCaracteres[i % 4]);
        fflush(stdout);
        usleep(10000);  //pausa por 10 milissegundos
    }
    printf("\n");
}

int main(){

  // 1. VERIFICA SE ESTA SENDO EXECUTADO COMO ROOT

  if (geteuid() != 0) {
      printf("Este programa precisa de acesso root. Por favor, execute como root.\n");
      exit(EXIT_FAILURE);
  }
  //se estiver sendo executado como root, continue
  printf("Operações privilegiadas podem ser realizadas aqui.\n\n");

  
  //comando "clear" para limpar o terminal
  printf("\033[2J\033[H"); // a sequência \033[2J limpa a tela, e \033[H move o cursor para a posição superior esquerda

  
  pcap_t *secaoDeCaptura;
  char cacheDeErros[PCAP_ERRBUF_SIZE];
  struct pcap_pkthdr cabecalhoInfo;
  const uint8_t *pacotes;

  // 2. ABRE A INTERFACE DE REDE WIRELESS
  
  int tamanho_maximo_do_pacote;
  printf("Tamanho maximo do pacote(em bytes)?\n");
  scanf("%i", &tamanho_maximo_do_pacote);
  if(tamanho_maximo_do_pacote = 0){
    tamanho_maximo_do_pacote = 1000000;
  }else{}

  int tempo_limite;
  printf("Tempo limite para cada pacote? (em ms)\n");
  scanf("%i", &tempo_limite);
  if(tempo_limite = 0){
    int tempo_limite = 1000;
  }else{}
  
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
    printf("Erro ao abrir a interface de rede: %s\n\n", cacheDeErros);
    return 1;
  }

  // 3. CRIA PASTA PARA GUARDAR OS PACOTES

  char *pasta = "Pacotes";

  int status = mkdir(pasta, 0777);

  //verifica se a pasta foi criada com sucesso
  if (status == 0) {
    printf("Pasta '%s' criada com sucesso.\n\n", pasta);
  } else {
    printf("Erro ao criar a pasta '%s'.\n\n", pasta);
  }

  // 4. CAPTURA OS PACOTES

  //animação de espera
  animacao_de_execucao();
  //limpa o terminal
  printf("\033[2J\033[H");

  printf("CAPTURA EM EXECUÇÃO! (Ctrl + c para cancelar!)\n");

  //loop que captura os pacotes continuamete
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

    //cria um nome de arquivo com base no carimbo de data/hora
    char filename[255];
    sprintf(filename, "Pacotes/%d.pcap", cabecalhoInfo.ts.tv_sec);

    //abre um arquivo de despejo para escrever
    pcap_dumper_t *pcapDumper = pcap_dump_open(secaoDeCaptura, filename);

    if (pcapDumper == NULL){
      fprintf(stderr, "Erro ao abrir o arquivo de despejo: %s\n", filename);
      return 1;
    }

    //grava o pacote no arquivo de despejo
    pcap_dump((unsigned char *) pcapDumper, &cabecalhoInfo, pacotes);
  
    //fecha o arquivo de despejo
    pcap_dump_close(pcapDumper);
  }

  //encerra interface de rede
  pcap_close(secaoDeCaptura);
  

  return 0;
}