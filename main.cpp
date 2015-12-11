#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CUADRO_TAM 40 // Tamaño de cada cuadro en la malla.
#define HEIGHT 700
#define NIMAGENES 8 // Numero de imagenes externas.
#define NOBJ 50
#define NSKINS 3
#define N 26
#define M 3
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
void creditos(); // Imprime creditos.
void dibujaHuesos(int huesos); // Recib el numero de huesos y los muestra en pantalla.
void dibujaSprite(String nombre, int x, int y); // Abre un archivo de texto y comienza a dibujar a partir de las coordenadas.
void dibujaVidas(int vidas); // Durante el juego dibuja las vidas del jugador.
void escribeAjustes(int huesos);
void iniciaEntorno(); // Se encarga de iniciar la parte gráfica y obtener las imágenes del juego.
void juego(int vidas); // Contiene todo el juego.
void obtenerDatos(int *huesos, int *skin); // Lee de un archivo externo los huesos obtenidos del jugador y su skin.
void menu(); // Dibujar el menú principal.
void pintaAmbiente(int pagina, int jungla); // Dibuja todo el escenario.
void pintaEscenario(Malla cab); // Dibuja los sprites de acuerdo a los valores de la malla.
void popup(); // Muestra en pantalla un mensaje.
void portada(); // Dibujar la pantalla principal.
void tienda(); // Dibuja todos los articulos disponibles.

// Variables globales.
void *imagenes[NIMAGENES]; // Arreglo de imagenes en el juego
int skin;

int main()
{
    initwindow(WIDTH, HEIGHT, "Run dino run");
    iniciaEntorno();
    return 0;
}

