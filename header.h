#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//  first two structures are for image declaration
typedef struct {
    unsigned char type;
    unsigned char red, green, blue;
} PPMPixel;

typedef struct {
    unsigned int width, height;
    PPMPixel *data;
} PPMImage;

//  define tree
typedef struct nod {
    PPMPixel info;
    unsigned int dim;
    unsigned char type;
    struct nod *fiu1, *fiu2, *fiu3, *fiu4;

} TNod, *TArb;

//  define simple list for the queue used at task 2
typedef struct celula {
    struct celula *urm;
    TArb info;
} TCelula, *TLista;

//  define queue for task 2
typedef struct coada {
    TLista inc, sf;
} TCelulaCoada, *TCoada;

//  define simple list for the queue used at task 3
typedef struct celula_pixel {
    struct celula_pixel *urm;
    PPMPixel info;
} TCelula_pixel, *TLista_pixel;

//  define queue for task 3
typedef struct coada_pixel {
    TLista_pixel inc, sf;
} TCelulaCoada_pixel, *TCoada_pixel;


int check(unsigned int lin, unsigned int col,
PPMPixel **grid, unsigned int width, int factor);
TArb arb_compresie(unsigned int lin, unsigned int col,
PPMPixel **grid, unsigned int width, int factor);
int Min(int a, int b, int c, int d);
int Max(int a, int b, int c, int d);
int niv_min_frunza(TArb a, int crt_level);
int info_nod_max(TArb a, int nivel, int niv_crt);
int Numara_Frunze(TArb a);
int NrNiveluri(TArb a);
TCoada InitQ(void);
void InsQ(TCoada c, TArb x);
TLista ExtQ(TCoada c);
void DistrQ(TCoada c);
TCoada_pixel InitQ_pixel(void);
void InsQ_pixel(TCoada_pixel c, PPMPixel x);
PPMPixel ExtQ_pixel(TCoada_pixel c);
void construieste_coada_pixeli(TCoada_pixel c, FILE *read);
TArb construieste_arbore(TCoada_pixel c, TArb r);
void Parcurge_Arb(TArb a, PPMPixel **grid, int x, int y, int lat);
void afisare_imagine(PPMPixel **grid, unsigned int dim, const char *filename);
void distruge(TArb r);
void DistrugereQ(TCoada_pixel c);
void afisare(TArb a, int nivel);
