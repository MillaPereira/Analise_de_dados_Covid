/* Klarine Mendonça Silva , matrícula: 2020103337
   Milla Pereira Miguel de Souza, matrícula: 2020101969 */


#include <ctype.h> //Conversão para maiúscula
#include <math.h>  //Desvio padrão
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h> //Diretótrio
#include <sys/stat.h> //Diretótrio


void lerArquivo();					       // ITEM 1: Essa função fará a leitura do arquivo csv e armazanerá as informações contidas nele no vetor de struct
int filtraData();                 // ITEM 2: Recebe uma string no formato "xxxx/xx/xx" e retorna um valor correspondente à data
int comparaOrdemAlfabetica();    // 3.1  Função usada no qsort dentro do item 3
void cidadesMaisDeNCasos();     // ITEM 3: Recebe um valor N e imprime as cidades em ordem alfabética com mais N casos
void contaCasos();				 	   // ITEM 4: Conta a quantidade de casos confirmados de covid dentro do período solicitado	
int comparaDecrescente();     // 5.1  Função usada no qsort dentro do item 3
void listaCidadesTopN();	   // ITEM 5: Mostra em ordem descrescente as primeiras N cidades com maiores casos de covid confirmados  
void porcentagemPessoas();  // ITEM 6: Imprime a porcentagem de internados, obitos e internados que foram a óbito em dada cidade
int checaComorbidade();    // 7.2 Função usada no item 7 para checar se um dado paciente tem alguma comorbidade
void mediaDesvioPadrao(); // ITEM 7: Calcula a média e o desvio padrão da idade das pessoas que morreram e a porcentagem das pessoas sem comorbidade que morrem


typedef struct { // Será usada para armazenar os dados do arquivo

	char DataCadastro[20];
	char DataObito[20];
	char Classificacao[12];
	char Municipio[30];
	int IdadeNaDataNotificacao;
	char ComorbidadePulmao[5];
	char ComorbidadeCardio[5];
	char ComorbidadeRenal[5];
	char ComorbidadeDiabetes[5];
	char ComorbidadeTabagismo[5];
	char ComorbidadeObesidade[5];
	char FicouInternado[15];
} ArquivoTodo;

typedef struct { //Irá ser usada para armazenar temporariamente as cidades com sua respectiva quantidade de casos
	char NomeCidade[30];
	int NumCasos;
} CidadesComNCasos;

ArquivoTodo dados[202362]; // Variável global na qual são armazenadas as informações do arquivo csv
CidadesComNCasos cidades[78]; // Variável global na qual são armazenadas temporariamente as cidades com sua respectiva quantidade de casos
char nomeDiretorio[20]; // Vai guardar o nome do diretório indicado na saída

int main() {
	int N = 0; // Será usada nos item 3 e 5
  char d1[11], d2[11]; // Serão usadas nos itens 4, 5 e 7 para armazenar as datas digitadas
  char municipio[20]; // Será usada no item 6 par armazenar o municipio digitado
	
  // Cria o diretório para guardar as saídas

  scanf ("%s\n", nomeDiretorio);

  mkdir(nomeDiretorio, 0777);

  // Item 1

  lerArquivo(dados);

  // Item 3
  
  scanf ("%d", &N);
  cidadesMaisDeNCasos(N);

  // Item 4

  scanf ("%s ", d1);
  scanf ("%s", d2);
  
  contaCasos(d1, d2);

  // Item 5
  
  scanf ("%d ", &N);
  scanf ("%s ", d1);
  scanf ("%s", d2);

  listaCidadesTopN(N, d1, d2);

  // Item 6 
  
  scanf ("\n%[^\n]", municipio);
  porcentagemPessoas(municipio);

  // Item 7
  
  scanf ("%s ", d1);
  scanf ("%s", d2);
	mediaDesvioPadrao(d1, d2);

	
  
  return 0;
}

///////////////// FUNÇÕES /////////////////


 // ITEM 1
 
