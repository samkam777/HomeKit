/*
 * wifi_info.h
 *
 *  Created on: 2023-10-18 (2023-10-18)
 *      Author: Mixiaoxiao (huang.guanxin)
 */

#ifndef WIFI_INFO_H_
#define WIFI_INFO_H_

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

const char *ssid = "509";//填写自己的WIFI名
const char *password = "dg88328811";//WIFI密码

// const char *ssid = "Xiaomi_16F1";//填写自己的WIFI名
// const char *password = "11111111";//WIFI密码

void wifi_connect() {
	WiFi.persistent(false);
	WiFi.mode(WIFI_STA);
	WiFi.setAutoReconnect(true);
	WiFi.begin(ssid, password);
	Serial.println("WiFi connecting...");
	while (!WiFi.isConnected()) {
		delay(100);
		Serial.print(".");
	}
	Serial.print("\n");
	Serial.printf("WiFi connected, IP: %s\n", WiFi.localIP().toString().c_str());
}

#endif /* WIFI_INFO_H_ */
