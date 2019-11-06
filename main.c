#include <stdlib.h>
#include <stdio.h>
#include <iup.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>

#define MAX 100
#define maxCaminho 39

int argc;
char **argv;
int globalVar = 0;
int deposita_saque = 0;
char pessoaAtual[23];
double saldo;
char nomeAtual[255];
char *globalAno = "0";

int globalMoradia = 0;
int globalEstudo = 0;
int globalTransporte = 0;
int globalTrabalho = 0;
int globalOutros = 0;
int globalAlimentacao = 0;

struct variaveis{
    Ihandle *dlg, *label, *vbox, *hbox;
    Ihandle *btn, *button_exit;
}IupVar;
struct info{
    int dia, mes, ano;
    double valor;
    char transacao;
    char *desc;
};
struct movimenta{
    Ihandle *diaT, *diaL, *mesT, *mesL, *anoT, *anoL, *tipo, *descricaoT, *descricaoL;
    Ihandle *movT, *movL;
    Ihandle *hbox2, *hbox3, *hbox4, *list;
}varMov;
struct guardaChar{
    char dia[100];
    char mes[100];
    char ano[100];
    char tipo;
    char valor[100];
    char desc[100];
}dados;

struct guardaChar geral[1000];
int posGeral = 0;

int botao_exit(Ihandle *self){
    IupVar.dlg = IupMessageDlg();

    IupSetAttribute(IupVar.dlg, "DIALOGTYPE", "WARNING");
    IupSetAttribute(IupVar.dlg, "BUTTONS", "OKCANCEL");
    IupSetAttribute(IupVar.dlg, "VALUE", "Tem certeza que desaja sair?");

    IupPopup(IupVar.dlg, IUP_CURRENT, IUP_CURRENT);

    char *op = IupGetAttribute(IupVar.dlg, "BUTTONRESPONSE");

    if(*op == '1'){
        return IUP_CLOSE;
    }

    IupDestroy(IupVar.dlg);
}

void aviso(char titulo[], char msgm[]){
    IupVar.dlg = IupMessageDlg();

    IupSetAttribute(IupVar.dlg, "DIALOGTYPE", "WARNING");
    IupSetAttribute(IupVar.dlg, "TITLE", titulo);
    IupSetAttribute(IupVar.dlg, "BUTTONS", "OK");
    IupSetAttribute(IupVar.dlg, "VALUE", msgm);

    IupPopup(IupVar.dlg, IUP_CURRENT, IUP_CURRENT);
    IupDestroy(IupVar.dlg);
}

void limpaCampos(){
    IupSetAttribute(varMov.diaT, "VALUE", "");
    IupSetAttribute(varMov.mesT, "VALUE", "");
    IupSetAttribute(varMov.anoT, "VALUE", "");
    IupSetAttribute(varMov.descricaoT, "VALUE", "");
    IupSetAttribute(varMov.list, "VALUE", "0");
    IupSetAttribute(varMov.movT, "VALUE", "");
}

int existeArq(char caminho[]){
    if(!fopen(caminho, "r"))
        //n existe
        return 0;
    return 1;
}

int atualizaGeral(int num, double valor){
    if(num == 0) {
        saldo += valor;
        //printf("%lf\n", saldo);
    }
    if(num == 1) {
        saldo -= valor;
        //printf("%lf\n", saldo);
    }

    char caminho[maxCaminho];
    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/geral.txt");

    FILE *arq = fopen(caminho, "w");
    fprintf(arq, "%s\n%.2lf", nomeAtual, saldo);
    fclose(arq);

    return EXIT_SUCCESS;
}
int ehMaior(struct guardaChar a, struct guardaChar b){
    if(atoi(a.ano) == atoi(b.ano)){
        if(atoi(a.mes) == atoi(b.mes))
            return atoi(a.dia) < atoi(b.dia);
        return atoi(a.mes) < atoi(b.mes);
    }
    return atoi(a.ano) < atoi(b.ano);
}
void troca(struct guardaChar *a, struct guardaChar *b){
    struct guardaChar temp = *a;
    *a = *b;
    *b = temp;
}

void ordena(struct guardaChar *guarda, int n){
    int i, j;

    for ( i = 0; i < n-1; ++i) {
        for (j = 0; j <n-1 ; ++j) {
            if(ehMaior(guarda[j], guarda[j+1]) == 1)
                troca(&guarda[j], &guarda[j+1]);
        }
    }
}

