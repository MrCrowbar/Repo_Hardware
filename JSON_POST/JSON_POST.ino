#include <ArduinoQueue.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "funciones.h"
#include <Ticker.h>
#include <ESP8266WiFi.h>

#define GMT_MEXICO -18000 //Ajustar tiempo internacional a México en segundos
#define OUTPUT_SIZE 500 //Salida para el JSON

#define INTERVALO_CONEXION 300 //Intervalo en segundos de revisar la conexión
#define INTERVALO_TIEMPO_LOCAL 60 //Intervalo en segundos para sumar los minutos del tiempo local


//----------Variables del lector RFID---------------------------------
#define bit_size 34
char tag_Data[bit_size]; //Arreglo que contendrá la informacion del lector
int W0 = 5; //Entrada Wiegand D0
int W1 = 4; //Entrada Wiegand D1
int cont_Size = 0; //Contador para el indice de cada bit recibido
int w0 = 1; //Valor de entrada Wiegand D0
int w1 = 1; //Valor de entrada Wiegand D1
//--------------------------------------------------------------------


//Declaracion de variables globales.
char output[OUTPUT_SIZE]; //Falta ajustar el tamaño para que no se desperdicie memoria.
String stringOutput; //Variable para almacenar el JSON que va a la petición.

//Servidor para tiempo.
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Clases de tickers
Ticker tiempo;
Ticker conectividad;


//Estructura de dato para guardar tanques detectados
struct tanque {
  String idLugar;
  byte idTanque;
  String fecha;
};
ArduinoQueue<tanque> queue_tanque(10);

//Estructura de dato para guardar tiempo local
struct local_time {
  String tiempo;
  int year;
  byte hour;
  byte minute;
  byte second;
  byte day;
  byte month;
}_time;


//Flag global del estado actual de la conexión
bool conexionFlagActual = false;
bool conexionFlagPasado = conexionFlagActual;


//Sumer tiempo local 5 minutos cada 5 minutos
void sumarTiempoLocal();


//Cambiar flag de conexión
void conexion(){
  conexionFlagActual = !conexionFlagActual;
}


void setup() {
  pinMode(W0, INPUT); //Entrada Wiegand D0
  pinMode(W1, INPUT); //Entrada Wiegand D1
  entablarConexiones(); //Hasta no entablar la conexion no se procede a generar recursos que no se pueden utilizar.
  timeClient.begin();
  timeClient.setTimeOffset(GMT_MEXICO);
  while (!timeClient.update()){
    Serial.println("Updating NTP server time..");
    timeClient.forceUpdate();
  }
  getDate();
  //El entero que va como argumento dentro de attach define cada cuándo se llama el ticker.
  //En caso de cambiar tiempo.attach() deben también cambiar el in
  conectividad.attach(INTERVALO_CONEXION, conexion); //cada 10 segundos cambiamos el flag de conexión
  tiempo.attach(INTERVALO_TIEMPO_LOCAL, sumarTiempoLocal); //cada 5 segundos accionar la función sumarTiempoLocal
  Serial.println("Setup exitoso, procediendo a void loop");
}


void loop() {
  //Revisamos el estado de los flags de conexión para actualizar el timeClient y dar el estatus de conexión.
  if (conexionFlagPasado != conexionFlagActual){
    if (WiFi.status() == WL_CONNECTED){
      Serial.println("Estado de conexión: Conectado");
      timeClient.update();
      while (!timeClient.update())timeClient.forceUpdate();
      getDate();
      Serial.print("Actualización de tiempo local con NTP: ");
      Serial.print(_time.tiempo);
    } else {
    Serial.println("Desconectado");
  }
  Serial.println(WiFi.status());
  conexionFlagPasado = !conexionFlagPasado;
  }
  
  //Cuando se detecta una lectura se procede a lo siguiente
  if (revisa_lectura()){
    Serial.println("Tarjeta detectada");
    crearTanque(binToInt(tag_Data)); //crear nuevo tanque con binToInt()
    stringOutput = setTanqueEsta(queue_tanque.getHead().idTanque,queue_tanque.getHead().idLugar,queue_tanque.getHead().fecha); //Crear JSON del tanque
    if(hacerPeticion(stringOutput))queue_tanque.dequeue(); //Si petición exitosa borrar tanque del queue
    else Serial.println("Petición fallida, no se borra tanque de queue"); //Si petición fallida no borrar tanque del queue
    Serial.print("No de items de queue: ");
    Serial.println(queue_tanque.itemCount());
  }
}

