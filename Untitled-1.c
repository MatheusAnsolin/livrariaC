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
void deleteLivro(int posicao);

int main() {
    int codigoAuxiliar, posicao, opcao;

    setlocale(LC_ALL, "pt_BR.UTF-8");
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);          

    createFile();
    do {
        menu();
        scanf("%d", &opcao);
        switch (opcao) {
            case 1:
                getLivros();
                printf("Qual código do livro vendido?");
                scanf("%d", &codigoAuxiliar);
                posicao = getLivro(codigoAuxiliar);
                if (posicao >= 0) {
                    deleteLivro(posicao);
                } else {
                    printf("Livro não encontrado ou já vendido.\n");
                }
                break;

            case 2:
                insert();
                break;

            case 3:
                getLivros();
                getch();
                break;

            case 0:
                printf("\nObrigado por utilizar! \n");
                break;

            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    return 0;
}

void createFile() {
    FILE *ponteiroCriar = fopen(Arquivo, "rb");
    if (ponteiroCriar) {
        printf("Aberto arquivo existente\n");
    } else {
        ponteiroCriar = fopen(Arquivo, "wb");
        printf("Criado novo arquivo\n");
    }
    fclose(ponteiroCriar);
}

void insert() {
    Livro livro;
    FILE *ponteiroInserir = fopen(Arquivo, "ab");
    if (!ponteiroInserir) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }

    printf("\nCadastro de livro: \n\n");

    printf("Qual código do livro? \n");
    scanf("%d", &livro.codigo);
    getchar();

    printf("Qual título do livro?\n");
    fgets(livro.titulo, sizeof(livro.titulo), stdin);
    livro.titulo[strcspn(livro.titulo, "\n")] = '\0'; // Remover '\n'

    printf("Ano de publicação: ");
    scanf("%d", &livro.anoDePublicacao);

    printf("Número de páginas: ");
    scanf("%d", &livro.numeroDePaginas);

    printf("Qual o preço de compra? \n");
    scanf("%lf", &livro.precoDeCompra);

    livro.vendido = 0; // Inicialmente não vendido

    fwrite(&livro, sizeof(Livro), 1, ponteiroInserir);
    fclose(ponteiroInserir);

    printf("\nLivro cadastrado com sucesso!\n");
}

void getLivros() {
    printf("\nLivros à venda: \n");
    Livro livro;
    FILE *ponteiroLer = fopen(Arquivo, "rb");

    if (!ponteiroLer) {
        printf("Erro ao abrir arquivo para leitura.\n");
        return;
    }

    while (fread(&livro, sizeof(Livro), 1, ponteiroLer)) {
        if (livro.vendido == 1) {
            printf("\nLivro Vendido: %s\n", livro.titulo);
        } else {
            printf("\n%s, de %d (%d páginas) à venda por R$ %.2f\n", livro.titulo, livro.anoDePublicacao, livro.numeroDePaginas, livro.precoDeCompra);
        }
    }

    fclose(ponteiroLer);

}

int getLivro(int cod) {
    Livro livroBuscado;
    FILE *ponteiroLer = fopen(Arquivo, "rb");

    if (!ponteiroLer) {
        printf("Erro ao abrir arquivo para leitura.\n");
        return -1;
    }

    int posicao = -1;
    int index = 0;

    while (fread(&livroBuscado, sizeof(Livro), 1, ponteiroLer)) {
        if (livroBuscado.vendido == 0 && livroBuscado.codigo == cod) {
            posicao = index;
            break;
        }
        index++;
    }

    fclose(ponteiroLer);
    return posicao;
}

void deleteLivro(int posicao) {
    Livro livro;
    FILE *ponteiroAtualizar = fopen(Arquivo, "rb+");

    if (!ponteiroAtualizar) {
        printf("Erro ao abrir arquivo para atualização.\n");
        return;
    }

    fseek(ponteiroAtualizar, sizeof(Livro) * posicao, SEEK_SET);
    fread(&livro, sizeof(Livro), 1, ponteiroAtualizar);
    
    livro.vendido = 1; // Atualizar o status para vendido

    fseek(ponteiroAtualizar, -sizeof(Livro), SEEK_CUR);
    fwrite(&livro, sizeof(Livro), 1, ponteiroAtualizar);

    fclose(ponteiroAtualizar);
    printf("\nParabéns pela venda do livro!\n");
}

void menu() {
    system("cls");
    printf("1. Marcar livro como vendido\n");
    printf("2. Cadastrar novo livro\n");
    printf("3. Listar livros\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}
