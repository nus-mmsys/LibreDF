# TMF

## Introduction

TMF is a framework for modeling stream processing system programs with a dataflow model. A dataflow model is a directed graph in which nodes represent computation tasks (called actors) and edges represent communication between a pair of actors. Modeling stream processing system programs with dataflow allows programs to be parallelized and distributed. TMF implements some OpenCV actors.

## Application development

Application developers specify the application graph in Dataflow Interchange Format (DIF). DIF is a general specification format for arbitrary dataflow models. TMF implements a subset of DIF explained below.

- Topology: The topology of the graph including the nodes and edges is specified in the ```topology``` module.
- Actors: Each actor has a number of properties that can be specified in the ```actor``` module. These properties are metadata that the actor needs during its execution (e.g. its computational behavior).
- Parameters: Parameters of the dataflow can be specified in the ```parameter``` module. 

```
df <name> {
    topology {
        nodes = <actor 1>, ..., <actor n>; 
	    edges = <edge 1> (<source actor>, <sink actor>), ...., <edge m>(<source actor>, <sink actor>);
    }
    actor <actor i> {
        computation = <actor i type>;
        <property x> = <value y>;
        ...
    }
    parameter {
    	<property z> = <value t>;
	...
    }
}
```


### Example (pedestrian detection)

This application reads a video from a file (pedestrian.mp4), detects the pedestrians (full bodies), draws a bounding box around them, and writes the resulting image on a file.

```
df pedestrian_detection {
    topology {
        nodes = A,B,C,D;
        edges = e1(A,B), e2(B,C), e3(C,D), e4(A,D);
    }
    actor A {
        computation = VideoCapture;
        file_name = pedestrian.mp4;
    }
    actor B {
        computation = CvtColor;
    }
    actor C {
        computation = EqualizeHist;
    }
    actor D {
        computation = CascadeClassifier;
	input_gray = e3;
	input_image = e4;
        classifier = haarcascade_fullbody.xml;
    }
}
```

### Example (distributed canny edge detection)
This application reads a video from a file (pedestrian.mp4), sends the frames over TCP connections to a color convertor and a canny edge detector, and another actor to write the decoded frames to png files.
```
df canny {
    topology {
        nodes = A,B,C,D;
	edges = e1(A,B), e2(B,C), e3(C,D);
    }
    actor A {
    	computation = VideoCapture;
        file_name = pedestrian.mp4;
	host = 192.168.1.9;
    }
    actor B {
    	computation = CvtColor;
	host = 192.168.1.10;
	input_port = 7300;
    }
    actor C {
    	computation = Canny;
	threshold = 100;
	ratio = 2;
	host = 192.168.1.9;
	input_port = 7200;
    }
    actor D {
    	computation = ImageWrite;
	host = 192.168.1.10;
	input_port = 7100;
    }
    parameter {
	distributed = true;
	discovery_host = 192.168.1.9;
	discovery_port = 7000;
    }
}
```

## Actor developement

TMF provides functions for actor developers. An actor inherits from the ```Actor``` class and defines a set of ports and their data type while construction. During the initialization, the actor can read the properties set in the dataflow specification in DIF. During the execution, the actor reads from the input ports and write to the output ports. Finally the actor destroys its ports.

- Construction: An actor can create input and output ports using ```createInputPort``` and ```createOutputPort```. For ports with variable number of inputs and outputs ```createInputPortVector``` and ```createOutputPortVector``` are used.
- Initialization: During the initialization phase, the actor can read its properties. ```propEmpty``` checks if a property value is empty. To retrieve the value of a property ```getProp```, ```getPropInt```, and ```getPropFloat``` are used (for string, integer, and float).
- Execution: During the execution, the actor needs to read from its inputs and to write to its outputs. ```consume``` is called for reading from input ports and ```produce``` for writing on output ports. The ```release``` function is called for releasing the buffers of the ports.
- Destruction: To destroy a port, ```destroyPort``` function is called.

### Example 

