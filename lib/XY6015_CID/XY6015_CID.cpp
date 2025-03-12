#include "XY6015_CID.h"
#include "Wire.h"

void XY6015::begin(unsigned long baud, HardwareSerial *serial)
{
    modbus.begin(baud, serial);
    HWSerial = serial;
    slaveAdress = 1;
    status = false;
}

void XY6015::setVoltage(float v)
{
    int voltage = v * 100;
    createFrame(0x0000, voltage, "SET");
}

void XY6015::setCurrent(float a)
{
    int current = a * 100;

    createFrame(0x0001, current, "SET");
}

void XY6015::awaitResponse()
{
    if (HWSerial->available())
    {
        HWSerial->readBytes(buffer, 40);
    }

    for (byte data : buffer)
    {
        Wire.print(data, HEX);
        Wire.print(" ");
    }

    Wire.println();
}

void XY6015::read()
{
    // 14 set for value for 14 registers to read
    createFrame(0x00, 14, "READ");
}

void XY6015::toggle()
{
    status = !status;

    createFrame(0x0012, ((status) ? 0x0001 : 0x0000), "SET");

    Serial.println("TOGGLED: " + String(status));
}

void XY6015::createFrame(byte add, int val, String mode)
{
    byte requestFrame[8];
    byte frameMode = 0;

    if (mode == "SET")
    {
        frameMode = 0x06;
    }
    else if (mode == "READ")
    {
        frameMode = 0x03;
    }

    if (frameMode != 0)
    {
        requestFrame[0] = slaveAdress; // Slave Address
        requestFrame[1] = frameMode;   // Function Code
        requestFrame[2] = add >> 8;    // Start Address High
        requestFrame[3] = add & 0xFF;  // Start Address Low
        requestFrame[4] = val >> 8;    // Register Count High
        requestFrame[5] = val & 0xFF;  // Register Count Low

        modbus.constructModbusRequest(requestFrame); // Create calculated CRC and append to frame

        modbus.sendModbusRequest(requestFrame, 8); // Send Request

        Serial.println(String(requestFrame[4]) + " | " + String(requestFrame[5]));
    }
    else
    {
        Serial.println("Invalid Frame Mode");
    }
}