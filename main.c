#include <stdlib.h>
#include <stdio.h>
#include <iup.h>
#include <sys/stat.h>

int globalVar = 0;
char pessoaAtual[22];

struct variaveis{
    Ihandle *dlg, *label, *vbox, *hbox;
    Ihandle *btn, *button_exit;
}IupVar;

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

int PegaData(Ihandle *self){
    Ihandle *pegaDia, *pegaMes, *pegaAno;

    pegaDia = IupGetDialogChild(self, "dia");
    pegaMes = IupGetDialogChild(self, "mes");
    pegaAno = IupGetDialogChild(self, "ano");

    int dia = IupGetInt(pegaDia, "VALUE");
    int mes = IupGetInt(pegaMes, "VALUE");
    int ano = IupGetInt(pegaAno, "VALUE");

    //printf("%d/%d/%d", dia, mes, ano);

    //IupSetStrf(pegaDia, "VALUE", "%d", dia);

    return IUP_DEFAULT;
}

void deposita(int argc, char **argv){
    IupOpen(&argc, &argv);

    Ihandle *diaT, *diaL, *mesT, *mesL, *anoT, *anoL, *descricao;
    Ihandle *movT, *movL;
    Ihandle * hbox2, *hbox3, *list;

    /*/////////////////////// ITENS /////////////////////////////*/

    //botão
    IupVar.btn = IupButton("TESTAR", NULL);

    //labels
    IupVar.label = IupLabel("Complete os campos a seguir:");
    IupSetAttribute(IupVar.label, "FONTSIZE", "10");

    diaL = IupLabel("Dia: ");
    mesL = IupLabel("Mês: ");
    anoL = IupLabel("Ano: ");

    movL = IupLabel("Valor a ser depositado: ");
    IupSetAttribute(movL, "FONTSIZE", "10");

    descricao = IupLabel("Descrição: ");

    //lista
    list = IupList(NULL);

    IupSetAttribute(list, "NAME", "lista");
    //IupSetAttribute(list, "EXPAND", "HORIZONTAL");
    IupSetAttribute(list, "DROPDOWN", "YES");
    IupSetAttribute(list, "1", "Moradia");
    IupSetAttribute(list, "2", "Estudo");
    IupSetAttribute(list, "3", "Transporte");
    IupSetAttribute(list, "4", "Alimentação");
    IupSetAttribute(list, "5", "Trabalho");
    IupSetAttribute(list, "VALUE", "0");

    //texts
    movT = IupText(NULL);
    IupSetAttribute(movT, "SIZE", "50");
    IupSetAttribute(movT, "NAME", "quantia");
    IupSetAttribute(movT, "MASK", IUP_MASK_INT);

    diaT = IupText(NULL);
    IupSetAttribute(diaT, "SIZE", "50");
    IupSetAttribute(diaT, "NAME", "dia");
    IupSetAttribute(diaT, "MASK", IUP_MASK_INT);

    mesT = IupText(NULL);
    IupSetAttribute(mesT, "SIZE", "50");
    IupSetAttribute(mesT, "NAME", "mes");
    IupSetAttribute(mesT, "MASK", IUP_MASK_INT);

    anoT = IupText(NULL);
    IupSetAttribute(anoT, "SIZE", "50");
    IupSetAttribute(anoT, "NAME", "ano");
    IupSetAttribute(anoT, "MASK", IUP_MASK_INT);

    IupSetAttribute(diaT, "VALUE", "");
    IupSetAttribute(mesT, "VALUE", "");
    IupSetAttribute(anoT, "VALUE", "");

    IupSetCallback(IupVar.btn, "ACTION", (Icallback)PegaData);

    IupVar.hbox = IupHbox(
            diaL, diaT,
            mesL, mesT,
            anoL, anoT,
            IupVar.btn,
            NULL);
    IupSetAttribute(IupVar.hbox, "GAP", "15");
    IupSetAttribute(IupVar.hbox, "ALIGNMENT", "ACENTER");

    hbox2 = IupHbox(movL, movT, NULL);
    //IupSetAttribute(hbox2, "MARGIN", "0x50");
    IupSetAttribute(hbox2, "GAP", "5");
    IupSetAttribute(hbox2, "ALIGNMENT", "ACENTER");

    hbox3 = IupHbox(descricao, list, NULL);
    IupSetAttribute(hbox3, "GAP", "5");
    IupSetAttribute(hbox3, "ALIGNMENT", "ACENTER");

    IupVar.vbox = IupVbox(
            IupVar.hbox,
            hbox2, hbox3,
            NULL);
    IupSetAttribute(IupVar.hbox, "MARGIN", "50x50");
    IupSetAttribute(IupVar.vbox, "GAP", "20");
    IupSetAttribute(IupVar.vbox, "ALIGNMENT", "ACENTER");

    //IupSetAttribute(movL, "FLOATING", "YES");

    IupVar.dlg = IupDialog(IupVar.vbox);
    IupSetAttribute(IupVar.dlg, "TITLE", "Depositar");

    /*/////////////////////// ITENS /////////////////////////////*/

    /*///////////////////// ARQUIVO ///////////////////////////*/

    char pasta[22] = "./Pessoas/";
    strcpy(pessoaAtual, pasta);

    /*///////////////////// ARQUIVO ///////////////////////////*/

    IupShowXY(IupVar.dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    IupClose();
}

void saque(int argc, char **argv){
    IupOpen(&argc, &argv);

}

void pegaNome(char nome[100]){
    int i;

    //printf("%s\n",pessoaAtual);
    char caminho[50];
    char var[50];

    strcpy(caminho, pessoaAtual);
    strcat(caminho, "/geral.txt");

    FILE *arq = fopen(caminho, "r");
    fgets(var, 255, (FILE*)arq);
    fclose(arq);
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

        char pasta[22] = "./Pessoas/";
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
        IupVar.dlg = IupMessageDlg();

        IupSetAttribute(IupVar.dlg, "DIALOGTYPE", "WARNING");
        IupSetAttribute(IupVar.dlg, "BUTTONS", "OK");

        if(tam > 11)
            IupSetAttribute(IupVar.dlg, "VALUE", "CPF contém mais de 11 dígitos.\n\nTente novamente.");
        else
            IupSetAttribute(IupVar.dlg, "VALUE", "CPF contém menos de 11 dígitos ou caracteres não numéricos.\n\nTente novamente.");

        IupPopup(IupVar.dlg, IUP_CURRENT, IUP_CURRENT);
        IupDestroy(IupVar.dlg);
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
    //mkdir("./Pessoas");

    /*pedeCPF(argc, argv);

    // já cadastrado
    if(globalVar==1){
        TelaInicial(argc, argv);
    }
    //não cadastrado
    else if(globalVar==2){
        CadastraUsuario(argc, argv);
    }*/

    deposita(argc, argv);
}
