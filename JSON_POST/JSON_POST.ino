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

//Declaracion de funciones.
//String setTanqueEsta(String tanqueID, String lugarID, String fecha);
//String createTanqueEsta(String tanqueID, String lugarID, String fecha);

//En teoria no vamos a crear tanques pero sirve como ejemplo para formular los JSON y para hacer pruebas.
//String createTanque(String tanqueID, String calidad, String estadoValvula, float pesoActual, float peso, String fechaEsperadaRetorno, int idEtiqueta, String idContenido, String idDueno, String fechaIngreso, String observaciones);


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
  //hacerPeticion(stringOutput); //Hacer la peticion POST, no hace falta pasar "stringOutput" porque es una variable global.
}

void loop() {
  //Esta vacio ya que no queremos mandar la misma peticion varias veces, por el momento solamente estamos mandandola una vez.
  Serial.println(getDate(timeClient));
  hacerPeticion(stringOutput);
  delay(5000);
}
/*
String createTanqueEsta(String tanqueID, String lugarID, String fecha){
  DynamicJsonDocument  root(200); //Documento raiz del JSON
  root["query"].set("mutation($tank: TanqueEstaInput){createTanqueEsta(tanqueEstaInput: $tank)}");
  JsonObject variables = root.createNestedObject("variables"); //Creacion de objeto anidado "variables" dentro de "root".
  JsonObject tank = variables.createNestedObject("tank"); //Creacion de objeto anidado "tank" dentro de "variables".
  JsonObject id = tank.createNestedObject("id"); //Creacion de objeto anidado "id" dentro de "tank".
  id["idTanque"].set(tanqueID);
  id["idLugar"].set(lugarID);
  tank["fecha"].set(fecha);
  serializeJson(root, output); //Serializar el documento JSON "root" a la variable output en el formato listo a enviar al servidor.
  serializeJsonPretty(root, Serial); //Imprimir en la terminal de manera completa el JSON generado.
  return output; //Regresar el archivo que va a cargarse en la peticion POST.
}

String createTanque(String tanqueID, String calidad, String estadoValvula, float pesoActual, float peso, String fechaEsperadaRetorno, int idEtiqueta, String idContenido, String idDueno, String fechaIngreso, String observaciones){
  DynamicJsonDocument root(500);
  root["query"].set("mutation($tank: TanqueInput!){createTanque(tanqueInput: $tank)}");
  JsonObject variables = root.createNestedObject("variables");
  JsonObject tank = variables.createNestedObject("tank");
  tank["id"].set(tanqueID);
  tank["calidad"].set(calidad);
  tank["pesoActual"].set(pesoActual);
  tank["idContenido"].set(idContenido);
  tank["idDueno"].set(idDueno);
  tank["fechaIngreso"].set(fechaIngreso);
  tank["fechaEsperadaRetorno"].set(fechaEsperadaRetorno);
  tank["idEtiqueta"].set(idEtiqueta);
  tank["peso"].set(peso);
  tank["observaciones"].set(observaciones);
  variables["id"].set(tanqueID);
  serializeJson(root, output);
  //Sacar el json root a la terminal bien bonito
  serializeJsonPretty(root, Serial);
  return output;
}

String setTanqueEsta(String tanqueID, String lugarID, String fecha){
  DynamicJsonDocument root(500);
  root["query"].set("mutation($tank: TanqueEstaInput!, $id:String!){setTanqueEsta(tanqueEstaInput: $tank, idTanqueEstaOriginal: $id)}");
  JsonObject variables = root.createNestedObject("variables");
  JsonObject tank = variables.createNestedObject("tank");
  tank["id"].set(tanqueID);
  tank["idLugar"].set(lugarID);
  tank["fecha"].set(fecha);
  variables["id"].set(tanqueID);
  serializeJson(root, output);
  serializeJsonPretty(root, Serial);
  return output;
}
*/
