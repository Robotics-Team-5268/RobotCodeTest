#ifndef SRC_SUBSYSTEMS_UDPRECEIVER_H_
#define SRC_SUBSYSTEMS_UDPRECEIVER_H_

#include "Commands/Subsystem.h"
#include "WPIlib.h"

#include <atomic>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

class UDPReceiver : public Subsystem {

private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities

	void ThreadBody();
	std::vector<std::string> split(std::string str, std::string delimiter);
	int strToInt(std::string str);

	// The order of these is important
	// So the thread doesn't start before everything's ready
	std::atomic_bool mRun;
	std::thread mThread;
	int sockfd;
	char buffer[256]; // Don't access this outside of receive

public:
	UDPReceiver();
	~UDPReceiver();
	void InitDefaultCommand();
	int GetCount() const; // Temporary
	void GetContours( // Switch to int vectors later
			std::vector<double>& centerX_out,
			std::vector<double>& centerY_out,
			std::vector<double>& width_out,
			std::vector<double>& height_out,
			std::vector<double>& area_out);
	void AddSmartDashboardItems();
	// May become private later
	std::vector<int> area, centerX, centerY, height, width;

};

#endif /* SRC_SUBSYSTEMS_UDPRECEIVER_H_ */
