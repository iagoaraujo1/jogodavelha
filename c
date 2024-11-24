#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char **matriz;
} Jogo;

typedef struct {
    int vitoriaX;
    int vitoria0;
    int rodadas;
} Estatisticas;

void inicializarMatriz(Jogo *jogo) {
    jogo->matriz = (char **)malloc(3 * sizeof(char *));
    for (int l = 0; l < 3; l++) {
        jogo->matriz[l] = (char *)malloc(3 * sizeof(char));
        for (int c = 0; c < 3; c++)
            jogo->matriz[l][c] = ' ';
    }
}

void liberarMatriz(Jogo *jogo) {
    for (int l = 0; l < 3; l++) {
        free(jogo->matriz[l]);
    }
    free(jogo->matriz);
}

void imprimir(Jogo *jogo) {
    printf("\n\n\t 0   1   2\n\n");
    for (int l = 0; l < 3; l++) {
        for (int c = 0; c < 3; c++) {
            if (c == 0)
                printf("\t");
            printf(" %c ", jogo->matriz[l][c]);
            if (c < 2)
                printf("|");
            if (c == 2)
                printf("   %d", l);
        }
        printf("\n");
        if (l < 2)
            printf("\t-----------\n");
    }
}

int ganhouPorLinha(Jogo *jogo, int l, char c) {
    return (jogo->matriz[l][0] == c && jogo->matriz[l][1] == c && jogo->matriz[l][2] == c);
}

int ganhouPorLinhas(Jogo *jogo, char c) {
    for (int l = 0; l < 3; l++) {
        if (ganhouPorLinha(jogo, l, c))
            return 1;
    }
    return 0;
}

int ganhouPorColuna(Jogo *jogo, int c, char j) {
    return (jogo->matriz[0][c] == j && jogo->matriz[1][c] == j && jogo->matriz[2][c] == j);
}

int ganhouPorColunas(Jogo *jogo, char j) {
    for (int c = 0; c < 3; c++) {
        if (ganhouPorColuna(jogo, c, j))
            return 1;
    }
    return 0;
}

int ganhouPorDiagPrin(Jogo *jogo, char c) {
    return (jogo->matriz[0][0] == c && jogo->matriz[1][1] == c && jogo->matriz[2][2] == c);
}

int ganhouPorDiagSec(Jogo *jogo, char c) {
    return (jogo->matriz[0][2] == c && jogo->matriz[1][1] == c && jogo->matriz[2][0] == c);
}

int ehValida(Jogo *jogo, int l, int c) {
    return (l >= 0 && l < 3 && c >= 0 && c < 3 && jogo->matriz[l][c] == ' ');
}

void lerCoordenadas(Jogo *jogo, char j) {
    int linha, coluna;

    printf("Digite a linha (0, 1, 2): ");
    scanf("%d", &linha);

    printf("Digite a coluna (0, 1, 2): ");
    scanf("%d", &coluna);

    while (!ehValida(jogo, linha, coluna)) {
        printf("Coordenadas invalidas! Digite outra linha e outra coluna.\n");
        
        printf("Digite a linha (0, 1, 2): ");
        scanf("%d", &linha);

        printf("Digite a coluna (0, 1, 2): ");
        scanf("%d", &coluna);
    }

    jogo->matriz[linha][coluna] = j;
}

int quantVazias(Jogo *jogo) {
    int quantidade = 0;

    for (int l = 0; l < 3; l++) {
        for (int c = 0; c < 3; c++) {
            if (jogo->matriz[l][c] == ' ')
                quantidade++;
        }
    }
    return quantidade;
}

void salvarEstatisticas(Estatisticas *estatisticas) {
    FILE *arquivo = fopen("estatisticas.bin", "wb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de estatísticas\n");
        return;
    }
    fwrite(estatisticas, sizeof(Estatisticas), 1, arquivo);
    fclose(arquivo);
}

void carregarEstatisticas(Estatisticas *estatisticas) {
    FILE *arquivo = fopen("estatisticas.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de estatísticas\n");
        return;
    }
    fread(estatisticas, sizeof(Estatisticas), 1, arquivo);
    fclose(arquivo);
}

void exibirMenuPrincipal() {
    printf("=== Menu Principal ===\n");
    printf("1. Iniciar Jogo\n");
    printf("2. Estatisticas\n");
    printf("3. Sair\n");
}

void exibirEstatisticas(Estatisticas *estatisticas) {
    printf("\n=== Estatisticas ===\n");
    printf("Vitorias Jogador 1 (X): %d\n", estatisticas->vitoriaX);
    printf("Vitorias Jogador 2 (0): %d\n", estatisticas->vitoria0);
    printf("Rodadas Jogadas: %d\n", estatisticas->rodadas);
}

int verificarVitoria(Jogo *jogo, char jogador) {
    return ganhouPorLinhas(jogo, jogador) || 
           ganhouPorColunas(jogo, jogador) || 
           ganhouPorDiagPrin(jogo, jogador) || 
           ganhouPorDiagSec(jogo, jogador);
}

void jogar() {
    Jogo jogo;
    Estatisticas estatisticas = {0, 0, 0};
    char jogador1 = 'X', jogador2 = '0';
    int jogador = 1;
    int faseAtual = 1;

    carregarEstatisticas(&estatisticas);

    do {
        inicializarMatriz(&jogo);

        int venceu = 0;
        do {
            imprimir(&jogo);
            if (jogador == 1) {
                lerCoordenadas(&jogo, jogador1);
                venceu = verificarVitoria(&jogo, jogador1);
                if (venceu) {
                    estatisticas.vitoriaX++;
                    printf("\nParabens Jogador 1. Voce venceu!!!\n");
                    break;
                }
                jogador = 2;
            } else {
                lerCoordenadas(&jogo, jogador2);
                venceu = verificarVitoria(&jogo, jogador2);
                if (venceu) {
                    estatisticas.vitoria0++;
                    printf("\nParabens Jogador 2. Voce venceu!!!\n");
                    break;
                }
                jogador = 1;
            }
        } while (quantVazias(&jogo) > 0);

        if (!venceu) {
            printf("\nEmpate! Ninguém venceu.\n");
        }

        estatisticas.rodadas++;
        salvarEstatisticas(&estatisticas);
        liberarMatriz(&jogo);
        faseAtual++;
    } while (faseAtual <= 5);
}

int main() {
    int opcao;

    do {
        exibirMenuPrincipal();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogar();
                break;
            case 2: {
                Estatisticas estatisticas;
                carregarEstatisticas(&estatisticas);
                exibirEstatisticas(&estatisticas);
                break;
            }
            case 3:
                printf("\nSaindo do jogo. Ate mais!\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 3);

    return 0;
}
