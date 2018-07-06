# TMF

## Build

```bash
  cd build
  ./build.sh
  # or
  ./build.sh debug # for debuging
```

## Run

```bash
  ./tmf ../test/basic/adder.df
```

## Commands

	graph		        display df graph.
	h		        display help menu.
	run		        run the data graph.

## Dataflow Interchange Format (DIF)

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
}
```


## Example (pedestrian detection)
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

## Example (distributed decoding)
This application reads a video from a file (pedestrian.mp4), sends the frames over TCP connections in order to write the decoded frames to png files.
```
df imwrite {
    topology {
        nodes = A,B;
        edges = e1(A,B);
    }
    actor A {
        computation = VideoCapture;
        file_name = pedestrian.mp4;
    }
    actor B {
        computation = ImageWrite;
        host = 127.0.0.1;
        input_port = 7010;
    }
    parameter {
        distributed = true;
        discovery_host = 127.0.0.1;
        discovery_port = 7000;
    }
}
```

## Actor types

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

## Actor developement

New actors must be placed in the ```src/dataflow/actors``` folder. An actor inherits from the Actor class and defines a set of ports and their data type. The actor then implements ```init()``` and ```run()``` functions.

```c++
// add.h

#include "core/df.h"
#include "tokens/opencv/mat.h"
#include <opencv2/core/core.hpp>

class Add: public df::Actor {
private:
  cv::Mat frame;
  df::InputPort<df::Mat> * input1;
  df::InputPort<df::Mat> * input2;
  df::OutputPort<df::Mat> * output;
  static df::ActorRegister<Add> reg;
public:
  Add(const string& name);
  virtual void init();
  virtual void run();
  virtual ~Add();
};
```

```c++
// add.cpp

#include "add.h"

using namespace df;

ActorRegister<Add> Add::reg("Add");

Add::Add(const string& name) : Actor(name) {
  input1 = createInputPort<df::Mat>("input1");
  input2 = createInputPort<df::Mat>("input2");
  output = createOutputPort<df::Mat>("output");
}

void Add::init() {
  // Initializations
}

void Add::run() {
  auto in1 = consume(input1);	
  auto in2 = consume(input2);	
  auto out = produce(output);
  out->set(*in1->get() + *in2->get());
  log("sending "+to_string(stepno));
  release(input1);
  release(input2);
  release(output);
}

Add::~Add() {
  destroyPort(input1);
  destroyPort(input2);
  destroyPort(output);
}
```

## Folder structure

	build
	src
	    dataflow
	        core
	        actors
	            machine-learning
	            opencv-core
	            opencv-highgui
	            opencv-imgproc
	            opencv-objdetect
	            basic
	    shell
	        parser
	        ui
	test
	    machine-learning
	    opencv
	    basic
	LICENCE
	README.md