void lerArquivo(){
  
    FILE *arquivo = fopen("covid19ES.csv","r");
    int i = 0; 
    //Irá abrir o arquivo csv
                
  fscanf(arquivo, "%*[^\n]%*c"); //Irá pular a primeira linha/cabeçalho

   //O for a seguir lerá as linhas do arquivo exceto a primeira
  for (i = 0; i < 202362; i++){ 
    fscanf(arquivo, "%[^,],", dados[i].DataCadastro); 
    fscanf(arquivo, "%[^,],", dados[i].DataObito); // Lendo a data de óbito
    fscanf(arquivo, "%[^,],", dados[i].Classificacao); // Lendo a classificação  
    fscanf(arquivo, "%[^,],", dados[i].Municipio); // Lendo município
    fscanf(arquivo, "%*c%d %*[^\"]%*c,", &dados[i].IdadeNaDataNotificacao); // Lendo idade e voltando como inteiro o valor de anos
    fscanf(arquivo, "%[^,],", dados[i].ComorbidadePulmao); // Lendo a comorbidade por pulmão
    fscanf(arquivo, "%[^,],", dados[i].ComorbidadeCardio); // Lendo a comorbidade cardíaca
    fscanf(arquivo, "%[^,],", dados[i].ComorbidadeRenal); // Lendo a comorbidade renal
    fscanf(arquivo, "%[^,],", dados[i].ComorbidadeDiabetes); // lendo a comorbidade por diabates
    fscanf(arquivo, "%[^,],", dados[i].ComorbidadeTabagismo); // Lendo a comorbidade por tagabismo
    fscanf(arquivo, "%[^,],", dados[i].ComorbidadeObesidade); // Lendo a comorbidade por obesidade
    fscanf(arquivo, "%[^\n]%*c", dados[i].FicouInternado); // Lendo se ficou internado

  }
 fclose(arquivo);  // Fecha o arquivo csv
}

// ITEM 2 

int filtraData(char data[]){  // Recebe uma string no formato "xxxx-xx-xx" que representa a data
  char valorNumericoData[5]; // Armazanerá os caracteres de mês e dia da string data. Por exemplo, se data for "2020-07-20" valorNumericoData será igual "0720"
  int valorData = 0; // Receberá o valor inteiro da data
  int i = 0;

   // O for pecorrerá e altera cada zero de valorNumericoData de acordo com a seguintes condições 
  for(i = 0; i < 4; i++){
        if (i < 2){ // Como os digitos do mes e dia estão separados por '-', na hora de passar os caracteres para valorNumericoData é preciso pular ele
        valorNumericoData[i] = data[(i + 5)];  // Todos os casos relatados são do ano de 2020, então não há necessidade de guardar o ano, então pulará o traço e lerá o mês
      }
      else{
        valorNumericoData[i] = data[(i + 6)]; //Irá pular o segundo traço e lerá o dia
      }
  }
  valorData = atoi(valorNumericoData); // Transforma a string valorNumericoData em um inteiro
  
  return valorData; // Retorna esse valor para que possa ser usado como comparação. Quanto maior o valor, mais recente a data
} 

// ITEM 3 

int comparaOrdemAlfabetica(const void * a, const void * b){
  int indicador = strcmp((*(CidadesComNCasos*)a).NomeCidade, (*(CidadesComNCasos*)b).NomeCidade);

  // Compara o dois coneúdos de "a" e "b" e retorna para a função qsort se "a" é maior, menor ou igual que "b"

    if (indicador == 0){
      return 0; // São iguais
    }
    else if (indicador < 0){
      return -1; //  A "a" vem antes da "b"
    }
    else {
      return 1; // A "a" vem depois da "b"
    }
}

void cidadesMaisDeNCasos(int N){
  int contCasos = 0, i = 0, j = 0, m = 0, iDaStruct = 0, indicador = 0;
  char comparador[12] = "Confirmados"; // Será usada para verificar se a pessoa foi confirmada com covid
  char nomeArquivo[20] = "/item3.txt";
  char nomeDiretorioCopia[20];

  // Organiza os nomes da cidades e a quantidade total de casos de cada uma no vetor de struct cidades[]

    // O for percorrerá o vetor de struct cidades
  for (j = 0; j < 79; j++){
    if (j == 0){ // Primeiro loop coloca os nomes das cidades no vetor de struct cidades
        // O for percorrerá o vetor de dados
      for (i = 0; i < 202362; i++){
          for (m = 0; m < iDaStruct; m++){
            // Percorre a struct cidades e verica se a cidade de posição i já foi colocada na struct cidades
            if (strcmp(cidades[m].NomeCidade, dados[i].Municipio) == 0){
              indicador = 1;// Se indicador = 1, a cidade já foi adicionada, caso contrário, ainda não foi adicionada
            }
          }
          if (indicador == 0){
            memcpy(cidades[iDaStruct].NomeCidade, dados[i].Municipio, strlen(dados[i].Municipio) + 1);
               // Copia o nome da cidade para a struct cidades
            cidades[iDaStruct].NumCasos = 0;  
            iDaStruct++; // Serve para percorrer a struct cidades e saber quais posições já foram preenchidas
          }
          indicador = 0;   
      }
      iDaStruct = 0;
    }
    
    else { // Loops seguintes contam a quantidade de casos de cada cidade
      for (i = 0; i < 202362; i++){
          if ((strcmp(cidades[iDaStruct].NomeCidade, dados[i].Municipio) == 0) && strcmp(comparador, dados[i].Classificacao) == 0){
            // Verifica se o caso aconteceu na cidade de posição iDaStruct, se foi confirmado e se o cadastro foi feito entre as datas indicadas
            cidades[iDaStruct].NumCasos++;
          }
      }
      iDaStruct++;
    }
  }

  // Coloca em ordem alfabética o vetor de strutc cidades[]

  qsort(cidades, 78, sizeof(CidadesComNCasos), comparaOrdemAlfabetica); 

  // Criação do arquivo e impressão das saídas. Esse padrão será repetido nos itens seguintes

  memcpy(nomeDiretorioCopia, nomeDiretorio, sizeof(nomeDiretorio)); // Copia o nome da pasta indicado na entrada para outra string

  strcat(nomeDiretorioCopia, nomeArquivo); // Concatenação do nome do diretório com o nome do arquivo: ./teste1/item3.txt

  FILE * arquivo3 = fopen(nomeDiretorioCopia, "w"); // Cria o arquivo

  for (i = 0; i < 78; i++){
    if (cidades[i].NumCasos > N){ // Imprime todos as cidades com mais de N casos
      fprintf(arquivo3, "- %s: %d casos\n", cidades[i].NomeCidade, cidades[i].NumCasos);
    }
  }
  fclose(arquivo3); // Fecha o arquivo
}

