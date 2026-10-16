#ifndef REQUESTS_H
#define REQUESTS_H
#include "message.h"

int decode_req(char message[],ADSBPacket *aircraft, int count);

#endif