#include <stdlib.h>
#include <stdio.h>
#include <graphics.h>

#define NIMAGENES 1 // Numero de imagenes externas
#define HEIGHT 700
#define WIDTH 1024

typedef char String[50];

void iniciaEntorno(); // Se encarga de iniciar la parte gráfica y obtener las imágenes del juego.
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
        "img/logotipo.gif"
    };

    for(i=0; i<NIMAGENES; i++)
    {
        bar(0,0,WIDTH,HEIGHT);
        readimagefile(nombres[i],0,0,798,303);
        imagenes[i]  = malloc(imagesize(0,0,798,303));
        getimage(0,0,798,303, imagenes[i]);
    }
}

void portada()
{
    bar(0, 0, WIDTH, HEIGHT);
    putimage(WIDTH/10, 50, imagenes[0], COPY_PUT);
}
