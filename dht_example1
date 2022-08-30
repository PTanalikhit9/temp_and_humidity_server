// DHT code from:
// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
// #define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);

// LCD
// LCD RS=3, EN=4, DS4=5, DS5=6, DS6=7, DS7=8
#include <LiquidCrystal.h>
LiquidCrystal lcd( 3, 4, 5, 6, 7, 8);

// EtherCard code from the RBBB Example
// This is a demo of the RBBB running as webserver with the Ether Card
// 2010-05-28 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[] = { 192,168,51,177 };
static byte gwip[] = { 192,168,51,1 };
byte Ethernet::buffer[500];
BufferFiller bfill;

// Global Variables Defined
// This is so we can see them in all functions
// t is the temperature, h is the humidity
// Timer is used to reduce the frequency of reading the DHT11 - I found reading it every loop
// caused timing issues with the Ethernet.
// The Animation loop is to provide some bling to the LCD diaplay

int t=0;
int h=0;
int Timer= 0;
int Animation = 0;

// Setup - bring Ethernet, LCD and DHT11 live
void setup () {
  if (ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0)
    Serial.println( "Failed to access Ethernet controller");
  ether.staticSetup(myip, gwip);
  lcd.begin(16,2);
  dht.begin();
}

//  Here we get values from the DHT11
static void ReadDHT11()
{
  h = dht.readHumidity();
  t = dht.readTemperature();
}

//  Here we build a web page and pass the t and h values into it
static word homePage() {
  //ReadDHT11();

  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
  "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='30'/>"
    "<title>Temp server</title>" 
    "<h1>Temp: $D C <br>Humidity: $D %</h1>"),
  t, h);
  return bfill.position();
}

// The main loop
//  We add to the timer and read the DHT11 values when Timer == 1, otherwise we loop without reading it
// Once the Timer value == to 1100 we zero it and start the count again.  This way the sensors are only
// read once every 1100 loops.  Every time we go through the loop we listen for a request on the Ethernet
//  If we see one we send the webpage.  We also add an animation on the LCD so we know the loop is working.

void loop () {

  Timer  = Timer+1;
  if(Timer==1)
  {
    ReadDHT11();
    // set the cursor to (0,0):
    lcd.setCursor(0, 0);
    // print from 0 to 9:

    lcd.print("Temp: ");
    lcd.print(t);
    lcd.print("C");
    // set the cursor to (16,1):
    lcd.setCursor(0,1);
    lcd.print("Humidity: ");
    lcd.print(h);
    lcd.print("%");

    if(Animation==0)
    {
      lcd.print("  *");
    }
    if(Animation==1)
    {
      lcd.print("  +");
    }
    Animation = Animation + 1;
    if(Animation==2)
    {
      Animation=0;
    }
  }
  if(Timer==1100)
  {
    Timer=0;
  }

  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if (pos)  // check if valid tcp data is received
  {
    ReadDHT11();
    ether.httpServerReply(homePage()); // send web page data
  }
}
