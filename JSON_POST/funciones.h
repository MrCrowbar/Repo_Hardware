#ifndef funciones
#define funciones

#include <ArduinoJson.h>
#include "ESP8266HTTPClient.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

void entablarConexiones();
bool hacerPeticion(String query);
bool hacerPeticionID(String query, String* id_tanque_peticion);
String getTanqueID(int tag_ID);
String setTanqueEsta(String tanqueID, String lugarID, String fecha);

void entablarConexiones(){
  //Aqui puse los ssid ya que no queremos que sean variables globales por ser credenciales de seguridad.
  const char* ssid = "INFINITUM9209_2.4"; //INFINITUM9209_2.4 INFINITUM8312_2.4
  const char* password = "4by3rg4JgD"; //4by3rg4JgD 76Mroar07m
  Serial.begin(115200);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  while (!Serial) continue;
}

bool hacerPeticion(String query){
  bool respuesta = true;
  //Al ser la direccion del servidor tampoco queremos que este de manera global asi que la deje dentro de esta funcion.
  String TOKEN("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZExlY3RvciI6IkxFQ1QwMDAwMDEiLCJpYXQiOjE1OTE2NzM4MzMsImV4cCI6MjQ1NTU4NzQzM30.ZjMCFuld8I1ZeGbnHXOFmk3v_rd3yvb-eizC8bS2v9nkjL-9xNyuyzdDh14sWDsOH85QtjtqxlyySFyop9L9om3U79s-MAGhLoV_PvTLZ80du_8GghIjL0wZxxUND1dREzWx1toDu1VJBbfi7s8p0lt5OLMtcOedmlwMAn0E6YSR94KzAwIkQPtTeZgZwUFNet75X1Rrr_oKS9p2K-T0KAKMQ-K-4CRYqYQ1FPOOggPi_Un-cmLmq3PG0BV9xjFTMZYnGf-uXrOC49PjKX1sm6AvR7q4kk5q9smezcqsCeJ7kYsSskpg3GfQtkzACn1wi2AU1jHd5L_DBBeoWtR4Zw");
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    http.begin(client,"http://200.23.108.91:5201/graphql");//http://192.168.1.66:5000/arduino
    http.addHeader("authorization", TOKEN);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(query);
    if(httpResponseCode>0){
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    }else{
      Serial.print("Error on sending POST Request: ");
      Serial.println(httpResponseCode);
      respuesta = false;
   }
   http.end();
 }else{
    Serial.println("Error in WiFi connection");
    respuesta = false;
 }
 return respuesta;
}

bool hacerPeticionID(String query, String* id_tanque_peticion){
  bool estado_salida = true;
  //Al ser la direccion del servidor tampoco queremos que este de manera global asi que la deje dentro de esta funcion.
  String response = "";
  String TOKEN("eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZExlY3RvciI6IkxFQ1QwMDAwMDEiLCJpYXQiOjE1OTE2NzM4MzMsImV4cCI6MjQ1NTU4NzQzM30.ZjMCFuld8I1ZeGbnHXOFmk3v_rd3yvb-eizC8bS2v9nkjL-9xNyuyzdDh14sWDsOH85QtjtqxlyySFyop9L9om3U79s-MAGhLoV_PvTLZ80du_8GghIjL0wZxxUND1dREzWx1toDu1VJBbfi7s8p0lt5OLMtcOedmlwMAn0E6YSR94KzAwIkQPtTeZgZwUFNet75X1Rrr_oKS9p2K-T0KAKMQ-K-4CRYqYQ1FPOOggPi_Un-cmLmq3PG0BV9xjFTMZYnGf-uXrOC49PjKX1sm6AvR7q4kk5q9smezcqsCeJ7kYsSskpg3GfQtkzACn1wi2AU1jHd5L_DBBeoWtR4Zw");
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    http.begin(client,"http://200.23.108.91:5201/graphql");//http://192.168.1.66:5000/arduino
    http.addHeader("authorization", TOKEN);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(query);
    if(httpResponseCode>0){
      response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
      DynamicJsonDocument ID_Response(1024);
      deserializeJson(ID_Response, response);
      *id_tanque_peticion = ID_Response["data"]["tanques"][0]["idTanque"].as<String>();
      Serial.print("El ID del tanque es: ");
      Serial.println(*id_tanque_peticion);
    }else{
      Serial.print("Error on sending POST Request: ");
      Serial.println(httpResponseCode);
      estado_salida = false;
   }
   http.end();
 }else{
    Serial.println("Error in WiFi connection");
    estado_salida = false;
 }
 return estado_salida;
}

String getTanqueID(int tag_ID){
  String output;
  DynamicJsonDocument root(500);
  root["query"].set("{tanques(idEtiqueta:"+String(tag_ID)+"){idTanque}}");;
  serializeJson(root, output);
  serializeJsonPretty(root, Serial);
  return output;
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
