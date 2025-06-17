#include "zlog.h"
#include <vector>
#include <regex>

#include <sys/stat.h>

zlog::zlog()
{
	_module = "dfield.log";
}

zlog::~zlog()
{
}

void zlog::write(const char* format, ...)
{
	va_list arglist;
	char buff[1024] = { 0 };

	va_start(arglist, format);
	vsprintf_s(buff, format, arglist);
	va_end(arglist);

	auto file = std::string(_getcwd(nullptr, 0)).append("\\").append(_module);
	
	struct stat fs;
	stat(file.c_str(), &fs);

	uint8_t mode = std::ios::app;
	if (fs.st_size > 500 * 1024) mode = std::ofstream::trunc; /*ÏÞÖÆ500K*/

	std::fstream of(file, std::ofstream::out | mode);
	if (!of.is_open()) return;

	std::string result = std::regex_replace(buff, std::regex("\r"), "\\r");

	of << get_time() << ": [ " << result << " ]" << std::endl;
	of.close();
}

std::string zlog::get_time()
{
	time_t now;
	time(&now);

	tm t;
	localtime_s(&t, &now);

	char buff[256] = { 0 };
	strftime(buff, 256, "%Y-%m-%d %H:%M:%S", &t);

	return std::move(std::string(buff));
}
//end private
