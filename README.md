# Repo_Hardware
Repositorio del equipo de Hardware correspondiente al Semestre-I 2020

Antes de compilar/subir el programa revisar la memoria técnica para revisar las librerías que tienen que instalar.

El programa está compuesto por 2 archivos: main.ino y funciones.h, el archivo main.ino contiene la lógica principal del programa mientras que el archivo funciones.h contiene funciones auxiliares para realizar peticiones al servidor de software.

El programa cuenta con mecanismos de validación de conectividad a WIFI/servidor, esto quiere decir que en caso de que no se pueda entablar una comunicación con el servidor o con la red WIFI el programa almacena los tanques registrados de manera local y cuando se detecta que ya hay conectividad se suben los tanques almacenados al servidor.

Aparte de esto cuenta con un reloj local en caso de que no se pueda actualizar la hora actual con el servidor NTP

Equipo de hardware - 12/06/2020
