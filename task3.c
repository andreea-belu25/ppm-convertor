/*BELU ANDREEA-DANIELA 314CB*/
#include "header.h"

//  initializare coada pixeli
TCoada_pixel InitQ_pixel(void)
{
	TCoada_pixel c = (TCoada_pixel)calloc(1, sizeof(TCelulaCoada_pixel));
	c->inc = c->sf = NULL;
	if (!c)
		return NULL;
	return c;
}
//  inserare pixel intr-o coada de pixeli
void InsQ_pixel(TCoada_pixel c, PPMPixel x)
{
	//  aloc spatiu pentru o celula noua in coada
	TLista_pixel aux1 = (TLista_pixel)calloc(1, sizeof(TCelula_pixel));
	if (!aux1)
		return;
	//  copiez pixelul
	aux1->info = x;
	//  e prima celula pe care urmeaza sa o adaug
	if (c->inc == NULL)
		c->inc = aux1;
	//  altfel, o leg la ultima celula adaugata in coada
	else
		c->sf->urm = aux1;
	//  modific sfarsitul cozii
	c->sf = aux1;
}
PPMPixel ExtQ_pixel(TCoada_pixel c)
{
	//  salvez prima celula din coada
	TLista_pixel aux = c->inc;
	PPMPixel p = aux->info;
	//  modificare varf coada
	c->inc = aux->urm;
	//  o singura valoare in coada = > coada vida
	if (c->inc == NULL)
		c->sf = NULL;
	//  eliberez spatiul alocat pt prima celula din coada
	free(aux);
	// returnez info din prima celula din coada (de tip pixel)
	return p;
}
void construieste_coada_pixeli(TCoada_pixel c, FILE *read)
{
	unsigned char type;
	while (fread(&type, sizeof(unsigned char), 1, read)) {
		PPMPixel pixel;
		pixel.type = type;
		if (pixel.type == 0) {
		//  daca tipul pixelului citit e 0 => e rad => nu are pixeli de culoare
			pixel.red = 0;
			pixel.green = 0;
			pixel.blue = 0;
		} else {
		//  tip pixel = 1 => e frunza => are pixeli de culoare
		//  pe care ii citesc din fisier
			fread(&pixel.red, sizeof(unsigned char), 1, read);
			fread(&pixel.green, sizeof(unsigned char), 1, read);
			fread(&pixel.blue, sizeof(unsigned char), 1, read);
		}
		//  dupa ce am pus inf unui pixel => il inserez in coada de pixeli
		InsQ_pixel(c, pixel);
	}
}
//  pt a construi arborele ma folosesc de o coada in care pun pixelii cititi
TArb construieste_arbore(TCoada_pixel c, TArb r)
{
	//  intra pe primul if prima data si creeaza rad intregului arbore
	if (r == NULL) {
		PPMPixel y = ExtQ_pixel(c);
		//  aloc o celula de tip arbore in care pun inf din pixelul extras
		TArb aux = (TArb)calloc(1, sizeof(TNod));
		if (!aux)
			return NULL;
		aux->info.type = y.type;
		aux->info.red = y.red;
		aux->info.green = y.green;
		aux->info.blue = y.blue;
		//  pentru creare legaturi
		return aux;
	}
	//  sunt la o rad => tipul e 0 =>
	//  are 4 fii pe care ii creez si ii leg la radacina
	if (r->info.type == 0) {
		r->fiu1 = construieste_arbore(c, r->fiu1);
		r->fiu2 = construieste_arbore(c, r->fiu2);
		r->fiu3 = construieste_arbore(c, r->fiu3);
		r->fiu4 = construieste_arbore(c, r->fiu4);
	}
	//  returnez arborele creat
	return r;
}
void Parcurge_Arb(TArb a, PPMPixel **grid, int x, int y, int lat)
{
	//  construire imagine => creare arbore si apoi matrice
	//  daca e frunza => un patrat din matrice va avea aceleasi info
	if (a->info.type == 1) {
		for (int i = x; i < x + lat; i++) {
			for (int j = y; j < y + lat; j++) {
				grid[i][j].type = a->info.type;
				grid[i][j].red = a->info.red;
				grid[i][j].green = a->info.green;
				grid[i][j].blue = a->info.blue;
			}
		}
		//  iesire din recursivitate
		return;
	}
	//  daca sunt la o rad => inaintez in arbore pana dau de frunze
	Parcurge_Arb(a->fiu1, grid, x, y, lat/2);
	Parcurge_Arb(a->fiu2, grid, x, y + lat/2, lat/2);
	Parcurge_Arb(a->fiu3, grid, x + lat/2, y + lat/2, lat/2);
	Parcurge_Arb(a->fiu4, grid, x + lat/2, y, lat/2);
}
void afisare_imagine(PPMPixel **grid, unsigned int dim, const char *filename)
{
	// se scrie in fisier binar imaginea
	FILE *write = fopen(filename, "wb");
	if (!write)
		return;
	fprintf(write, "P6\n");
	fprintf(write, "%u %u\n", dim, dim);
	fprintf(write, "255\n");
	//  afisare matrice creata la pasul anterior
	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim; j++) {
			fwrite(&grid[i][j].red, sizeof(unsigned char), 1, write);
			fwrite(&grid[i][j].green, sizeof(unsigned char), 1, write);
			fwrite(&grid[i][j].blue, sizeof(unsigned char), 1, write);
		}
	}
	// inchid fisierul binar in care am scris info
	fclose(write);
}
//  fct pentru eliberarea spatiului ocupat de un arb cuaternar
void distruge(TArb r)
{
	if (!r)
		return;
	distruge(r->fiu1);
	distruge(r->fiu2);
	distruge(r->fiu3);
	distruge(r->fiu4);
	free (r);
}
//  fct pentru distrugerea cozii de pixeli
void DistrugereQ(TCoada_pixel c)
{
	TLista_pixel p = c->inc, aux = c->inc;
	//  parcurg coada
	while (p) {
		aux = p;
		p = p->urm;
		free(aux);
	}
	free(c);
	//  ma asigur ca a devenit NULL pointerul catre coada
	c = NULL;
}