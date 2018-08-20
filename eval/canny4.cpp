#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int cvt = CV_BGR2GRAY;
int thrshold = 100;
int rtio = 2;
int kernel_size = 3;
int stepno = 0;

struct PMat {
	Mat mat;
	Mat gray;
	Mat canny;
	bool consumed;
	condition_variable cons;
	condition_variable prod;
	mutex mux;
};

void canny_detection(PMat * pmat) {
	
	while(1) {

		unique_lock<mutex> locker(pmat->mux);
		if (pmat->consumed) {
			pmat->cons.wait(locker);
		}
		
		cv::cvtColor(pmat->mat, pmat->gray, cvt);
		cv::blur(pmat->gray, pmat->gray, Size(3,3));  
		cv::Canny(pmat->gray, pmat->canny, thrshold, thrshold*rtio, kernel_size);
		pmat->consumed = true;
		pmat->prod.notify_all();
	}	
}

int main(int argc, char ** argv) {

	string file_name = "pedestrian.mp4";
	if (argc >= 2) {
		file_name = argv[1];
	}
	Mat frame;
	int level = 2;
	PMat * out[level];
	thread * th[level];
	clock_t start, end;
	string home_path = getenv("HOME");
  	string dfout_path = home_path + "/Documents/df/outputs/";
	string file_in =  home_path + "/Documents/df/" + file_name;
	string file_out;

	start = clock();

	for (int i=0; i<level*level; i++) {
		out[i] = new PMat();
		th[i] = new thread(&canny_detection, out[i]);
	}

	VideoCapture * cap = new VideoCapture(file_in);

	if(!cap->isOpened()){
    		cout << "Error opening video stream or file" << endl;
  	}

	*cap >> frame;
	
	int tilew = frame.cols / level;
        int tileh = frame.rows / level;

	while (!frame.empty()) {
	
		for (int j=0; j<level ; j++) {
			for (int i=0; i<level ; i++) {
				Rect tile(i*tilew, j*tileh, tilew, tileh);
				int idx = j*level+i;
				unique_lock<mutex> locker(out[idx]->mux);
				if (!out[idx]->consumed) {
					out[idx]->prod.wait(locker);
				}
				out[idx]->mat = (frame(tile)).clone();
				out[idx]->consumed = false;
				out[idx]->cons.notify_all();
			}
		}
		file_out = dfout_path + to_string(stepno) + ".png";
		//cv::imwrite(file_out, canny); 
		*cap >> frame;
		cout << "[" << stepno << "]" << " processing frame " << "\n";
		stepno++;
	}

	for (int i=0; i<level*level; i++) {
		th[i]->join();
	}

	end = clock();
	cout << "Execution time = "+to_string(double(end - start)/CLOCKS_PER_SEC) << "\n"; 
	return 0;
}
