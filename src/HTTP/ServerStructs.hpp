#pragma once

#include <iostream>

struct ListenInfo {
	std::string ip;
	uint32_t port;

	ListenInfo() : ip(""), port(0) {};
	ListenInfo(std::string ip, uint32_t port) : ip(ip), port(port) {};
};

inline bool operator==(const ListenInfo &lhs, const ListenInfo &rhs) {
  return lhs.ip == rhs.ip && lhs.port == rhs.port;
}
