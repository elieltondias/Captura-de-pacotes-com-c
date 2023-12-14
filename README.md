# Esta é codigo simples escrita em C que captura os pacotes da rede.
Esta é um codigo simples, escrita em C, que permite a captura de pacotes de rede.
Ideal para quem deseja analisar o tráfego de rede, realizar diagnósticos ou aprender sobre o funcionamento interno das comunicações de dados.

## Como Usar
Certifique-se de ter a biblioteca libpcap-dev ou libpcap-devel (depende da distribuição) instalada

Certifique-se de ter um compilador C instalado no seu sistema.

Este codigo suporta a captura de pacotes de rede em tempo real.

A interface de rede pode ser alterada no arquivo /src/capturar_pacotes.c

Para compilar o código-fonte, execute o seguinte comando no terminal:
1. **Compilação:**
   gcc -o sniffer sniffer.c -lpcap

3. **Execução**
   sudo ./sniffer
