
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <conio.h>  // lib getch()
#include <locale.h>//lib setlocale
#include <windows.h>  //lib SetConsoleOutputCP
#define Arquivo "livros.bd.txt"

typedef struct {
  int codigo;
  char titulo[50];
  int anoDePublicacao;
  int numeroDePaginas;
  double precoDeCompra;
  int vendido; //flag livro vendido. 1 significa vendido
} Livro;

void createFile();

int getLivro(int cod);
void getLivros();
void insert();
void menu();
void deleteLivro(posicao);


int main(){
    int codigoAuxiliar, posicao, opcao;

    setlocale(LC_ALL, "pt_BR.UTF-8");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);          

    createFile();
    do{

    menu();
    scanf("%d", &opcao);
    switch(opcao){
        case 1: printf("Qual código do livro vendido?");
                scanf("%d", &codigoAuxiliar);
                getLivro(codigoAuxiliar);
                posicao = getLivro(codigoAuxiliar);
                if(posicao >= 0 ){
                deleteLivro(posicao);
                }
        break;

        case 2: insert();
        break;

        case 3: getLivros();
        break;

        case 0: printf("\nObrigado por utilizar! \n");
    }
  } while(opcao != 0);

}


void createFile(){
    FILE *ponteiroCriar;
    ponteiroCriar = fopen(Arquivo, "rb");
    if(ponteiroCriar){
        printf("Aberto arquivo existente\n");
    } else {
        ponteiroCriar = fopen(Arquivo, "wb");
        printf("Criado novo arquivo\n");
     }
    fclose(ponteiroCriar);
}
void insert(){
    Livro livro;
    FILE *ponteiroInserir;
    ponteiroInserir = fopen(Arquivo, "ab");
    printf("\nCadastro de livro: \n\n");

    printf("Qual código do livro? \n");
    scanf("%d", &livro.codigo);
    getchar();

    printf("Qual título do livro?\n");
    fgets(livro.titulo, sizeof(livro.titulo), stdin);

    printf("Ano de publicação: ");
    scanf("%d", &livro.anoDePublicacao);

    printf("Número de páginas: ");
    scanf("%d", &livro.numeroDePaginas);

    printf("Qual o preço de compra? \n");
    scanf("%f", &livro.precoDeCompra);

    printf("\nEntendido: livro %s, publicado em %d, com %d páginas à venda por R$ %f",
        livro.titulo,
        livro.anoDePublicacao,
        livro.numeroDePaginas,
        livro.precoDeCompra);

    fwrite(&livro, sizeof(Livro), 1, ponteiroInserir);

    fclose(ponteiroInserir);
    printf("\nDados Gravados! \n");
}
void getLivros(){
  printf("\nLivros à venda: \n");
  Livro livros;
  FILE *ponteiroLer = fopen(Arquivo, "rb");
  if(ponteiroLer != NULL){
    printf("\n Cod   Nome \t Idade \n");
    //enquanto conseguir ler UM REGISTRO TOTAL NO ARQUIVO, MOSTRA
    while( fread(&livros, sizeof(Livro), 1, ponteiroLer) ) {
        if(livros.vendido == 1){
            printf("\n Livro Vendido, obrigado!");
        } else {
            printf("\n %s, de %d (%d páginas) à venda por R$ %f", livros.titulo, livros.anoDePublicacao, livros.numeroDePaginas, livros.precoDeCompra);
        }
    }

    printf("\n --------- fim da lista de pessoas --------- \n");

  fclose(ponteiroLer); //fecho o arquivo que foi aberto para leitura
  }
}
int getLivro(int cod){
    Livro livroBuscado;
    int posicao;
    FILE *ponteiroLer = fopen(Arquivo, "rb");
    if(ponteiroLer!=NULL){
        while(fread(&livroBuscado, sizeof(Livro), 1, ponteiroLer)){
            if(livroBuscado.vendido == 0 && livroBuscado.codigo == cod){
                posicao = ftell(ponteiroLer) / sizeof(livroBuscado);
                return posicao;
            }
        }
        fclose(ponteiroLer);
        return posicao;
    }


    return posicao;
}
void deleteLivro(){
    Livro livroVendido;
    int posicao = -1;
    FILE *ponteiroExcluir = fopen(Arquivo, "rb+");
    if(ponteiroExcluir!=NULL){

    fseek(ponteiroExcluir, sizeof(livroVendido) * (posicao - 1), SEEK_SET);
    fread(&livroVendido, sizeof(Livro), 1, Arquivo);
    livroVendido.vendido = 1;
    fseek(ponteiroExcluir, -sizeof(livroVendido), SEEK_CUR);
    fwrite(&livroVendido, sizeof(Livro), 1, ponteiroExcluir);
    fclose(ponteiroExcluir);
    printf("\nParabéns pela venda!\n");
    }
  getch();
}

void menu(){
    system("cls");
    printf("Crud de gravação dados em arquivos com C");
    printf("");
}