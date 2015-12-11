#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CUADRO_TAM 80 // Tamaño de cada cuadro en la malla.
#define HEIGHT 700
#define NIMAGENES 2 // Numero de imagenes externas.
#define NSKINS 3
#define N 7
#define M 9
#define PIXEL_TAM 4
#define WIDTH 1040

typedef char String[50];

// Estructura principal del juego.
typedef struct nodo
{
    int tipo;
    struct nodo *arriba;
    struct nodo *abajo;
    struct nodo *izq;
    struct nodo *der;
}*Malla;

int creaMalla(Malla *cab, Malla *dino); // Crea la matriz donde iran los personajes y escenario.
int creaNodo(Malla *nodo, int dato); // Pide memoria para la creación de un nodo.
void dibujaSprite(String nombre, int x, int y); // Abre un archivo de texto y comienza a dibujar a partir de las coordenadas.
void dibujaVidas(int vidas); // Durante el juego dibuja las vidas del jugador.
void iniciaEntorno(); // Se encarga de iniciar la parte gráfica y obtener las imágenes del juego.
void juego(int nivel, int vidas); // Contiene todo el juego.
void obtenerDatos(int *huesos, int *skin); // Lee de un archivo externo los huesos obtenidos del jugador y su skin.
void menu(); // Dibujar el menú principal.
void pintaAmbiente(int pagina); // Dibuja todo el escenario.
void pintaEscenario(Malla cab); // Dibuja los sprites de acuerdo a los valores de la malla.
void portada(); // Dibujar la pantalla principal.
void tienda(); // Dibuja todos los articulos disponibles.

// Variables globales.
void *imagenes[NIMAGENES]; // Arreglo de imagenes en el juego
int skin;

int main()
{
    initwindow(WIDTH, HEIGHT, "Run dino run");
    iniciaEntorno();

    getch();
    return 0;
}

int creaMalla(Malla *cab, Malla *dino)
{
    Malla cx1, cx2, cy, cxAux;
    int res=1;
    int x,y;
    for(y=0; y<M && res; y++, cxAux=cy)
        for(x=0; x<N && res; x++)
        {
            if( x==3 && y==M-1)
            {
                res=creaNodo(&cx1,0);
                *dino=cx1;
            }
            else
                res=creaNodo(&cx1,0);
            if(res)
                if(!y)
                    if(!x)
                        *cab=cx2=cy=cx1;
                    else
                    {
                        cx1->izq = cx2;
                        cx2->der = cx1;;
                        cx2=cx1;
                    }
                else
                {
                    if(!x)
                        cx2=cy=cx1;
                    else
                    {
                        cx1->izq = cx2;
                        cx2->der = cx1;
                        cx2 = cx1;
                    }
                    cx1->arriba=cxAux;
                    cxAux->abajo=cx1;
                    cxAux = cxAux->der;
                }
        }

    return(res);
}

int creaNodo(Malla *nodo, int dato)
{
    int res=0;
    *nodo = (Malla) malloc(sizeof(struct nodo));
    if(nodo)
    {
        res=1;
        (*nodo)->tipo=dato;
        (*nodo)->abajo=(*nodo)->arriba=(*nodo)->izq=(*nodo)->der=NULL;
    }
    return(res);
}

void dibujaSprite(String nombre, int x, int y)
{
    int i, j, n, m, color, xIni, yIni;
    FILE *f;
    xIni = x;

    //strcat(nombre,".txt");
    //sprintf(nombre, "%s.txt", nombre);
    int arr[23][23];

    f = fopen(nombre,"r");
    if(f)
    {
        fscanf(f, "%d\t%d\n", &n, &m);
        for(i=0; i<n; i++, fscanf(f,"\n"), y+=PIXEL_TAM)
            for(j=0, x=xIni; j<m; j++, x+=PIXEL_TAM)
            {
                fscanf(f,"%d ", &color);
                arr[i][j] = color;
                if(color!=1010)
                {
                    setfillstyle(1, color);
                    bar(x, y, x+PIXEL_TAM, y+PIXEL_TAM);
                }
            }
    }

    fclose(f);


    /*for(i=0; i<m; i++, y+=PIXEL_TAM)
        for(j=0, x=xIni; j<n; j++, x+=PIXEL_TAM)
            if(arr[j][i]!=1010)
            {
                setfillstyle(1, arr[j][i]);
                bar(x, y, x+PIXEL_TAM, y+PIXEL_TAM);
            }
    */
}

void dibujaVidas(int vidas)
{
    int i, xi;

    for(i=0, xi = 20; i<vidas; i++, xi+=PIXEL_TAM*20)
        dibujaSprite("corazon.txt", xi, 120);
}

void iniciaEntorno()
{
    int i;

    setfillstyle(1, COLOR(79, 182, 225));
    setbkcolor(COLOR(79, 182, 225));
    setcolor(WHITE);

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
    portada();
}

