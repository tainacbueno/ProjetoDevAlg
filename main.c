#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>


struct cadastro{
    char nome[100];
};

void cadastra(){
    FILE *arq, *teste;
    struct cadastro cad;
    fflush(stdin);

    puts("=====================");
    puts("Cadastro de usuário");
    puts("=====================");
    puts("Nome: ");
    fgets(cad.nome, 100, stdin);
    fflush(stdout);

    char *temp = malloc(sizeof(char)*104);
    strcpy(temp, cad.nome);
    temp = strtok(temp, "\n");
    char txt[5] = ".txt";
    strcat(temp, txt);

    teste = fopen(temp, "r");

    if (!teste){
        printf("\n\nNÃO tem arquivo\n");

        arq = fopen(temp, "w+");
        fprintf(arq, "%s",cad.nome);
        printf("criado\n");
        fclose(arq);
    }
    else {
        printf("SIM, tem arquivo\n");
    }

    fclose(teste);
    free(temp);
}

void menu(){
    while(1) {
        puts("=====================");
        puts("Bem-vindo ao seu gerenciador financeiro!");
        puts("=====================");
        puts("MENU\n");
        puts("1 - Cadastro de usuário");
        puts("2 - Cadastro de receitas e gastos");
        puts("3 - Relatórios");
        puts("0 - Sair");
        int op;
        puts("Que operação deseja realizar? ");
        scanf("%d", &op);
        if(op == 0)
            break;
        else if (op == 1)
            cadastra();
        else if (op == 2)
            puts("oi");
            //gastos;
        else if (op == 3)
            puts("oi");
            //relatorios;
        else
            puts("Opção inválida. Tente novamente.");
    }
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    menu();
    return 0;
}