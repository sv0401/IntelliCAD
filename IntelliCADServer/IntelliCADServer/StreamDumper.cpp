#include "stdafx.h"
#include <fstream>
#include "StreamDumper.h"

using namespace std;

namespace StreamDumper
{
	bool dump(ReadStream &stream, ostream &out, const int bufferSize)
	{
		if (!out)
			return false;

		//stream.rewind();
		ubyte *pBuffer = new ubyte[bufferSize];

		do
		{
			const int VALID_COUNT = stream.get(pBuffer, bufferSize);

			if (!VALID_COUNT)
				break;

			out.write(reinterpret_cast<char *>(pBuffer), VALID_COUNT);
		} while (true);

		delete[] pBuffer;
		return true;
	}

	bool fileDump(ReadStream &stream, const string &path, const int bufferSize)
	{
		ofstream fout(path, ofstream::trunc | ofstream::binary);

		return dump(stream, fout, bufferSize);
	}
};