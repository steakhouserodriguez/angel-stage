#include <iostream>
#include <iomanip>
#include <cstdio>
#include "ClientsConnection.hxx"
#include "Packet.hxx"

ClientsConnection::ClientsConnection(uint16_t port)
{
	IPaddress ip;
	
	if(SDLNet_ResolveHost(&ip, NULL, port) != 0)
		throw "SDLNet_ResolveHost error";
	
	listenSock = SDLNet_TCP_Open(&ip);
	if(listenSock == NULL)
		throw "SDLNet_TCP_Open error";
	
	running = false;
}

ClientsConnection::~ClientsConnection()
{
	if(running) stop();
}

void ClientsConnection::start(){
//	boost::thread listener( &ClientsConnection::listen );
//	boost::thread listener( this->listen );

//boost::thread* thr = new boost::thread(boost::bind(&Foo::some_function, this));	

	running = true;
	
		listenThread = new boost::thread(
			boost::bind(&ClientsConnection::listen, this));

}

void ClientsConnection::stop()
{
	running = false;
	listenThread->join();
	delete listenThread;
	listenThread = NULL;
	if(listenSock)
	{
		SDLNet_TCP_Close(listenSock);
		listenSock = NULL;
	}
}

void ClientsConnection::tick()
{
	if(listenSock == NULL)
		throw "listenSock is null!";
	TCPsocket clientSock;
	while((clientSock = SDLNet_TCP_Accept(listenSock)) != NULL)
	{
		//std::cout << "asdf\n";
		//SDLNet_TCP_Close(clientSock);
		PacketTransporter *nm = new PacketTransporter(clientSock);
		IPaddress *ip = SDLNet_TCP_GetPeerAddress(clientSock);
		nm->peer_ip = ((uint64_t)(ip->host) << 16) | (ip->port);
		//std::cout << "Connected to client " << std::hex << nm->peer_ip << "\n";
		printf("Connected to client %012lX\n", nm->peer_ip);
		boost::thread *new_peer_thread_read = new boost::thread(boost::bind(PacketTransporter::peer_thread_read, nm));
		boost::thread *new_peer_thread_write = new boost::thread(boost::bind(PacketTransporter::peer_thread_write, nm));
		
		nm->read_thread = new_peer_thread_read;
		nm->write_thread = new_peer_thread_write;
		
		//stuff to send to client on login
		PacketMap *pm = new PacketMap(PACKET_MAP);
		pm->size = 48;
		nm->addTXPacket(pm);
		
		nm_mutex.lock();
			packetTransporters.push_back(nm);
		nm_mutex.unlock();
	}
}

void ClientsConnection::listen(){
	try {
		while (running) {
			this->tick();
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		}
	} catch (const char *e){
		std::cout << e << std::endl;
	}



}

void ClientsConnection::sendPacket(Packet *p, uint64_t client)
{
	nm_mutex.lock();
		for(int i = 0; i < packetTransporters.size(); i++)
		{
			PacketTransporter *pt = packetTransporters[i];
			if(pt == NULL) continue;
			if(pt->peer_ip == client || client == 0)
			{
				pt->addTXPacket(p);
				break;
			}
		}
	nm_mutex.unlock();
}
