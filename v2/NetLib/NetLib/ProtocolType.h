#pragma once

enum class ProtocolType {
	NONE = 0b000,
	TCP  = IPPROTO_TCP,
	UDP  = IPPROTO_UDP
};