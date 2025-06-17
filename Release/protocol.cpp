#include "protocol.h"
#include <regex>
#include "utils/sys/io.hpp"

std::atomic_uint8_t protocol::_proto;

protocol::protocol()
{
	_proto = USB;
	_info = "";
	_opened = false;
	_timeout = 10000;
	_event = nullptr;
}

protocol::~protocol()
{
}

bool protocol::is_opened()
{
	return _opened;
}

void protocol::get_error(char * buff, int len)
{
	memset(buff, 0, len);
	if (recv_error == _status) {
		std::lock_guard<std::mutex> lock(_dat_mtx);
		memcpy(&buff[0], &(_data[0]), _data.size() - 1);//È¥µô\r
	}
}

bool protocol::read(char * buff, int & size)
{
	if (nullptr == buff || recv_error == _status) return false;

	std::string recved;
	{
		std::lock_guard<std::mutex> lock_(_dat_mtx);
		recved.assign(_data.data(), _data.size());

		std::vector<char>().swap(_data);
	}

	zl::str::replace(recved, "\rOK\r", "");

	size = recved.size();
	memcpy(buff, recved.c_str(), recved.length());

	return true;
}

void protocol::process(const char * recv, int len)
{
	if(len < 1024) zlog::instance()->write("receive [ %s ]", recv);

	std::lock_guard<std::mutex> lock(_dat_mtx);
	_data.insert(_data.end(), recv, recv + len);

	const std::string last(_data.data(), _data.size());
	if (std::regex_search(last, std::regex("E[0-9][0-9]\\r"))) {
		_status = recv_error;
	}
	else if (last.find("OK\r") != last.npos) {
		_status = recv_ok;
	}
}