int creaMalla(Malla *cab, Malla *dino)
{
    Malla cx1, cx2, cy, cxAux;
    int res=1, x, y;

    for(y=0; y<M && res; y++, cxAux=cy)
        for(x=0; x<N && res; x++)
        {
            if( x==3 && y==M-1)
            {
                res=creaNodo(&cx1,0);
                *dino=cx1;
            } else
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

void creditos()
{
    putimage(0,0, imagenes[6], COPY_PUT);
    getch();
    iniciaEntorno();
}

void escribeAjustes(int huesos)
{
    FILE *f;

    f = fopen("ajustes.txt", "w");
    if(f)
        fprintf(f, "%X %d", huesos, skin);

    fclose(f);
}

void dibujaHuesos(int huesos)
{
    String aux;

    setfillstyle(1, COLOR(79, 182, 225));
    setbkcolor(COLOR(79, 182, 225));
    setcolor(WHITE);

    dibujaSprite("hueso.txt", WIDTH-80, -20);
    sprintf(aux, "%d", huesos);
    settextstyle(3, HORIZ_DIR, 49);
    outtextxy(WIDTH-180, 25, aux);
}

void dibujaSprite(String nombre, int x, int y)
{
    int i, j, n, m, color, xIni, yIni, arr[23][24];
    FILE *f;
    xIni = x;

    //strcat(nombre,".txt");
    //sprintf(nombre, "%s.txt", nombre);

    f = fopen(nombre,"r");
    if(f)
    {
        fscanf(f, "%d\t%d\n", &n, &m);
        for(i=0; i<n; i++, fscanf(f,"\n"))//, y+=PIXEL_TAM)
            for(j=0, x=xIni; j<m; j++)//, x+=PIXEL_TAM)
            {
                fscanf(f,"%d ", &color);
                arr[i][j] = color;
                /*if(color!=1010)
                {
                    setfillstyle(1, color);
                    bar(x, y, x+PIXEL_TAM, y+PIXEL_TAM);
                }*/
            }
    }
    fclose(f);

    for(i=0; i<m; i++, y+=PIXEL_TAM)
        for(j=0, x=xIni; j<n; j++, x+=PIXEL_TAM)
            if(arr[j][i]!=1010)
            {
                setfillstyle(1, arr[j][i]);
                bar(x, y, x+PIXEL_TAM, y+PIXEL_TAM);
            }

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
        "img/jungla.jpg",
        "img/jungla1.jpg",
        "img/jungla2.jpg",
        "img/jungla3.jpg",
        "img/jungla4.jpg",
        "img/jungla5.jpg",
        "img/creditos.jpg",
        "img/logotipo.gif"
    };

    // jungla
    for(i=0; i<6; i++)
    {
        bar(0,0,WIDTH,HEIGHT);
        readimagefile(nombres[i],0,0,WIDTH,300);
        imagenes[i]  = malloc(imagesize(0,0,WIDTH,300));
        getimage(0,0,WIDTH,300, imagenes[i]);
    }

    // Creditos
    bar(0,0,WIDTH,HEIGHT);
    readimagefile(nombres[i],0,0,1040,390);
    imagenes[i]  = malloc(imagesize(0,0,1040,390));
    getimage(0,0,1040,390, imagenes[i]);

    // logo
    bar(0,0,WIDTH,HEIGHT);
    readimagefile(nombres[++i],0,0,798,303);
    imagenes[i]  = malloc(imagesize(0,0,798,303));
    getimage(0,0,798,303, imagenes[i]);

    portada();
}

void juego(int vidas)
{
    char tecla;
    Malla cab = NULL, dino, aux;
    int spriteH, // Altura de cualquier sprite
        retraso = 180,
        pagina = 1,
        i, j = 0,
        huesos,
        jungla = 0;
    String nSkin;

    srand(time(NULL));
    spriteH = 592 - PIXEL_TAM*23;
    sprintf(nSkin, "dino%d.0.txt", skin);

	pintaAmbiente(pagina, jungla);
    obtenerDatos(&huesos, &i);
    // Inicia malla
	creaMalla(&cab, &dino);
	setactivepage(pagina);
    setfillstyle(1, COLOR(79, 182, 225));
    bar(0,0, WIDTH, HEIGHT);

	setactivepage(pagina!=pagina);
    setfillstyle(1, COLOR(79, 182, 225));
    bar(0,0, WIDTH, HEIGHT);

    setvisualpage(pagina);
    do
    {
        setactivepage(pagina=!pagina);
        jungla+=jungla<=4?1:-4;
        pintaAmbiente(pagina, jungla);
        delay(retraso);
        pintaEscenario(cab);
        dibujaVidas(vidas);
        dibujaHuesos(huesos);
        if(j>6)
        {
            retraso-=retraso>50?8:0;
            j=0;
            aux=cab;
            for(i=0; i<N-1; i++)
                aux = aux->der;

            for(i=0; i<M-1; i++)
                aux = aux->abajo;

            aux->tipo=rand()%4+1;
        }
        j+=rand()%3;
        setvisualpage(pagina);

        if(kbhit())
        {
            dino = dino->arriba->arriba;
            tecla = getch();
            putimage(0, 400, imagenes[jungla], COPY_PUT);
            pintaEscenario(cab);
            dibujaSprite(nSkin, 100, spriteH);
            delay(retraso);
            dino = dino->abajo->abajo;
        }

        switch(dino->tipo)
        {
            case 1:huesos+=5; break;
            case 2:huesos++; break;
            case 3:vidas--;  break;
            case 4:vidas--;  break;
        }
        printf("\nVidas %d\tHuesos %d", vidas, huesos);

    }while(vidas>0);

    escribeAjustes(huesos);
    popup();
    getch();
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
            juego(3);
            break;
        case 1:
            tienda();
            break;
        case 2:
            //ayuda("ayuda.txt", 0, HEIGHT/2-100, WIDTH, HEIGHT);
            break;
        case 3:
            creditos();
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

void pintaAmbiente(int pagina, int jungla)
{
    String nSkin;
    setfillstyle(1, COLOR(79, 182, 225));
    bar(0, 0, WIDTH, 400);
    // Dino
    sprintf(nSkin, "dino%d.%d.txt", skin, pagina);
    putimage(0, 400, imagenes[rand()%3+1], COPY_PUT);
    dibujaSprite(nSkin, 100, 592);
}

void pintaEscenario(Malla cab)
{
    Malla auxy;
    int i, j, x, y;
    auxy=cab;

    x = 0;
    y = 510;
    for(j=0; j<M; j++, y+=CUADRO_TAM, x=0)
    {
        for(i=0; i<N; i++, x+=CUADRO_TAM, cab=cab->der)
        {
            if(cab->tipo)
                switch(cab->tipo)
                {
                    case 1:
                        dibujaSprite("hueso.txt", x, y);
                        break;
                    case 2:
                        dibujaSprite("huesito.txt", x, y);
                        break;
                    case 3:
                        dibujaSprite("arbusto.txt", x, y);
                        break;
                    case 4:
                        dibujaSprite("arbusto2.txt", x, y);
                        break;
                }
            putpixel(rand()%WIDTH, rand()%10+690, BROWN);
            putpixel(rand()%WIDTH, rand()%10+690, GREEN);
            putpixel(rand()%WIDTH, rand()%10+690, GREEN);
        }
        putpixel(rand()%WIDTH, rand()%50+600, WHITE);
        putpixel(rand()%WIDTH, rand()%80+600, WHITE);
        if(j!=M-1)
        {
            auxy=auxy->abajo;
            cab=auxy;
        }
    }
    cab=auxy;
    for(i=0;i<N;i++)
    {
        for(j=0;j<M;j++)
        {
            if(i!=N-1)
            {
               cab->tipo=cab->der->tipo;
               cab=cab->arriba;
               printf("");
            }
            else
            {
                //cab->tipo=0;
                cab->tipo=0;
                cab=cab->arriba;
            }
        }
        auxy=auxy->der;
        cab=auxy;
    }
}

void popup()
{
    int x1,x2,y1,y2, i,
        nRec = 13; // Numero de rectangulos
    x1 = x2 = (WIDTH/2);
    y1 = y2 = (HEIGHT/2);

    setfillstyle(1,0x00CCFF);
    for(i=0; i<nRec; x1-=15, y1-=10, x2+=15, y2+=10, i++)
    {
        bar(x1, y1, x2, y2);
        delay(15);
    }

    // ¡PERDISTE!
    setbkcolor(0x00CCFF);
    setcolor(0x0054D3);
    settextstyle(3, HORIZ_DIR, 6);
    outtextxy(WIDTH/2-textwidth("¡PERDISTE!")/2, y1+textheight("P")/2, "¡PERDISTE!");

    setcolor(0x4b78F2);
    settextstyle(3, HORIZ_DIR, 3);
    outtextxy(WIDTH/2-textwidth("Presiona cualquier tecla")/2, y1+textheight("P")*4, "Presiona cualquier tecla");

    settextstyle(3, HORIZ_DIR, 3);
    outtextxy(WIDTH/2-textwidth("para continuar")/2, y1+textheight("P")*5, "para continuar");
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
        putimage(WIDTH/10, i, imagenes[NIMAGENES-1], COPY_PUT);
        delay(2);
    }

    // Escribe numero de huesos
    obtenerDatos(&huesos, &skin);
    dibujaHuesos(huesos);
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
