/*
---Trabalho da disciplina de Programação de Baixo Nível
--Realizado por Luísa Kirsch Silva Zarth e Luthero Boscardin Vargas
-Semestre 2025/1
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int r, g, b;
} Pixel;

// Função para ler a imagem
Pixel **ler_imagem(const char *nome_arquivo, int *largura, int *altura, int *maxValor) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    char tipo[3];
    int i, j;
    
    fscanf(arquivo, "%s", tipo);
    if (tipo[0] != 'P' || tipo[1] != '3') {
        printf("Arquivo nao esta no formato P3.\n");
        fclose(arquivo);
        return NULL;
    }

    fscanf(arquivo, "%d %d", largura, altura);
    fscanf(arquivo, "%d", maxValor);

    Pixel **imagem = (Pixel **)malloc((*altura) * sizeof(Pixel *));
    for (i = 0; i < *altura; i++) {
        imagem[i] = (Pixel *)malloc((*largura) * sizeof(Pixel));
    }

    for (i = 0; i < *altura; i++) {
        for (j = 0; j < *largura; j++) {
            fscanf(arquivo, "%d %d %d", &imagem[i][j].r, &imagem[i][j].g, &imagem[i][j].b);
        }
    }

    fclose(arquivo);
    return imagem;
}

// Função para salvar imagem
void salvar_imagem(Pixel **imagem, int largura, int altura, int maxValor) {
	char nomeSaida[100];
    printf("digite o nome do arquivo de saida: (exemplo.ppm)\n");
	scanf("%s", nomeSaida);
	
	FILE *arquivo = fopen(nomeSaida, "w");
    int i, j;
    if (arquivo == NULL) {
        perror("Erro ao salvar imagem");
        return;
    }
	
    fprintf(arquivo, "P3\n%d %d\n%d\n", largura, altura, maxValor);
    for (i = 0; i < altura; i++) {
        for (j = 0; j < largura; j++) {
            fprintf(arquivo, "%d %d %d\n", imagem[i][j].r, imagem[i][j].g, imagem[i][j].b);
        }
    }
    
	fclose(arquivo);
	printf("Imagem salva\n");
}

// função para aplicar o negativo
void aplicar_negativo(Pixel **imagem, int largura, int altura, int maxValor) {
    int i, j;
    for (i = 0; i < altura; i++) {
        for (j = 0; j < largura; j++) {
            imagem[i][j].r = maxValor - imagem[i][j].r;
            imagem[i][j].g = maxValor - imagem[i][j].g;
            imagem[i][j].b = maxValor - imagem[i][j].b;
        }
    }
}

// função para deixar em tons de cinza
void aplicar_cinza(Pixel **imagem, int largura, int altura) {
    int i, j;
    for (i = 0; i < altura; i++) {
        for (j = 0; j < largura; j++) {
            int media = (imagem[i][j].r + imagem[i][j].g + imagem[i][j].b) / 3;
            imagem[i][j].r = imagem[i][j].g = imagem[i][j].b = media;
        }
    }
}

// função para envelhecer a imagem
void aplicar_envelhecida(Pixel **imagem, int largura, int altura){
    int i, j;
    for (i = 0; i < altura; i++) {
        for (j = 0; j < largura; j++) {
            int r = imagem[i][j].r;
            int g = imagem[i][j].g;
            int b = imagem[i][j].b;

            int novo_r = (int)(0.393 * r + 0.769 * g + 0.189 * b);
            int novo_g = (int)(0.349 * r + 0.686 * g + 0.168 * b);
            int novo_b = (int)(0.272 * r + 0.534 * g + 0.131 * b);

            if (novo_r > 255) novo_r = 255;
            if (novo_g > 255) novo_g = 255;
            if (novo_b > 255) novo_b = 255;

            imagem[i][j].r = novo_r;
            imagem[i][j].g = novo_g;
            imagem[i][j].b = novo_b;
        }
    }
}

//função para tornar raio-x
void aplicar_raiox(Pixel **imagem, int largura, int altura){
	int i, j;
    
    for(i = 0; i < altura; i++){
        for(j = 0; j < largura; j++){
            int cinza = (int)(imagem[i][j].r * 0.299 + imagem[i][j].g * 0.587 + imagem[i][j].b * 0.114);
            
            int novo_valor = 255 - cinza;

            if(novo_valor > 255) novo_valor = 255;
            if(novo_valor < 0) novo_valor = 0;

            imagem[i][j].r = imagem[i][j].g = imagem[i][j].b = novo_valor;
        }
    }
}

//função para rotacionar em 90 graus
void aplicar_rotacao90(Pixel ***imagem, int *largura, int *altura){
	int i, j;
	int novaLargura = *altura;
    int novaAltura = *largura;

    Pixel **novaImagem = (Pixel **)malloc(novaAltura * sizeof(Pixel *));
    for (i = 0; i < novaAltura; i++) {
        novaImagem[i] = (Pixel *)malloc(novaLargura * sizeof(Pixel));
    }

    for (i = 0; i < *altura; i++) {
        for (j = 0; j < *largura; j++) {
            novaImagem[j][*altura - 1 - i] = (*imagem)[i][j];
        }
    }
    
    for (i = 0; i < *altura; i++) {
        free((*imagem)[i]);
    }
    free(*imagem);

    *imagem = novaImagem;
    *largura = novaLargura;
    *altura = novaAltura;
}

//função para rotacionar em 180 graus
void aplicar_rotacao180(Pixel ***imagem, int largura, int altura) {
	int i, j;
    for (i = 0; i < altura / 2; i++) {
        for (j = 0; j < largura; j++) {
            Pixel temp = (*imagem)[i][j];
            (*imagem)[i][j] = (*imagem)[altura - 1 - i][largura - 1 - j];
            (*imagem)[altura - 1 - i][largura - 1 - j] = temp;
        }
    }

    // Se altura for ímpar, inverte a linha do meio
    if (altura % 2 == 1) {
        int meio = altura / 2;
        for (j = 0; j < largura / 2; j++) {
            Pixel temp = (*imagem)[meio][j];
            (*imagem)[meio][j] = (*imagem)[meio][largura - 1 - j];
            (*imagem)[meio][largura - 1 - j] = temp;
        }
    }
}

// Função principal
int main() {
    int largura, altura, maxValor;
    char nomeEntrada[100];
    Pixel **imagem = NULL;
    int opcao;
    int i;

    printf("digite o nome do arquivo a ser lido: ");
    scanf("%s", nomeEntrada);

    imagem = ler_imagem(nomeEntrada, &largura, &altura, &maxValor);
    if (imagem == NULL) {
        printf("nao foi possivel ler a imagem\n");
        return 1;
    }

    do {
    	printf("--------------------\n");
        printf("1: Aplicar efeito negativo\n");
        printf("2: Aplicar tons de cinza\n");
        printf("3: Aplicar envelhecimento\n");
        printf("4: Aplicar efeito raio-X\n");
        printf("5: Aplicar rotacao de 90 graus\n");
        printf("6: Aplicar rotacao de 180 graus\n");
        printf("0: Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                aplicar_negativo(imagem, largura, altura, maxValor);
                printf("efeito negativo aplicado\n");
                salvar_imagem(imagem, largura, altura, maxValor);
                break;
            case 2:
                aplicar_cinza(imagem, largura, altura);
                printf("efeito tons de cinza aplicado\n");
                salvar_imagem(imagem, largura, altura, maxValor);
                break;
            case 3:
            	aplicar_envelhecida(imagem, largura, altura);
            	printf("efeito de envelhecimento aplicado\n");
            	salvar_imagem(imagem, largura, altura, maxValor);
            	break;
            case 4:
            	aplicar_raiox(imagem, largura, altura);
            	printf("efeito de raio-X aplicado\n");
            	salvar_imagem(imagem, largura, altura, maxValor);
            	break;
            case 5:
            	aplicar_rotacao90(&imagem, &largura, &altura);
            	printf("rotacao de 90 graus aplicada\n");
				salvar_imagem(imagem, largura, altura, maxValor);
            	break;
			case 6:
				aplicar_rotacao180(&imagem, largura, altura);
				printf("rotacao de 180 graus aplicada\n");
				salvar_imagem(imagem, largura, altura, maxValor);
				break;		
            case 0:
                printf("fechando programa\n");
                break;
            default:
                printf("Essa opcao nao existe\n");
        }
    } while (opcao != 0);

  	//metodo para liberar a memoria
    for (i = 0; i < altura; i++) {
        free(imagem[i]);
    }
    free(imagem);

    return 0;
}

