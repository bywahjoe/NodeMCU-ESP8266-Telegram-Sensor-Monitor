#include "DHT.h"

#define cahayaPIN D7
#define hujanPIN D6
#define DHTPIN D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void cekSensor(int kondisi = 0) ;

void setup() {
  pinMode(cahayaPIN, INPUT);
  pinMode(hujanPIN, INPUT);

  Serial.begin(9600);
  dht.begin();

}
void loop() {
  cekSensor(1);
  Serial.println();
  delay(2000);
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
}
