// ----------------------------------------------
// CDR - Reproductor de CDs para DOS
// ----------------------------------------------
// Creado por Kronoman
// En memoria de mi querido padre
// (c) 2001, Kronoman - Freeware
// ----------------------------------------------
// Info agregada Octubre-2019
// ----------------------------------------------
// Necesita la libreria "libcda" para DJGPP
//
// libcda is "A simple, cross-platform CD playing library".  
// It was designed to be used in computer games, 
// but obviously this is not necessary.
// 
// Descarga desde:
// https://github.com/wangp/libcda
// ----------------------------------------------
// Compilar con DJGPP para MS DOS
//
// gcc cdr.c -o cdr.exe -lcda
// ----------------------------------------------
// Utilidad: permite reproducir un CD desde
// la linea de comandos, SIN interface
// ----------------------------------------------
// Comandos:
//
// CDR PLAY [pista]  Toca de la [pista] hasta el final del disco
//                   si se omite [pista], se asume 1
//
// CDR STOP          Para el CD
//
// CDR NEXT          Pasar a la proxima pista
//
// CDR PREV          Pista previa
//
// CDR RAND          Elije una pista al azar y la toca
//
// CDR I             Muestra informacion del CD
//
// CDR PAUSE         Pausar la reproduccion
//
// CDR RESUME        Continuar luego de una pausa
//
// CDR EJECT         Abrir la compactera (si es posible)
//
// CDR CLOSE         Cerrar la compactera (si es posible)
//
// CDR VOL  [vol]    Setea el volumen a [vol] 0..255; default = 255
// ----------------------------------------------

// ----------------------------------------------
// Inclusiones
// ----------------------------------------------

#include <stdio.h>

#include <stdlib.h>

#include <time.h> // para el random

#include "libcda.h" // Libreria de audio CD

// ----------------------------------------------
// Ayuda del programa
// ----------------------------------------------
void ayuda() {
  printf("----------------------------------------------------------------------------\n");
  printf("CDR - Reproductor de CDs\n");
  printf("----------------------------------------------------------------------------\n");
  printf("Creado por Kronoman\n");
  printf("En memoria de mi querido padre\n");
  printf("(c) 2001, Kronoman - Freeware\n");
  printf("----------------------------------------------------------------------------\n");
  printf("Uso del programa:\n");
  printf("CDR PLAY [pista]  Toca de la [pista] hasta el final del disco\n");
  printf("                  si se omite [pista], se asume 1\n");
  printf("CDR STOP          Para el CD\n");
  printf("CDR NEXT          Pasar a la proxima pista\n");
  printf("CDR PREV          Pista previa\n");
  printf("CDR RAND          Elije una pista al azar y la toca\n");
  printf("CDR I             Muestra informacion del CD\n");
  printf("CDR PAUSE         Pausar la reproduccion\n");
  printf("CDR RESUME        Continuar luego de una pausa\n");
  printf("CDR EJECT         Abrir la compactera (si es posible)\n");
  printf("CDR CLOSE         Cerrar la compactera (si es posible)\n");
  printf("CDR VOL  [vol]    Setea el volumen a [vol] 0..255; default = 255\n");
  printf("----------------------------------------------------------------------------\n");
}

