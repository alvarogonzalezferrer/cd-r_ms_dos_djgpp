----------------------------------------------
CDR - Reproductor de CDs para DOS
----------------------------------------------
Creado por Kronoman
En memoria de mi querido padre
(c) 2001, Kronoman - Freeware
----------------------------------------------
Info agregada Octubre-2019
----------------------------------------------
Necesita la libreria "libcda" para DJGPP

libcda is "A simple, cross-platform CD playing library".  
It was designed to be used in computer games, 
but obviously this is not necessary.

Descarga desde:
https:github.com/wangp/libcda
----------------------------------------------
Compilar con DJGPP para MS DOS

gcc cdr.c -o cdr.exe -lcda
----------------------------------------------
Utilidad: permite reproducir un CD desde
la linea de comandos, SIN interface
----------------------------------------------
Comandos:

CDR PLAY [pista]  Toca de la [pista] hasta el final del disco
                  si se omite [pista], se asume 1

CDR STOP          Para el CD

CDR NEXT          Pasar a la proxima pista

CDR PREV          Pista previa

CDR RAND          Elije una pista al azar y la toca

CDR I             Muestra informacion del CD

CDR PAUSE         Pausar la reproduccion

CDR RESUME        Continuar luego de una pausa

CDR EJECT         Abrir la compactera (si es posible)

CDR CLOSE         Cerrar la compactera (si es posible)

CDR VOL  [vol]    Setea el volumen a [vol] 0..255; default = 255
----------------------------------------------
