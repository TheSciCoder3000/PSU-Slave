#ifndef WIRE_SLAVE_H
#define WIRE_SLAVE_H
#include <Wire.h>

#define SLAVE_ADDR 69
#define MASTER_ADDR 1

class WireSlave
{
public:
    void begin();
    void transmit(char *payload);
    void transmit(String payload);
};
#endif