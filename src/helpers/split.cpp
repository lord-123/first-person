#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> split(std::stringstream& stream, char delim)
{
	std::vector<std::string> ret;

	std::string partition;

	while (std::getline(stream, partition, delim))
	{
		ret.push_back(partition);
	}

	return ret;
}