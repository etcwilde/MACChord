
/*
 * main
 * File:     main.cpp
 * Author:   Evan Wilde                    <etcwilde@uvic.ca>
 * Date:     Jun 07 2015
 */

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <regex>
#include <utility>

#include <stdio.h>

#include "Chord.hpp"
#include "HashTable.hpp"
#include "Socket.hpp"
#include "chord_message.pb.h"
#include "logging.hpp"


#ifdef BUILD_CLIENT
#else // BUILD_DAEMON
#define DEFAULT_CONFIGFILE 	"ddns.conf"
#define DEFAULT_SOCKETFILE 	"ddns.sock"
#define DEFAULT_HOSTSFILE	"hosts"

//#define HOSTNAME_PATTERN "^hostname[ \t]*?=[ \t]*?([0-9a-zA-Z\\.]*?)[ \t]*?(?:#@.*)?$"


#define HOSTNAME_PATTERN "hostname[ \t]*?=[ \t]*?([0-9a-zA-Z\\.]+?)[ \t]*?(?:#.*)?"
#define HOSTFILE_PATTERN "^hostfile[ \t]*?=[ \t]*?([0-9a-zA-Z\\/.]+?)[ \t]*?(?:#.*)?$"
#define SOCKET_PATTERN "^socketfile[ \t]*?=[ \t]*?([0-9a-zA-Z\\/.]+?)[ \t]*?(?:#.*)?$"
#define HOSTS_PATTERN "^((?:[0-9]{1,3}[.]){3}[0-9]{1,3})[ \t]+([a-zA-Z0-9._-]+)(?:[ \t]+([a-zA-Z0-9_-]+)?)?[ \t]?#?.*$"

// Read the configuration file
// Read hosts file
// if IP/Port passed
// 	use those to connect
// else
// 	Attempt to connect to any from hosts file
//
// Setup IPC unix sockets
// Listen for requests forever
// Service requests

int main(int argc, char** argv)
{
	// Things that need setting
	// hostsfile
	// hostname

	// IP / Hostname / Seek position

	std::vector<std::tuple<std::string, std::string, unsigned int>> hosts;

	// Read the configuration file
	std::ifstream conf_file(DEFAULT_CONFIGFILE);
	std::string hostname = "";
	std::string hostfile_name = DEFAULT_HOSTSFILE;
	std::string sockfile_name = DEFAULT_SOCKETFILE;

	std::string line;
	std::smatch groups;
	if (conf_file.is_open())
	{
		while(getline(conf_file, line))
		{
			if (line.c_str()[0] == '#') continue;
			if (std::regex_match(line, groups, std::regex(HOSTNAME_PATTERN)))
			{
				std::cout << " Hostname found\n";
				std::cout << groups[1] << '\n';
				hostname = groups[1];
			}
			else if (std::regex_match(line, groups, std::regex(HOSTFILE_PATTERN)))
			{
				std::cout << " Hosts file found\n";
				std::cout << groups[1] << '\n';
				hostfile_name = groups[1];

			}
			else if (std::regex_match(line, groups, std::regex(SOCKET_PATTERN)))
			{
				std::cout << " Socket File\n";
				std::cout << groups[1] << '\n';
				sockfile_name = groups[1];
			}
		}
		conf_file.close();
	}
	else
	{
		std::cerr << "Failed to open configuration file\n";
		return 1;
	}

	if (hostname.compare("") == 0)
	{
		std::cerr << "No hostname specified\n";
		return 1;
	}
	std::cout << "Hostname: " << hostname << '\n';
	std::cout << "Hostfile: " << hostfile_name << '\n';
	// Read hosts file
	std::fstream hostfile(hostfile_name);
	if (hostfile.is_open())
	{
		std::cout << "hostsfile exists\n";
		while(getline(hostfile, line))
		{
			//std::cout << line << '\n';
			if (line.c_str()[0] == '#') continue;
			if(std::regex_match(line, groups, std::regex(HOSTS_PATTERN)))
			{
				// IP: 		groups[1]
				// Name: 	groups[2/3]
				// Position:	hostfile.tellg();
				if(groups.length(3) == 0) hosts.push_back(
						std::tuple<std::string, std::string, unsigned int>(
							groups[1], groups[2],
							hostfile.tellg()));
				else hosts.push_back(
						std::tuple<std::string, std::string, unsigned int>(
							groups[1], groups[3],
							hostfile.tellg()));
			}
		}
	}
	else
	{
		// Create hostfile
		std::cout << "hostsfile does not exist\n";
	}

	hostfile.close();

	std::cout << "Host Data\n";
	for (auto x: hosts)
	{
		std::cout << std::get<0>(x) << ":" << std::get<1>(x) << '\t' <<
			std::get<2>(x) << '\n';
	}

	// if IP/Port passed
	// 	use those to connect maybe
	// else
	// 	Attempt to connect to any from hosts file

	// Setup IPC unix sockets

	// Listen for requests forever

	// Service requests
	return 0;
}
#endif
