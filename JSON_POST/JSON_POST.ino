<<<<<<< HEAD
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
=======
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "funciones.h"

#define GMT_MEXICO -18000
#define OUTPUT_SIZE 500
>>>>>>> funciones-separadas

//Declaracion de variables globales.
char output[OUTPUT_SIZE]; //Falta ajustar el tamaño para que no se desperdicie memoria.
String tanqueID, lugarID, fecha, stringOutput;

//Servidor para tiempo.
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
String weekDays[7] = {"Domingo","Lunes","Martes","Miercoles","Jueves","Viernes","Sabado"};
String months[12] = {"Enero","Febrero","Marzo","Abril","Mayo","Junio","Julio","Agosto","Septiembre","Octubre","Noviembre","Diciembre"};

//Declaracion de funciones.
<<<<<<< HEAD
String setTanqueEsta(String tanqueID, String lugarID, String fecha);
String createTanqueEsta(String tanqueID, String lugarID, String fecha);
void entablarConexiones();
void hacerPeticion(String peticion);
=======
//String setTanqueEsta(String tanqueID, String lugarID, String fecha);
//String createTanqueEsta(String tanqueID, String lugarID, String fecha);
>>>>>>> funciones-separadas

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
<<<<<<< HEAD
  hacerPeticion("GET"); //Hacer la peticion POST, no hace falta pasar "stringOutput" porque es una variable global.
=======
  //hacerPeticion(stringOutput); //Hacer la peticion POST, no hace falta pasar "stringOutput" porque es una variable global.
>>>>>>> funciones-separadas
}

void loop() {
  hacerPeticion("POST");
  delay(3000);
  //Esta vacio ya que no queremos mandar la misma peticion varias veces, por el momento solamente estamos mandandola una vez.
  Serial.println(getDate(timeClient));
  hacerPeticion(stringOutput);
  delay(5000);
}
<<<<<<< HEAD
void hacerPeticion(String peticion){
  //Al ser la direccion del servidor tampoco queremos que este de manera global asi que la deje dentro de esta funcion.
  String TOKEN("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJjb3JyZW8iOiJmbG9yb0BxdWltb2Jhc2ljb3MuY29tIiwiaWF0IjoxNTkxMjIzMzU3LCJleHAiOjE2MjI3ODA5NTd9.rnKJUiTUARs6mETAG2Y-08iw4V5ZMBsiWQ8KdY6Kyir7x_xbgiuBmBOI_BPsX4Zz5SquIumNC3jmjJJsYi6KfrIS-RNmaJRfIQRTBHEo5DjdngSzPLh_7UK9A-P-YqEl5Grf9Z8y6XfiQJMluqIRsKcsLC5wlCna0pC-lymvArYgyBpw5lUJrF8ZTQfsGVpbH2ZYYhswbDcHv0_tBUmIUv32HXDhA1sNLNEZsFnItqcr3jqFQwG-_0BosuJKj3B395dVxzb8wDrECjNlvFgRl-m6NuZrhFokWCD4aTlM_pw8heX_aYUQgSU8iABP68UluNmuQIgWpkVULvHDGBXpKA");
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    http.begin(client,"http://3.12.196.48:5201/graphql");
    http.addHeader("authorization", TOKEN);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = 0;
    if (peticion == "POST"){
      httpResponseCode = http.POST(stringOutput);
    } else {
      httpResponseCode = http.GET();
    }

    if(httpResponseCode>0){
      String response = http.getString();   
      Serial.println(httpResponseCode);
      Serial.println(response);          
    }else{
      Serial.print("Error on sending " + peticion + " Request: ");
      Serial.println(httpResponseCode);
   }
   http.end();
 }else{
    Serial.println("Error in WiFi connection");
 }
}

void entablarConexiones(){
  //Aqui puse los ssid ya que no queremos que sean variables globales por ser credenciales de seguridad.
  const char* ssid = "INFINITUM8312_2.4";
  const char* password = "76Mroar07m";
  Serial.begin(115200);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  while (!Serial) continue;
}

=======
/*
>>>>>>> funciones-separadas
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
