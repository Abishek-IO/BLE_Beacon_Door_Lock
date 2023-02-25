#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define relay 2

String knownBLEAddresses[] = {"e4:41:22:4d:8d:45", "e4:41:22:9d:78:1e"};
int RSSI_THRESHOLD = -100;
bool device_found;
int scanTime = 1;
BLEScan* pBLEScan;
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      for (int i = 0; i < (sizeof(knownBLEAddresses) / sizeof(knownBLEAddresses[0])); i++)
      {
        //Uncomment to Enable Debug Information
        //Serial.println("*************Start**************");
        //Serial.println(sizeof(knownBLEAddresses));
        //Serial.println(sizeof(knownBLEAddresses[0]));
        //Serial.println(sizeof(knownBLEAddresses)/sizeof(knownBLEAddresses[0]));
        //Serial.println(advertisedDevice.getAddress().toString().c_str());
        //Serial.println(knownBLEAddresses[i].c_str());
        //Serial.println("*************End**************");
        if (strcmp(advertisedDevice.getAddress().toString().c_str(), knownBLEAddresses[i].c_str()) == 0)
        {
          device_found = true;
          break;
        }
        else
          device_found = false;
      }
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};
void setup() {
  Serial.begin(115200); 
  Serial.println("Scanning..."); 
  pinMode(relay, OUTPUT);
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); 
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); 
  pBLEScan->setActiveScan(true); 
  pBLEScan->setInterval(100); 
  pBLEScan->setWindow(99);  
}
void loop() 
{
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  
  for (int i = 0; i < foundDevices.getCount(); i++)
  {
    BLEAdvertisedDevice device = foundDevices.getDevice(i);
    int rssi = device.getRSSI();
    Serial.print("RSSI: ");
    Serial.println(rssi);
    if (rssi > RSSI_THRESHOLD && device_found == true)
    {
      Serial.println("Door Open");
      digitalWrite(relay, HIGH);
      delay(5000);
      Serial.println("Door Close");
      digitalWrite(relay, LOW);
    }

  }
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
}
