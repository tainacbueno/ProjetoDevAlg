#include<stdlib.h>
#include<stdio.h>
#include<iup.h>

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

int main(int argc, char **argv){
	
	double din = 389.87;

	FILE* arq = fopen("teste.txt", "a");
	
	//fprintf(arq, din);
	
	Ihandle *dlg, *vbox, *button_exit, *button_deposita, *button_saque, *label;
	Ihandle *transacoes_menu, *movimenta_menu; 
	Ihandle *item_coloca, *item_retira;
	Ihandle *item_moradia, *item_estudo, *item_tranporte, *item_alimentcao, *item_trabalho;
	Ihandle *item_12mes, *item_categoriames;	
	Ihandle *sub_menu_transacoes, *sub_menu_movimenta, *menu;
	
	IupOpen(&argc, &argv);
	
	item_coloca = IupItem("Depositar", NULL);
	item_retira = IupItem("Sacar", NULL);
	
	item_moradia = IupItem("Moradia", NULL);
	item_estudo = IupItem("Estudo", NULL);
	item_tranporte = IupItem("Transporte", NULL);
	item_alimentcao = IupItem("Alimentação", NULL);
	item_trabalho = IupItem("Trabalho", NULL);
	
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
		NULL
	);
	
	sub_menu_transacoes = IupSubmenu("Transações", transacoes_menu);
	sub_menu_movimenta = IupSubmenu("Movimentação", movimenta_menu);
	menu = IupMenu(
		sub_menu_transacoes,
		sub_menu_movimenta,
		NULL
	);
	
	
	label = IupLabel("PW\n\n\nBem-vindo a sua carteira pessoal!");
	
	button_exit = IupButton("SAIR", NULL);
	IupSetCallback(item_coloca, "ACTION", (Icallback)deposita);
	IupSetCallback(item_retira, "ACTION", (Icallback)saque);
	vbox = IupVbox(label, button_exit,  NULL);
	
	dlg = IupDialog(vbox);
	
	IupSetAttribute(dlg, "TITLE", "Carteirinha");
	IupSetAttributeHandle(dlg, "MENU", menu);
	IupSetAttribute(label, "ALIGNMENT", "ACENTER");	
	
	IupSetAttribute(vbox, "GAP", "80");
	IupSetAttribute(vbox, "MARGIN", "200x150");
	
	IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");	
	IupSetAttribute(button_exit, "SIZE", "35x20");
	IupSetCallback(button_exit, "ACTION", (Icallback)botao);
	
	IupShowXY(dlg, IUP_CENTER, IUP_CENTER);
	
	IupMainLoop();
	
	IupClose();
	return EXIT_SUCCESS;																				
}


