//
// FileStream_POSIX.h
//
// $Id: //poco/1.3/Foundation/include/Poco/FileStream_POSIX.h#1 $
//
// Library: Foundation
// Package: Streams
// Module:  FileStream
//
// Definition of the FileStreamBuf, FileInputStream and FileOutputStream classes.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_FileStream_POSIX_INCLUDED
#define Foundation_FileStream_POSIX_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/BufferedBidirectionalStreamBuf.h"
#include <istream>
#include <ostream>


namespace Poco {


class Foundation_API FileStreamBuf: public BufferedBidirectionalStreamBuf
	/// This stream buffer handles Fileio
{
public:
	FileStreamBuf();
		/// Creates a FileStreamBuf.
		
	~FileStreamBuf();
		/// Destroys the FileStream.

	void open(const std::string& path, std::ios::openmode mode);
		/// Opens the given file in the given mode.

	void close();
		/// Closes the File stream buffer.

	std::streampos seekoff(std::streamoff off, std::ios::seekdir dir, std::ios::openmode mode = std::ios::in | std::ios::out);
		/// Change position by offset, according to way and mode.

	std::streampos seekpos(std::streampos pos, std::ios::openmode mode = std::ios::in | std::ios::out);
		/// Change to specified position, according to mode.

protected:
	enum
	{
		BUFFER_SIZE = 4096
	};

	int readFromDevice(char* buffer, std::streamsize length);
	int writeToDevice(const char* buffer, std::streamsize length);

private:
	std::string _path;
	int _fd;
	std::streamoff _pos;
};


} // namespace Poco


#endif // Foundation_FileStream_WIN32_INCLUDED
