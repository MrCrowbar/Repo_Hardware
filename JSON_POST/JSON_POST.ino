#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//Declaracion de variables globales.
char output[500]; //Falta ajustar el tamaño para que no se desperdicie memoria.
String tanqueID, lugarID, fecha, stringOutput;

//Declaracion de funciones.
String setTanqueEsta(String tanqueID, String lugarID, String fecha);
String createTanqueEsta(String tanqueID, String lugarID, String fecha);
void entablarConexiones();
void hacerPeticion(String peticion);

//En teoria no vamos a crear tanques pero sirve como ejemplo para formular los JSON y para hacer pruebas.
String createTanque(String tanqueID, String calidad, String estadoValvula, float pesoActual, float peso, String fechaEsperadaRetorno, int idEtiqueta, String idContenido, String idDueno, String fechaIngreso, String observaciones);


void setup() {
  entablarConexiones(); //Hasta no entablar la conexion no se procede a generar recursos que no se pueden utilizar.
  stringOutput = String();
  tanqueID = String("EURO5149ZZ");
  lugarID = String("AMC");
  fecha = String("2122-10-03T10:00:00Z");
  stringOutput = setTanqueEsta(tanqueID,lugarID,fecha); //Esta variable almacena el formato JSON que va a cargarse en el POST.
  Serial.println(stringOutput);
  hacerPeticion("GET"); //Hacer la peticion POST, no hace falta pasar "stringOutput" porque es una variable global.
}

void loop() {
  hacerPeticion("GET");
  delay(3000);
  //Esta vacio ya que no queremos mandar la misma peticion varias veces, por el momento solamente estamos mandandola una vez.
}
void hacerPeticion(String peticion){
  //Al ser la direccion del servidor tampoco queremos que este de manera global asi que la deje dentro de esta funcion.
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;   
    http.begin(client,"http://192.168.1.66:5000/arduino");
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
  const char* ssid = "INFINITUM9209_2.4";
  const char* password = "4by3rg4JgD";
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
