1) Tiny Multimedia Framework (tmf)
===============================

Tiny Multimedia Framework is a simple multimedia framework to implement multimedia application. A multimedia application consists of a pipeline and a series of filters connected to each other. An application developer need to know about filter types, and how to connect them to each other.

Each filter has a number of input and output ports. Each port has a buffer type and can be connected to other ports with the same type of data. Filters with same type of ports can be connected to each other. A port push the data to its accessors.

In section 2 we explain how a multimedia application developers create an application using the APIs of the framework. In section 3 we explain how plug-in developers create a new filter using APIs of the framework.

2) Folder structure
===================

Here is how the project is organized.

	build (cmake build directory)
	core (core package)
	doc (documentations)
	  design
	    tmf.docx (desing document in docx)
	    tmf.pdf (design document in pdf)
	    tmf.eap (UML diagrams in Enterprise Architect)
	  latex (reference manuals in latex)
	  html (reference manuals in html)
	examples (contains examples using tmf library)
	filters (implementation of the filters)
	tmffactory (implementations of factories used by application developers)
	tools (tools to be used by filter implementors)
	types (implementation of new buffer types)
	LICENCE
	README.md
	tmf.doxyfile

3) Installation
===============
```bash
	cd build
	./build.sh
	# or
	./build-debug.sh # for debuging
```
Before compilation make sure that the following libraries are installed:

	libavformat-dev
	libavcodec-dev
	libavutil-dev
	libswscale-dev
	libsdl1.2-dev
	zlib1g-dev

4) Development envirement
======================
Tiny Multimedia Framework is configured to be developed under KDevelop IDE.
	
	kdevelop -p tmf.kdev4
	
The build directory:

	build-kdevelop

5) User manual
===========

To create an application we need to create the pipeline as well as the filters, connect filters to each other and run the pipeline. TMF provides APIs to do it.

Example:
```c++
	TMF tmf;
	// create pipeline
	tmf.createPipeline("Player");
	// create decoder filter
	Filter* videoDecoder = tmf.createFilter(VIDEO_DECODER_FILTER, "videoDecoder");
	// create encoder filter
	Filter* videoEncoder = tmf.createFilter(VIDEO_ENCODER_FILTER, "videoEncoder"); 
	// create muxer filter
	Filter* videoMuxer = tmf.createFilter(VIDEO_MUXER_FILTER, "videoMuxer");

	// set properties of the filters
	videoDecoder->setProp("input_video", "video.mp4");
	videoEncoder->setProp("bitrate", "1000000");
	videoEncoder->setProp("framerate", "25");
	videoEncoder->setProp("output_video", "video.avi");

	// connect filters to each other
	pipe->connectFilters(videoDecoder, imageScaler);
	pipe->connectFilters(imageScaler, videoEncoder);
	pipe->connectFilters(videoEncoder, videoMuxer);
	
	// initialize and run the pipeline
	pipe->init();
	pipe->run();

	// destroy the pipeline
	tmf.destroyPipeline(pipe);
```	

6) Plugin developer manual
=======================

Plug-in developers must implement new filters in filter folder. A filter inherits from the Filter class. Each filter must define a set of ports and their data type. The filter then implements init() and process() functions.

Here is an example of a filter implementation:
```c++
	class ImageScalerFilter : public Filter {

	private:

		// This is a class implemented in "tools" to ease implementing this filter
		VideoScaler * videoScaler;

		// input port which has the buffer of RawFrame type
		InputPort<RawFrame> * inputPortFrame;
		
		// output port which has the buffer of RawFrame type
		OutputPort<RawFrame> * outputPortFrame;

	public:

		// filter constructor - it has to call the constructor of the Filter as well
		ImageScalerFilter(string name) : Filter(name) {

			inputPortFrame = new InputPort<RawFrame>("imageScaler, input, Frame",
				this);

			outputPortFrame = new OutputPort<RawFrame>("imageScaler, output, Frame",
				this);

			// all ports of subclass filter must be added to the list of superclass filters
			inputPorts.push_back(inputPortFrame);
			outputPorts.push_back(outputPortFrame);

			videoScaler = 0;
		}

		FilterStatus init() {

			MessageError err;

			// get the properties which are set by the user
			string width = getProp("width");
			string height = getProp("height");

			int dstWidth = std::stoi(width);
			int dstHeight = std::stoi(height);

			int srcWidth, srcHeight, srcFormatInt;

			// read input message from previous filter
			err = inMsg->getPropInt("width", srcWidth);
			if (err == MSG_NOT_FOUND)
				return FILTER_WAIT_FOR_INPUT;

			err = inMsg->getPropInt("height", srcHeight);
			if (err == MSG_NOT_FOUND)
				return FILTER_WAIT_FOR_INPUT;

			err = inMsg->getPropInt("format", srcFormatInt);
			if (err == MSG_NOT_FOUND)
				return FILTER_WAIT_FOR_INPUT;

			AVPixelFormat srcFormat = static_cast<AVPixelFormat>(srcFormatInt);

			videoScaler = new VideoScaler(srcWidth, srcHeight, srcFormat, dstWidth, dstHeight, srcFormat);

			for (int i=0; i<outputPortFrame->getBuffer()->getSize(); i++) {
				RawFrame * frame = outputPortFrame->getBuffer()->getNode(i);
				frame->fill(dstWidth, dstHeight, srcFormat);
			}

			// write output message for the next filters
			outMsg->setProp("width", width);
			outMsg->setProp("height", height);
			outMsg->setPropInt("format", srcFormatInt);

			return FILTER_SUCCESS;

		}

		FilterStatus process() {

			// read input frame from the input port
			RawFrame * inFrame = inputPortFrame->read();

			// Get a reference to the output frame from the output port
			RawFrame * outFrame = outputPortFrame->getBuffer()->getNextNode();

			// scale the frame
			videoScaler->scale(inFrame, outFrame);
			
			// push the putput frame so that the next filters start using them
			outputPortFrame->produce(outFrame);
			
			// processing the output port executes next filters
			outputPortFrame->process();

			return FILTER_SUCCESS;
		}


		~ImageScalerFilter() {
			delete inputPortFrame;
			delete outputPortFrame;
		}
	};
```
