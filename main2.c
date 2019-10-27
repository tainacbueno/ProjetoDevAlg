#include <stdlib.h>
#include <stdio.h>
#include <iup.h>
#include <sys/stat.h>

int globalVar = 0;
char pessoaAtual[50];

int botao_exit(Ihandle *self){
    Ihandle* dlg = IupMessageDlg();

    IupSetAttribute(dlg, "DIALOGTYPE", "WARNING");
    IupSetAttribute(dlg, "TITLE", "IupMessageDlg Test");
    IupSetAttribute(dlg, "BUTTONS", "OKCANCEL");
    IupSetAttribute(dlg, "VALUE", "Tem certeza que desaja sair?");

    IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);
    char *op = IupGetAttribute(dlg, "BUTTONRESPONSE");

    if(*op == '1'){
        return IUP_CLOSE;
    }

    IupDestroy(dlg);
}

double deposita(){
    printf("Depositou\n");
    return 3;
}
double saque(){
    printf("Saque\n");
    return 3;
}

void pegaNome(char nome[100]){
    int i;

    printf("%s\n",pessoaAtual);
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
    printf("%s", nome);
};

int TelaInicial(int argc, char **argv){
    Ihandle *dlg, *vbox, *button_exit, *button_deposita, *button_saque, *label;
    Ihandle *transacoes_menu, *movimenta_menu;
    Ihandle *item_coloca, *item_retira;
    Ihandle *item_moradia, *item_estudo, *item_tranporte, *item_alimentcao, *item_trabalho, *item_geral;
    Ihandle *item_12mes, *item_categoriames;
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
    char nome[100] = "Bem-vindo(a) ";
    pegaNome(nome);
    strcat(nome, " a sua carteira pessoal!");

    label = IupLabel(nome);

    IupSetAttribute(label, "FONTSIZE", "20");

    //botão saída
    button_exit = IupButton("SAIR", NULL);
    IupSetCallback(button_exit, "ACTION", (Icallback)botao_exit);
    IupSetAttribute(button_exit, "SIZE", "35x20");

    //vbox
    vbox = IupVbox(
        label,
        button_exit,
        NULL
    );
    IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(vbox, "MARGIN", "200x200");
    IupSetAttribute(vbox, "GAP", "40");

    dlg = IupDialog(vbox);

    IupSetAttribute(dlg, "TITLE", "Carteira Pessoal");
    IupSetAttributeHandle(dlg, "MENU", menu);

    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    IupClose();
    return EXIT_SUCCESS;
}

int analisaCadastro(Ihandle *entrada){
    Ihandle *nome = IupGetDialogChild(entrada, "NOME");
    Ihandle *valor = IupGetDialogChild(entrada, "VALOR");
    double v = IupGetDouble(valor, "VALUE");
    char *n = IupGetAttribute(nome, "VALUE");
    printf("%.2lf\n", v);
    printf("%s\n", n);
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
    Ihandle *dlg, *label, *btcadastra,  *vbox;
    Ihandle *labelNome, *labelValor;
    Ihandle *nome, *valor;

    IupOpen(&argc, &argv);

    label = IupLabel("Olá novo usuario(a), cadastre-se para desfrutar de nossos servicos : \n\n\n");
    IupSetAttribute(label, "FONT", "DEFAULTFONT , 14");
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
    vbox = IupVbox(
        label,
        labelNome,
        nome,
        labelValor,
        valor,
        btcadastra,
        NULL
    );

    IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(vbox, "GAP", "30");
    IupSetAttribute(vbox, "MARGIN", "150x150");

    dlg = IupDialog(vbox);
    IupSetAttribute(dlg, "TITLE", "Carteira Pessoal");

    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    IupClose();
    EXIT_SUCCESS;

    TelaInicial(argc, argv);
}

int analisaCPF(Ihandle *inserecpf){
    Ihandle *a = IupGetDialogChild(inserecpf, "CPF");
    Ihandle *b = IupGetDialogChild(inserecpf, "ERROU");
    double value = IupGetDouble(a, "VALUE");

    printf("%.0lf\n", value);

    char cpf[30];
    sprintf(cpf, "%.0lf", value);
    unsigned long int tam_cpf = strlen(cpf);

    if(tam_cpf==11){
        char pasta[50] = "./Pessoas/";
        strcat(pasta,cpf);
        strcpy(pessoaAtual, pasta);
        //printf("%s\n", pessoaAtual);
        IupSetAttribute(b, "VISIBLE", "NO");

        //printf("%s", pasta);
        //printf("%lu", tam_cpf);

        if(mkdir(pasta)){
            globalVar = 1;
        }
        else{
            globalVar = 2;
        }
        return IUP_CLOSE;
    }

    else{
        printf("Digite novamente\n");
        IupSetAttribute(b, "VISIBLE", "YES");
    }
}
void pedeCPF(int argc, char **argv){
    Ihandle *dlg, *btconfirma, *label, *vbox, *inserecpf, *denovo;

    IupOpen(&argc, &argv);

    label = IupLabel("Digite o seu CPF (apenas numeros):\n");

    denovo = IupLabel("CPF nao contem 11 digitos!");
    IupSetAttribute(denovo, "NAME", "ERROU");
    IupSetAttribute(denovo, "VISIBLE", "NO");

    btconfirma = IupButton("Enviar", NULL);
    IupSetCallback(btconfirma, "ACTION", (Icallback) analisaCPF);

    inserecpf = IupText(NULL);
    IupSetAttribute(inserecpf, "EXPAND", "YES");
    IupSetAttribute(inserecpf, "VALUE", "");
    IupSetAttribute(inserecpf, "NAME", "CPF");

    vbox= IupVbox(label, denovo, inserecpf, btconfirma, NULL);
    IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(vbox, "GAP", "10");
    IupSetAttribute(vbox, "MARGIN", "100x100");
    IupSetAttribute(vbox, "GAP", "20");
    dlg = IupDialog(vbox);
    IupSetAttribute(dlg, "TITLE", "Carteira Pessoal");

    IupShowXY(dlg, IUP_CENTER, IUP_CENTER);

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
}
