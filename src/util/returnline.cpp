#include <string>
#include <sstream>

std::string returnline(std::stringstream& stream)
{
	std::string ret;
	std::getline(stream, ret);

	return ret;
}

std::string returnline(std::stringstream& stream, char delim)
{
	std::string ret;
	std::getline(stream, ret, delim);

	return ret;
}