// ITEM 4

void contaCasos (char d1[], char d2[]){
  int data1 = 0, data2 = 0, contador = 0, i = 0;
  char comparador[12] = "Confirmados"; // Será usada para verificar se a pessoa foi confirmada com covid
  char nomeArquivo[20] = "/item4.txt";
  char nomeDiretorioCopia[20];

  data1 = filtraData(d1); // Trasforma as strings de data recebidas para inteiro
  data2 = filtraData(d2);

  for (i = 0; i < 202362; i++){
    if ((filtraData(dados[i].DataCadastro) >= data1 && filtraData(dados[i].DataCadastro) <= data2) && strcmp(dados[i].Classificacao, comparador) == 0){ // Verifica se o caso foi cadastrado entre as datas indicadas e se foi confirmado
      contador++;
    }
  }
  
  // Criação do arquivo e impressão das saídas

  memcpy(nomeDiretorioCopia, nomeDiretorio, sizeof(nomeDiretorio));

  strcat(nomeDiretorioCopia, nomeArquivo);

  FILE * arquivo4 = fopen(nomeDiretorioCopia, "w"); //Cria o arquivo

    fprintf (arquivo4, "- Total de pessoas: %d", contador);
    fclose(arquivo4); //Fecha o arquivo
}

// ITEM 5

int comparaOrdemDecrescente(const void * a, const void * b){ // Compara os de conteúdos "a" e "b" e retorna para a função qsort se "a" é maior, menor ou igual que "b"

  if ((*(CidadesComNCasos*)a).NumCasos == (*(CidadesComNCasos*)b).NumCasos){
    return 0; // Iguais
  }
  else if ((*(CidadesComNCasos*)a).NumCasos > (*(CidadesComNCasos*)b).NumCasos){
    return -1; //  A "a" vem antes da "b"
  }
  else{
    return 1; //  A "a" vem depois da "b"
  }
}

