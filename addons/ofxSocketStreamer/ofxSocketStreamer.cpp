 //	«Copyright 2013 devint»
 //	Contact:
 //		devint.co
 //		info@devint.co

 //	This file is part of "devint add-on for ccv v1.5".

 //   "devint add-on for ccv v1.5" is free software: you can redistribute it and/or modify
 //   it under the terms of the GNU General Public License as published by
 //   the Free Software Foundation, either version 3 of the License, or
 //   (at your option) any later version.

 //   "devint add-on for ccv v1.5" is distributed in the hope that it will be useful,
 //   but WITHOUT ANY WARRANTY; without even the implied warranty of
 //   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 //   GNU General Public License for more details.

 //   You should have received a copy of the GNU General Public License
 //   along with "devint add-on for ccv v1.5".  If not, see <http://www.gnu.org/licenses/>.


#include "ofxSocketStreamer.h"
#include <time.h> 

ofxSocketStreamer::ofxSocketStreamer()
{
	port = 0;
	height = 500;
	width = 600;
	socketConnected = false;
	streamActive	= false;
	pixels	=	NULL;
	readBuff =	new char[READING_SOCKSTREAM_BUFFER_SIZE]();
	pixels	=	new unsigned char[PIXELS_BUFFER_SIZE]();
	setVerbose(true);
}

ofxSocketStreamer::~ofxSocketStreamer()
{
	closeSocket();
	delete[] readBuff;
	delete[] pixels;
}

/// \brief	Initializes the tcp server and wait for a connection
bool	ofxSocketStreamer::openSocket(int socket)
{
	if( !TCPServer.Create() )	//Create Server
	{
		if(verbose)printf("ofxTCPManager: create() failed\n");
		return false;
	}
	if( !TCPServer.Bind(static_cast<unsigned short>(socket)) )	//Bind server to port
	{
		if(verbose)printf("ofxTCPManager: bind(port = %i) failed\n", socket);
		return false;
	}
	if( !TCPServer.Listen(1) )	//Wait for connections
	{
		if(verbose)printf("ofxTCPManager: listen() failed\n");
		return false;
	}
	if( !TCPServer.Accept(TCPConnectionHandler) )	//accept incoming connection
	{
			if(verbose)printf("ofxTCPManager: Accept() failed\n");
			return false;
	}

	socketConnected		= true;
	streamActive	= false;
	port			= socket;
	//connection established and ready to read
	return true;
}

void 	ofxSocketStreamer::closeSocket()
{
	socketConnected		= false;
	streamActive	= false;
	if(socketConnected) 
	{
		TCPConnectionHandler.Close();
		TCPServer.Close();
	}
}

bool ofxSocketStreamer::isFrameNew()
{
	//this variable holds if there are uncomplete pieces of
	static int readingOffset = 0;
	//Check connection
	if(!socketConnected)
	{
		width = 0;
		height = 0;
		pixels = NULL;
		streamActive = false;
		return false;
	}


	//read socket
	int recv_c = TCPConnectionHandler.Receive(&readBuff[readingOffset] ,READING_SOCKSTREAM_BUFFER_SIZE - readingOffset);
	if(recv_c == SOCKET_ERROR)
	{
		//Recieve error
		if(verbose)printf("ofxTCPManager: Receive() failed\n");
		streamActive = false;
		return false;
	}
	else if(recv_c == SOCKET_TIMEOUT)
	{
		//Timeout error
		if(verbose)printf("ofxTCPManager: Receive() timed out\n");
		streamActive = false;
		return false;
	}

	//look into recieve buffer for header

	int pckInitIndex = -1;	//Index of the beggining of the last complete data package
	int pckSizeField = -1;	//Size of the package (data readed from the field packagesize of the las package)
	int allBufSize = recv_c + readingOffset;
	readingOffset = allBufSize;

	//verify the minimum size of the data of the incoming data
	if(allBufSize < 7)
	{
		//data incomplete but stream active
		streamActive = true;
		return false;
	}

	for(int i=0; i<allBufSize; i++)
	{	//Search header
		if(allBufSize-i >= 3)
		{
			if((readBuff[i]=='<') && (readBuff[i+1]=='<') && (readBuff[i+2]=='<'))
			{
				int temp_pckInitIndex=i;
				i+=3;
				//search package size
				if(allBufSize-i >= 4)
				{
					int temp_pckSizeField = ((readBuff[i+3] << 24) & 0xFF000000) | ((readBuff[i+2] << 16) & 0x00FF0000) | ((readBuff[i+1] << 8) & 0x0000FF00) | (readBuff[i] & 0x000000FF);
					i+=4;
					//Verify packag6e size
					if(allBufSize-i >= temp_pckSizeField)
					{
						i+=temp_pckSizeField;
						pckInitIndex = temp_pckInitIndex;	//point to the begining of the last full frame
						pckSizeField = temp_pckSizeField;	//stores the size of the last full frame
						readingOffset = allBufSize - i;
					}
					else
						break;
				}
				else
					break;
			}
		}
		else
			break;
	}

	//now check if there were a full frame on the readed data
	if((pckInitIndex == -1) || (pckSizeField == -1))
	{	
		//Readed data does not contain a full frame but the streaming is alive
		streamActive = true;
		return false;
	}

	//At this point there is a frame on the reading buffer and its location has been found

	//get the frame info
	width = ((readBuff[pckInitIndex+8] << 8)& 0x0000FF00) | (readBuff[pckInitIndex+7]& 0x000000FF);
	height= ((readBuff[pckInitIndex+10] << 8)& 0x0000FF00) | (readBuff[pckInitIndex+9]& 0x000000FF);
	memcpy(pixels, &readBuff[pckInitIndex+11], pckSizeField);

	//Put the remaining data on the begining of the reading buffer
	if(readingOffset!=0)
		memcpy(readBuff, &readBuff[allBufSize-readingOffset], readingOffset);

	return true;

}

unsigned char *	ofxSocketStreamer::getPixels()
{
	return pixels;
}

void ofxSocketStreamer::setVerbose(bool _verbose){
	verbose = _verbose;
}