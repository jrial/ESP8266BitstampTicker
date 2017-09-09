# ESP8266BitstampTicker
Bitstamp ticker with ESP8266 and SSD1331 OLED 96x64 display

Displays BTC/ETH price info for the last 24h, taken from Bitstamp. The coins
and fiat currency denomination are configurable by changing the variables
coinName1/2 and tickerUrl1/2.

See the [Bitstamp API page](https://www.bitstamp.net/api/) for possible values.

The data is refreshed every minute, and the display alternates between the
following two displays every 10 seconds.

## Hardware

The hardware consists of an ESP8266, connected to a SSD-1331 OLED display. The
connections are as follows:

ESP8266  |  SSD1331  |  Notes
---      |  ---      |  ---
GND      |  GND      |
3V3      |  VCC      |
D5       |  SCL      |
D7       |  SDA      |
3V3      |  RES      |  Connects through a resistor in the 4k7 to 10k range.
D1       |  DC       |
D2       |  CS       |

## Software

Apart from this sketch, you'll also need sumotoy's [SSD_13XX library](https://github.com/sumotoy/SSD_13XX).

At the top of the sketch, there are some configuration variables that can be
set:

Key          |  Purpose
---          |  ---
essid        |  WiFi network name.
wifiKey      |  WiFi Password (WEP/WPA).
coinName1    |  Display name of the first cryptocoin.
tickerUrl1   |  Ticker URL for the first cryptocoin. Determines both coin and fiat currency.
coinName2    |  Display name of the second cryptocoin.
tickerUrl2   |  Ticker URL for the second cryptocoin. Determines both coin and fiat currency.