```c++
// cvtcolor.h

#include "core/df.h"
#include "tokens/opencv/mat.h"
#include <opencv2/core/core.hpp>
#include <opencv2/core/imgproc.hpp>

class CvtColor: public df::Actor {
private:
  cv::Mat frame;
  df::InputPort<df::Mat> * input;
  df::OutputPort<df::Mat> * output;
  int cvt;
  static df::ActorRegister<CvtColor> reg;
public:
  CvtColor(const string& name);
  virtual void init();
  virtual void run();
  virtual ~CvtColor();
};
```

```c++
// cvtcolor.cpp

#include "cvtcolor.h"

using namespace df;

ActorRegister<CvtColor> CvtColor::reg("CvtColor");

CvtColor::CvtColor(const string& name) : Actor(name) {
  input = createInputPort<df::Mat>("input");
  output = createOutputPort<df::Mat>("output");
}

void CvtColor::init() {
  if (!propEmpty("cvt"))
        cvt = getPropInt("cvt");
  else
        cvt = CV_BGR2GRAY;
}

void CvtColor::run() {
  auto in = consume(input);
  auto out = produce(output);
  cv::cvtColor(*in->get(), *out->get(), cvt);
  log("sending frame "+to_string(stepno));
  release(input);
  release(output);
}

CvtColor::~CvtColor() {
  destroyPort(input);
  destroyPort(output);
}
```

### Actor types

The following actor types are already implemented.

    machine-learning
        Convolution         % output:Mat = filter2D(input:Mat) ;
                              /* with modified kernel and anchor */
                              reads a frame, applies a convolution filter on it,
                              and sends the filtered frame.
        Pool                % output:Mat = resize(input:Mat) ;
                              reads a frame, re-scales it,
                              and sends the re-scaled frame.

    opencv-core
        Add                 % output:Mat = input1:Mat + input2:Mat ;
                              adds two frames and sends the result.
        Compare             % output:Mat = compare(input1:Mat, input2:Mat, operation) ;
                              compares two frames and sends the result.
                              opertions are "eq, gt, ge, lt, le, ne".
        Dct                 % output:Mat = dct(input1:Mat) ;
                              sends the dct of the input.
        Dft                 % output:Mat = dft(input1:Mat) ;
                              sends the dft of the input.
        Gemm                % output:Mat = gemm(input1:Mat, input2:Mat) ;
                              sends the general matrix multiplication between
                              intput1 and input2.
        Idct                 % output:Mat = idct(input1:Mat) ;
                              sends the idct of the input.
        Idft                 % output:Mat = idft(input1:Mat) ;
                              sends the idft of the input.
        MatMerge            % splits a frame into tiles
        MatSplit            % merges the tiles frames into one frame.
        Multiply            % output:Mat = multiply(input1:Mat, input2:Mat, scale) ;
                              sends the per-element scaled multiplication between
                              intput1 and input2.
        Randn               % output:Mat = randn() ;
                              sends a normally-distributed random frame. 	
        Randu               % output:Mat = randu() ;
                              sends a uniformly-distributed random frame.

    opencv-imgproc
        BoxFilter           % output:Mat = boxFilter(input:Mat) ;
                              reads a frame, applies a dilate (mean) filter on it,
                              and sends the filtered frame.
        Canny               % output:Mat = Canny(input:Mat) ;
                              reads a frame, applies a Canny filter on it,
                              and sends the filtered frame.
        CvtColor            % output:Mat = cvtColor(input:Mat, BGR2GRAY) ;
                              reads a frame, applies converts its colors to gray,
                              and sends the filtered frame.
        Dilate              % output:Mat = dilate(input:Mat) ;
                              reads a frame, applies a dilate (max) filter on it,
                              and sends the filtered frame.
        EqualizeHist        % output:Mat = equalizeHist(input:Mat) ;
                              reads a frame, applies equalize histogram filter,
                              and sends the filtered frame.
        Erode               % output:Mat = erode(input:Mat) ;
                              reads a frame, applies an erode (min) filter on it,
                              and sends the filtered frame.
        Filter2D            % output:Mat = filter2D(input:Mat) ;
                              reads a frame, applies a filter2d on it,
                              and sends the filtered frame.
        Sobel               % output:Mat = Sobel(input:Mat) ;
                              reads a frame, applies a Sobel filter on it,
                              and sends the filtered frame.

    opencv-highgui
        ImageRead           % output:Mat = imread() ; 
                              reads an image from a dataset and writes it to its output port.
        ImageWrite          % imwrite(input:Mat) ;
                              reads a frame from its input port and writes it to a png file.
        VideoCapture        % output:Mat = VideoCapture() ;
                              reads a frame from a video stream and sends it.
        VideoWriter         % VideoWriter.write(input:Mat) ;
                              reads a frame and writes it to a video stream.

    opencv-video

    opencv-objdetect
        CascadeClassifier   % reads a gray frame from one port, detects the objects, 
                              reads a color frame from another port, 
                              draws rectangles containing chosen objects 
                              (e.g. face, eye, full body, etc.) and 
                              writes the result to a png file.

    opencv-ml

    basic
        Addition            % reads two integers x and y and sends x+y.
        Doubling            % reads an integer x and sends 2*x.
        Duplicate           % reads a string s and sends ss.
        Increment           % reads an integer x and sends x+1.
        IntConsumer         % reads an integer.
        IntProducer         % sends an integer [1..100].
        Multiplication      % reads two integers x and y and sends x*y.
        StringConsumer      % reads a string.
        StringProducer      % sends a string ["1".."100"].

