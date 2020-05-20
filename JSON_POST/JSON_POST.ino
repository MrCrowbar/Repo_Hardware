#include <ArduinoJson.h>
#include "ESP8266HTTPClient.h"
#include "ESP8266WiFi.h"
#include "generador.h"

//Declaracion de variables globales
char output[500];
String tanqueID, lugarID, fecha, stringOutput;

//Declaracion de funciones
String setTanqueEsta(String tanqueID, String lugarID, String fecha);
String createTanqueEsta(String tanqueID, String lugarID, String fecha);
String creatTanque(String tanqueID, String calidad, String estadoValvula, float pesoActual, float peso, String fechaEsperadaRetorno, int idEtiqueta, String idContenido, String idDueno, String fechaIngreso, String observaciones);
void entablarConexiones();

void setup() {
  entablarConexiones();
  stringOutput = String();
  tanqueID = String("EURO5149ZZ");
  lugarID = String("AMC");
  fecha = String("2122-10-03T10:00:00Z");
  //Generador miGenerador();
  //miGenerador.createTanqueEsta(tanqueID,lugarID,fecha);
  
  //stringOutput = createTanqueEsta(tanqueID,lugarID,fecha);
  //stringOutput = createTanque(tanqueID,lugarID,"estadoJacobo",50,100,"2020-10-10T10:00:00Z",777,"Comida","JacoboID","2020-05-05T10:00:00Z","Se ve mamalon");
  stringOutput = setTanqueEsta(tanqueID,lugarID,fecha);
  Serial.println(stringOutput);
  if(WiFi.status()== WL_CONNECTED){
   HTTPClient http;   
   http.begin("http://18.219.108.70:5201/graphql");
   http.addHeader("Content-Type", "application/json");
   int httpResponseCode = http.POST(stringOutput);
   if(httpResponseCode>0){
    String response = http.getString();   
    Serial.println(httpResponseCode);
    Serial.println(response);          
   }else{
    Serial.print("Error on sending POST Request: ");
    Serial.println(httpResponseCode);
   }
   http.end();
 }else{
    Serial.println("Error in WiFi connection");
 }
}

void loop() {
  /*Esta comentado para no tener que hacer peticiones si aun no sirve el query*/
  

  delay(5000);
}

String createTanqueEsta(String tanqueID, String lugarID, String fecha){
  //Generacion del JSON
  //Se genera unicamente el json correspondiente a "tanqueEstaInput" ya que posteriormente se le agregara el inicio y final del query
  DynamicJsonDocument  root(200);
  root["query"].set("mutation($tank: TanqueEstaInput){createTanqueEsta(tanqueEstaInput: $tank)}");
  JsonObject variables = root.createNestedObject("variables");
  JsonObject tank = variables.createNestedObject("tank");
  JsonObject id = tank.createNestedObject("id");
  id["idTanque"].set(tanqueID);
  id["idLugar"].set(lugarID);
  tank["fecha"].set(fecha);
  //Sacar el json root a la variable output
  serializeJson(root, output);
  //Sacar el json root a la terminal bien bonito
  serializeJsonPretty(root, Serial);
  return output;
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

void entablarConexiones(){
  // Initialize Serial port
  const char* ssid = "INFINITUM9209_2.4";
  const char* password = "4by3rg4JgD";
  Serial.begin(9600);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  while (!Serial) continue;
}
