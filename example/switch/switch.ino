/*
 * switch.ino
 *
 *  Created on: 2020-05-15 (2023-10-18)
 *      Author: Mixiaoxiao (modified by huang.guanxin)
 *
 * HAP section 8.38 Switch
 * An accessory contains a switch.
 *
 * This example shows how to:
 * 1. define a switch accessory and its characteristics (in my_accessory.c).
 * 2. get the switch-event sent from iOS Home APP.
 * 3. report the switch value to HomeKit.
 *
 * You should:
 * 1. read and use the Example01_TemperatureSensor with detailed comments
 *    to know the basic concept and usage of this library before other examples。
 * 2. erase the full flash or call homekit_storage_reset() in setup()
 *    to remove the previous HomeKit pairing storage and
 *    enable the pairing with the new accessory of this new HomeKit example.
 */

#include <Arduino.h>
#include <arduino_homekit_server.h>
#include "wifi_info.h"

#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

// access your HomeKit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_switch_on;
extern "C" homekit_characteristic_t cha_switch_off;


void setup() {
	Serial.begin(9600);
	wifi_connect(); // in wifi_info.h
	//homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
	my_homekit_setup();
  
}

void loop() {
	my_homekit_loop();
  // Serial.println(cha_switch_on.value.bool_value);
	delay(10);
}

//==============================
// HomeKit setup and loop
//==============================



static uint32_t next_heap_millis = 0;

#define PIN_SWITCH_ON 5//硬件IO端口
#define PIN_SWITCH_OFF 13//硬件IO端口
// #define PIN_SWITCH_STOP 14//硬件IO端口

//Called when the switch value is changed by iOS Home APP
void cha_switch_on_setter(const homekit_value_t value) {
	bool on = value.bool_value;
	cha_switch_on.value.bool_value = on;	//sync the value

  // 假设OFF的开关处于点亮true状态，同时点亮了ON开关，则将OFF开关置为false状态
  if(cha_switch_off.value.bool_value == true){
    cha_switch_off.value.bool_value = false;
  }
	LOG_D("Switch ON: %s", on ? "ON" : "OFF");
	digitalWrite(PIN_SWITCH_ON, on ? HIGH : LOW);
  // delay(1000);
  // digitalWrite(PIN_SWITCH_ON, on ? LOW : HIGH);
}

void cha_switch_off_setter(const homekit_value_t value) {
	bool off = value.bool_value;
	cha_switch_on.value.bool_value = off;	//sync the value

  if(cha_switch_on.value.bool_value == true){
    cha_switch_on.value.bool_value = false;
  }
	LOG_D("Switch OFF: %s", off ? "ON" : "OFF");
	digitalWrite(PIN_SWITCH_OFF, off ? HIGH : LOW);
}

void my_homekit_setup() {
	pinMode(PIN_SWITCH_ON, OUTPUT);
  pinMode(PIN_SWITCH_OFF, OUTPUT);
  // pinMode(PIN_SWITCH_STOP, OUTPUT);
	digitalWrite(PIN_SWITCH_ON, LOW);
  digitalWrite(PIN_SWITCH_OFF, LOW);
  // digitalWrite(PIN_SWITCH_STOP, LOW);

	//Add the .setter function to get the switch-event sent from iOS Home APP.
	//The .setter should be added before arduino_homekit_setup.
	//HomeKit sever uses the .setter_ex internally, see homekit_accessories_init function.
	//Maybe this is a legacy design issue in the original esp-homekit library,
	//and I have no reason to modify this "feature".
	cha_switch_on.setter = cha_switch_on_setter;
  cha_switch_off.setter = cha_switch_off_setter;
	arduino_homekit_setup(&config);

	//report the switch value to HomeKit if it is changed (e.g. by a physical button)
	//bool switch_is_on = true/false;
	//cha_switch_on.value.bool_value = switch_is_on;
	//homekit_characteristic_notify(&cha_switch_on, cha_switch_on.value);
}

void my_homekit_loop() {
	arduino_homekit_loop();
	const uint32_t t = millis();
	if (t > next_heap_millis) {
		// show heap info every 5 seconds
		next_heap_millis = t + 5 * 1000;
		// LOG_D("Free heap: %d, HomeKit clients: %d",
		// 		ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

	}
}
