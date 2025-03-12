#include "Modbus_CID.h"

void Modbus::begin(unsigned long baud, HardwareSerial *serial)
{
    HWSerial = serial;
    serial->begin(baud);
}

void Modbus::constructModbusRequest(byte *frame)
{

    uint16_t crc = calculateCRC(frame, 6);
    frame[6] = crc & 0xFF;        // CRC low byte
    frame[7] = (crc >> 8) & 0xFF; // CRC high byte
}

void Modbus::sendModbusRequest(byte *frame, byte length)
{
    for (byte i = 0; i < length; i++)
    {
        HWSerial->write(frame[i]);
    }
}

uint16_t Modbus::calculateCRC(byte *frame, byte length)
{
    uint16_t crc = 0xFFFF; // Initialize CRC to 0xFFFF
    for (byte i = 0; i < length; i++)
    {
        crc ^= frame[i];
        for (byte j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc >>= 1;
                crc ^= 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}