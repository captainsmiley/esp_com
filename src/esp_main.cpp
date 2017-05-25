#include <ESP8266WiFi.h>
//#include <Arduino.h>
#include <TickerScheduler.h>
#include "tgesp.h"

	tgesp esp;
TickerScheduler ts(4);





void handleRoot() {
//	server.send(200, "text/html", "<h1>You are connected</h1>");
}

void send_udp()
{
	//Serial.println("Send udp");
    	esp.send_udp();
}
void listen_for_clients()
{
	esp.listen_for_clients();
}

void readSerial()
{
	//Serial.println("read serial");
    esp.readSerial();
}

void read_udp()
{
	esp.read_udp();
}


void setup() {

	Serial.begin(115200);
	Serial.println("\nStart of esp program");
	//esp.scanWifi();
	bool connected = false;
	//connected = esp.connectToWifi("TP-LINK_28D578","50044801");
	if(!connected)
	{
	//connected = esp.connectToWifi("HackNet1","scaniahack");
	}
	if(!connected)
	{
		esp.createAP();
	}

	esp.setup();
	ts.add(2,60,listen_for_clients);
	ts.add(1,5,readSerial);
	ts.add(0, 20, send_udp);
	ts.add(3,10,read_udp);

}

void loop() {
	//static int b = 0;

    //esp.readSerial();
	//Serial.print("main loop esp");
    //esp.readSerial();
    ts.update();

}
