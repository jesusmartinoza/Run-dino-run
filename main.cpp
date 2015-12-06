#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CUADRO_TAM 80 // Tamaño de cada cuadro en la malla.
#define HEIGHT 700
#define NIMAGENES 2 // Numero de imagenes externas.
#define WIDTH 1040

typedef char String[50];

// Estructura principal del juego.
typedef struct nodo
{
    int x1,y1;
    int x2,y2;
    int existe;
    struct nodo *arriba;
    struct nodo *abajo;
    struct nodo *izq;
    struct nodo *der;
}Malla;

int creaMalla(Malla *cab, int n, int m); // Crea la matriz donde iran los personajes y escenario.
int creaNodo(Malla **cab, int x, int y); // Pide memoria para la creación de un nodo.
int enlaza(Malla *m);
int insertaHorizontal(Malla *cab, int x, int y);
int insertaVertical(Malla *cab, int x, int y);
void iniciaEntorno(); // Se encarga de iniciar la parte gráfica y obtener las imágenes del juego.
void juego(); // Inicia juego
void menu(); // Dibujar el menú principal
void portada(); // Dibujar la pantalla principal

// Variables globales.
void *imagenes[NIMAGENES]; // Arreglo de imagenes en el juego

int main()
{
    iniciaEntorno();
    portada();

    getch();
    return 0;
}

int creaMalla(Malla *cab, int n, int m)
{
    int i, j, x=0, y=285, y2=y+CUADRO_TAM, res=0;
    Malla *aux;
    for(i=0; i<n; i++)
    {
        insertaHorizontal(cab, x+CUADRO_TAM*i, y);
        for(aux=cab; aux; aux=aux->der);
        for(j=0; j<m; j++)
            insertaVertical(aux, x+CUADRO_TAM*i, y2+CUADRO_TAM*j);
    }
    res = enlaza(cab);
    return res;
}

int creaNodo(Malla **cab, int x, int y)
{
    int res = 0;
    *cab = (Malla*)malloc(sizeof(Malla));

    if(*cab)
    {
        (*cab)->x1=x;
        (*cab)->y1=y;
        (*cab)->x2=x+CUADRO_TAM;
        (*cab)->y2=y+CUADRO_TAM;
        (*cab)->arriba=(*cab)->abajo=(*cab)->izq=(*cab)->der=NULL;
        res=1;
    }
    return(res);
}

int enlaza(Malla *l)
{
    Malla *aux,*aux2,*aux3,*aux4;
    int res=0;

    if(l)
        for(aux=l; aux; aux=aux->der)
            for(aux2=aux; aux2; aux2=aux2->abajo)
                if(aux2->abajo && aux->der)
                {
                    aux3 = aux->der->abajo;
                    aux4 = aux->abajo;
                    while(aux3 && aux4)
                    {
                        aux3->izq = aux4;
                        aux4->der = aux3;
                        aux3 = aux3->abajo;
                        aux4 = aux4->abajo;
                    }
                }
    return(res);
}

void iniciaEntorno()
{
    initwindow(WIDTH, HEIGHT, "Run dino run");
    int i;
    setfillstyle(1, COLOR(79, 182, 225));

    String nombres[NIMAGENES] = {
        "img/logotipo.gif",
        "img/jungla.jpg"
    };

    // logo
    bar(0,0,WIDTH,HEIGHT);
    readimagefile(nombres[0],0,0,798,303);
    imagenes[0]  = malloc(imagesize(0,0,798,303));
    getimage(0,0,798,303, imagenes[0]);

    // jungla
    bar(0,0,WIDTH,HEIGHT);
    readimagefile(nombres[1],0,0,WIDTH,300);
    imagenes[1]  = malloc(imagesize(0,0,WIDTH,300));
    getimage(0,0,WIDTH,300, imagenes[1]);
}

int insertaHorizontal(Malla *cab, int x, int y)
{
    int res=0, band=1;
    Malla *nuevo,*aux;

    if(creaNodo(&nuevo, x, y))
        if(!cab)
         cab=nuevo;
        else
        {
            for(aux=cab; aux!=NULL && band; aux=aux->der)
                if(aux->der==NULL)
                {
                    aux->der=nuevo;
                    nuevo->izq=aux;
                }
            setcolor(11);
            rectangle(nuevo->x1,nuevo->y1,nuevo->x2,nuevo->y2);
        }

    return res;
}

int insertaVertical(Malla *l, int x, int y)
{
    int res=1;
    Malla *nuevo, *aux;

    if(creaNodo(&nuevo,x,y))
    {
        if(!l)
        {
            l=nuevo;
            setcolor(5);
            setfillstyle(1, rand()%15);
            if(rand()%2)
                bar((l)->x1, (l)->y1, (l)->x2, (l)->y2);
        }
        else
        {
            aux=l;
            while(aux->abajo)
                aux=aux->abajo;
            aux->abajo=nuevo;
            nuevo->arriba=aux;
            setcolor(11);
            rectangle(nuevo->x1,nuevo->y1,nuevo->x2,nuevo->y2);
        }
        res=1;
    }
    return(res);
}

void juego()
{
    // Borra todo y pone el fondo.
    setfillstyle(1, COLOR(79, 182, 225));
    bar(0,0, WIDTH, HEIGHT);
    putimage(0, 400, imagenes[1], COPY_PUT);

    // Inicia malla
    Malla *cab = NULL;
	creaMalla(cab, 13, 4);
    /*for(i=0;i<226; i++)
    {
        //setfillstyle(1, COLOR(0,i,i));
        setcolor(COLOR(79,182,i));
        line(0,i,WIDTH,i);
        delay(1);
    }*/
}

void menu()
{
    int op=-1, xm, ym, i, margin = 70;
    String opciones[] = {"Jugar", "Tienda", "Ayuda", "Créditos"};
    setbkcolor(0x0066F4);
    setfillstyle(1, 0x0066F4);
    setcolor(0x003988);
    settextstyle(2, HORIZ_DIR, 8);

    for(i=0; i<4; i++, margin+=50)
    {
        bar(WIDTH/2-textwidth(opciones[3])/2-5, HEIGHT/2+margin-10, WIDTH/2+textwidth(opciones[3])/2+5, HEIGHT/2+margin+textheight(opciones[3])+10);
        outtextxy(WIDTH/2-textwidth(opciones[i])/2, HEIGHT/2+margin, opciones[i]);
    }

    do
    {
        while(!ismouseclick(WM_LBUTTONDOWN));
        getmouseclick(WM_LBUTTONDOWN, xm, ym);
        margin = 70;
        for(i=0; i<4; i++, margin+=50)
            if(xm>WIDTH/2-textwidth(opciones[3])/2-5
               && xm<WIDTH/2+textwidth(opciones[0])/2-10
               && ym>HEIGHT/2+margin-10
               && ym<HEIGHT/2+margin+textheight(opciones[3])+10)
                {
                    op = i;
                    i=5;
                    break;
                }
    }while(op==-1);
    switch(op)
    {
        case 0:
            juego();
            break;
        case 1:
            //imprimeRegistro();
            break;
        case 2:
            //ayuda("ayuda.txt", 0, HEIGHT/2-100, WIDTH, HEIGHT);
            break;
        case 3:
            //creditos();
            break;
    }
}

void portada()
{
    int i;
    putimage(0, 400, imagenes[1], COPY_PUT);

    for(i=0; i<50; i++)
    {
        bar(0, 0, WIDTH,305);
        putimage(WIDTH/10, i, imagenes[0], COPY_PUT);
        delay(2);
    }
    menu();
}
