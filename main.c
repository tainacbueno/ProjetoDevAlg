#include <stdlib.h>
#include <iup.h>

int verif( Ihandle *self )
{
	Ihandle* dlg = IupMessageDlg();

	IupSetAttribute(dlg, "DIALOGTYPE", "WARNING");
	IupSetAttribute(dlg, "TITLE", "Confirmação");
	IupSetAttribute(dlg, "BUTTONS", "OKCANCEL");
	IupSetAttribute(dlg, "VALUE", "Tem certeza que deseja sair?");
	
	IupPopup(dlg, IUP_CURRENT, IUP_CURRENT);
	
	char *a = IupGetAttribute(dlg, "BUTTONRESPONSE");
	
	if (*a == '1')
		return IUP_CLOSE;
	
	IupDestroy(dlg);
}

int main(int argc, char *argv[]){
	
	Ihandle *dialog, *quit_bt, *vbox, *label;
	Ihandle *multitext, *file_menu, *item_exit, *item_open, *item_saveas;
  	Ihandle *sub1_menu, *menu;

    IupOpen(&argc, &argv);

    //botão para sair
    quit_bt = IupButton("Sair", "btn_exit");
    IupSetAttribute(quit_bt, "SIZE", "35x20");
    //botão chama função verif
    IupSetCallback(quit_bt, "ACTION", (Icallback)verif);
  
    //texto
    label = IupLabel("Bem-vindo(a)!");

    vbox = IupVbox(
      label,
      quit_bt,
      NULL);
	IupSetAttribute(vbox, "ALIGNMENT", "ACENTER");
    IupSetAttribute(vbox, "MARGIN", "200x200");
    IupSetAttribute(vbox, "GAP", "20");

    //dialog
    dialog = IupDialog(vbox);
    IupSetAttribute(dialog, "Carteira Pessoal", "Carteira Pessoal");
    //IupSetAttributeHandle(dialog, "DEFAULTESC", quit_bt);

    IupShowXY(dialog, IUP_CENTER, IUP_CENTER);

    IupMainLoop();

    IupClose();
    return EXIT_SUCCESS;
}
