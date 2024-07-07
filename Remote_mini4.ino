// This example code is in the Public Domain (or CC0 licensed, at your option.)
// By Evandro Copercini - 2018
//
// This example creates a bridge between Serial and Classical Bluetooth (SPP)
// and also demonstrate that SerialBT have the same functionalities of a normal Serial
// Note: Pairing is authenticated automatically by this device

#include "BluetoothSerial.h"
#include <Arduino.h>
//#include <analogWrite.h>

String device_name = "ESP32-BT-Slave";
int IN1 = 19; 
int IN2 =21; 
int PWMCH1 = 0;
int PWMCH2 = 1;
int PWM_VALUE = 90; 
int STATUS = 1; 

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
//  ledcSetup(PWMCH1, 490, 8);
//  ledcSetup(PWMCH2, 490, 8);
//  ledcAttachPin(IN1, PWMCH1);
//  ledcAttachPin(IN2, PWMCH2);
  ledcAttach(IN1, 490, 8);
  ledcAttach(IN2, 490, 8);
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    //Serial.write(SerialBT.read());
  
    String inputString = SerialBT.readStringUntil(0x0d);
    Serial.println(inputString); 
    if(inputString == "invert"){
      if(STATUS == 1) STATUS = 2;
      else STATUS = 1;
 
      Serial.println("STATUS changed");   
    }
    else{
    // 入力されたデータを整数に変換
    PWM_VALUE = inputString.toInt();
    int div_value = PWM_VALUE % 255;
    // 変換された数字をシリアルモニターに表示
    Serial.print("PWM_VALUE: ");
    Serial.println(PWM_VALUE);
    Serial.print("div_VALUE: ");
    Serial.println(div_value);
    
    }
  
  }
  if (STATUS == 1){
//    analogWrite(IN1,PWM_VALUE); 
//    analogWrite(IN2,0); 
      ledcWrite(IN1, PWM_VALUE);
      ledcWrite(IN2, 0);  
  
  }
  if (STATUS == 2){
//    analogWrite(IN1,0); 
//    analogWrite(IN2,PWM_VALUE);
    ledcWrite(IN2, PWM_VALUE);
    ledcWrite(IN1, 0);
  }
    
  delay(20);
}