void listaCidadesTopN(int N, char d1[], char d2[]){
  int i = 0, j = 0, m = 0, iDaStruct = 0, indicador = 0;
  int data1 = 0, data2 = 0;
  char comparador[12] = "Confirmados"; // Será usada para verificar se a pessoa foi confirmada com covid
  char nomeArquivo[20] = "/item5.txt";
  char nomeDiretorioCopia[20];

  data1 = filtraData(d1); // Trasforma as strings de data recebidas para inteiro
  data2 = filtraData(d2);

   // O for percorrerá o vetor de struct cidades
  for (j = 0; j < 79; j++){ 
    if (j == 0){ // Primeiro loop coloca os nomes das cidades no vetor de struct cidades
        // O for percorrerá o vetor de dados
      for (i = 0; i < 202362; i++){ 
          for (m = 0; m < iDaStruct; m++){ 
            // Percorre a struct cidades e verica se a cidade de posição i já foi colocada na struct cidades
            if (strcmp(cidades[m].NomeCidade, dados[i].Municipio) == 0){
              indicador = 1; // Se indicador = 1, a cidade já foi adicionada, caso contrário, ainda não foi adicionada
            }
          }
          if (indicador == 0){
            memcpy(cidades[iDaStruct].NomeCidade, dados[i].Municipio, strlen(dados[i].Municipio) + 1); // Copia o nome da cidade para a struct cidades
            cidades[iDaStruct].NumCasos = 0;  
            iDaStruct++; // Serve para percorrer a struct cidades e saber quais posições já foram preenchidas
          }
          indicador = 0;   
      }
      iDaStruct = 0;
    }
    
    else { // Loops seguintes contam a quantidade de casos de cada cidade
      for (i = 0; i < 202362; i++){
          if ((strcmp(cidades[iDaStruct].NomeCidade, dados[i].Municipio) == 0) && strcmp(comparador, dados[i].Classificacao) == 0 && filtraData(dados[i].DataCadastro) >= data1 && filtraData(dados[i].DataCadastro) <= data2){ // Verifica se o caso aconteceu na cidade de posição iDaStruct, se foi confirmado e se o cadastro foi feito entre as datas indicadas
            cidades[iDaStruct].NumCasos++;
          }
        }
      iDaStruct++;
    }
  }

  // Organiza em ordem decrescente de numero de casos no vetor struct cidades
  
  qsort(cidades, 78, sizeof(CidadesComNCasos), comparaOrdemDecrescente);

  // Criação do arquivo e impressão das saídas

    memcpy(nomeDiretorioCopia, nomeDiretorio, sizeof(nomeDiretorio));

    strcat(nomeDiretorioCopia, nomeArquivo);

    FILE * arquivo5 = fopen(nomeDiretorioCopia, "w"); //Cria o arquivo

  if (cidades[0].NumCasos != 0){ // mudança teste 8
    for (i = 0; i < N; i++){ // Imprime as top N
        fprintf(arquivo5, "- %s: %d casos\n", cidades[i].NomeCidade, cidades[i].NumCasos);
    }
  }
    fclose(arquivo5); //Fecha o arquivo
}
// ITEM 6

void porcentagemPessoas(char municipio[]){
  int i = 0;
  float contObitos = 0, contInternados = 0, contObitosInternados = 0, contTotal = 0; 
  char m[6] = "TODAS"; // Será usada para verificar se a string de entrada é "TODAS"
  char p[12] = "Confirmados"; // Será usada para verificar se a pessoa foi confirmada com covid
  char r[5] = "Sim";  // Será usada para verificar se ficou internada
  char nomeArquivo[20] = "/item6.txt";
  char nomeDiretorioCopia[20];

  for (i = 0; i < strlen(municipio); i++){ 
    municipio[i] = toupper(municipio[i]); // Muda para letra maiscula
  }

  if (strcmp(municipio, m) == 0){ //Quando for "todas"
        // O for percorrerá o vetor de dados
    for (i = 0; i < 202362; i++){
      if(strcmp(p, dados[i].Classificacao) == 0){
        contTotal++;           //Compara se a pessoa está confirmada para covid
        if ((filtraData(dados[i].DataObito) != 0)){ 
          contObitos++;       //Quando a pessoa estiver morta e confirmada para covid
        }
        if (strcmp(r, dados[i].FicouInternado) == 0){ 
          contInternados++;    //Quando a pessoa foi internada e confirmada para covid
          if (filtraData(dados[i].DataObito) != 0){
            contObitosInternados++;   //Quando a pessoa foi internada e confirmada para covid e acabou morrendo
          }
        } 
      } 
    }
  }
  else{
    for (i = 0; i < 202362; i++){
      if (strcmp(municipio, dados[i].Municipio) == 0){ //Quando for algum municipio
        if(strcmp(p, dados[i].Classificacao) == 0){
          contTotal++;           ///Compara se a pessoa está confirmada para covid

          if ((filtraData(dados[i].DataObito) != 0)){ 
            contObitos++;       //Quando a pessoa estiver morta e confirmada para covid
          }
          if (strcmp(r, dados[i].FicouInternado) == 0){ 
            contInternados++;    //Quando a pessoa foi internada e confirmada para covid
            if (filtraData(dados[i].DataObito) != 0){
              contObitosInternados++;   //Quando a pessoa foi internada e confirmada para covid e acabou morrendo
            }
          } 
        }
      }
    }
  }
  
  // Criação do arquivo e impressão das saídas

  memcpy(nomeDiretorioCopia, nomeDiretorio, sizeof(nomeDiretorio));

  strcat(nomeDiretorioCopia, nomeArquivo);

  FILE * arquivo6 = fopen(nomeDiretorioCopia, "w"); //Cria o arquivo

  fprintf(arquivo6,"- Resultados para %s:\n", municipio);
  fprintf(arquivo6, "- A %% de pessoas com Covid-19 que ficaram internadas: %.3f%%\n", ((contInternados/contTotal)*100));
  fprintf(arquivo6, "- A %% de pessoas com Covid-19 que morreram: %.3f%%\n", ((contObitos/contTotal)*100));
  fprintf(arquivo6, "- A %% de pessoas que ficaram internadas e morreram: %.3f%%", ((contObitosInternados/contObitos)*100));

  fclose(arquivo6); //Fecha o arquivo
}

