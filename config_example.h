/*
 * Define your network and preferred currencies here.
 * You can define two currencies.
 */

const char* essid = "MyWiFi";                 // WiFi network name
const char* wifiKey = "WiFiPass";             // WiFi WEP/WPA key (password)
char* coinName1 = "BTC";                      // Display name for upper coin
char* tickerUrl1 = "/api/v2/ticker/btceur/";  // Bitstamp URL for upper coin, relative to https://www.bitstamp.net
char* coinName2 = "ETH";                      // Display name for lower coin
char* tickerUrl2 = "/api/v2/ticker/etheur/";  // Bitstamp URL for lower coin, relative to https://www.bitstamp.net
