#ifndef PACKET_H
#define PACKET_H
#include <stdint.h>
#include "SDL/SDL_net.h"

const uint8_t PACKET_PING=			0x41;
const uint8_t PACKET_DISCONNECT=	0x55;

/////////////////////////////////////////////// CORE ////////////////////////////////////////////////////////////////////////////

class Packet
{
public:
	Packet(TCPsocket _sock, uint8_t _type);
	static Packet *readByType(TCPsocket _sock, uint8_t _type);
	virtual void read() = 0;
	virtual void write() = 0;

	uint8_t type;

protected:
	TCPsocket sock;
	void writeHeader();
};

/////////////////////////////////////////////// PACKET_PING ////////////////////////////////////////////////////////////////////////////

class PacketPing: public Packet
{
public:
	PacketPing(TCPsocket _sock, uint8_t _type);
	void read();
	void write();

public:
	uint32_t pingstuff;
};

/////////////////////////////////////////////// PACKET_DISCONNECT ////////////////////////////////////////////////////////////////////////////

class PacketDisconnect: public Packet
{
public:
	PacketDisconnect(TCPsocket _sock, uint8_t _type);
	void read();
	void write();
};

#endif
