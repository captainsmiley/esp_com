

#include "commands.h"
#include <Servo.h>
#include <WiFiUdp.h>
#include "tgesp.h"

#define SERVO_H_POS_ADDR 0x00
#define SERVO_L_POS_ADDR 0x01
#define SERVO_DELAY_ADDR 0x02

Commands::Commands(tgesp* e) :
    servo_h_pos(SERVO_H_POS),
    servo_l_pos(SERVO_L_POS),
	client_ip(192,168,0,105),
	esp(e)
{
  EEPROM.begin(128);
}

Commands::~Commands(){}


void Commands::test_gpio(const char * p)
{
    String p_s(p);
    pinMode(p_s.toInt(),OUTPUT);
    if (p_s.endsWith("l")){

        digitalWrite(p_s.toInt(),LOW);
    }
    if (p_s.endsWith("h")){

        digitalWrite(p_s.toInt(),HIGH);
    }

}

void Commands::set_client_ip(const char *p)
{
	String p_s(p);
	int index1 = p_s.indexOf('.');
	int index2 = p_s.indexOf('.',index1+1);
	int index3 = p_s.indexOf('.',index2+1);
	if (!index1 || !index2 || !index3){
		Serial.println("warning, bad ip");
		return;
	}


	IPAddress ip(
			p_s.substring(0,index1).toInt(),
			p_s.substring(index1+1,index2).toInt(),
			p_s.substring(index2+1,index3).toInt(),
			p_s.substring(index3+1).toInt()
			);
	client_ip = ip;
}

void Commands::wifi_setup(const char *p)
{
	String p_s(p);
	int index1 = p_s.indexOf('&');
	if (!index1){
		Serial.println("warning, bad input");
		return;
	}
	String ap = p_s.substring(0,index1);


	if(ap=="ap")
	{
	esp->createAP();
	return;
	}
	else
	{
		char b1[30];
		char b2[30];
		ap.toCharArray(b1,30,0);
		p_s.substring(index1+1).toCharArray(b2,30,0);
		Serial.print(b1);Serial.println(b2);
		if (!esp->connectToWifi(b1,b2))
		{
			Serial.println("warning, no connection");
			esp->createAP();
		}
	}

}


void Commands::send_udp(const char * ca,size_t n,WiFiUDP & udp)
{
	 udp.beginPacket(client_ip, 11000);
	 udp.write(ca,n);
	 udp.endPacket();
}
void Commands::connect(const char * str)
{
	String p_s(str);
	int index = p_s.indexOf("&");
	if(!index){
		Serial.println("warning bad input");
		return;
	}
}

void Commands::send_serial(const char * str)
{
	Serial.print(str);
}
void Commands::send_params_serial(const char * str)
{
	Serial.print('&');
	Serial.print(str);
	Serial.print('&');
}

void Commands::read_client() {}
void Commands::read_distance() {}
void Commands::http_get() {
Serial.println("runs http_get");
}
Servo Commands::sv = Servo();
void Commands::test_servo(const char * p)
{
    String p_s(p);
    sv.attach(SERVO_PIN);
    sv.write(p_s.toInt());
}

void Commands::servo_h()
{
    sv.attach(SERVO_PIN);
    sv.write(EEPROM.read(SERVO_H_POS_ADDR));
    uint16_t d_time = (EEPROM.read(SERVO_DELAY_ADDR+1) << 8) | EEPROM.read(SERVO_DELAY_ADDR);
    delay(d_time);
//    delay( (EEPROM.read(SERVO_DELAY_ADDR+1) << 8) | EEPROM.read(SERVO_DELAY_ADDR));
    sv.detach();
}
void Commands::set_servo_delay(const char *p)
{
  String p_s(p);
  uint16_t s_delay = uint16_t(p_s.toInt());
  EEPROM.write(SERVO_DELAY_ADDR, uint8_t( s_delay & 0x00FF) );
  EEPROM.write(SERVO_DELAY_ADDR+1, uint8_t( (s_delay & 0xFF00) >> 8 ));
  EEPROM.commit();

}
void Commands::servo_l()
{
    sv.attach(SERVO_PIN);
    sv.write(EEPROM.read(SERVO_L_POS_ADDR));
    uint16_t d_time = (EEPROM.read(SERVO_DELAY_ADDR+1) << 8) | EEPROM.read(SERVO_DELAY_ADDR);
    delay(d_time);
    sv.detach();
}

void Commands::set_servo_h_pos(const char *p)
{
    String p_s(p);
    EEPROM.write(SERVO_H_POS_ADDR,uint8_t(p_s.toInt()));
    EEPROM.commit();
}
void Commands::set_servo_l_pos(const char *p)
{
    String p_s(p);
    EEPROM.write(SERVO_L_POS_ADDR,uint8_t(p_s.toInt()));
    EEPROM.commit();
}
