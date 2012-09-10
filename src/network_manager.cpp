#include <iostream>
#include <boost/thread/thread.hpp>
#include <stdint.h>

#include "network_manager.h"
#include "packet.h"

NetworkManager::NetworkManager(TCPsocket _sock)
{
	sock = _sock;
	closed = false;
}

void NetworkManager::peer_thread_read(NetworkManager *nm)
{
	while(1)
	{
		//std::cout << "peer_thread\n";
		nm->processNetworkRead();
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		if(nm->closed)
			break;
	}
}

void NetworkManager::processNetworkRead()
{
	uint8_t type;
	
	std::cout << "processNetworkRead\n";
	
	SDLNet_TCP_Recv(sock, &type, 1);
	
	//std::cout << type << len;
	Packet *p = Packet::readByType(sock, type);
	
	rx_mutex.lock();
		rx_queue.push_back(p);
	rx_mutex.unlock();
}

Packet *NetworkManager::getRXPacket()
{
	Packet *p = 0;
	rx_mutex.lock();
		if(rx_queue.size() != 0)
		{
			p = rx_queue.front();
			rx_queue.pop_front();
		}
	rx_mutex.unlock();
	
	return p;
}

void NetworkManager::peer_thread_write(NetworkManager *nm)
{
	while(1)
	{
		//std::cout << "peer_thread\n";
		nm->processNetworkWrite();
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		if(nm->closed)
			break;
	}
}

void NetworkManager::processNetworkWrite()
{
	//std::cout << "processNetworkWrite\n";
	
	tx_mutex.lock();
		while(tx_queue.size() > 0)
		{
			Packet *pout = tx_queue.front();
			tx_queue.pop_front();
			pout->write();
			delete pout;
		}
	tx_mutex.unlock();
}

void NetworkManager::addTXPacket(Packet *pkt)
{
	tx_mutex.lock();
		tx_queue.push_back(pkt);
	tx_mutex.unlock();
}

void NetworkManager::close()
{
	closed = true;
	read_thread->join();
	write_thread->join();
	SDLNet_TCP_Close(sock);
}
