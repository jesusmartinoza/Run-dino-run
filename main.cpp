#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>

#define NIMAGENES 2 // Numero de imagenes externas
#define HEIGHT 700
#define WIDTH 1024

typedef char String[50];

void iniciaEntorno(); // Se encarga de iniciar la parte gráfica y obtener las imágenes del juego.
void menu();
void portada(); // Dibujar el menú principal

// Variables globales.
void *imagenes[NIMAGENES]; // Arreglo de imagenes en el juego

int main()
{
    iniciaEntorno();
    portada();

    getch();
    return 0;
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
    readimagefile(nombres[1],0,0,1024,300);
    imagenes[1]  = malloc(imagesize(0,0,1024,300));
    getimage(0,0,1024,300, imagenes[1]);
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
                    break;
                }
    }while(op==-1);
    switch(op)
    {
        case 0:
            //juego(1, 0, 3);
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
        bar(0, 0, WIDTH, 400);
        putimage(WIDTH/10, i, imagenes[0], COPY_PUT);
        delay(2);
    }
    menu();
}