## Parallelization

Data parallelization is a mean to improve the latency and throughput of a dataflow. In data parallelization, a token is split into several tokens to be processed by different actors in parallel.

Data parallelization is possible only if the smaller tokens after splitting do not have dependencies. For example, in the Canny edge detection, after capturing a video frame, it is possible to split a token into multiple tokens and perform Canny edge detection on smaller parts.

Two special actors called ```MatSplit``` and ```MatMerge``` are provided in TMF for splitting and merging OpenCV ```Mat``` structure. These actors use the ports of variable arity (```createInputPortVector``` and ```createOutputPortVector```).

## Distribution

Computation powers can be limited limited and as a result, the execution time of an actor on a local processor can become too costly. In such cases, if the communication bandwidth is high enough, the actor can be distributed on a remote processor. 

TMF provides support for such actor distribution using two tools. 

- tmf-deploy: It deploys a dataflow on multiple servers. It communicate with specified tmf-servers. 
- tmf-server: receives command from tmf-deploy and executes only actors specified for the machine on which it is running. tmf-server runs on all machines hosting actors.

The application developer specifies on which machine and which port each actor should run. Then the specification (in DIF format) is passed to the tmf-deploy. The dataflow must also specify the host on which the actor discovery service is running, so that the actors can find the machine addresses and port number of the machine they need to connect to.

## Libraries

The repository contains the following libraries :

- libdataflow : dataflow, actor, input/output port, buffer, token, synchronizer, and socket implementation as well as actor implementations and token serialization and deserialization.
- libtmfparser : parser for Dataflow Intechange Format (DIF)

## Getting started

### Build

```bash
  cd build
  ./build.sh
  # or
  ./build.sh debug # for debuging
```

### Run


#### tmf

```tmf``` displays and analyses the graph and runs the graph on shared memory and on tcp locally.

```bash
  ./tmf <df file> (e.g., ../test/opencv/pedestrian_detection.df)
```

The following commands can be used.

	graph		        display the graph.
	h		        display help menu.
	run		        run the graph on shared memory.
	runtcp		        run the graph on tcp.

#### tmf-server

```tmf-server``` runs part of the graph deployed on a server.

```bash
  ./tmf-server
```

#### tmf-deploy

```tmf-deploy``` deploys a dataflow on multiple servers.

```bash
  ./tmf-deploy <df file> (e.g., ../test/opencv/canny.df)
```

### Folder structure

	build
	src
	    analyse
	    deploy
	    parser
	    rdf
	    server
	    dataflow
	    	core
	        actors
	            machine-learning
	            opencv-core
	            opencv-highgui
	            opencv-imgproc
	            opencv-objdetect
	            basic
		tokens
		    opencv
		    basic
	test
	    machine-learning
	    opencv
	    basic
	LICENCE
	README.md
