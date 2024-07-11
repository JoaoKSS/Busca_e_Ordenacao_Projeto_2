#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

int acessos = 0;

void printExecutionTime(chrono::steady_clock::time_point start, chrono::steady_clock::time_point end) {
    auto diff = end - start;
    cout << "Tempo de execucao em segundos: "
         << chrono::duration<double, ratio<1>>(diff).count() << " s" << endl;
}

void imprimir(const char* texto, int inicio, int fim, int linhaAtual) {
    cout << "[Linha " << linhaAtual << "]\tPadrao encontrado:\t";
    for (int i = inicio; i <= fim; i++) {
        cout << texto[i];
    }
    cout << endl;
}


//modelando a tabela de prefixo (LPS)
vector<int> lps(const char* pattern, int tamPattern){

  vector<int> LPS(tamPattern, 0);
  int j = 0;
  int i = 1;

  while (i < tamPattern) {
    
      if (pattern[i] == pattern[j]) {
          j++;
          LPS[i] = j;
          i++;
      } else {
          if (j != 0) {
              j = LPS[j - 1];
          } else {
              LPS[i] = 0;
              i++;
          }
      }
  } //fim do while

  return LPS; //retorna o vetor (tabela) de prefixos
}


//buscando o padrão pattern no texto, algoritmo KMP
void kmp(const char* texto, const char* pattern, int linhaAtual) {
  
    int tamPattern = strlen(pattern);
    int tamTexto = strlen(texto);

    //modelando a tabela de prefixo (LPS), chama a função lps()
    vector<int> LPS = lps(pattern, tamPattern);
    //realizando a busca usando o KMP
    int j = 0; 
    int posInicial = -1;

    for (int i = 0; i < tamTexto; ) {
      
        if (pattern[j] == texto[i]) {
            if (posInicial == -1) {
                posInicial = i;
            }
            i++;
            j++;
        }

        if (j == tamPattern) {
            //significa que o padrão foi encontrado
            int posFinal = i - 1;
            while (posInicial >= 0 && texto[posInicial] != ' ' && texto[posInicial] != '\n') {
                posInicial--;
            }
            while (posFinal < tamTexto && texto[posFinal] != ' ' && texto[posFinal] != '\n') {
                posFinal++;
            }
            imprimir(texto, posInicial + 1, posFinal - 1, linhaAtual);
            j = LPS[j - 1];
            posInicial = -1; //reinicia a posição inicial da correspondência
            acessos++;
        } else if (i < tamTexto && pattern[j] != texto[i]) {
            if (j != 0) {
                j = LPS[j - 1];
            } else {
                i++;
            }
            posInicial = -1; //reinicia a posição inicial da correspondência
        }
      
    } //fim do laço for
}

//--------- FUNÇÃO MAIN -----------------------------------------------------

int main() {

    //---- VARIÁVEIS E ESTRUTURAS ------------------
  
    chrono::steady_clock::time_point begin, end;

    const int tamanhoDoBuffer = 2048; //qtdd de caracteres do buffer que declaro abaixo
    char buffer[tamanhoDoBuffer];     //armazenará um "bloco" temporário de texto da entrada.txt
  
    vector<const char*> patterns = {"inte", "comp", "tica", "log"}; //padrões buscados

    cout << endl << "ALGORITMO KNUTH-MORRIS-PRATT" << endl << endl;
  
    //---- REALIZANDO LEITURA E BUSCA DOS PADRÕES ------------

    begin = chrono::steady_clock::now(); 
  
    for(int i=0; i < patterns.size(); i++){

      cout << ">> Buscando o padrao \"" << patterns[i] << "\":" << endl;

      //o arquivo deve ser lido novamente a cada padrão buscado
      ifstream arquivo("entrada.txt");

      if (!arquivo) {
          cout << "Erro ao abrir o arquivo de entrada!" << endl;
          exit(0);
      }

       acessos++;

      int linhaAtual = 1; 
      
      //bufferização: armazena 2048 caracteres no arquivo em um bloco temporário
      while (arquivo.getline(buffer, tamanhoDoBuffer)) {
          kmp(buffer, patterns[i], linhaAtual);
          linhaAtual++;
          acessos++;
      }

      /* bufferização: provavelmente irão sobrar caracteres, 
       * pq dificilmente a qtdd de caracteres do texto será um múlltiplo de 2048
       */
      
      int bytesRestantes = arquivo.gcount();
      if (bytesRestantes > 0) {
          kmp(buffer, patterns[i], linhaAtual);
          acessos++;
      }
      
      arquivo.close();

      cout << endl;
      
    } //fim do laço for()
  
    end = chrono::steady_clock::now();

    //imprime o tempo de begin até o end (nesse caso, compreende todo o laço for)
    printExecutionTime(begin, end);
    cout << "Quantidade de acessos realizados: " << acessos << endl;

    return 0;
}
