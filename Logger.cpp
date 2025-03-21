#include "Logger.h"


Logger::Logger() {
	//get client socket setup before setting up
	//producer consumer
	initSocket();

	//init /proc/kmesg
	//initFile();

	m_Sender = std::thread(&Logger::sender, this);
	m_Receiver = std::thread(&Logger::receiver, this);


}

Logger::~Logger() {

	m_Done = true;
	//join threads and close socket and file
	if(m_Receiver.joinable()) {
		m_Receiver.join();
	}
	if(m_Sender.joinable()) {
		m_Sender.join();
	}

	if(sockfd != -1) {
		close(sockfd);
	}

}

//Init UDP Client socket to send data
void Logger::initSocket() {

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0) {
		perror("UDP Client creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

}

void Logger::sender() {

	//open file and write every line to the queue
	std::ifstream file(path);

	assert(file.is_open());
    	std::cout << "file is valid pointer" << std::endl;

	std::string line;
	while(std::getline(file, line)) {
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_queue.push(line);
		}
		m_CV.notify_one();
	}
    std::cout << "Sender done" << std::endl;


}

void Logger::receiver() {

	while(true) {

		std::unique_lock<std::mutex> lock(m_Mutex);
		m_CV.wait(lock, [&] { return !m_queue.empty(); });

		while(!m_queue.empty()) {

			std::string log = m_queue.front();
			m_queue.pop();
			lock.unlock();

			std::cout << "LOG RECV: " << log << std::endl;

			lock.lock();

		}

	}

}
