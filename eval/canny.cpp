#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char ** argv) {

	string file_name = "pedestrian.mp4";
	if (argc >= 2) {
		file_name = argv[1];
	}

	Mat frame, gray, canny;
	int cvt = CV_BGR2GRAY;
	int threshold = 100;
	int ratio = 2;
	int kernel_size = 3;
	int stepno = 0;
	clock_t start, end;
	string home_path = getenv("HOME");
  	string dfout_path = home_path + "/Documents/df/outputs/";
	string file_in =  home_path + "/Documents/df/" + file_name;
	string file_out;

	start = clock();

	VideoCapture * cap = new VideoCapture(file_in);

	if(!cap->isOpened()){
    		cout << "Error opening video stream or file" << endl;
  	}

	*cap >> frame;
	
	while (!frame.empty()) {
		
		cv::cvtColor(frame, gray, cvt);
		cv::blur(gray, gray, Size(3,3));  
		cv::Canny(gray, canny, threshold, threshold*ratio, kernel_size);
		file_out = dfout_path + to_string(stepno) + ".png";
		cv::imwrite(file_out, canny); 
		*cap >> frame;
		cout << "[" << stepno << "]" << " processing frame " << "\n";
		stepno++;
	}

	end = clock();
	cout << "Execution time = "+to_string(double(end - start)/CLOCKS_PER_SEC) << "\n"; 
	return 0;
}
