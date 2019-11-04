#include <stdlib.h>
#include <stdio.h>
#include <iup.h>
#include <sys/stat.h>

#define MAX 100

int globalVar = 0;
//deposita = 0, saque = 1
int deposita_saque = 0;
char pessoaAtual[23];

struct variaveis{
    Ihandle *dlg, *label, *vbox, *hbox;
    Ihandle *btn, *button_exit;
}IupVar;
struct info{
    int dia, mes, ano;
    float valor;
    char *desc;
};
struct movimenta{
    Ihandle *diaT, *diaL, *mesT, *mesL, *anoT, *anoL, *tipo, *descricaoT, *descricaoL;
    Ihandle *movT, *movL;
    Ihandle *hbox2, *hbox3, *hbox4, *list;
}varMov;

/*/////funções/////*/
int botao_exit(Ihandle *self);
void aviso(char titulo[], char msgm[]);
int existeArq(char caminho[]);
void moradia(struct info dados);
void pegaDeposito(Ihandle *self);
void deposita(int argc, char **argv);
void saque(int argc, char **argv);
void pegaNome(char nome[255]);
int TelaInicial(int argc, char **argv);
int analisaCadastro(Ihandle *entrada);
void CadastraUsuario(int argc, char **argv);
int contaDigitos(unsigned long long int valor);
int analisaCPF(Ihandle *inserecpf);
void pedeCPF(int argc, char **argv);
/*/////funções/////*/

void atualizaGeral(){
    char caminho[34];
    char var[255];
    double val;
    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/geral.txt");

    FILE *arq = fopen(caminho, "r");
    fscanf(arq, "%*s\n%lf", &val);
    fclose(arq);

    printf("%lf", val);
}


