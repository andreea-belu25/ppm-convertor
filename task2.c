/*BELU ANDREEA-DANIELA 314CB*/
#include "header.h"

TCoada InitQ(void)
{
	TCoada c = (TCoada)calloc(1, sizeof(TCelulaCoada));
	 //  verific daca a reusit alocarea
	if (!c)
		return NULL;
	// pointer catre coada daca a reusit alocarea
	return c;
}
void InsQ(TCoada c, TArb x)
{
	//  vreau sa inserez intr-o coada => aloc o celula noua pentru coada
	TLista aux1 = (TLista)calloc(1, sizeof(TCelula));
	//  verific alocarea
	if (!aux1)
		return;
	//  info e de tip TArb => trebuie alocata
	aux1->info = (TArb)calloc(1, sizeof(TNod));
	if (!aux1->info) {
		free(aux1);
		return;
	}
	//  daca au reusit alocarile => copiez inf in celula pe care vreau
	//  sa o adaug in coada
	aux1->info->dim = x->dim;
	aux1->info->fiu1 = x->fiu1;
	aux1->info->fiu2 = x->fiu2;
	aux1->info->fiu3 = x->fiu3;
	aux1->info->fiu4 = x->fiu4;
	aux1->info->type = x->type;
	aux1->info->info.red = x->info.red;
	aux1->info->info.green = x->info.green;
	aux1->info->info.blue = x->info.blue;
	//  e prima celula pe care urmeaza sa o adaug
	if (c->inc == NULL)
		c->inc = aux1;
	//  nu e prima celula inserata => o leg la ultima celula din coada
	else
		c->sf->urm = aux1;
	//  actualizez pointerul sf
	c->sf = aux1;
}
TLista ExtQ(TCoada c)
{
	//  nu am valori de extras
	if (c->inc == NULL)
		return NULL;
	//  extrag prima celula din coada
	TLista aux = c->inc;
	//  modificare varf stiva
	c->inc = c->inc->urm;
	//  o singura valoare in coada = > coada vida
	if (c->inc == NULL) {
		c->sf = NULL;
	}
	return aux;
}
//  cand nu mai am nevoie de coada => o distrug
void DistrQ(TCoada c)
{
	TLista p = c->inc, aux = c->inc;
	//  parcurg coada
	while (p) {
		aux = p;
		p = p->urm;
		free(aux->info);
		free(aux);
	}
	free(c);
	//  ma asigur ca a devenit NULL pointerul catre coada
	c = NULL;
}