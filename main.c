#include <stdlib.h>
#include <stdio.h>
#include <iup.h>

int botao(Ihandle *self){
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

int ChamaIup(int argc, char **argv){
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
    label = IupLabel("Bem-vindo(a) à sua carteira pessoal!");
    IupSetAttribute(label, "FONTSIZE", "20");

    //botão saída
    button_exit = IupButton("SAIR", NULL);
    IupSetCallback(button_exit, "ACTION", (Icallback)botao_exit);
    IupSetAttribute(button_exit, "SIZE", "35x20");

    //vbox
    vbox = IupVbox(
            label,
            button_exit,
            NULL);
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

int main(int argc, char **argv){
    ChamaIup(argc, argv);
}
