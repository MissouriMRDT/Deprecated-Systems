// RoveComm.c
// Author: Gbenga Osibodu

#include "RoveComm.h"
#include <string.h>

#define ROVECOMM_VERSION 1
#define ROVECOMM_HEADER_LENGTH 8
#define ROVECOMM_PORT 11000

#define UDP_TX_PACKET_MAX_SIZE 1500
#define ROVECOMM_MAX_SUBSCRIBERS 5

#define ROVECOMM_ACKNOWLEDGE_FLAG   0x0001

#define ROVECOMM_PING               0x0001
#define ROVECOMM_PING_REPLY         0x0002
#define ROVECOMM_SUBSCRIBE          0x0003
#define ROVECOMM_UNSUBSCRIBE        0x0004
#define ROVECOMM_FORCE_UNSUBSCRIBE  0x0005
#define ROVECOMM_ACKNOWLEDGE_ID     0x0006


uint8_t RoveCommBuffer[UDP_TX_PACKET_MAX_SIZE];
roveIP RoveCommSubscribers[ROVECOMM_MAX_SUBSCRIBERS]; 

void RoveCommSendMsgTo(uint16_t dataID, size_t size, const void* const data, uint16_t seqNum, uint8_t flags, roveIP destIP, uint16_t destPort);
static void RoveCommParseMsg(uint8_t* buffer, uint16_t* dataID, size_t* size, void* data, uint16_t* seqNum, uint8_t* flags);
static void RoveCommHandleSystemMsg(uint8_t* buffer, uint16_t* dataID, size_t* size, void* data, uint16_t* seqNum, uint8_t* flags, roveIP IP);
static bool RoveCommAddSubscriber(roveIP IP);

void RoveCommBegin(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet) {
  roveIP IP = roveSetIP(first_octet, second_octet, third_octet, fourth_octet);
  
  roveNetworkingStart(IP);
  
  roveUdpSocketListen(ROVECOMM_PORT);
  
  int i;
  for (i=0; i < ROVECOMM_MAX_SUBSCRIBERS; i++) {
    RoveCommSubscribers[i] = INADDR_NONE;
  }
}

void RoveCommGetMsg(uint16_t* dataID, size_t* size, void* data) {
  uint8_t flags = 0;
  uint16_t seqNum = 0;
  roveIP senderIP;
  
  *dataID = 0;
  *size = 0;
  
  if (RoveCommGetUdpMsg(&senderIP, RoveCommBuffer, sizeof(RoveCommBuffer)) == true) {
    RoveCommParseMsg(RoveCommBuffer, dataID, size, data, &seqNum, &flags);  
    RoveCommHandleSystemMsg(RoveCommBuffer, dataID, size, data, &seqNum, &flags, senderIP);
  }
}

static void RoveCommParseMsg(uint8_t* buffer, uint16_t* dataID, size_t* size, void* data, uint16_t* seqNum, uint8_t* flags) {
  int protocol_version = buffer[0];
  switch (protocol_version) {
    case 1:
      *seqNum = buffer[1];
      *seqNum = (*seqNum << 8) | buffer[2];
      *flags = buffer[3];
      *dataID = buffer[4];
      *dataID = (*dataID << 8) | buffer[5];
      *size = buffer[6];
      *size = (*size << 8) | buffer[7];
      
      memcpy(data, &(buffer[8]), *size);
  }
}

void RoveCommSendMsgTo(uint16_t dataID, size_t size, const void* const data, uint16_t seqNum, uint8_t flags, roveIP destIP, uint16_t destPort) {
  size_t packetSize = size + ROVECOMM_HEADER_LENGTH;
  uint8_t buffer[packetSize];
  
  buffer[0] = ROVECOMM_VERSION;
  buffer[1] = seqNum >> 8;
  buffer[2] = seqNum & 0x00FF;
  buffer[3] = flags;
  buffer[4] = dataID >> 8;
  buffer[5] = dataID & 0x00FF;
  buffer[6] = size >> 8;
  buffer[7] = size & 0x00FF;
  
  memcpy(&(buffer[8]), data, size);

  RoveCommSendUdpPacket(destIP, destPort, buffer, packetSize);
}

void RoveCommSendMsg(uint16_t dataID, size_t size, const void* const data) {
  int i = 0; 
  
  for (i=0; i < ROVECOMM_MAX_SUBSCRIBERS; i++) {
    if (!(RoveCommSubscribers[i] == INADDR_NONE)) {
      RoveCommSendMsgTo(dataID, size, data, 0x00FF, 0, RoveCommSubscribers[i], ROVECOMM_PORT);
    }
  }
}

static bool RoveCommAddSubscriber(roveIP IP) {
  int i = 0;

  for (i=0; i<ROVECOMM_MAX_SUBSCRIBERS; i++) {
    if (RoveCommSubscribers[i] == IP) {
      return true;
    }
    if (RoveCommSubscribers[i] == INADDR_NONE) {
      RoveCommSubscribers[i] = IP;
      return true;
    }
  }
  
  return false;
}

static void RoveCommHandleSystemMsg(uint8_t* buffer, uint16_t* dataID, size_t* size, void* data, uint16_t* seqNum, uint8_t* flags, roveIP IP) {
  if (*flags & ROVECOMM_ACKNOWLEDGE_FLAG == ROVECOMM_ACKNOWLEDGE_FLAG) {
    RoveCommSendMsgTo(ROVECOMM_ACKNOWLEDGE_ID, sizeof(uint16_t), dataID, 0x00FF, 0, IP, ROVECOMM_PORT);
  }

  switch (*dataID) {
    case ROVECOMM_PING:
      RoveCommSendMsgTo(ROVECOMM_PING_REPLY, sizeof(uint16_t), seqNum, 0x00FF, 0, IP, ROVECOMM_PORT);
      break;
    case ROVECOMM_PING_REPLY:
      break;
    case ROVECOMM_SUBSCRIBE:
      RoveCommAddSubscriber(IP);
      break;
    default:
      return;
  }
  *dataID = 0;
  *size = 0;
}

