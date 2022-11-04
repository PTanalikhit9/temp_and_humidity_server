#include <EtherCard.h>
#include <SPI.h>
#include <SD.h>

#include "DHT.h"

DHT dht;
File myFile;
//DHT dht2; // for more sensor

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

float temp = 0;
float humid = 0;
int temperature = 0;
int humidity = 0;
int temp_first_digit = 0;
int humid_first_digit = 0;
int temp_second_digit = 0;
int humid_second_digit = 0;
int Timer = 0;

/////////////////////////////////////////////////////////////////////////////////////////////


void setup () {
  // Change 'SS' to your Slave Select pin, if you arn't using the default pin
  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  ether.staticSetup(myip);
  dht.setup(4); // data pin 2
  //dht2.setup(2); // data pin 2 for more sensor
}


// GET TEMP AND HUMID ////////////////////////////////////////////////////////////////////////

//  Here we get values from the DHT11

static void ReadDHT22()
{
  humid = dht.getHumidity();
  temp = dht.getTemperature();
  temperature = (temp*100.0)/100;
  humidity = (humid*100.0)/100;
  int T = temp*100;
  int H = humid*100;
  int T1 = temp*10;
  int H1 = humid*10;
  temp_first_digit = T1%10;
  humid_first_digit = H1%10;
  temp_second_digit = T%10;
  humid_second_digit = H%10;
  
}

/////////////////////////////////////////////////////////////////////////////////////////////


//create/design the webpage

static word homePage() {
  

  bfill = ether.tcpOffset();
  bfill.emit_p(PSTR(
  "HTTP/1.0 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Pragma: no-cache\r\n"
    "\r\n"
    "<meta http-equiv='refresh' content='30'/>"
    "<title>Temperature and Humidity Server</title>" // server name
    "<h1>Temperature: $D.$D$D degree celsius <br> Humidity: $D.$D$D %</h1>"), 
      temperature, temp_first_digit, temp_second_digit, humidity, humid_first_digit, humid_second_digit);
  return bfill.position();
  
}

void loop () {
  //////////////////////////////////////////////////////////////////////////////////////////
  Timer = Timer + 1;
  
  if(Timer == 1)
  {
    ReadDHT22();
   }
  
  if(Timer == 10) // initially 1100
  {
    Timer = 0;
  }
  //////////////////////////////////////////////////////////////////////////////////////////
    
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if (pos)  // check if valid tcp data is received
  {
    ReadDHT22();
    ether.httpServerReply(homePage()); // send web page data
  }
}
