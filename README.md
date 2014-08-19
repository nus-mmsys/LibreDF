1) Tiny Multimedia Framework (tmf)
===============================

Tiny Multimedia Framework is a simple multimedia framework to implement multimedia application. A multimedia application consists of a pipeline and a series of filters connected to each other. An application developer need to know about filter types, and how to connect them to each other.

Each filter has a number of unput and output ports. Each port has a buffer type and can be connected to other ports with the same type of data. Filters with same type of ports can be connected to each other.

In section 2 we explain how a multimedia application developers create an application using the APIs of the framework. In section 3 we exlpain how plugin developers create a new filter using APIs of the framework.


2) User manual
===========

To create an application we need to create the pipeline as well as the filters, connect filters to each other and run the pipeline. TMF provides APIs to do it.

Example:

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
	

3) Plugin developer manual
=======================

Plugin developers must implement new filters in filter folder. A filter inherits from the Filter class. Each filter must define a set of ports and their data type. The filter then implements init() and process() functions.

Here is an example of a filter implementation:
	
	class ImageScalerFilter : public Filter {

	private:

		VideoScaler * videoScaler;

		InputPort<RawFrame> * inputPortFrame;
		OutputPort<RawFrame> * outputPortFrame;

	public:

		ImageScalerFilter(string name) : Filter(name) {

			inputPortFrame = new InputPort<RawFrame>("imageScaler, input, Frame",
				this);

			outputPortFrame = new OutputPort<RawFrame>("imageScaler, output, Frame",
				this);

			inputPorts.push_back(inputPortFrame);
			outputPorts.push_back(outputPortFrame);

			videoScaler = 0;
		}

		FilterStatus init() {

			MessageError err;
			FilterStatus status = FILTER_SUCCESS;

			string width = getProp("width");
			string height = getProp("height");

			int dstWidth = std::stoi(width);
			int dstHeight = std::stoi(height);

			int srcWidth, srcHeight, srcFormatInt;

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

			outMsg->setProp("width", width);
			outMsg->setProp("height", height);
			outMsg->setPropInt("format", srcFormatInt);

			return status;

		}

		FilterStatus process() {
			FilterStatus status = FILTER_SUCCESS;

			RawFrame * inFrame = inputPortFrame->read();

			RawFrame * outFrame = outputPortFrame->getBuffer()->getNextNode();

			videoScaler->scale(inFrame, outFrame);

			outputPortFrame->produce(outFrame);
			outputPortFrame->process();

			return status;
		}


		~ImageScalerFilter() {
			delete inputPortFrame;
			delete outputPortFrame;
		}
};