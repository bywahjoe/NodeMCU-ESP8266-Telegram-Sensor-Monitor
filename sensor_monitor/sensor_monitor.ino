#include "CTBot.h"
#include "DHT.h"


#define chatID 1028722393

#define cahayaPIN D4
#define hujanPIN D3
#define DHTPIN D2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
CTBot myBot;

//WIFI CONFIG
String ssid = "robotku";
String pass = "robot1234";

//BOT TOKEN
String token = "1491692543:AAGB01jbJXx6U-7ewzaZGD1uKjrMA6rg_zw";

//SENSOR
bool cekCahaya;
bool cekHujan;
void cekSensor(int kondisi = 0) ;
void setup() {
  pinMode(cahayaPIN, INPUT);
  pinMode(hujanPIN, INPUT);

  Serial.begin(9600);
  Serial.println("\nSTART TELEGRAM -- CONECT WIFI");
  dht.begin();
  myBot.wifiConnect(ssid, pass); //WIFI CONECT
  myBot.setTelegramToken(token); //TOKEN SETT

  //TEST KONEKSI
  if (myBot.testConnection())
    Serial.println("\nSIGNAL OK");
  else
    Serial.println("\nERROR NO SIGNAL");

  firstMessage();

}

void loop() {
//  cekSensor(1);
//  Serial.println();
//  delay(2000);
  
//    float t=dht.readTemperature();
//    float h=dht.readHumidity();
//  
//    Serial.println(t);
//    Serial.println(h);
//    delay(2000);
  TBMessage msg;
  if (myBot.getNewMessage(msg)) {

    if (msg.text.equalsIgnoreCase("/help")) help();
    else if (msg.text.equalsIgnoreCase("/sensor")) cekSensor();
    else if (msg.text.equalsIgnoreCase("/condition")) cekSensor(1);
    else {
      Serial.println(msg.text);
    }
  }
  delay(500);
  //Serial.println(".");
}
void kirim(String pesan) {
  myBot.sendMessage(chatID, pesan);
}
void firstMessage() {
  String pesan;
  pesan = "**Welcome to Telegram Monitoring BOT** \n\n /help  -- for help \n /sensor -- check sensor only \n /condition -- condition + sensor \n";

  kirim(pesan);
}
void help() {
  String pesan;
  pesan = "COMMAND AVAILABLE: \n\n";
  pesan += "/help -- help command \n";
  pesan += "/sensor -- check sensor only\n";
  pesan += "/condition -- condition + sensor\n";
  kirim(pesan);
}
String dataCahaya() {
  String stts;
  bool value = !(digitalRead(cahayaPIN));
  if (value) {
    stts = "CAHAYA TERANG";
  } else {
    stts = "CAHAYA GELAP";
  }
  return stts;
}
String dataHujan() {
  String stts;
  bool value = !(digitalRead(hujanPIN));
  if (value) {
    stts = "HUJAN";
  } else {
    stts = "TIDAK HUJAN";
  }
  return stts;
}
String condition() {
  float t = dht.readTemperature();
  //float h=dht.readHumidity();
  bool cahaya = !digitalRead(cahayaPIN);
  bool hujan = !digitalRead(hujanPIN);

  if (t >= 35 && t <= 38 && cahaya == 1 && hujan == 1) return "PANAS HUJAN";
  else if (t >= 30 && t <= 35 && cahaya == 0 && hujan == 1) return "SEJUK HUJAN";
  else if (t >= 35 && t <= 38 && cahaya == 0 && hujan == 0) return "PANAS BERAWAN";
  else if (t >= 35 && t <= 38 && cahaya == 1 && hujan == 0) return "PANAS TERANG";
  else if (t >= 30 && t <= 35 && cahaya == 0 && hujan == 0) return "SEJUK BERAWAN";
  else if (t >= 30 && t <= 35 && cahaya == 1 && hujan == 0) return "SEJUK TERANG";
  else {
    return "WASPADA";
  };

}

void cekSensor(int kondisi) {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  String pesan;
  if (kondisi == 1) {
    pesan = "Status Cuaca : " + condition();
  }
  pesan += "\n ________________________";
  pesan += "\n Sensor Cahaya : " + dataCahaya();
  pesan += "\n Sensor Hujan  : " + dataHujan();
  pesan += "\n Sensor Suhu   : " + String(t) + " °C ";
  pesan += "\n Sensor Kelembapan   : " + String(h);

  Serial.println(pesan);
  kirim(pesan);
}
