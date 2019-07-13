#pragma once

#include <string>
#include "ReadStream.hpp"

namespace StreamDumper
{
	bool dump(ReadStream &stream, std::ostream &out, int bufferSize = 1024);
	bool fileDump(ReadStream &stream, const std::string &path, int bufferSize = 1024);
};