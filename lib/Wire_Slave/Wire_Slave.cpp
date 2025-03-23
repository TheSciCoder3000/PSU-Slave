#include "Wire_Slave.h"

void WireSlave::begin()
{
    Wire.begin(SLAVE_ADDR);
}

void WireSlave::transmit(char *payload)
{
    Wire.beginTransmission(MASTER_ADDR);
    Wire.println(payload);
    Wire.endTransmission();
}
void WireSlave::transmit(String payload)
{
    Wire.beginTransmission(MASTER_ADDR);
    Wire.println(payload);
    Wire.endTransmission();
}