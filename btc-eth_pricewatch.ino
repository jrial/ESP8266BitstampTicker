/*
 *  Fetch the latest BTC/ETH price from Bitstamp and display them on a 96x64px
 *  SSD1331 OLED display.
 *  It depends on the SSD_13XX library by sumotoy, a faster alternative to the
 *  Adafruit
 *  Download link: https://github.com/sumotoy/SSD_13XX
 */
#include <SPI.h>
#include <SSD_13XX.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define __CS  4   //GPIO4 (D2) or GPIO2
#define __DC  5   //(D1)
//SCLK, CLK:connect to D5
//MOSI, RES:connect to D7


/*
 * Define your network and preferred currencies here.
 * You can define two currencies.
 */
const char* essid = "WIFI_NAME";
const char* wifiKey = "WIFI_PASS";
char* coinName1 = "BTC";
char* tickerUrl1 = "/api/v2/ticker/btceur/";
char* coinName2 = "ETH";
char* tickerUrl2 = "/api/v2/ticker/etheur/";

SSD_13XX tft = SSD_13XX(__CS, __DC);

const char* host = "www.bitstamp.net";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "D0 26 AB 06 64 07 BC 88 56 6D 83 BE 0A 29 00 B5 10 E5 27 D2";
WiFiClientSecure client;

String dataLine1 = "{\"high\": \"0000.00\", \"last\": \"0000.00\", \"low\": \"0000.00\"}";
String dataLine2 = "{\"high\": \"0000.00\", \"last\": \"0000.00\", \"low\": \"0000.00\"}";

void setup() {
  Serial.begin(115200);
  tft.begin();
  tft.clearScreen();
  tft.setCursor(0, 0);
  tft.println("Connecting");
  Serial.println("Connecting...");
  //WiFi.mode(WIFI_STA);
  WiFi.begin(essid, wifiKey);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  tft.print(WiFi.localIP());
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  bool fresh1, fresh2;
  if (fresh1 = connectToHost()) {
    dataLine1 = fetchUrl(tickerUrl1);
  }
  if (fresh2 = connectToHost()) {
    dataLine2 = fetchUrl(tickerUrl2);
  }

  // Switch screen every 10 seconds, 6 times. This means 1 refresh
  // of the data pe minute. We do 2 requests, and Bitstamp has a
  // limit of 600 requests per 10 minutes before it blocks your IP.
  for (int i = 0; i < 6; i++) {
    tft.clearScreen();
    if (dataLine1.startsWith("{")) {
      printPriceData(fresh1, 0, 0, coinName1, dataLine1, i % 2);
    }
    if (dataLine2.startsWith("{")) {
      printPriceData(fresh2, 0, 1, coinName2, dataLine2, i % 2);
    }
    delay(10000);
  }
}

bool connectToHost() {
  /*
   * Connect to the host over HTTPS. Also verify the certificate's
   * fingerprint. Return true if the connection succeeds.
   */
  Serial.print("Connecting to Bitstamp   ");
  bool connection = client.connect(host, httpsPort);
  if (connection) {
    Serial.println("OK");
  } else {
    Serial.println("FAIL");
  }
  Serial.print("Verifying fingerprint    ");
  connection = connection && client.verify(fingerprint, host);
  if (connection) {
    Serial.println("OK");
  } else {
    Serial.println("FAIL");
  }
  return connection;
}

void printPriceData(bool fresh, int x, int y, char* unit, String line, int stage) {
  /*
   * Print the current price, and some extra data on the second row.
   * Second row alternates between (low, high) and (total_diff, diff_since_open)
   * Total diff is the percentage difference between the daily low and high.
   * Diff since open is the percentage difference between the opening price and
   * the current one.
   */
  tft.setCursor(x, y * 32);
  tft.setTextColor(MAGENTA);
  tft.print(unit);
  tft.print(": ");

  int highPos = line.indexOf("\"high\":") + 9;
  int highEndPos = line.indexOf("\"", highPos);
  String high = line.substring(highPos, highEndPos);

  int lowPos = line.indexOf("\"low\":") + 8;
  int lowEndPos = line.indexOf("\"", lowPos);
  String low = line.substring(lowPos, lowEndPos);

  int lastPos = line.indexOf("\"last\":") + 9;
  int lastEndPos = line.indexOf("\"", lastPos);
  String last = line.substring(lastPos, lastEndPos);

  int openPos = line.indexOf("\"open\":") + 9;
  int openEndPos = line.indexOf("\"", openPos);
  String open = line.substring(openPos, openEndPos);

  // See if the price went up or down so we can color the price accordingly.
  float last_f = last.toFloat();
  float open_f = open.toFloat();
  float pct_since_open = 100.0 * (last_f - open_f) / open_f;

  // First print the current price.
  tft.setCursor(48, y * 32);
  if (fresh) {
    if (pct_since_open >= 0.1) {
      tft.setTextColor(GREEN);
    } else if (pct_since_open <= -0.1) {
      tft.setTextColor(RED);
    } else {
      tft.setTextColor(WHITE);
    }
  } else {
    tft.setTextColor(YELLOW);
  }
  tft.print(last);

  // Pick the correct data to display on the 2nd line.
  if (stage == 0) {
    if (fresh) {
      tft.setTextColor(WHITE);
    } else {
      tft.setTextColor(YELLOW);
    }
    tft.setCursor(0, y * 32 + 16);
    tft.print(low);
    tft.setCursor(48, y * 32 + 16);
    tft.print(high);
  } else if (stage == 1) {
    float high_f = high.toFloat();
    float low_f = low.toFloat();
    float low_high_spread = 100.0 * (high_f - low_f) / low_f;

    tft.setTextColor(WHITE);
    tft.setCursor(0, y * 32 + 16);
    tft.print(low_high_spread, 2);
    tft.print("%%");

    tft.setCursor(48, y * 32 + 16);
    if (pct_since_open >= 01) {
      tft.setTextColor(GREEN);
      tft.print("+");
    } else if (pct_since_open <= -0.1){
      tft.setTextColor(RED);
      tft.print("-");
    } else {
      tft.setTextColor(GREEN);
      if (pct_since_open >=0) {
        tft.print("+");
      } else {
        tft.print("-");
      }
    }
    tft.print(abs(pct_since_open), 2);
    tft.print("%%");
  }
}

String fetchUrl(char* url) {
  /*
   * Fetch a URL and return the data. In our case,
   * everything comes on one line, which saves us
   * some headaches.
   */
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266 Bitstamp Ticker <https://github.com/jrial/ESP8266BitstampTicker>\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println(String("Fetched https://") + host + "/" + url);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  Serial.println("Received data:");
  Serial.println(line);
  return line;
}
