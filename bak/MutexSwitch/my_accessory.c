/*
 * my_accessory.c
 * Define the accessory in C language using the Macro in characteristics.h
 *
 *  Created on: 2020-05-15 (2023-10-18)
 *      Author: Mixiaoxiao (modified by huang.guanxin)
 */

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

// 定义按钮事件回调函数
// void my_switch_event_callback(uint8_t switch_event) {
//     if (switch_event == 0 || switch_event == 1) {
//       printf("Button pressed\n");
//         // 按钮被按下
//         // 执行按钮按下时的操作

//         // 设置一个定时器或延迟来自动关闭按钮
//         // ...
//     }
// };

void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}

// Switch (HAP section 8.38)
// required: ON
// optional: NAME

// format: bool; HAP section 9.70; write the .setter function to get the switch-event sent from iOS Home APP.
homekit_characteristic_t cha_switch_on = HOMEKIT_CHARACTERISTIC_(ON, false);
homekit_characteristic_t cha_switch_off = HOMEKIT_CHARACTERISTIC_(ON, false);

// format: string; HAP section 9.62; max length 64
homekit_characteristic_t cha_name_on = HOMEKIT_CHARACTERISTIC_(NAME, "Switch ON");
homekit_characteristic_t cha_name_off = HOMEKIT_CHARACTERISTIC_(NAME, "Switch OFF");

// homekit_characteristic_t cha_switch_event = HOMEKIT_CHARACTERISTIC_PROGRAMMABLE_SWITCH_EVENT(cha_switch_event, my_switch_event_callback);
// homekit_characteristic_t cha_switch_event = HOMEKIT_CHARACTERISTIC_(PROGRAMMABLE_SWITCH_EVENT, 0);

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_switch, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Daojiao Door"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
            HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
    // 第一个参数定义为switch类型，属性来指定是否将其作为主要服务
		HOMEKIT_SERVICE(SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
      // cha button state,
			&cha_switch_on,
			&cha_name_on,
			NULL
		}),
    HOMEKIT_SERVICE(SWITCH, .characteristics=(homekit_characteristic_t*[]){
			&cha_switch_off,
			&cha_name_off,
			NULL
		}),
    // HOMEKIT_SERVICE(STATELESS_PROGRAMMABLE_SWITCH, .primary=true, .characteristics=(homekit_characteristic_t*[]){
		// 	&cha_switch_event,
    //   // &cha_switch_on,
		// 	NULL
		// }),
        NULL
    }),
    NULL
};



homekit_server_config_t config = {
		.accessories = accessories,
		.password = "111-11-111"
};
