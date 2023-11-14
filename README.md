# Esta é uma ferramenta de terminal escrita em C que captura os pacotes da rede de forma promíscua.

Bem-vindo ao projeto que desenvolvemos na materia de Técnicas de Desenvolvimento de Algoritmos no curso de ciencia da computação!
Esta é uma ferramenta de terminal simples, escrita em C, que permite a captura de pacotes de rede.
Ideal para quem deseja analisar o tráfego de rede, realizar diagnósticos ou aprender sobre o funcionamento interno das comunicações de dados.

## Como Usar
Certifique-se de ter a biblioteca libpcap instalada

1. **Compilação:**
   Certifique-se de ter um compilador C instalado no seu sistema.
   Para compilar o código-fonte, execute o seguinte comando no terminal:
   ~gcc -o pacoteswlan0 PacotesWlan0.c -lpcap

3. **Execução**
   #gcc ./pacoteswlan0

Este capturador suporta a captura de pacotes de rede em tempo real.
Permite a escolha to tamanho maximo de cada pacote e o tempo limite para capturalos.
