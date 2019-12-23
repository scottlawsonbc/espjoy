#ifndef BLE_HID_H
#define BLE_HID_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEHIDDevice.h>
#include <BLE2902.h>

static BLEHIDDevice* hid;
BLECharacteristic* input;
bool connected;

class BLECallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer){
    connected = true;
  }

  void onDisconnect(BLEServer* pServer){
    connected = false;
  }
};

void taskBLE(void*) {
  BLEDevice::init("espjoy");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new BLECallbacks());
  hid = new BLEHIDDevice(pServer);
  input = hid->inputReport(1); // <-- input REPORTID from report map
  hid->manufacturer()->setValue("Scott Lawson");
  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->hidInfo(0x00,0x01);

  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setKeySize();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

  const uint8_t reportMap[] = {
    0x05, 0x01,                   // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                   // USAGE (Game Pad)
    0xa1, 0x01,                   // COLLECTION (Application)
    
    0xa1, 0x02,                   //    COLLECTION (Logical)
    0x85, 0x01,                   //      REPORT_ID (1)

    0x75, 0x08,                   //      REPORT_SIZE (8)
    0x95, 0x03,                   //      REPORT_COUNT (3)
    0x05, 0x01,                   //      USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                   //      USAGE (X)
    0x09, 0x31,                   //      USAGE (Y)
    0x09, 0x33,                   //      USAGE (Rx)
    0x15, 0x81,                   //      LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                   //      LOGICAL_MAXIMUM (127)
    0x81, 0x02,                   //      INPUT (Data, Var, Abs)
    
    0x75, 0x01,                   //      REPORT_SIZE (1)
    0x95, 0x05,                   //      REPORT_COUNT (5)
    0x15, 0x00,                   //      LOGICAL_MINIMUM (0)
    0x25, 0x01,                   //      LOGICAL_MAXIMUM (1)
    0x05, 0x09,                   //      USAGE_PAGE (Button)
    0x19, 0x01,                   //      USAGE_MINIMUM (Button 1)
    0x29, 0x05,                   //      USAGE_MAXIMUM (Button 5)
    0x81, 0x02,                   //      INPUT (Data, Var, Abs)
    // PADDING for byte alignment
    0x75, 0x01,                   //      REPORT_SIZE (1)
    0x95, 0x03,                   //      REPORT_COUNT (3)
    0x81, 0x03,                   //      INPUT (Constant, Var, Abs)
    
    0xc0,                         //   END_COLLECTION
    0xc0                          // END_COLLECTION
  };

  hid->reportMap((uint8_t*)reportMap, sizeof(reportMap));
  hid->startServices();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->setAppearance(HID_GAMEPAD);
  pAdvertising->addServiceUUID(hid->hidService()->getUUID());
  pAdvertising->start();

  hid->setBatteryLevel(5);
  delay(portMAX_DELAY);
}

void initBLE() {
  xTaskCreate(taskBLE, "server", 20000, NULL, 5, NULL);
}

#endif