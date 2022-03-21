#pragma once

#include <iostream>

struct ListenInfo {
	std::string ip;
	int port;

	ListenInfo() : ip(""), port(0) {};
	ListenInfo(std::string ip, int port) : ip(ip), port(port) {};
};

inline bool operator==(const ListenInfo &lhs, const ListenInfo &rhs) {
  return lhs.ip == rhs.ip && lhs.port == rhs.port;
}