// ITEM 7

int checaComorbidade(int i){ // Função que retorna um valor indicando se a pessoa de posição i tem ou não comorbidade
  char n[5] = "Sim";

  if (strcmp(dados[i].ComorbidadePulmao, n) != 0 && strcmp(dados[i].ComorbidadeCardio, n) != 0 && strcmp(dados[i].ComorbidadeRenal, n) != 0 && strcmp(dados[i].ComorbidadeDiabetes, n) != 0 && strcmp(dados[i].ComorbidadeTabagismo, n) != 0 && strcmp(dados[i].ComorbidadeObesidade, n) != 0){ // Verifica se todos os campos de comorbidade estão diferentes de "Sim"
    return 0; // Não tem nenhuma comorbidade
  }
  else{
    return 1; // Tem alguma comorbidade
  }
}

void mediaDesvioPadrao(char d1[], char d2[]){ 
  float totalIdade = 0, contTotal = 0, contMorreuSemComorbidade = 0, porcentagemMorreuSemComorbidade = 0, desvioPadrao = 0, media = 0;
  int data1 = 0, data2 = 0;
  int i = 0;
  char comparador[12] = "Confirmados";  // Será usada para verificar se a pessoa foi confirmada com covid
  char nomeArquivo[20] = "/item7.txt";
  char nomeDiretorioCopia[20];

  data1 = filtraData(d1); // Trasforma as strings de data recebidas para inteiro
  data2 = filtraData(d2);

  for (i = 0; i < 202362; i++){ // Percorre todo o vetor com os dados
    if (filtraData (dados[i].DataCadastro) >= data1 && filtraData (dados[i].DataCadastro) <= data2 && strcmp(dados[i].Classificacao, comparador) == 0 && filtraData(dados[i].DataObito) != 0){ // Verifica se a pessoa foi cadastrada entre as datas indicadas, se foi confirmado com covid e se morreu
      totalIdade = totalIdade + dados[i].IdadeNaDataNotificacao; // Vai somar todas as idades para depois calcular a média
      contTotal++; // Quantidade total de pessoas

      if (checaComorbidade(i) == 0){ // Verifica se a pessoa tinha aguma comorbidade
          contMorreuSemComorbidade++; // Conta a quantidade de pessoas que morreram sem comorbidade
      }
    }                  
  }

  if (contTotal == 0){ // Se ninguém morrer, todas as variáveis recebem 0: mudança teste 8
    media = 0;
    porcentagemMorreuSemComorbidade = 0;
    desvioPadrao = 0;
  } 
  else{
    media = totalIdade/contTotal;
    porcentagemMorreuSemComorbidade = (contMorreuSemComorbidade/contTotal)*100;

    // Calculo do desvio padrão:

  // Percorrá todo o vetor com os dados
  for (i = 0; i < 202362; i++){ 
    if (filtraData (dados[i].DataCadastro) >= data1 && filtraData (dados[i].DataCadastro) <= data2 && strcmp(dados[i].Classificacao, comparador) == 0 && filtraData(dados[i].DataObito) != 0){ // Verifica se a pessoa foi cadastrada entre as datas indicadas, se foi confirmado com covid e se morreu
      desvioPadrao += pow(dados[i].IdadeNaDataNotificacao - media, 2); // Parte do somatório no cáculo do desvio padrão
    }
  }
  desvioPadrao = sqrt(desvioPadrao/(contTotal - 1)); // Desvio padrão final
  }
  
  // Criação do arquivo texto e impressão das saídas 

  memcpy(nomeDiretorioCopia, nomeDiretorio, sizeof(nomeDiretorio)); 

  strcat(nomeDiretorioCopia, nomeArquivo);

  FILE * arquivo7 = fopen(nomeDiretorioCopia, "w");  //Cria o arquivo

  fprintf (arquivo7, "A media e desvio padrao da idade: %.3f -- %.3f\n", media, desvioPadrao);
  fprintf (arquivo7, "A %% de pessoas que morreram sem comorbidade: %.3f%%", porcentagemMorreuSemComorbidade);

  fclose(arquivo7);  //Fecha o arquivo
}