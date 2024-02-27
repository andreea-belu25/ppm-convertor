/*BELU ANDREEA-DANIELA 314 CB*/
#include "header.h"

//  functia check ma ajuta sa stiu daca un nod e frunza sau nu conform cerintei
int check(unsigned int lin, unsigned int col,
PPMPixel **grid, unsigned int width, int factor)
{
	unsigned long long red = 0, green = 0, blue = 0;
	unsigned long long mean = 0;
	for (int i = lin; i < lin + width; i++)
		for (int j = col; j < col + width; j++) {
			red = red + grid[i][j].red;
			green = green + grid[i][j].green;
			blue = blue + grid[i][j].blue;
		}
	red /= width * width;
	green /= width * width;
	blue /= width * width;
	for (int i = lin; i < lin + width; i++)
		for (int j = col; j < col + width; j++) {
			mean += (red - grid[i][j].red) * (red - grid[i][j].red);
			mean += (green - grid[i][j].green) * (green - grid[i][j].green);
			mean += (blue - grid[i][j].blue) * (blue - grid[i][j].blue);
		}
	mean = mean / (3 * width * width);
	if (mean <= factor)
	//  nodul e frunza
		return 1;
	//  nodul nu e frunza
	return 0;
}
//  functie de construire arbore de compresie
TArb arb_compresie(unsigned int lin, unsigned int col,
PPMPixel **grid, unsigned int width, int factor)
{

	unsigned long long red = 0, green = 0, blue = 0;
	if (check(lin, col, grid, width, factor) == 1) {
	//  nu mai tb divizat => e frunza => nod nou in care pun info necesare
	//  apoi, il returnez pentru legaturi
		TArb aux1 = (TArb)calloc(1, sizeof(TNod));
		if (!aux1)  //  verificare alocare
			return NULL;
		for (int i = lin; i < lin + width; i++)
			for (int j = col; j < col + width; j++) {
				red = red + grid[i][j].red;
				green = green + grid[i][j].green;
				blue = blue + grid[i][j].blue;
			}
		aux1->dim = width;
		aux1->info.red = red / (width * width);
		aux1->info.green = green / (width * width);
		aux1->info.blue = blue / (width * width);
		aux1->type = 1;
		return aux1;
	} else {
		//  nu e frunza => aloc nodul si apoi il divizez in patru
		TArb aux = (TArb)calloc(1, sizeof(TNod));
		//  verificare alocare
		if (!aux)
			return NULL;
		aux->fiu1 = arb_compresie(lin, col, grid, width/2, factor);
		aux->fiu2 = arb_compresie(lin, col + width/2, grid, width/2, factor);
		aux->fiu3 = arb_compresie(lin + width/2, col + width/2,
		grid, width/2, factor);
		aux->fiu4 = arb_compresie(lin + width/2, col, grid, width/2, factor);
		aux->type = '0';
		return aux;
	}
}
//  fct Min ma ajuta la fct niv_min_frunza
int Min(int a, int b, int c, int d)
{
	int minim = a;
	if (b < minim)
		minim = b;
	if (c < minim)
		minim = c;
	if (d < minim)
		minim = d;
	return minim;
}
//  fct Max ma ajuta la fct info_nod_max
int Max(int a, int b, int c, int d)
{
	int maxim = a;
	if (b > maxim)
		maxim = b;
	if (c > maxim)
		maxim = c;
	if (d > maxim)
		maxim = d;
	return maxim;
}
//  niv_min_frunza + info_nod_max pentru dim maxima latura nedivizata
//  dim maxima latura nedivizata = cea mai apropiata frunza de radacina
//  determin nivelul minim pe care se afla o frunza
int niv_min_frunza(TArb a, int crt_level)
{
	if (a == NULL)
	//  MAX_POSIBIL (am ajuns sub arbore)
		return 10000;
	//  e frunza => returnez nivelul curent
	if (a->fiu1 == NULL && a->fiu2 == NULL) {
		if (a->fiu3 == NULL && a->fiu4 == NULL) {
			return crt_level;
		}
	}
	//  daca nu e frunza => continui sa inaintez in subarbori
	int min1 = niv_min_frunza(a->fiu1, crt_level + 1);
	int min2 = niv_min_frunza(a->fiu2, crt_level + 1);
	int min3 = niv_min_frunza(a->fiu3, crt_level + 1);
	int min4 = niv_min_frunza(a->fiu4, crt_level + 1);
	//  determin minimul dintre cele 4 valori pentru nodul curent
	return Min(min1, min2, min3, min4);
}
//  determin informatia maxima din frunza de pe niv minim
int info_nod_max(TArb a, int nivel, int niv_crt)
{
	if (a == NULL)
		return -1;
	//  nivelul minim al frunzei = nivelul curent
	if (nivel == niv_crt)
	//  returnez dimensiunea frunzei
		return a->dim;
	//  nu sunt pe un  nivel egal cu cel minim => inaintez in cei 4 subarbori
	int info_nod1 = info_nod_max(a->fiu1, nivel, niv_crt + 1);
	int info_nod2 = info_nod_max(a->fiu2, nivel, niv_crt + 1);
	int info_nod3 = info_nod_max(a->fiu3, nivel, niv_crt + 1);
	int info_nod4 = info_nod_max(a->fiu4, nivel, niv_crt + 1);
	//  returnez maximul dintre cele 4 valori obtinute in recursivitate
	return Max(info_nod1, info_nod2, info_nod3, info_nod4);

}
//  functie suplimentara pentru debugging
void afisare(TArb a, int nivel)
{
	if (a == NULL)
		return;
	printf("Frunza: %d %d %d %d\n", a->info.red,
	a->info.green, a->info.blue, nivel);
	afisare(a->fiu1, nivel + 1);
	afisare(a->fiu2, nivel + 1);
	afisare(a->fiu3, nivel + 1);
	afisare(a->fiu4, nivel + 1);
}
//  functie care imi returneaza nr de frunze dintr-un arbore
int Numara_Frunze(TArb a)
{
	int k = 0, nr_fiu1 = 0, nr_fiu2 = 0, nr_fiu3 = 0, nr_fiu4 = 0;
	//  am ajuns sub arbore
	if (a == NULL)
		return 0;
	//  am ajuns la o frunza => k = 1
	if (a->fiu1 == NULL && a->fiu2 == NULL)
		if (a->fiu3 == NULL && a->fiu4 == NULL)
			k = 1;
	//  nu sunt la o frunza => inaintez in subarbori
	nr_fiu1 = Numara_Frunze(a->fiu1);
	nr_fiu2 = Numara_Frunze(a->fiu2);
	nr_fiu3 = Numara_Frunze(a->fiu3);
	nr_fiu4 = Numara_Frunze(a->fiu4);
	return k + nr_fiu1 + nr_fiu2 + nr_fiu3 + nr_fiu4;
}
//  functie ce imi returneaza numarul de niveluri din arbore
int NrNiveluri(TArb a)
{
	//  am ajuns sub arbore
	if (a == NULL)
		return 0;
	//  inaintez in cei 4 subarbori
	int nfiu1 = NrNiveluri(a->fiu1);
	int nfiu2 = NrNiveluri(a->fiu2);
	int nfiu3 = NrNiveluri(a->fiu3);
	int nfiu4 = NrNiveluri(a->fiu4);
	//  pentru un nod radacina, determin maximul dintre nr de niveluri
	//  din recursivitate
	int Max = nfiu1;
	if (nfiu2 > Max)
		Max = nfiu2;
	if (nfiu3 > Max)
		Max = nfiu3;
	if (nfiu4 > Max)
		Max = nfiu4;
	return 1 + Max;
}