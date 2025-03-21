#include <iostream>
#include <cassert>
#include <fstream>
#include <chrono>
#include <thread>
#include <string>

const std::string log_file = "../kmesg_sim.log";

void generate_logs() {

	std::ofstream file(log_file, std::ios::trunc);

	assert(file.is_open());

	for(int i = 0; i < 50; i++) {

		file << "[ " << i * 1.2345 << " ] kernel: Simulated log entry " << i << "\n";
		file.flush();
		//Might be needed in the future when we want multiple IDs from multiple fake machines
		//std::this_thread::sleep_for(std::chrono::seconds(1));

	}

}

int main() {

	//TODO: Add ID as arg in multithreaded case where this becomes a worker function
	generate_logs();
	return 0;

}