void juego(int nivel, int vidas)
{
    char tecla;
    Malla cab = NULL, dino, aux;
    int spriteH, // Altura de cualquier sprite
        pagina = 1,
        vel = 0,
        i, j = 0,
        dinoPos = 3, rpos,rposant, vselec;
    String nSkin;

    srand(time(NULL));
    spriteH = 592 - PIXEL_TAM*23;

	pintaAmbiente(pagina);

    // Inicia malla
	creaMalla(&cab, &dino);
    setvisualpage(pagina);
    do
    {
        setactivepage(pagina=!pagina);
        pintaAmbiente(pagina);
        pintaEscenario(cab);
        dibujaVidas(vidas);
        if(j==6)
        {
            j=0;
            vel+=10;
            aux=cab;
            vselec=rand()%2+2;
            do
                rpos=rand()%7;
            while(rpos==rposant);

            rposant=rpos;
            for(i=0;i<rpos;i++)
                aux=(aux->der);

            if(vselec==2)
                aux->tipo=2;
            else
                aux->tipo=3;
        }
        j++;
        setvisualpage(pagina);

        if(kbhit())
        {
            putimage(0, 400, imagenes[1], COPY_PUT);
            switch(tecla = getch())
            {
                case 72:
                    strcpy(nSkin, "");
                    sprintf(nSkin, "dino%d.0.txt", skin);
                    dibujaSprite(nSkin, 10, spriteH);
                    delay(200);
                    break;
            }
        }
    }while(tecla!=27);
    iniciaEntorno();
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

    fflush(stdin);
    switch(op)
    {
        case 0:
            juego(1, 3);
            break;
        case 1:
            tienda();
            break;
        case 2:
            //ayuda("ayuda.txt", 0, HEIGHT/2-100, WIDTH, HEIGHT);
            break;
        case 3:
            //creditos();
            break;
    }
}

void obtenerDatos(int *huesos, int *skin)
{
    FILE *f;

    f = fopen("ajustes.txt", "r");

    if(f)
        fscanf(f, "%X %d\n", huesos, skin);

    fclose(f);
}

void pintaAmbiente(int pagina)
{
    String nSkin;
    // Borra todo y pone el fondo.
    setfillstyle(1, COLOR(79, 182, 225));
    bar(0,0, WIDTH, HEIGHT);
    putimage(0, 400, imagenes[1], COPY_PUT);

    // Dino
    /*sprintf(nSkin, "dino%d.%d.txt", skin, pagina);
    putimage(0, 400, imagenes[1], COPY_PUT);
    dibujaSprite(nSkin, 10, 592);
    delay(120);*/
    delay(120);
}

void pintaEscenario(Malla cab)
{
    Malla auxy;
    int i,j;
    auxy=cab;
    for(j=0;j<M;j++)
    {
        for(i=0;i<N;i++)
        {
            if(cab->tipo==2)
            {
                dibujaSprite("dino3.0.txt",15+i*PIXEL_TAM,50*j);
            }
            else
            {
                if(cab->tipo==3)
                {
                dibujaSprite("dino3.1.txt",15+i*PIXEL_TAM,50*j);

                }
            }
            cab=cab->der;
        }
        if(j!=(M-1))
        {
            auxy=auxy->abajo;
            cab=auxy;
        }
    }
    cab=auxy;
    for(j=0;j<M;j++)
    {
        for(i=0;i<N;i++)
        {
            if(j!=M-1)
            {
               cab->tipo=cab->arriba->tipo;
               cab=cab->der;
            }
            else
            {
                cab->tipo=0;
                cab=cab->der;
            }


        }
        auxy=auxy->arriba;
        cab=auxy;
    }
}

void portada()
{
    int i, huesos;
    putimage(0, 400, imagenes[1], COPY_PUT);
    String aux;

    // Animacion del titulo del juego.
    for(i=0; i<50; i++)
    {
        bar(0, 0, WIDTH,305);
        putimage(WIDTH/10, i, imagenes[0], COPY_PUT);
        delay(2);
    }

    // Escribe numero de huesos
    obtenerDatos(&huesos, &skin);
    dibujaSprite("hueso.txt", WIDTH-100, 0);
    sprintf(aux, "%d", huesos);
    settextstyle(3, HORIZ_DIR, 49);
    outtextxy(WIDTH-180, 25, aux);

    menu();
}

void tienda()
{
    int i, xi, pag = 1;
    char tecla;
    String aux;

    setvisualpage(pag);
    while(tecla!=27)
    {
        setactivepage(pag=!pag);
        setfillstyle(1, COLOR(79, 182, 225));
        bar(0,0, WIDTH, HEIGHT);
        if(kbhit())
            tecla = getch();

        for(i=0, xi=WIDTH/3; i<NSKINS; i++, xi+=PIXEL_TAM*25)
        {
            sprintf(aux, "dino%d.%d.txt", i+1,pag);
            dibujaSprite(aux, xi, 450);
            if(skin==i+1)
            {
                setfillstyle(1, 0x00f);
                bar(xi, 450+PIXEL_TAM*25, xi+PIXEL_TAM*20, 460+PIXEL_TAM*25);
            }
        }
        setvisualpage(pag);
        delay(80);
    }

    iniciaEntorno();
}
