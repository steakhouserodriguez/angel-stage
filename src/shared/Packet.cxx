#include "Packet.hxx"

/////////////////////////////////////////////// CORE ////////////////////////////////////////////////////////////////////////////

Packet::Packet(uint8_t _type)
{
	type = _type;
}

void Packet::writeHeader(TCPsocket sock)
{
	SDLNet_TCP_Send(sock, &type, 1);
}

Packet *Packet::readByType(TCPsocket _sock, uint8_t _type)
{
	Packet *p = 0;
	switch(_type)
	{
	case PACKET_PING:
		p = new PacketPing(_type);
		break;
	case PACKET_DISCONNECT:
		p = new PacketDisconnect(_type);
		break;
	}
	if(!p) return 0;
	p->read(_sock);
	return p;
}

/////////////////////////////////////////////// PACKET_PING ////////////////////////////////////////////////////////////////////////////

PacketPing::PacketPing(uint8_t _type) : Packet(_type)
{
}

void PacketPing::read(TCPsocket sock)
{
	SDLNet_TCP_Recv(sock, &pingstuff, 4);
}

void PacketPing::write(TCPsocket sock)
{
	writeHeader(sock);
	SDLNet_TCP_Send(sock, &pingstuff, 4);
}

/////////////////////////////////////////////// PACKET_DISCONNECT ////////////////////////////////////////////////////////////////////////////

PacketDisconnect::PacketDisconnect(uint8_t _type) : Packet(_type)
{
}

void PacketDisconnect::read(TCPsocket sock)
{
	//no extra bytes
}

void PacketDisconnect::write(TCPsocket sock)
{
	writeHeader(sock);
	//just in case to work around issue with blocking read
	uint8_t dummy = 0;
	SDLNet_TCP_Send(sock, &dummy, 1);
}
