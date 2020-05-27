#ifndef funciones
#define funciones

#include <NTPClient.h>
#include <ArduinoJson.h>
#include "ESP8266HTTPClient.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

void entablarConexiones();
void hacerPeticion(String query);
String getDate(NTPClient timeClient);
String setTanqueEsta(String tanqueID, String lugarID, String fecha);

void entablarConexiones(){
  //Aqui puse los ssid ya que no queremos que sean variables globales por ser credenciales de seguridad.
  const char* ssid = "";
  const char* password = "";
  Serial.begin(9600);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  while (!Serial) continue;
}

void hacerPeticion(String query){
  //Al ser la direccion del servidor tampoco queremos que este de manera global asi que la deje dentro de esta funcion.
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;   
    http.begin("http://18.219.108.70:5201/graphql");
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(query);
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

String getDate(NTPClient timeClient){
  String fecha; //YYYY-MM-DD
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *p_tm = gmtime((time_t *)&epochTime);
  int monthDay = p_tm->tm_mday;
  int currentMonth = p_tm->tm_mon+1;
  int currentYear = p_tm->tm_year+1900;
  fecha = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  return fecha;
}

String setTanqueEsta(String tanqueID, String lugarID, String fecha){
  String output;
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

#endif
