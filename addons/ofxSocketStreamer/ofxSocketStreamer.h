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


#ifndef _OF_SOCKET_STREAMER
#define _OF_SOCKET_STREAMER

#include <iostream>
#include "ofxTCPManager.h"

//This class is added to the Comunity Core Vision project, as an add-on to this project.
//This implementation was developed by the devint team www.devint.co and is released open as it is

/// \brief	This clas opens a socket for reading a image streaming via TCP
///			The image streaming format must be
///			1)Heder:		'<<<'
///			2)packagesize:	int (4 chars) (image_Width+image_Height+image_data)
///			3)image_Width:	word (2 chars)
///			4)image_Height:	word (2 chars)
///			5)image_data:	(packagesize-4 chars)  (The image format must be 24 bit: rgb)
///
///			Example
///					<<<sssswwhhdddddddddddddddd.....ddddd
///					|_||__|||||________________________|
///					 1   2 3 4             5
///
///
///			Note:
///				Although this protocol allows a change the size of the image at any time wile the image are being streamed.
///				The implementation of this program does not allow to change the size of the images.
///				So the images size are defined on the streamer side but that size must remain fixed during the streaming.
///
#define READING_SOCKSTREAM_BUFFER_SIZE	10000000
#define PIXELS_BUFFER_SIZE	10000000
class ofxSocketStreamer{

	public:


		ofxSocketStreamer ();	//imlement
		~ofxSocketStreamer();	

		bool 				openSocket(int socket);	//implement
		void 				closeSocket();
	//	void 				close();

	//	void				update();			//same as idleMovie
	//	void 				idleMovie();		// rename to updateMovie?
	//	void 				play();
	//	void 				stop();

		
	//	float  				speed;

	
		/// \brief	Return true if there is a avaiable new frame to read in the incoming buffer
		///			Also updates the values pixels, height and width
		bool 				isFrameNew();		///Implement
		unsigned char * 	getPixels();		///Implement
	//	float 				getPosition();
	//	float 				getSpeed();
	//	float 				getDuration();
	//	bool				getSocketClosed();

	//	void 				setPosition(float pct);
	//	void 				setVolume(int volume);
	//	void 				setLoopState(int state);
	//	void   				setSpeed(float speed);
	//	void				setFrame(int frame);  // frame 0 = first frame...

	//	void 				setUseTexture(bool bUse);
	//	ofTexture &			getTextureReference();
	//	void 				draw(float x, float y, float w, float h);
	//	void 				draw(float x, float y);

		//the anchor is the point the image is drawn around.
		//this can be useful if you want to rotate an image around a particular point.
    //    void				setAnchorPercent(float xPct, float yPct);	//set the anchor as a percentage of the image width/height ( 0.0-1.0 range )
    //    void				setAnchorPoint(int x, int y);				//set the anchor point in pixels
    //    void				resetAnchor();								//resets the anchor to (0, 0)

	//	void 				setPaused(bool bPause);

	//	int					currentLoopState;
	//	int					getCurrentFrame();
	//	int					getTotalNumFrames();

	//	void				firstFrame();
	//	void				nextFrame();
	//	void				previousFrame();

		inline int 			getHeight(){return height;}
		inline int			getWidth(){return width;}

	//	bool				isPaused();
		bool				isConnected(){return socketConnected;}
		bool				isStreaming(){return streamActive;}

		void				setVerbose(bool _vervose);
/*
		//--------------------------------------
		#ifdef OF_VIDEO_PLAYER_QUICKTIME
		//--------------------------------------
			MovieDrawingCompleteUPP myDrawCompleteProc;
			MovieController  	thePlayer;
			GWorldPtr 			offscreenGWorld;
			Movie 			 	moviePtr;
			unsigned char * 	offscreenGWorldPixels;	// 32 bit: argb (qt k32ARGBPixelFormat)
			void				qtGetFrameCount(Movie & movForcount);
		//--------------------------------------
		#endif
		//--------------------------------------

		//--------------------------------------
		#ifdef OF_VIDEO_PLAYER_GSTREAMER
		//--------------------------------------
		ofGstUtils 			gstUtils;
		//--------------------------------------
		#endif
		//--------------------------------------
*/
	//	int					nFrames;				// number of frames
		unsigned char * 	pixels;					// 24 bit: rgb
		bool 				bHavePixelsChanged;
	//	ofTexture 			tex;					// a ptr to the texture we are utilizing
	//	bool 				bUseTexture;			// are we using a texture
	//	bool				allocated;				// so we know to free pixels or not

	protected:

		int port;
		bool connected;
		ofxTCPManager		TCPServer;
		ofxTCPManager		TCPConnectionHandler;
		int 				width, height;
		bool 				socketConnected;
		bool				streamActive;

		char *				readBuff;

		bool				verbose;
/*
		void 				start();
		void 				createImgMemAndGWorld();
		bool 				bStarted;
		bool 				bPlaying;
		bool 				bPaused;
		bool 				bIsFrameNew;			// if we are new

*/


};
#endif	//_OF_SOCKET_STREAMER