//Añadir nuevo tanque al queue_tanque
void crearTanque(byte _idTanque){
  struct tanque _tanque;
  _tanque.idLugar = "AMC";
  _tanque.idTanque = _idTanque;
  _tanque.fecha = _time.tiempo;
  queue_tanque.enqueue(_tanque);
  byte idTanque = queue_tanque.getHead().idTanque;
  String idLugar = queue_tanque.getHead().idLugar;
  String fecha = queue_tanque.getHead().fecha;
  Serial.println("Añadiendo nuevo tanque con lo siguiente: ");
  Serial.print("ID tanque: ");
  Serial.print(idTanque);
  Serial.print("ID lugar: ");
  Serial.print(idLugar);
  Serial.print("Fecha: ");
  Serial.print(fecha);
  Serial.println();
}

//Se suma a nuestro tiempo local 5 minutos cada 5 minutos.
void sumarTiempoLocal(){
  String hora;
  String minuto;
  String dia;
  String mes;
 
  _time.minute += INTERVALO_TIEMPO_LOCAL;
  if (_time.minute >= 60){
    _time.minute = 0;
    _time.hour += 1;
    if (_time.hour >= 24){
      _time.hour = 0;
    }
  }
  hora = String(_time.hour);
  minuto = String(_time.minute);
  dia = String(_time.day);
  mes = String(_time.month);
  if (_time.hour < 10) hora = "0" + String(_time.hour);
  if (_time.minute < 10) minuto = "0" + String(_time.minute);
  if (_time.day < 10) dia = "0" + String(_time.day);
  if (_time.month < 10) mes = "0" + String(_time.month);
  
  _time.tiempo = String(_time.year) + "-" + mes + "-" + dia + "T" + hora + ":" + minuto + ":" + String(_time.second) + "Z";
  Serial.print("Sumando a tiempo local: ");
  Serial.print(_time.tiempo);
  Serial.println();
}

//Actualizar hora local con el servidor NTP
void getDate(){
  String hora; //HH:MM:SS
  String mes;
  String dia;
  hora = timeClient.getFormattedTime();
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *p_tm = gmtime((time_t *)&epochTime);
  int monthDay = p_tm->tm_mday;
  int currentMonth = p_tm->tm_mon+1;
  int currentYear = p_tm->tm_year+1900;
  if (monthDay < 10) dia = "0" + String(monthDay);
  if (currentMonth < 10) mes = "0" + String(currentMonth);
  _time.hour = timeClient.getHours();
  _time.minute = timeClient.getMinutes();
  _time.second = timeClient.getSeconds();
  _time.day = monthDay;
  _time.month = currentMonth;
  _time.year = currentYear;
  _time.tiempo = String(currentYear) + "-" + mes + "-" + dia + "T" + hora + "Z";
}

//--------------------Funciones del lector RFID------------------
int binToInt(String in){
//Convierte la informacion obtenida por el lector de binario a entero
    int mult = 1; //Factor de multiplicaci'on binario, establece el peso de la posicion de cada bit
    int result = 0; //Valor a retornar
    for (int i = bit_size-2 ; i > 0; i--) { 
      /* 
        Conversion de datos, se ignoran el primer y
        el último bit recibido, pues son bits de 
        paridad. 
      */
      if(tag_Data[i] =='1')
        result += mult; //Suma el valor del peso de la posición del bit al resultado
      mult *= 2; //Mueve el peso de la posición a la siguiente
    }
    return result;
}

bool revisa_lectura(){
  bool lectura = false;
  if ((w0 & w1)) //Si ambas entradas están en alto no se esta recibiendo mensaje
  { 
   w0 = digitalRead(W0); //Actualiza el valor de W0
   w1 = digitalRead(W1); //Actualiza el valor de W1
  }
  else {
    if (w0 == 0 && w1 == 1) tag_Data[cont_Size] = '0'; // Se recibe un 1 en el mensaje
    if (w1 == 0 && w0 == 1) tag_Data[cont_Size] = '1'; // Se recibe un 0 en el mensaje
    cont_Size++; //Se actualiza el indice del bit del mensaje
    while (!(w0 & w1))//El mensaje termina cuando ambas entradas vuelvan a estar en 1
    {
      w0 = digitalRead(W0); //Actualiza el valor de W0
      w1 = digitalRead(W1); //Actualiza el valor de W1
    }
  }
  if (cont_Size == bit_size){ //Si ya se recibieron todos los bits (34 por estandar) empieza a convertirlos 
    cont_Size = 0;
    lectura = true;
  }
  return lectura;
}
//--------------------------------------------------------------