int botao_exit(Ihandle *self){
    IupVar.dlg = IupMessageDlg();

    IupSetAttribute(IupVar.dlg, "DIALOGTYPE", "WARNING");
    //IupSetAttribute(dlg, "TITLE", "Saída");
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

int existeArq(char caminho[]){
    if(!fopen(caminho, "r"))
        //n existe
        return 0;
    return 1;
}

void escreveArquivo(struct info dados, int op, int num){
    int verif;
    FILE *file;
    char caminho[39];

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

    //printf("%s\n", caminho);

    verif = existeArq(caminho);

    if (verif == 1){
        file = fopen(caminho, "a");
        if(num == 0)
            fprintf(file, "%d %d %d d %.2lf %s\n", dados.dia, dados.mes, dados.ano, dados.valor, dados.desc);
        else
            fprintf(file, "%d %d %d s %.2lf %s\n", dados.dia, dados.mes, dados.ano, dados.valor, dados.desc);
        fclose(file);
        //printf("existe e deu certo\n");
    }
    else {
        file = fopen(caminho, "w");
        if(num == 0)
            fprintf(file, "%d %d %d d %.2lf %s\n", dados.dia, dados.mes, dados.ano, dados.valor, dados.desc);
        else
            fprintf(file, "%d %d %d s %.2lf %s\n", dados.dia, dados.mes, dados.ano, dados.valor, dados.desc);
        fclose(file);
        //printf("n existe e deu certo\n");
    }
}

void pegaDados(Ihandle *self){
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

    printf("%s\n", dados.desc);
    printf("%llu\n", strlen(dados.desc));

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
    else if (categoria == 6 && strlen(dados.desc) == 0)
        aviso("Erro", "Por favor escreva uma descrição.");
    else {
        if (deposita_saque == 0)
            escreveArquivo(dados, categoria, 0);
        else
            escreveArquivo(dados, categoria, 1);
        aviso("Sucesso", "Operação realizada!");
        //IupClose();
    }
}

void deposita(int argc, char **argv){
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

    varMov.descricaoL = IupLabel("Descrição: ");

    //lista
    varMov.list = IupList(NULL);

    IupSetAttribute(varMov.list, "NAME", "lista");
    //IupSetAttribute(list, "EXPAND", "HORIZONTAL");
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
    //IupSetAttribute(movT, "MASK", IUP_MASK_INT);

    varMov.diaT = IupText(NULL);
    IupSetAttribute(varMov.diaT, "SIZE", "50");
    IupSetAttribute(varMov.diaT, "NAME", "dia");
    //IupSetAttribute(diaT, "MASK", IUP_MASK_INT);

    varMov.mesT = IupText(NULL);
    IupSetAttribute(varMov.mesT, "SIZE", "50");
    IupSetAttribute(varMov.mesT, "NAME", "mes");
    //IupSetAttribute(mesT, "MASK", IUP_MASK_INT);

    varMov.anoT = IupText(NULL);
    IupSetAttribute(varMov.anoT, "SIZE", "50");
    IupSetAttribute(varMov.anoT, "NAME", "ano");
    //IupSetAttribute(anoT, "MASK", IUP_MASK_INT);

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
    //IupSetAttribute(IupVar.hbox, "GAP", "15");
    //IupSetAttribute(IupVar.hbox, "ALIGNMENT", "ACENTER");

    varMov.hbox2 = IupHbox(varMov.movL, varMov.movT, NULL);
    //IupSetAttribute(hbox2, "MARGIN", "0x50");
    //IupSetAttribute(hbox2, "GAP", "5");
    IupSetAttribute(varMov.hbox2, "ALIGNMENT", "ACENTER");

    varMov.hbox3 = IupHbox(varMov.tipo, varMov.list, NULL);
    //IupSetAttribute(hbox3, "GAP", "5");
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

    //IupSetAttribute(movL, "FLOATING", "YES");

    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Depositar");
    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    /*/////////////////////// ITENS /////////////////////////////*/

    IupMainLoop();
}

void saque(int argc, char **argv){
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

    varMov.descricaoL = IupLabel("Descrição: ");

    //lista
    varMov.list = IupList(NULL);

    IupSetAttribute(varMov.list, "NAME", "lista");
    //IupSetAttribute(list, "EXPAND", "HORIZONTAL");
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
    //IupSetAttribute(movT, "MASK", IUP_MASK_INT);

    varMov.diaT = IupText(NULL);
    IupSetAttribute(varMov.diaT, "SIZE", "50");
    IupSetAttribute(varMov.diaT, "NAME", "dia");
    //IupSetAttribute(diaT, "MASK", IUP_MASK_INT);

    varMov.mesT = IupText(NULL);
    IupSetAttribute(varMov.mesT, "SIZE", "50");
    IupSetAttribute(varMov.mesT, "NAME", "mes");
    //IupSetAttribute(mesT, "MASK", IUP_MASK_INT);

    varMov.anoT = IupText(NULL);
    IupSetAttribute(varMov.anoT, "SIZE", "50");
    IupSetAttribute(varMov.anoT, "NAME", "ano");
    //IupSetAttribute(anoT, "MASK", IUP_MASK_INT);

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
    //IupSetAttribute(IupVar.hbox, "GAP", "15");
    //IupSetAttribute(IupVar.hbox, "ALIGNMENT", "ACENTER");

    varMov.hbox2 = IupHbox(varMov.movL, varMov.movT, NULL);
    //IupSetAttribute(hbox2, "MARGIN", "0x50");
    //IupSetAttribute(hbox2, "GAP", "5");
    IupSetAttribute(varMov.hbox2, "ALIGNMENT", "ACENTER");

    varMov.hbox3 = IupHbox(varMov.tipo, varMov.list, NULL);
    //IupSetAttribute(hbox3, "GAP", "5");
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

    //IupSetAttribute(movL, "FLOATING", "YES");

    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Sacar");
    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    /*/////////////////////// ITENS /////////////////////////////*/

    IupMainLoop();
}

void pegaNome(char nome[255]){
    int i;

    //printf("%s\n",pessoaAtual);
    char caminho[34];
    char var[255];
    char dindin[255];

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/geral.txt");

    FILE *arq = fopen(caminho, "r");
    fgets(var, 255, (FILE*)arq);
    fgets(dindin, 255, (FILE*)arq);
    fclose(arq);
    
    printf("%s",dindin);
    double dindinreal = atof(dindin);
    printf("%lf", dindinreal+10);

    for(i=0; var[i]!='\0'; i++){
        if(var[i]=='\n'){
            var[i]= '\0';
            break;
        }
    }
    strcat(nome, var);
    //printf("%s", nome);
};

int TelaInicial(int argc, char **argv){
    //printf("%s", pessoaAtual);

    Ihandle *transacoes_menu, *movimenta_menu;
    Ihandle *item_coloca, *item_retira;
    Ihandle *item_moradia, *item_estudo, *item_tranporte, *item_alimentcao, *item_trabalho, *item_geral;
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
    item_geral = IupItem("Geral", NULL);

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
            item_geral,
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

    /*/////////////////////////// MENU ////////////////////////////////////*/

    //mensagem inicial

    char nome[100] = "Bem-vindo(a), ";
    pegaNome(nome);
    strcat(nome, ", à sua carteira pessoal!");

    IupVar.label = IupLabel(nome);
    IupSetAttribute(IupVar.label, "FONTSIZE", "20");

    //botão saída
    IupVar.button_exit = IupButton("SAIR", NULL);
    IupSetCallback(IupVar.button_exit, "ACTION", (Icallback)botao_exit);
    IupSetAttribute(IupVar.button_exit, "SIZE", "35x20");

    //vbox
    IupVar.vbox = IupVbox(
            IupVar.label,
            IupVar.button_exit,
            NULL);
    IupSetAttribute(IupVar.vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(IupVar.vbox, "MARGIN", "200x200");
    IupSetAttribute(IupVar.vbox, "GAP", "40");

    IupVar.dlg = IupDialog(IupVar.vbox);

    IupSetAttribute(IupVar.dlg, "TITLE", "Carteira Pessoal");
    IupSetAttributeHandle(IupVar.dlg, "MENU", menu);
    //IupSetAttribute(IupVar.dlg, "BACKGROUND", "BACKGROUND");

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    IupClose();
    return EXIT_SUCCESS;
}

int analisaCadastro(Ihandle *entrada){
    Ihandle *nome = IupGetDialogChild(entrada, "NOME");
    Ihandle *valor = IupGetDialogChild(entrada, "VALOR");
    double v = IupGetDouble(valor, "VALUE");
    char *n = IupGetAttribute(nome, "VALUE");
    //printf("%.2lf\n", v);
    //printf("%s\n", n);
    char pessoa[50];
    strcpy(pessoa, pessoaAtual);
    strcat(pessoa, "/geral.txt");
    //printf("%s\n", pessoa);
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

    IupVar.label = IupLabel("Olá novo usuario(a), cadastre-se para desfrutar de nossos servicos : \n\n\n");
    IupSetAttribute(IupVar.label, "FONT", "DEFAULTFONT , 14");
    labelNome = IupLabel("Insira seu nome :");
    IupSetAttribute(labelNome, "FONT", "DEFAULTFONT , 12");
    labelValor = IupLabel("Quantia inicial da conta (utilize ponto) :");
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

    TelaInicial(argc, argv);
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
        /*
        printf("%s\n", pessoaAtual);
        printf("%s", pasta);
        printf("%lu", tam_cpf);
        */
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
    Ihandle *btconfirma, *inserecpf;

    IupOpen(&argc, &argv);

    IupVar.label = IupLabel("Digite o seu CPF (apenas números):\n");

    btconfirma = IupButton("Enviar", NULL);
    IupSetCallback(btconfirma, "ACTION", (Icallback) analisaCPF);

    inserecpf = IupText(NULL);
    IupSetAttribute(inserecpf, "EXPAND", "YES");
    IupSetAttribute(inserecpf, "VALUE", "");
    IupSetAttribute(inserecpf, "NAME", "CPF");

    IupVar.vbox = IupVbox(IupVar.label, inserecpf, btconfirma, NULL);
    IupSetAttribute(IupVar.vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(IupVar.vbox, "GAP", "10");
    IupSetAttribute(IupVar.vbox, "MARGIN", "100x100");
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
    }

    //deposita(argc, argv);
}
