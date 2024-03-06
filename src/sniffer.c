#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>


// cria uma animação curta para a execução
void animacao_de_execucao(){
    char animacaoDeCaracteres[] = {'|', '/', '-', '\\'};

    int i;
    for (i = 0; i < 100; i++) {
        printf("\r%c Captura sera iniciada... (Ctrl + c para cancelar!) %c", animacaoDeCaracteres[i % 4], animacaoDeCaracteres[i % 4]);
        fflush(stdout);

        //pausa por 10 milissegundos
        usleep(10000);  
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
  // a sequência \033[2J limpa a tela, e \033[H move o cursor para a posição superior esquerda
  printf("\033[2J\033[H");

  int tamanho_maximo_do_pacote, tempo_limite;
  pcap_t *secaoDeCaptura;
  char cacheDeErros[PCAP_ERRBUF_SIZE];
  struct pcap_pkthdr cabecalhoInfo;
  const uint8_t *pacotes;

  // 2. ABRE A INTERFACE DE REDE WIRELESS  
  secaoDeCaptura = pcap_open_live("any", 1000000, 1, 1000, cacheDeErros); 

  if (secaoDeCaptura == NULL) {
    printf("Erro ao abrir a interface de rede: %s\n\n", cacheDeErros);
    return 1;
  }

  // 3. CRIA UMA PASTA PARA GUARDAR OS PACOTES

  char *pasta = "Pacotes";

  int status = mkdir(pasta, 0777);

  // verifica se a pasta foi criada com sucesso
  if (status == 0) {
    printf("Pasta '%s' criada com sucesso.\n\n", pasta);
  } else {
    printf("Erro ao criar a pasta '%s'.\n\n", pasta);
  }

  // 4. CAPTURA OS PACOTES

  // animação de espera
  animacao_de_execucao();
  // limpa o terminal
  printf("\033[2J\033[H");

  printf("CAPTURA EM EXECUÇÃO! (Ctrl + c para cancelar!)\n");

  // loop que captura os pacotes continuamete
  while(1){
    pacotes = pcap_next(secaoDeCaptura, &cabecalhoInfo);
    if (pacotes == NULL){
      break;
    }
  
    // grava o pacote em um arquivo

    // cria um nome de arquivo com base no carimbo de data/hora
    char nome_do_arquivo[255];
    sprintf(nome_do_arquivo, "Pacotes/%d.pcap", cabecalhoInfo.ts.tv_sec);

    // abre um arquivo de despejo para escrever
    pcap_dumper_t *pcapDumper = pcap_dump_open(secaoDeCaptura, nome_do_arquivo);

    if (pcapDumper == NULL){
      fprintf(stderr, "Erro ao abrir o arquivo de despejo: %s\n", nome_do_arquivo);
      return 1;
    }

    // grava o pacote no arquivo de despejo
    pcap_dump((unsigned char *) pcapDumper, &cabecalhoInfo, pacotes);
  
    // fecha o arquivo de despejo
    pcap_dump_close(pcapDumper);
  }

  // encerra interface de rede
  pcap_close(secaoDeCaptura);
  
  return 0;
}
