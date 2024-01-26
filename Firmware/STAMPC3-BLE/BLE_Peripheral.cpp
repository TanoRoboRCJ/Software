#include "BLE_Peripheral.h"

BLE_PERIPHERAL::BLE_PERIPHERAL(const char* deviceName)
    : _isDeviceConnected(false),
      _wasDeviceConnected(false),
      _pServer(NULL),
      _pCharacteristic(NULL) {
    _deviceName = deviceName;

    uuidGenerate(deviceName);
}

void BLE_PERIPHERAL::uuidGenerate(const char* tag) {
    // tagから適当にUUIDらしきものを生成する
    unsigned long uuidTag = 0;
    for (int i = 0; i < strlen(tag); i++) {
        uuidTag += tag[i] * i;
    }
    uuidTag = uuidTag % 2000;

    sprintf(_serviceUuid, "%08ld-%04ld-%04ld-%04ld-%012ld\0", uuidTag,
            uuidTag * 2, uuidTag * 3, uuidTag * 4, uuidTag * 5);

    uuidTag *= 4;
    uuidTag = uuidTag % 2000;

    sprintf(_characteristicUuid, "%08ld-%04ld-%04ld-%04ld-%012ld\0", uuidTag,
            uuidTag * 2, uuidTag * 3, uuidTag * 4, uuidTag * 5);
}

void BLE_PERIPHERAL::init() {
    // Create the BLE Device
    BLEDevice::init(_deviceName);
    if (_isDebugMode) {
        Serial.println("BLE Device created");
        Serial.print("Device name: ");
        Serial.println(_deviceName);
    }

    // Create the BLE Server
    _pServer = BLEDevice::createServer();
    _pServer->setCallbacks(new MyServerCallbacks(this));
    if (_isDebugMode) {
        Serial.println("BLE Server created");
    }

    // Create the BLE Service
    BLEService* pService = _pServer->createService(_serviceUuid);
    if (_isDebugMode) {
        Serial.println("BLE Service created");
        Serial.print("Service UUID: ");
        Serial.println(_serviceUuid);
    }

    _pCharacteristic = pService->createCharacteristic(
        _characteristicUuid, BLECharacteristic::PROPERTY_READ |
                                 BLECharacteristic::PROPERTY_WRITE |
                                 BLECharacteristic::PROPERTY_NOTIFY |
                                 BLECharacteristic::PROPERTY_INDICATE);
    if (_isDebugMode) {
        Serial.println("BLE Characteristic created");
        Serial.print("Characteristic UUID: ");
        Serial.println(_characteristicUuid);
    }

    _pCharacteristic->addDescriptor(new BLE2902());
    if (_isDebugMode) {
        Serial.println("Descriptor added to the characteristic");
    }

    _pCharacteristic->setCallbacks(new MyCallbacks(this));
    if (_isDebugMode) {
        Serial.println("Callbacks set for the characteristic");
    }

    pService->start();
    if (_isDebugMode) {
        Serial.println("Service started");
    }

    BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(_serviceUuid);
    if (_isDebugMode) {
        Serial.println("Service UUID added to the advertising");
    }

    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    if (_isDebugMode) {
        Serial.println("Advertising settings updated");
    }

    BLEDevice::startAdvertising();
    if (_isDebugMode) {
        Serial.println("Advertising started");
    }
}

void BLE_PERIPHERAL::enableDebugMode() {
    _isDebugMode = true;
}

bool BLE_PERIPHERAL::checkConnection() {
    if (!_isDeviceConnected && _wasDeviceConnected) {
        _pServer->startAdvertising();
        _wasDeviceConnected = _isDeviceConnected;

        if (_isDebugMode) {
            Serial.println("Waiting a client connection to notify...");
        }
    }
    if (_isDeviceConnected && !_wasDeviceConnected) {
        _wasDeviceConnected = _isDeviceConnected;

        if (_isDebugMode) {
            Serial.println("Connected");
        }
    }

    return _isDeviceConnected;
}

int BLE_PERIPHERAL::available() {
    int length = _rxValue.length();
    return length;
}

char BLE_PERIPHERAL::read() {
    char data = _rxValue[0];
    _rxValue = _rxValue.substr(1);

    return data;
}

void BLE_PERIPHERAL::write(char* data, size_t length) {
    _pCharacteristic->setValue((uint8_t*)data, length);
    _pCharacteristic->notify();
}

BLE_PERIPHERAL::MyServerCallbacks::MyServerCallbacks(BLE_PERIPHERAL* peripheral)
    : peripheral(peripheral) {
}

void BLE_PERIPHERAL::MyServerCallbacks::onConnect(BLEServer* pServer) {
    peripheral->_isDeviceConnected = true;
}

void BLE_PERIPHERAL::MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    peripheral->_isDeviceConnected = false;
}

BLE_PERIPHERAL::MyCallbacks::MyCallbacks(BLE_PERIPHERAL* peripheral)
    : peripheral(peripheral) {
}

void BLE_PERIPHERAL::MyCallbacks::onWrite(BLECharacteristic* pCharacteristic) {
    peripheral->_rxValue = pCharacteristic->getValue();
}