// ----------------------------------------------
// Funcion que se ejecuta al salir
// Libera memoria, etc
// ----------------------------------------------
void salir_func() {
  // Limpiar la libreria del CD
  cd_exit();
  printf("\n(c) 2001, Kronoman - En memoria de mi querido padre\n");
}
// ----------------------------------------------
// int main -> comienzo del programa
// ----------------------------------------------
int main(int argc, unsigned char * argv[]) {
  int c; // contador pedorro
  int pista; // pista a tocar
  int pista_i = 1, pista_f = 1; // pista inicio y final
  int vol_i = 255, vol_d = 255; // volumen izquierdo y derecho

  atexit(salir_func);

  /* iniciar numeros aleatorios */
  srand(time(NULL));

  // printf ("\nCantidad de parametros: %d\n\n", argc); // DEBUG - Eliminar

  if (cd_init() < 0) {
    printf("Error inicializando el control del CD! \n");
    return 1;
  }

  // Si no puso parametros, mostrar la ayuda
  if (argc <= 1) {
    ayuda();
    return (0);
  } // DEBUG

  /* chequear parametros */
  /* IN-SENSIBLE a las mayusculas/minusculas! */
  for (c = 1; c < argc; c++) {

    // volumen  [defaul = al mango!]
    if (stricmp(argv[c], "vol") == 0) {
      if (argc < 3)
        vol_i = 255;
      else
        vol_i = atoi(argv[c + 1]);

      if (vol_i < 0) vol_i = 0;
      if (vol_i > 255) vol_i = 255;

      vol_d = vol_i;

      cd_set_volume(vol_i, vol_d);

      cd_get_volume( & vol_i, & vol_d);
      printf("Volumen: \n %000d - %000d \n", vol_i, vol_d);
    }

    // I = informacion
    if (stricmp(argv[c], "i") == 0) {
      printf("Informacion: \n");
      cd_get_volume( & vol_i, & vol_d);
      printf("Volumen: %000d - %000d \n", vol_i, vol_d);

      cd_get_tracks( & pista_i, & pista_f); // tomar las pistas

      printf("Pista inicial: %d \n", pista_i);
      printf("Pista final  : %d \n", pista_f);

      if (cd_is_paused() != 0) printf("El CD esta pausado \n");
      if (cd_current_track() == 0) printf("El CD esta parado \n");
    }

    // Tocar al azar, comienza de una pista al azar...
    if (stricmp(argv[c], "rand") == 0) {
      cd_get_tracks( & pista_i, & pista_f); // tomar las pistas
      // elegir pista
      pista = (rand() % (pista_f - pista_i + 1)) + pista_i;

      if (cd_play_from(pista) != 0) {
        printf("ERROR:\nOcurrio un error intentando tocar la pista %d\n", pista);
        return 1;
      }
    }

    // PLAY [pista]
    if (stricmp(argv[c], "play") == 0) {
      cd_get_tracks( & pista_i, & pista_f); // tomar las pistas

      if (argc < 3)
        pista = pista_i; // pista inicial
      else
        pista = atoi(argv[c + 1]);

      if (pista < 1) pista = 1;

      // Ver, si es pista de datos, adelantar a la siguiente o hasta el final
      while ((pista <= pista_f) && (cd_is_audio(pista) == 0)) pista++;

      if (cd_play_from(pista) != 0) {
        printf("ERROR:\nOcurrio un error intentando tocar la pista %d\n", pista);
        return 1;
      }
    } // listo del play

    // STOP
    if (stricmp(argv[c], "stop") == 0) cd_stop();

    // Pista proxima
    if (stricmp(argv[c], "next") == 0) {
      pista = cd_current_track();
      pista++;
      if (pista < 1) pista = 1;
      if (cd_play_from(pista) != 0) {
        printf("ERROR:\nOcurrio un error intentando tocar la pista %d\n", pista);
        return 1;
      }
    } // fin pista proxima

    // Pista anterior
    if (stricmp(argv[c], "prev") == 0) {
      pista = cd_current_track();
      pista--;
      if (pista < 1) pista = 1;
      if (cd_play_from(pista) != 0) {
        printf("ERROR:\nOcurrio un error intentando tocar la pista %d\n", pista);
        return 1;
      }
    } // fin pista anterior

    // Pausa
    if (stricmp(argv[c], "pause") == 0) {
      cd_pause();
      printf(" -- PAUSA -- \n ");
    }

    // Continuar
    if (stricmp(argv[c], "resume") == 0) {
      cd_resume();
      printf(" -- CONTINUAR -- \n ");
    }

    // Ejectar (si es posible)
    if (stricmp(argv[c], "eject") == 0) cd_eject();

    // Cerrar (si es posible)
    if (stricmp(argv[c], "close") == 0) cd_close();

  } // fin del for

  if (cd_current_track() > 0) printf("Pista actual : %d\n\n", cd_current_track());

  return 0;
}
