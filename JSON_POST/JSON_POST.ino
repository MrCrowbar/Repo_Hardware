#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "funciones.h"

#define GMT_MEXICO -18000
#define OUTPUT_SIZE 500

//Declaracion de variables globales.
char output[OUTPUT_SIZE]; //Falta ajustar el tamaño para que no se desperdicie memoria.
String tanqueID, lugarID, fecha, stringOutput;

//Servidor para tiempo.
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
String weekDays[7] = {"Domingo","Lunes","Martes","Miercoles","Jueves","Viernes","Sabado"};
String months[12] = {"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre"};

void setup() {
  entablarConexiones(); //Hasta no entablar la conexion no se procede a generar recursos que no se pueden utilizar.
  timeClient.begin();
  timeClient.setTimeOffset(GMT_MEXICO);
  timeClient.update();
  stringOutput = String();
  tanqueID = String("EURO5149ZZ");
  lugarID = String("AMC");
  fecha = String("3000-10-03T10:00:00Z");
  stringOutput = setTanqueEsta(tanqueID,lugarID,fecha); //Esta variable almacena el formato JSON que va a cargarse en el POST.
  Serial.println(stringOutput);
  hacerPeticion(stringOutput); //Hacer la peticion POST, no hace falta pasar "stringOutput" porque es una variable global.
}

void loop() {
  //Esta vacio ya que no queremos mandar la misma peticion varias veces, por el momento solamente estamos mandandola una vez.
  Serial.println(getDate(timeClient));
  hacerPeticion(stringOutput);
  delay(5000);
}
