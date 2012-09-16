#include <iostream>
#include "NetworkConnecter.hxx"

NetworkConnecter::NetworkConnecter(const char * server, uint16_t port)
{
	IPaddress ip;
	
	if(SDLNet_ResolveHost(&ip, server, port) != 0)
		throw "SDLNet_ResolveHost error";
	
	clientSock = SDLNet_TCP_Open(&ip);
	if(clientSock == NULL)
		throw "SDLNet_TCP_Open error";
}

NetworkConnecter::~NetworkConnecter()
{
	if(clientSock)
		SDLNet_TCP_Close(clientSock);
}

void NetworkConnecter::connect()
{
	if(clientSock == NULL)
		throw "clientSock is null!";
	PacketTransporter *nm = new PacketTransporter(clientSock);
	boost::thread *new_peer_thread_read = new boost::thread(boost::bind(PacketTransporter::peer_thread_read, nm));
	boost::thread *new_peer_thread_write = new boost::thread(boost::bind(PacketTransporter::peer_thread_write, nm));
	
	nm->read_thread = new_peer_thread_read;
	nm->write_thread = new_peer_thread_write;
	
	network_manager = nm;
}