void imprimeRelatorio(struct guardaChar guarda[1000], int i, int relat){
    globalAno = "0";
    ordena(guarda, i);
    Ihandle *texto = IupText(NULL);
    IupSetAttribute(texto, "MULTILINE", "YES");
    IupSetAttribute(texto, "READONLY", "YES");
    IupSetAttribute(texto, "NAME", "CAIXA");
    IupSetAttribute(texto, "SIZE", "200x80");

    if(relat == 0)
        IupVar.label = IupLabel("Relátorio total:\n\n");
    else if (relat == 1)
        IupVar.label = IupLabel("Relátorio anual:\n\n");
    else
        IupVar.label = IupLabel("Relátorio mensal:\n\n");

    IupSetAttribute(IupVar.label, "FONTSIZE", "15");

    IupVar.vbox = IupVbox(IupVar.label, texto, NULL);
    IupSetAttribute(IupVar.vbox, "MARGIN", "50x20");
    IupSetAttribute(IupVar.vbox, "ALIGNMENT", "ACENTER");

    IupVar.dlg = IupDialog(IupVar.vbox);

    IupSetAttribute(IupVar.dlg, "TITLE", "Relatório");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    int j;

    char letra[9];
    char prom[4];

    for (j = 0; j < i; ++j) {
        if (guarda[j].tipo == 'd') {
            strcpy(letra, "depositou");
            strcpy(prom, "de");
        }
        else {
            strcpy(letra, "sacou");
            strcpy(prom, "para");
        }

        char buf[1000];
        snprintf(buf, sizeof buf, "Em %s/%s/%s %s R$%s %s %s\n\n", guarda[j].dia, guarda[j].mes, guarda[j].ano, letra, guarda[j].valor, prom,
                 guarda[j].desc);
        IupSetAttribute(texto, "INSERT", buf);
    }
    IupSetAttribute(texto, "CARET", "1");
    IupMainLoop();
    IupExitLoop();
}
int relatorioMoradia(){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/moradia.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(globalAno,"0") == 0){
                guarda[i] = dados;
                i++;
            }
            else{
                if(strcmp(globalAno, dados.ano) == 0){
                    //printf("MORADIAAAAA\n");
                    geral[posGeral] = dados;
                    posGeral++;
                }
            }
        }
        fclose(arq);
        if(strcmp(globalAno,"0") == 0)
            imprimeRelatorio(guarda, i, 0);
    }
    else if (strcmp(globalAno,"0") == 0){
        aviso("Erro", "Não há nada registrado em Moradia. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}
int relatorioEstudo(){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/estudo.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(globalAno,"0") == 0){
                guarda[i] = dados;
                i++;
            }
            else{
                if(strcmp(globalAno, dados.ano) == 0){
                    //printf("ESTUDOOOOOOO\n");
                    geral[posGeral] = dados;
                    posGeral++;
                }
            }
        }
        fclose(arq);
        if(strcmp(globalAno,"0") == 0)
            imprimeRelatorio(guarda, i, 0);
    }
    else if (strcmp(globalAno,"0") == 0){
        aviso("Erro", "Não há nada registrado em Estudo. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}
int relatorioTrabalho(){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/trabalho.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(globalAno,"0") == 0){
                guarda[i] = dados;
                i++;
            }
            else{
                if(strcmp(globalAno, dados.ano) == 0){
                    //printf("MORADIAAAAA\n");
                    geral[posGeral] = dados;
                    posGeral++;
                }
            }
            //printf("%s %s %s %c %s\n", dados.dia, dados.mes, dados.ano, dados.tipo, dados.valor);
        }
        fclose(arq);
        if(strcmp(globalAno,"0") == 0)
            imprimeRelatorio(guarda, i, 0);
    }
    else if (strcmp(globalAno,"0") == 0){
        aviso("Erro", "Não há nada registrado em Trabalho. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}
int relatorioOutros(){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/outros.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(globalAno,"0") == 0){
                guarda[i] = dados;
                i++;
            }
            else{
                if(strcmp(globalAno, dados.ano) == 0){
                    //printf("MORADIAAAAA\n");
                    geral[posGeral] = dados;
                    posGeral++;
                }
            }
        }
        fclose(arq);
        if(strcmp(globalAno,"0") == 0)
            imprimeRelatorio(guarda, i, 0);
    }
    else if (strcmp(globalAno,"0") == 0){
        aviso("Erro", "Não há nada registrado em Outros. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}
int relatorioAlimentacao(){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/alimentacao.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(globalAno,"0") == 0){
                guarda[i] = dados;
                i++;
            }
            else{
                if(strcmp(globalAno, dados.ano) == 0){
                    printf("MORADIAAAAA\n");
                    geral[posGeral] = dados;
                    posGeral++;
                }
            }
        }
        fclose(arq);
        if(strcmp(globalAno,"0") == 0)
            imprimeRelatorio(guarda, i, 0);
    }
    else if (strcmp(globalAno,"0") == 0){
        aviso("Erro", "Não há nada registrado em Alimentação. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}

int relatorioTransporte(){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/transporte.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(globalAno,"0") == 0){
                guarda[i] = dados;
                i++;
            }
            else{
                if(strcmp(globalAno, dados.ano) == 0){
                    //printf("MORADIAAAAA\n");
                    geral[posGeral] = dados;
                    posGeral++;
                }
            }
        }
        fclose(arq);
        if(strcmp(globalAno,"0") == 0)
            imprimeRelatorio(guarda, i, 0);
    }
    else if (strcmp(globalAno,"0") == 0){
        aviso("Erro", "Não há nada registrado em Transporte. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}
int moradiaMes(char mes[], char ano[]){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/moradia.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(mes, dados.mes) == 0 && strcmp(ano, dados.ano) == 0){
                //printf("MORADIAAAAA\n");
                guarda[i] = dados;
                i++;
            }
        }
        fclose(arq);
        imprimeRelatorio(guarda, i, 2);
    }
    else{
        aviso("Erro", "Não há nada registrado em Moradia. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}
int estudoMes(char mes[], char ano[]){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/estudo.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(mes, dados.mes) == 0 && strcmp(ano, dados.ano) == 0){
                //printf("MORADIAAAAA\n");
                guarda[posGeral] = dados;
                i++;
            }
        }
        fclose(arq);
        imprimeRelatorio(guarda, i, 2);
    }
    else{
        aviso("Erro", "Não há nada registrado em Moradia. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}
int trabalhoMes(char mes[], char ano[]){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/trabalho.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(mes, dados.mes) == 0 && strcmp(ano, dados.ano) == 0){
                //printf("MORADIAAAAA\n");
                guarda[posGeral] = dados;
                i++;
            }
        }
        fclose(arq);
        imprimeRelatorio(guarda, i, 2);
    }
    else{
        aviso("Erro", "Não há nada registrado em Moradia. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}
int outrosMes(char mes[], char ano[]){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/outros.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(mes, dados.mes) == 0 && strcmp(ano, dados.ano) == 0){
                //printf("MORADIAAAAA\n");
                guarda[posGeral] = dados;
                i++;
            }
        }
        fclose(arq);
        imprimeRelatorio(guarda, i, 2);
    }
    else{
        aviso("Erro", "Não há nada registrado em Moradia. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}
int alimentacaoMes(char mes[], char ano[]){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/alimentacao.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(mes, dados.mes) == 0 && strcmp(ano, dados.ano) == 0){
                //printf("MORADIAAAAA\n");
                guarda[posGeral] = dados;
                i++;
            }
        }
        fclose(arq);
        imprimeRelatorio(guarda, i, 2);
    }
    else{
        aviso("Erro", "Não há nada registrado em Moradia. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}
int transporteMes(char mes[], char ano[]){
    struct guardaChar guarda[1000];
    char caminho[maxCaminho];
    int i = 0;

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/estudo.txt");

    FILE *arq;

    if(fopen(caminho, "r")){
        arq = fopen(caminho, "r");

        while (fscanf(arq,"%s %s %s %c %s %s\n", dados.dia, dados.mes, dados.ano, &(dados.tipo), dados.valor, dados.desc) != EOF)
        {
            if(strcmp(mes, dados.mes) == 0 && strcmp(ano, dados.ano) == 0){
                //printf("MORADIAAAAA\n");
                guarda[posGeral] = dados;
                i++;
            }
        }
        fclose(arq);
        imprimeRelatorio(guarda, i, 2);
    }
    else{
        aviso("Erro", "Não há nada registrado em Moradia. Deposite ou saque e tente novamente.");
    }
    return EXIT_SUCCESS;
}

int pegaMes(Ihandle *self){
    Ihandle *a = IupGetDialogChild(self, "MES");
    char mes[2];
    strcpy(mes, IupGetAttribute(a, "VALUE"));

    if(atoi(mes) < 1 || atoi(mes) > 12)
        aviso("Erro", "Mês inválido. Tente novamente.");
    else{
        time_t mytime;
        mytime = time(NULL);
        struct tm tm = *localtime(&mytime);
        int ano = tm.tm_year + 1900;
        char anoc[5];
        sprintf(anoc, "%d", ano);

        if (globalMoradia == 1) {
            globalMoradia = 0;
            moradiaMes(mes, anoc);
        } else if (globalEstudo == 1) {
            globalEstudo = 0;
            estudoMes(mes, anoc);
        } else if (globalTrabalho == 1) {
            globalTrabalho = 0;
            trabalhoMes(mes, anoc);
        } else if (globalAlimentacao == 1) {
            globalAlimentacao = 0;
            alimentacaoMes(mes, anoc);
        } else if (globalTransporte == 1) {
            globalTransporte = 0;
            transporteMes(mes, anoc);
        } else if (globalEstudo == 0) {
            globalOutros = 0;
            outrosMes(mes, anoc);
        }
    }
    return EXIT_SUCCESS;
}
int pedeMes(int num){
    if (num == 1)
        globalMoradia = 1;
    else if(num == 2)
        globalEstudo = 1;
    else if(num == 3)
        globalTrabalho = 1;
    else if(num == 4)
        globalAlimentacao = 1;
    else if(num == 5)
        globalTransporte = 1;
    else if(num == 6)
        globalOutros = 1;
    IupOpen(&argc, &argv);

    IupVar.label = IupLabel("Digite o mês:\n");
    IupSetAttribute(IupVar.label, "FONTSIZE", "15");

    IupVar.btn = IupButton("Enviar", NULL);
    IupSetCallback(IupVar.btn, "ACTION", (Icallback) pegaMes);
    IupSetAttribute(IupVar.btn, "FONTSIZE", "10");

    varMov.anoT = IupText(NULL);
    IupSetAttribute(varMov.anoT, "VALUE", "");
    IupSetAttribute(varMov.anoT, "NAME", "MES");
    IupSetAttribute(varMov.anoT, "SIZE", "30");
    IupSetAttribute(varMov.anoT, "FONTSIZE", "12");

    IupVar.vbox = IupVbox(IupVar.label, varMov.anoT, IupVar.btn, NULL);
    IupSetAttribute(IupVar.vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(IupVar.vbox, "GAP", "10");
    IupSetAttribute(IupVar.vbox, "MARGIN", "60x60");
    IupSetAttribute(IupVar.vbox, "GAP", "20");
    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Relatório por mês");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    IupClose();
    return IUP_CLOSE;
}
int pegaAno(Ihandle *self){
    Ihandle *a = IupGetDialogChild(self, "ANO");
    globalAno = IupGetAttribute(a, "VALUE");
    posGeral = 0;

    if(isdigit(*globalAno) == 0) {
        aviso("Erro", "Ano inválido. Tente novamente.");
    }
    else {
        relatorioMoradia();
        relatorioEstudo();
        relatorioTrabalho();
        relatorioAlimentacao();
        relatorioTransporte();
        relatorioOutros();

        imprimeRelatorio(geral, posGeral, 1);

        IupSetAttribute(varMov.anoT, "VALUE", "");
    }

    IUP_CLOSE;
    return EXIT_SUCCESS;
}
int pedeAno(){
    IupOpen(&argc, &argv);

    IupVar.label = IupLabel("Digite o ano:\n");
    IupSetAttribute(IupVar.label, "FONTSIZE", "15");

    IupVar.btn = IupButton("Enviar", NULL);
    IupSetCallback(IupVar.btn, "ACTION", (Icallback) pegaAno);
    IupSetAttribute(IupVar.btn, "FONTSIZE", "10");

    varMov.anoT = IupText(NULL);
    IupSetAttribute(varMov.anoT, "VALUE", "");
    IupSetAttribute(varMov.anoT, "NAME", "ANO");
    IupSetAttribute(varMov.anoT, "SIZE", "30");
    IupSetAttribute(varMov.anoT, "FONTSIZE", "12");

    IupVar.vbox = IupVbox(IupVar.label, varMov.anoT, IupVar.btn, NULL);
    IupSetAttribute(IupVar.vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(IupVar.vbox, "GAP", "10");
    IupSetAttribute(IupVar.vbox, "MARGIN", "60x60");
    IupSetAttribute(IupVar.vbox, "GAP", "20");
    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Carteira Pessoal");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    IupClose();
    return IUP_CLOSE;
}

int pegaMoradia(Ihandle *self, int v){
    Ihandle *total = IupGetDialogChild(self, "total");
    Ihandle *mensal = IupGetDialogChild(self, "mensal");
    IupSetAttribute(total, "VALUE", "OFF");
    IupSetAttribute(mensal, "VALUE", "OFF");
    char *a = IupGetAttribute(self, "NAME");
    if (strcmp(a, "total") == 0){
        relatorioMoradia();
    }
    else{
        pedeMes(1);
    }

    return EXIT_SUCCESS;
}
int chamaMoradia(){
    Ihandle *toggle, *toggle2, *teste;

    toggle = IupToggle("Relatório Total", NULL);
    IupSetAttribute(toggle, "NAME", "total");
    IupSetAttribute(toggle, "FONTSIZE", "15");
    IupSetAttribute(toggle, "VALUE", "0");
    IupSetCallback(toggle, "ACTION", (Icallback)pegaMoradia);
    toggle2 = IupToggle("Relatório Mensal", NULL);
    IupSetAttribute(toggle2, "FONTSIZE", "15");
    IupSetAttribute(toggle2, "NAME", "mensal");
    IupSetAttribute(toggle2, "VALUE", "0");
    IupSetCallback(toggle2, "ACTION", (Icallback)pegaMoradia);

    Ihandle *lbl2 = IupLabel("\n\n");
    IupVar.label = IupLabel("\n\n\n");
    Ihandle *lbl3 = IupLabel("\n\n");

    IupVar.vbox = IupVbox(lbl2, toggle, IupVar.label, toggle2, lbl3, IupVar.btn, NULL);
    IupSetAttribute(IupVar.vbox, "MARGIN", "30x30");
    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Escolha de relatório");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    return IUP_CLOSE;
}

int pegaEstudo(Ihandle *self, int v){
    Ihandle *total = IupGetDialogChild(self, "total");
    Ihandle *mensal = IupGetDialogChild(self, "mensal");
    IupSetAttribute(total, "VALUE", "OFF");
    IupSetAttribute(mensal, "VALUE", "OFF");
    char *a = IupGetAttribute(self, "NAME");
    if (strcmp(a, "total") == 0){
        relatorioEstudo();
    }
    else{
        pedeMes(2);
    }
    return EXIT_SUCCESS;
}
int chamaEstudo(){
    Ihandle *toggle, *toggle2, *teste;

    toggle = IupToggle("Relatório Total", NULL);
    IupSetAttribute(toggle, "NAME", "total");
    IupSetAttribute(toggle, "FONTSIZE", "15");
    IupSetAttribute(toggle, "VALUE", "0");
    IupSetCallback(toggle, "ACTION", (Icallback)pegaEstudo);
    toggle2 = IupToggle("Relatório Mensal", NULL);
    IupSetAttribute(toggle2, "NAME", "mensal");
    IupSetAttribute(toggle2, "FONTSIZE", "15");
    IupSetAttribute(toggle2, "VALUE", "0");
    IupSetCallback(toggle2, "ACTION", (Icallback)pegaEstudo);

    Ihandle *lbl2 = IupLabel("\n\n");
    IupVar.label = IupLabel("\n\n\n");
    Ihandle *lbl3 = IupLabel("\n\n");

    IupVar.vbox = IupVbox(lbl2, toggle, IupVar.label, toggle2, lbl3, NULL);
    //IupSetAttribute(IupVar.vbox, "SIZE", "100");
    IupSetAttribute(IupVar.vbox, "MARGIN", "30x30");
    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Escolha de relatório");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    return IUP_CLOSE;
}

int pegaTrabalho(Ihandle *self, int v){
    Ihandle *total = IupGetDialogChild(self, "total");
    Ihandle *mensal = IupGetDialogChild(self, "mensal");
    IupSetAttribute(total, "VALUE", "OFF");
    IupSetAttribute(mensal, "VALUE", "OFF");
    char *a = IupGetAttribute(self, "NAME");
    if (strcmp(a, "total") == 0){
        relatorioTrabalho();
    }
    else{
        pedeMes(3);
    }
    return EXIT_SUCCESS;
}
int chamaTrabalho(){
    Ihandle *toggle, *toggle2, *teste;

    toggle = IupToggle("Relatório Total", NULL);
    IupSetAttribute(toggle, "NAME", "total");
    IupSetAttribute(toggle, "FONTSIZE", "15");
    IupSetAttribute(toggle, "VALUE", "0");
    IupSetCallback(toggle, "ACTION", (Icallback)pegaTrabalho);
    toggle2 = IupToggle("Relatório Mensal", NULL);
    IupSetAttribute(toggle2, "NAME", "mensal");
    IupSetAttribute(toggle2, "FONTSIZE", "15");
    IupSetAttribute(toggle2, "VALUE", "0");
    IupSetCallback(toggle2, "ACTION", (Icallback)pegaTrabalho);

    Ihandle *lbl2 = IupLabel("\n\n");
    IupVar.label = IupLabel("\n\n\n");
    Ihandle *lbl3 = IupLabel("\n\n");

    IupVar.vbox = IupVbox(lbl2, toggle, IupVar.label, toggle2, lbl3, NULL);
    IupSetAttribute(IupVar.vbox, "MARGIN", "30x30");
    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Escolha de relatório");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    return IUP_CLOSE;
}

int pegaOutros(Ihandle *self, int v){
    Ihandle *total = IupGetDialogChild(self, "total");
    Ihandle *mensal = IupGetDialogChild(self, "mensal");
    IupSetAttribute(total, "VALUE", "OFF");
    IupSetAttribute(mensal, "VALUE", "OFF");
    char *a = IupGetAttribute(self, "NAME");
    if (strcmp(a, "total") == 0){
        relatorioOutros();
    }
    else{
        pedeMes(6);
    }
    return EXIT_SUCCESS;
}
int chamaOutros(){
    Ihandle *toggle, *toggle2, *teste;

    toggle = IupToggle("Relatório Total", NULL);
    IupSetAttribute(toggle, "NAME", "total");
    IupSetAttribute(toggle, "FONTSIZE", "15");
    IupSetAttribute(toggle, "VALUE", "0");
    IupSetCallback(toggle, "ACTION", (Icallback)pegaOutros);
    toggle2 = IupToggle("Relatório Mensal", NULL);
    IupSetAttribute(toggle2, "NAME", "mensal");
    IupSetAttribute(toggle2, "FONTSIZE", "15");
    IupSetAttribute(toggle2, "VALUE", "0");
    IupSetCallback(toggle2, "ACTION", (Icallback)pegaOutros);

    Ihandle *lbl2 = IupLabel("\n\n");
    IupVar.label = IupLabel("\n\n\n");
    Ihandle *lbl3 = IupLabel("\n\n");

    IupVar.vbox = IupVbox(lbl2, toggle, IupVar.label, toggle2, lbl3, NULL);
    IupSetAttribute(IupVar.vbox, "MARGIN", "30x30");
    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Escolha de relatório");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    return IUP_CLOSE;
}

int pegaAlimentacao(Ihandle *self, int v){
    Ihandle *total = IupGetDialogChild(self, "total");
    Ihandle *mensal = IupGetDialogChild(self, "mensal");
    IupSetAttribute(total, "VALUE", "OFF");
    IupSetAttribute(mensal, "VALUE", "OFF");
    char *a = IupGetAttribute(self, "NAME");
    if (strcmp(a, "total") == 0){
        relatorioAlimentacao();
    }
    else{
        pedeMes(4);
    }
    return EXIT_SUCCESS;
}
int chamaAlimentacao(){
    Ihandle *toggle, *toggle2, *teste;

    toggle = IupToggle("Relatório Total", NULL);
    IupSetAttribute(toggle, "NAME", "total");
    IupSetAttribute(toggle, "FONTSIZE", "15");
    IupSetAttribute(toggle, "VALUE", "0");
    IupSetCallback(toggle, "ACTION", (Icallback)pegaAlimentacao);
    toggle2 = IupToggle("Relatório Mensal", NULL);
    IupSetAttribute(toggle2, "NAME", "mensal");
    IupSetAttribute(toggle2, "FONTSIZE", "15");
    IupSetAttribute(toggle2, "VALUE", "0");
    IupSetCallback(toggle2, "ACTION", (Icallback)pegaAlimentacao);

    Ihandle *lbl2 = IupLabel("\n\n");
    IupVar.label = IupLabel("\n\n\n");
    Ihandle *lbl3 = IupLabel("\n\n");

    IupVar.vbox = IupVbox(lbl2, toggle, IupVar.label, toggle2, lbl3, NULL);
    IupSetAttribute(IupVar.vbox, "MARGIN", "30x30");
    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Escolha de relatório");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    return IUP_CLOSE;
}

int pegaTransporte(Ihandle *self, int v){
    Ihandle *total = IupGetDialogChild(self, "total");
    Ihandle *mensal = IupGetDialogChild(self, "mensal");
    IupSetAttribute(total, "VALUE", "OFF");
    IupSetAttribute(mensal, "VALUE", "OFF");
    char *a = IupGetAttribute(self, "NAME");
    if (strcmp(a, "total") == 0){
        relatorioTransporte();
    }
    else{
        pedeMes(5);
    }
    return EXIT_SUCCESS;
}
int chamaTrasporte(){
    Ihandle *toggle, *toggle2, *teste;

    toggle = IupToggle("Relatório Total", NULL);
    IupSetAttribute(toggle, "NAME", "total");
    IupSetAttribute(toggle, "FONTSIZE", "15");
    IupSetAttribute(toggle, "VALUE", "0");
    IupSetCallback(toggle, "ACTION", (Icallback)pegaTransporte);
    toggle2 = IupToggle("Relatório Mensal", NULL);
    IupSetAttribute(toggle2, "NAME", "mensal");
    IupSetAttribute(toggle2, "FONTSIZE", "15");
    IupSetAttribute(toggle2, "VALUE", "0");
    IupSetCallback(toggle2, "ACTION", (Icallback)pegaTransporte);

    Ihandle *lbl2 = IupLabel("\n\n");
    IupVar.label = IupLabel("\n\n\n");
    Ihandle *lbl3 = IupLabel("\n\n");

    IupVar.vbox = IupVbox(lbl2, toggle, IupVar.label, toggle2, lbl3, NULL);
    IupSetAttribute(IupVar.vbox, "MARGIN", "30x30");
    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Escolha de relatório");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    return IUP_CLOSE;
}

int escreveArquivo(struct info dados, int op, int num){
    int verif;
    FILE *file;
    char caminho[maxCaminho];

    strcpy(caminho, pessoaAtual);

    if(op == 1)
        strcat(caminho, "/moradia.txt");
    else if(op == 2)
        strcat(caminho, "/estudo.txt");
    else if(op == 3)
        strcat(caminho, "/transporte.txt");
    else if(op == 4)
        strcat(caminho, "/alimentacao.txt");
    else if(op == 5)
        strcat(caminho, "/trabalho.txt");
    else if(op == 6)
        strcat(caminho, "/outros.txt");

    verif = existeArq(caminho);

    if (verif == 1){
        file = fopen(caminho, "a");
        if(num == 0) {
            fprintf(file, "%d %d %d d %.2lf %s\n", dados.dia, dados.mes, dados.ano, dados.valor, dados.desc);
            atualizaGeral(0, dados.valor);
        }
        else {
            fprintf(file, "%d %d %d s %.2lf %s\n", dados.dia, dados.mes, dados.ano, dados.valor, dados.desc);
            atualizaGeral(1, dados.valor);
        }
        fclose(file);
    }
    else {
        file = fopen(caminho, "w");
        if(num == 0) {
            fprintf(file, "%d %d %d d %.2lf %s\n", dados.dia, dados.mes, dados.ano, dados.valor, dados.desc);
            atualizaGeral(0, dados.valor);
        }
        else {
            fprintf(file, "%d %d %d s %.2lf %s\n", dados.dia, dados.mes, dados.ano, dados.valor, dados.desc);
            atualizaGeral(1, dados.valor);
        }
        fclose(file);
    }
}

int pegaDados(Ihandle *self){
    //printf("%s\n", pessoaAtual);
    Ihandle *pegaDia, *pegaMes, *pegaAno, *pegaQt;
    Ihandle *pegaOpcao, *pegaDesc;

    struct info dados;

    pegaDia = IupGetDialogChild(self, "dia");
    pegaMes = IupGetDialogChild(self, "mes");
    pegaAno = IupGetDialogChild(self, "ano");
    pegaQt = IupGetDialogChild(self, "quantia");
    pegaOpcao = IupGetDialogChild(self, "lista");
    pegaDesc = IupGetDialogChild(self, "descricao");

    dados.dia = IupGetInt(pegaDia, "VALUE");
    dados.mes = IupGetInt(pegaMes, "VALUE");
    dados.ano = IupGetInt(pegaAno, "VALUE");
    dados.valor = IupGetDouble(pegaQt, "VALUE");
    dados.desc = IupGetAttribute(pegaDesc, "VALUE");

    int categoria = IupGetInt(pegaOpcao, "VALUE");

    if(dados.dia < 1 || dados.dia > 31)
        aviso("Erro", "Dia inválido. Tente novamente.");
    else if(dados.mes < 1 || dados.mes > 12)
        aviso("Erro", "Mês inválido. Tente novamente.");
    else if(dados.ano < 0)
        aviso("Erro", "Ano inválido. Tente novamente.");
    else if(strlen(dados.desc) > MAX)
        aviso("Erro", "Descrição muito grande. Tente novamente.");
    else if(categoria == 0)
        aviso("Erro", "Por favor selecione uma categoria.");
    else if (strlen(dados.desc) == 0)
        aviso("Erro", "Por favor escreva uma descrição.");
    else {
        if (deposita_saque == 0)
            escreveArquivo(dados, categoria, 0);
        else
            escreveArquivo(dados, categoria, 1);
        aviso("Sucesso", "Operação realizada!");
        limpaCampos();
    }
}
int deposita(){
    deposita_saque = 0;
    IupOpen(&argc, &argv);

    /*/////////////////////// ITENS /////////////////////////////*/

    //botão
    IupVar.btn = IupButton("DEPOSITAR", NULL);
    IupSetCallback(IupVar.btn, "ACTION", (Icallback)pegaDados);

    //labels
    IupVar.label = IupLabel("Complete os campos a seguir:\n\n\n");
    IupSetAttribute(IupVar.label, "FONTSIZE", "10");

    varMov.diaL = IupLabel("Dia: ");
    varMov.mesL = IupLabel("Mês: ");
    varMov.anoL = IupLabel("Ano: ");

    varMov.movL = IupLabel("Valor a ser depositado (caso o valor seja decimal, use o ponto):   R$");
    IupSetAttribute(varMov.movL, "FONTSIZE", "10");

    varMov.tipo = IupLabel("Categoria: ");

    varMov.descricaoL = IupLabel("Descrição (no máximo uma palavra): ");

    //lista
    varMov.list = IupList(NULL);

    IupSetAttribute(varMov.list, "NAME", "lista");
    IupSetAttribute(varMov.list, "DROPDOWN", "YES");
    IupSetAttribute(varMov.list, "1", "Moradia");
    IupSetAttribute(varMov.list, "2", "Estudo");
    IupSetAttribute(varMov.list, "3", "Transporte");
    IupSetAttribute(varMov.list, "4", "Alimentação");
    IupSetAttribute(varMov.list, "5", "Trabalho");
    IupSetAttribute(varMov.list, "6", "Outros");
    IupSetAttribute(varMov.list, "VALUE", "0");

    //texts
    varMov.movT = IupText(NULL);
    IupSetAttribute(varMov.movT, "SIZE", "50");
    IupSetAttribute(varMov.movT, "NAME", "quantia");

    varMov.diaT = IupText(NULL);
    IupSetAttribute(varMov.diaT, "SIZE", "50");
    IupSetAttribute(varMov.diaT, "NAME", "dia");

    varMov.mesT = IupText(NULL);
    IupSetAttribute(varMov.mesT, "SIZE", "50");
    IupSetAttribute(varMov.mesT, "NAME", "mes");

    varMov.anoT = IupText(NULL);
    IupSetAttribute(varMov.anoT, "SIZE", "50");
    IupSetAttribute(varMov.anoT, "NAME", "ano");

    varMov.descricaoT = IupText(NULL);
    IupSetAttribute(varMov.descricaoT, "SIZE", "100");
    IupSetAttribute(varMov.descricaoT, "NAME", "descricao");

    IupVar.hbox = IupHbox(
            varMov.diaL, varMov.diaT,
            varMov.mesL, varMov.mesT,
            varMov.anoL, varMov.anoT,
            NULL);

    varMov.hbox2 = IupHbox(varMov.movL, varMov.movT, NULL);
    IupSetAttribute(varMov.hbox2, "ALIGNMENT", "ACENTER");

    varMov.hbox3 = IupHbox(varMov.tipo, varMov.list, NULL);
    IupSetAttribute(varMov.hbox3, "ALIGNMENT", "ACENTER");

    varMov.hbox4 = IupHbox(varMov.descricaoL, varMov.descricaoT, NULL);
    IupSetAttribute(varMov.hbox4, "ALIGNMENT", "ACENTER");

    Ihandle *lb = IupLabel("");

    IupVar.vbox = IupVbox(
            IupVar.label,
            IupVar.hbox,
            varMov.hbox2, varMov.hbox3, varMov.hbox4,
            lb,
            IupVar.btn,
            NULL);
    IupSetAttribute(IupVar.vbox, "MARGIN", "40x30");
    IupSetAttribute(IupVar.vbox, "GAP", "5");
    IupSetAttribute(IupVar.vbox, "ALIGNMENT", "ACENTER");

    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Depositar");
    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    /*/////////////////////// ITENS /////////////////////////////*/

    IupMainLoop();
    IupExitLoop();
}
int saque(){
    deposita_saque = 1;
    IupOpen(&argc, &argv);

    /*/////////////////////// ITENS /////////////////////////////*/

    //botão
    IupVar.btn = IupButton("SACAR", NULL);
    IupSetCallback(IupVar.btn, "ACTION", (Icallback)pegaDados);

    //labels
    IupVar.label = IupLabel("Complete os campos a seguir:\n\n\n");
    IupSetAttribute(IupVar.label, "FONTSIZE", "10");

    varMov.diaL = IupLabel("Dia: ");
    varMov.mesL = IupLabel("Mês: ");
    varMov.anoL = IupLabel("Ano: ");

    varMov.movL = IupLabel("Valor a ser sacado (caso o valor seja decimal, use o ponto):   R$");
    IupSetAttribute(varMov.movL, "FONTSIZE", "10");

    varMov.tipo = IupLabel("Categoria: ");

    varMov.descricaoL = IupLabel("Descrição (no máximo uma palavra): ");

    //lista
    varMov.list = IupList(NULL);

    IupSetAttribute(varMov.list, "NAME", "lista");
    IupSetAttribute(varMov.list, "DROPDOWN", "YES");
    IupSetAttribute(varMov.list, "1", "Moradia");
    IupSetAttribute(varMov.list, "2", "Estudo");
    IupSetAttribute(varMov.list, "3", "Transporte");
    IupSetAttribute(varMov.list, "4", "Alimentação");
    IupSetAttribute(varMov.list, "5", "Trabalho");
    IupSetAttribute(varMov.list, "6", "Outros");
    IupSetAttribute(varMov.list, "VALUE", "0");

    //texts
    varMov.movT = IupText(NULL);
    IupSetAttribute(varMov.movT, "SIZE", "50");
    IupSetAttribute(varMov.movT, "NAME", "quantia");

    varMov.diaT = IupText(NULL);
    IupSetAttribute(varMov.diaT, "SIZE", "50");
    IupSetAttribute(varMov.diaT, "NAME", "dia");

    varMov.mesT = IupText(NULL);
    IupSetAttribute(varMov.mesT, "SIZE", "50");
    IupSetAttribute(varMov.mesT, "NAME", "mes");

    varMov.anoT = IupText(NULL);
    IupSetAttribute(varMov.anoT, "SIZE", "50");
    IupSetAttribute(varMov.anoT, "NAME", "ano");

    IupSetAttribute(varMov.diaT, "VALUE", "");
    IupSetAttribute(varMov.mesT, "VALUE", "");
    IupSetAttribute(varMov.anoT, "VALUE", "");

    varMov.descricaoT = IupText(NULL);
    IupSetAttribute(varMov.descricaoT, "SIZE", "100");
    IupSetAttribute(varMov.descricaoT, "NAME", "descricao");

    IupVar.hbox = IupHbox(
            varMov.diaL, varMov.diaT,
            varMov.mesL, varMov.mesT,
            varMov.anoL, varMov.anoT,
            NULL);

    varMov.hbox2 = IupHbox(varMov.movL, varMov.movT, NULL);
    IupSetAttribute(varMov.hbox2, "ALIGNMENT", "ACENTER");

    varMov.hbox3 = IupHbox(varMov.tipo, varMov.list, NULL);
    IupSetAttribute(varMov.hbox3, "ALIGNMENT", "ACENTER");

    varMov.hbox4 = IupHbox(varMov.descricaoL, varMov.descricaoT, NULL);
    IupSetAttribute(varMov.hbox4, "ALIGNMENT", "ACENTER");

    Ihandle *lb = IupLabel("");

    IupVar.vbox = IupVbox(
            IupVar.label,
            IupVar.hbox,
            varMov.hbox2, varMov.hbox3, varMov.hbox4,
            lb,
            IupVar.btn,
            NULL);
    IupSetAttribute(IupVar.vbox, "MARGIN", "40x30");
    IupSetAttribute(IupVar.vbox, "GAP", "5");
    IupSetAttribute(IupVar.vbox, "ALIGNMENT", "ACENTER");

    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Sacar");
    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    /*/////////////////////// ITENS /////////////////////////////*/

    IupMainLoop();
    IupExitLoop();
}

void pegaNome(char nome[255]){
    int i;

    char caminho[maxCaminho];
    char dindin[255];

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/geral.txt");

    FILE *arq = fopen(caminho, "r");
    fgets(nomeAtual, 255, (FILE*)arq);
    fgets(dindin, 255, (FILE*)arq);
    fclose(arq);

    saldo = atof(dindin);

    for(i=0; nomeAtual[i]!='\0'; i++){
        if(nomeAtual[i]=='\n'){
            nomeAtual[i]= '\0';
            break;
        }
    }
    strcat(nome, nomeAtual);
}

int consultaSaldo(){
    char guardaSaldo[1020];
    snprintf(guardaSaldo, sizeof guardaSaldo, "%s %.2lf", "Seu saldo atual é de R$", saldo);

    IupVar.label = IupLabel(guardaSaldo);
    IupSetAttribute(IupVar.label, "FONTSIZE", "10");
    IupMessage("Saldo", guardaSaldo);
}

int TelaInicial(int argc, char **argv){

    Ihandle *transacoes_menu, *movimenta_menu;
    Ihandle *item_coloca, *item_retira;
    Ihandle *item_moradia, *item_estudo, *item_tranporte, *item_alimentcao, *item_trabalho, *item_outros, *item_anual;
    Ihandle *sub_menu_transacoes, *sub_menu_movimenta, *menu;

    IupOpen(&argc, &argv);

    /*/////////////////////////// MENU ////////////////////////////////////*/

    item_coloca = IupItem("Depositar", NULL);
    item_retira = IupItem("Sacar", NULL);

    item_moradia = IupItem("Moradia", NULL);
    item_estudo = IupItem("Estudo", NULL);
    item_tranporte = IupItem("Transporte", NULL);
    item_alimentcao = IupItem("Alimentação", NULL);
    item_trabalho = IupItem("Trabalho", NULL);
    item_outros = IupItem("Outros", NULL);
    item_anual = IupItem("Anual", NULL);

    transacoes_menu = IupMenu(
            item_coloca,
            item_retira,
            NULL
    );

    movimenta_menu = IupMenu(
            item_moradia,
            item_estudo,
            item_tranporte,
            item_alimentcao,
            item_trabalho,
            item_outros,
            item_anual,
            NULL
    );

    sub_menu_transacoes = IupSubmenu("Transações", transacoes_menu);
    sub_menu_movimenta = IupSubmenu("Relatórios", movimenta_menu);
    menu = IupMenu(
            sub_menu_transacoes,
            sub_menu_movimenta,
            NULL
    );

    IupSetCallback(item_coloca, "ACTION", (Icallback)deposita);
    IupSetCallback(item_retira, "ACTION", (Icallback)saque);
    //
    IupSetCallback(item_moradia, "ACTION", (Icallback) chamaMoradia);
    //
    IupSetCallback(item_estudo, "ACTION", (Icallback)chamaEstudo);
    IupSetCallback(item_tranporte, "ACTION", (Icallback)chamaTrasporte);
    IupSetCallback(item_alimentcao, "ACTION", (Icallback)chamaAlimentacao);
    IupSetCallback(item_trabalho, "ACTION", (Icallback)chamaTrabalho);
    IupSetCallback(item_outros, "ACTION", (Icallback)chamaOutros);
    IupSetCallback(item_anual, "ACTION", (Icallback)pedeAno);

    /*/////////////////////////// MENU ////////////////////////////////////*/

    //mensagem inicial

    char nome[100] = "Bem-vindo(a) à sua carteira pessoal, ";
    pegaNome(nome);
    strcat(nome, "!\n\n");

    IupVar.label = IupLabel(nome);
    IupSetAttribute(IupVar.label, "FONTSIZE", "20");

    Ihandle *btn2 = IupButton("Consultar saldo", NULL);
    IupSetAttribute(btn2, "FONTSIZE", "15");
    IupSetCallback(btn2, "ACTION", consultaSaldo);

    //botão saída
    IupVar.button_exit = IupButton("SAIR", NULL);
    IupSetCallback(IupVar.button_exit, "ACTION", (Icallback)botao_exit);
    IupSetAttribute(IupVar.button_exit, "SIZE", "35x20");

    //vbox
    IupVar.vbox = IupVbox(
            IupVar.label,
            btn2,
            IupVar.button_exit,
            NULL);
    IupSetAttribute(IupVar.vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(IupVar.vbox, "MARGIN", "100x60");
    IupSetAttribute(IupVar.vbox, "GAP", "40");

    IupVar.dlg = IupDialog(IupVar.vbox);

    IupSetAttribute(IupVar.dlg, "TITLE", "Carteira Pessoal");
    IupSetAttributeHandle(IupVar.dlg, "MENU", menu);

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    return EXIT_SUCCESS;
}

int analisaCadastro(Ihandle *entrada){
    Ihandle *nome = IupGetDialogChild(entrada, "NOME");
    Ihandle *valor = IupGetDialogChild(entrada, "VALOR");
    double v = IupGetDouble(valor, "VALUE");
    char *n = IupGetAttribute(nome, "VALUE");
    char pessoa[50];
    strcpy(pessoa, pessoaAtual);
    strcat(pessoa, "/geral.txt");
    FILE *arq = fopen(pessoa, "w");

    fprintf(arq,"%s\n",n);
    fprintf(arq,"%.2lf\n",v);

    fclose(arq);
    return IUP_CLOSE;
}
void CadastraUsuario(int argc, char **argv){
    Ihandle *btcadastra;
    Ihandle *labelNome, *labelValor;
    Ihandle *nome, *valor;

    IupOpen(&argc, &argv);

    IupVar.label = IupLabel("Olá, novo usuário(a), cadastre-se para desfrutar de nossos serviços!\n\n\n");
    IupSetAttribute(IupVar.label, "FONT", "DEFAULTFONT , 14");
    labelNome = IupLabel("Insira seu nome :");
    IupSetAttribute(labelNome, "FONT", "DEFAULTFONT , 12");
    labelValor = IupLabel("Quantia inicial da conta (para número decimal, utilize o ponto) :");
    IupSetAttribute(labelValor, "FONT", "DEFAULTFONT , 12");

    nome = IupText(NULL);
    IupSetAttribute(nome, "SIZE", "150");
    IupSetAttribute(nome, "NAME", "NOME");
    IupSetAttribute(nome, "VALUE", "");
    IupSetAttribute(nome, "EXPAND", "YES");

    valor = IupText(NULL);
    IupSetAttribute(valor, "SIZE", "100");
    IupSetAttribute(valor, "NAME", "VALOR");
    IupSetAttribute(valor, "VALUE", "");

    btcadastra = IupButton("Cadastrar", NULL);
    IupSetAttribute(btcadastra, "SIZE", "80x20");
    IupSetCallback(btcadastra, "ACTION", (Icallback)analisaCadastro);
    IupVar.vbox = IupVbox(
            IupVar.label,
            labelNome,
            nome,
            labelValor,
            valor,
            btcadastra,
            NULL
    );

    IupSetAttribute(IupVar.vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(IupVar.vbox, "GAP", "30");
    IupSetAttribute(IupVar.vbox, "MARGIN", "150x150");

    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Carteira Pessoal");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    IupClose();
    EXIT_SUCCESS;
}

int contaDigitos(unsigned long long int valor){
    int tam = 0;
    if (valor == 0)
        tam = 1;
    else{
        while (valor != 0){
            tam += 1;
            valor /= 10;
        }
    }
    return tam;
}

int analisaCPF(Ihandle *inserecpf){
    Ihandle *a = IupGetDialogChild(inserecpf, "CPF");
    unsigned long long int value = IupGetDouble(a, "VALUE");

    int tam = contaDigitos(value);

    if (tam == 11){
        char cpf[12];
        sprintf(cpf, "%.llu", value);

        char pasta[23] = "./Pessoas/";
        strcat(pasta, cpf);
        strcpy(pessoaAtual, pasta);
        if(mkdir(pasta)){
            globalVar = 1;
        }
        else{
            globalVar = 2;
        }
        return IUP_CLOSE;
    }
    else{
        if(tam > 11)
            aviso("Erro", "CPF contém mais de 11 dígitos.\n\nTente novamente.");
        else
            aviso("Erro", "CPF contém menos de 11 dígitos ou caracteres não numéricos.\n\nTente novamente.");
    }
}
void pedeCPF(int argc, char **argv){
    Ihandle *inserecpf, *label2, *fill;

    IupOpen(&argc, &argv);

    label2 = IupLabel("Olá, usuário!\n\n");
    IupSetAttribute(label2, "FONTSIZE", "15");

    IupVar.label = IupLabel("Digite o seu CPF (apenas números):\n\n");
    IupSetAttribute(IupVar.label, "FONTSIZE", "12");

    IupVar.btn = IupButton("Enviar", NULL);
    IupSetAttribute(IupVar.btn, "FONTSIZE", "10");
    IupSetCallback(IupVar.btn, "ACTION", (Icallback) analisaCPF);

    inserecpf = IupText(NULL);
    IupSetAttribute(inserecpf, "SIZE", "60");
    IupSetAttribute(inserecpf, "FONTSIZE", "10");
    IupSetAttribute(inserecpf, "VALUE", "");
    IupSetAttribute(inserecpf, "NAME", "CPF");

    IupVar.vbox = IupVbox(label2, IupVar.label, inserecpf, IupVar.btn, NULL);
    IupSetAttribute(IupVar.vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(IupVar.vbox, "GAP", "10");
    IupSetAttribute(IupVar.vbox, "MARGIN", "70x40");
    IupSetAttribute(IupVar.vbox, "GAP", "20");
    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Carteira Pessoal");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    IupClose();

    EXIT_SUCCESS;
};

int main(int argc, char **argv){
    mkdir("./Pessoas");

    pedeCPF(argc, argv);

    // já cadastrado
    if(globalVar==1){
        TelaInicial(argc, argv);
    }
    //não cadastrado
    else if(globalVar==2){
        CadastraUsuario(argc, argv);
        TelaInicial(argc, argv);
    }
}
