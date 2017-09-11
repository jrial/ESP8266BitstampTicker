# ESP8266BitstampTicker
Bitstamp ticker with ESP8266 and SSD1331 OLED 96x64 display

Displays BTC/ETH price info for the last 24h, taken from Bitstamp. The coins
and fiat currency denomination are configurable by changing the variables
coinName1/2 and tickerUrl1/2.

See the [Bitstamp API page](https://www.bitstamp.net/api/) for possible values.

The data is refreshed every minute, and the display alternates between the
following two displays every 10 seconds.

Display 1:
---

Left                    |  Right
---                     |  ---
Name of Coin 1          |  Current price of Coin 1
Last 24h low of Coin 1  |  Last 24h high of Coin 1
Name of Coin 2          |  Current price of Coin 2
Last 24h low of Coin 2  |  Last 24h high of Coin 2

Display 2:
---

Left                              |  Right
---                               |  ---
Name of Coin 1                    |  Current price of Coin 1
Difference % high/low for Coin 1  |  Gain/loss % since open for Coin 1
Name of Coin 2                    |  Current price of Coin 2
Difference % high/low for Coin 2  |  Gain/loss % since open for Coin 2

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

If your SSD-1331 display is full-color, the price and % gain/loss since open
will be colour coded. Yellow means stale info: we couldn't fetch data during
last connection attempt. Red is loss, green gain, and white means less than
0.1% change, or "more or less the same".

I don't have a monochrome or dual-colour OLED to test on, but I assume colour
changes on those have no effect.

## Software

Apart from this sketch, you'll also need sumotoy's [SSD_13XX library](https://github.com/sumotoy/SSD_13XX).

Configuring the software is done by copying config_example.h to a file named
config.h and editing the configuration variables within. The configuration
variables are:

Key          |  Purpose
---          |  ---
essid        |  WiFi network name.
wifiKey      |  WiFi Password (WEP/WPA).
coinName1    |  Display name of the first cryptocoin (the one diplayed at the top).
tickerUrl1   |  Ticker URL for the first cryptocoin. Determines both coin and fiat currency.
coinName2    |  Display name of the second cryptocoin (the one displayed at the bottom).
tickerUrl2   |  Ticker URL for the second cryptocoin. Determines both coin and fiat currency.
