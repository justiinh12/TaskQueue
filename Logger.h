#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <queue>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <atomic>

#define PORT 8000
#define MAXLINE 1024

/**
 * Class to handle parsing simulated dmesg formatted output.
 * We use basic locking with condition variables to push and pop
 * from a queue containing the strings
 */
class Logger {

public:
	
	Logger();
	~Logger();

	//Create socket to start 
	void initSocket();

	//send log over udp
	int sendLog();

	//thread functions
	void sender(); //reads from /proc
	void receiver(); //sends over UDP

private:
	//Simulated Machine ID
	//std::string m_ID;
	
	const std::string path = "../kmesg_sim.log";

	std::atomic<bool> m_Done;

	//Socket fd and addr struct
	int sockfd;
	struct sockaddr_in servaddr;

	//message queue to send over udp
	std::queue<std::string> m_queue;

	//File descriptor for opening /dev/kmsg
	//int kmesgfd;
	
	//two threads to handle sending and receiving logs
	std::thread m_Sender;
	std::thread m_Receiver;
	
	//cond var and mutex to handle when queue is ready
	std::condition_variable m_CV;
	std::mutex m_Mutex;


};

#endif
