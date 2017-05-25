/*
 * tgesp.h
 *
 *  Created on: Sep 12, 2016
 *      Author: captainsmiley
 */

#ifndef SRC_TGESP_H_
#define SRC_TGESP_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define SERIAL_BUFF_READ_SIZE 1000

#include "commands.h"
//class Commands;

class tgesp {
public:
	tgesp();
	void scanWifi();
	void setup();
	void stop();
	void createAP();
	void connectToWifi();
	bool connectToWifi(const char * ssid, const char * pass);
    void read_client(WiFiClient & client);
    void listen_for_clients();
    void handle_command(const char *);
    void handle_get(String &);
    void readSerial();
    void send_udp();
    void read_udp();
    void handle_http_request(String &);
    const char * cmp_input(const char * input, const char * cmp);
	virtual ~tgesp();
private:
    void output(const char *);
    void send_response();
    char * ssid;
    char * pass;
    static char serial_buff[];
    static size_t serial_buff_pos;
    WiFiServer server;
    WiFiUDP udp;
    bool client_connected;
    WiFiClient connected_client;
    String response_content;
    Commands cmds;
    bool serial_dbg;
    static char packetBuffer[];
};

#endif /* SRC_TGESP_H_ */
