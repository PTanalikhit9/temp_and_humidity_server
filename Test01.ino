// This is a demo of the RBBB running as webserver with the EtherCard
// 2010-05-28 <jc@wippler.nl>
//
// License: GPLv2

#include <EtherCard.h>

#include "DHT.h"
#define DHTPIN 2  
#define DHTTYPE DHT11 // actually it should be DHT 22, but the program may have a problem
DHT dht(DHTPIN, DHTTYPE);


// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[] = { 192,168,0,19 };

byte Ethernet::buffer[500];
BufferFiller bfill;

// GLOBAL VARIABLES ///////////////////////////////////////////////////////////////////////////

// Global Variables Defined
// This is so we can see them in all functions
// temp is the temperature, humid is the humidity
// Timer is used to reduce the frequency of reading the DHT11 - I found reading it every loop
// caused timing issues with the Ethernet.

int temp = 0;
int humid = 0;
int Timer = 0;

/////////////////////////////////////////////////////////////////////////////////////////////


void setup () {
  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  ether.staticSetup(myip);
  dht.begin();
}


// GET TEMP AND HUMID ////////////////////////////////////////////////////////////////////////

//  Here we get values from the DHT11

static void ReadDHT11()
{
  temp = dht.readTemperature(); // sometimes, it uses dht.getTemperature
  humid = dht.readHumidity();
}

/////////////////////////////////////////////////////////////////////////////////////////////


//create/design the webpage

static word homePage() {
  
//   long t = millis() / 1000;
//   word h = t / 3600;
//   byte m = (t / 60) % 60;
//   byte s = t % 60;
//   bfill = ether.tcpOffset();
//   bfill.emit_p(PSTR(
//     "HTTP/1.0 200 OK\r\n"
//     "Content-Type: text/html\r\n"
//     "Pragma: no-cache\r\n"
//     "\r\n"
//     "<meta http-equiv='refresh' content='1'/>"
//     "<title>RBBB server</title>"
//     "<h1>$D$D:$D$D:$D$D</h1>"),
//       h/10, h%10, m/10, m%10, s/10, s%10);
//   return bfill.position();
  long t = millis() / 1000;
  word h = t / 3600;
  byte m = (t / 60) % 60;
  byte s = t % 60;
  
  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
  "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='30'/>"
    "<title>Temperature and Humidity Server</title>" // server name
    "<h1>Temperature: $D degree celsius <br>Humidity: $D %</h1>"),
  temp, humid);
    "<h1>$D$D:$D$D:$D$D</h1>"),
      h/10, h%10, m/10, m%10, s/10, s%10);
  return bfill.position();
  
}

void loop () {
  
  //////////////////////////////////////////////////////////////////////////////////////////
  Timer = Timer + 1;
  
  if(Timer == 1)
  {
    ReadDHT11();
   }
  
  // What about the other values? How these conditions work?
  
  if(Timer == 1100)
  {
    Timer=0;
  }
  //////////////////////////////////////////////////////////////////////////////////////////
    
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if (pos)  // check if valid tcp data is received
  {
    ReadDHT11();
    ether.httpServerReply(homePage()); // send web page data
  }
}
