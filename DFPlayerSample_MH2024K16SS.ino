// this example will play a track and then 
// every five seconds play another track
//
// it expects the sd card to contain these three mp3 files
// but doesn't care whats in them
//
// sd:/mp3/0001.mp3
// sd:/mp3/0002.mp3
// sd:/mp3/0003.mp3

#define _DEBUG

#include <SoftwareSerial.h>
#include "DFMiniMp3.h"

// forward declare the notify class, just the name
//
class Mp3Notify; 

// define a handy type using serial and our notify class
//
//typedef DFMiniMp3<HardwareSerial, Mp3Notify> DfMp3; 
typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3; 

// instance a DfMp3 object, 
//
SoftwareSerial mySerial(9, 10);
// DfMp3 dfmp3(Serial1);
DfMp3 dfmp3(mySerial);

// Some arduino boards only have one hardware serial port, so a software serial port is needed instead.
// comment out the above definitions and use these
//SoftwareSerial secondarySerial(10, 11); // RX, TX
//typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;
// DfMp3 dfmp3(secondarySerial);

// implement a notification class,
// its member methods will get called 
//
class Mp3Notify
{
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action)
  {
    if (source & DfMp3_PlaySources_Sd) 
    {
        Serial.print("SD Card, ");
    }
    if (source & DfMp3_PlaySources_Usb) 
    {
        Serial.print("USB Disk, ");
    }
    if (source & DfMp3_PlaySources_Flash) 
    {
        Serial.print("Flash, ");
    }
    Serial.println(action);
  }
  static void OnError(DfMp3& mp3, uint16_t errorCode)
  {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    switch (errorCode)
    {
    case DfMp3_Error_Busy:
      Serial.println("Busy");
      break;
    case DfMp3_Error_Sleeping:
      Serial.println("Sleeping");
      break;
    case DfMp3_Error_SerialWrongStack:
      Serial.println("Serial Wrong Stack");
      break;

    case DfMp3_Error_RxTimeout:
      Serial.println("Rx Timeout!!!");
      break;
    case DfMp3_Error_PacketSize:
      Serial.println("Wrong Packet Size!!!");
      break;
    case DfMp3_Error_PacketHeader:
      Serial.println("Wrong Packet Header!!!");
      break;
    case DfMp3_Error_PacketChecksum:
      Serial.println("Wrong Packet Checksum!!!");
      break;

    default:
      Serial.println(errorCode, HEX);
      break;
    }
  }
  static void OnPlayFinished(DfMp3& mp3, DfMp3_PlaySources source, uint16_t track)
  {
    Serial.print("Play finished for #");
    Serial.println(track);

    Serial.println("Play #1");
    mp3.playGlobalTrack(0);
  }
  static void OnPlaySourceOnline(DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "online");
  }
  static void OnPlaySourceInserted(DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "inserted");
  }
  static void OnPlaySourceRemoved(DfMp3& mp3, DfMp3_PlaySources source)
  {
    PrintlnSourceAction(source, "removed");
  }
};

void setup() 
{
  Serial.begin(115200);

  delay(3000);

  Serial.println("Initializing...");
  dfmp3.begin(9600, 1000);

  Serial.println("RESET");
  dfmp3.reset();

  // Serial.println("TRACK COUNT");
  // uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  // Serial.print("files ");
  // Serial.println(count);

  Serial.println("Set volume 15"); 
  dfmp3.setVolume(15);
  delay(100);

  Serial.println("First play!"); 
  dfmp3.playGlobalTrack(1);
  delay(100);

  Serial.println("Initialized.");
  Serial.println();
}

void waitMilliseconds(uint16_t msWait)
{
  uint32_t start = millis();
  
  while ((millis() - start) < msWait)
  {
    // if you have loops with delays, its important to 
    // call dfmp3.loop() periodically so it allows for notifications 
    // to be handled without interrupts
    dfmp3.loop(); 
    delay(1);
  }
}

void loop() 
{
  waitMilliseconds(1000);
}