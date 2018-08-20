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

class PMat {
private:
	condition_variable cons;
	condition_variable prod;
	mutex mux;
	bool consumed;
public:
	Mat mat;
	bool last;

	PMat() {
		last = false;
		consumed = true;
	}
	void consumerLock() {
		unique_lock<mutex> locker(mux);
		while (consumed)
			cons.wait(locker);
	}
	void consumerUnlock() {
		lock_guard<mutex> locker(mux);
		consumed = true;
		prod.notify_all();
	}
	void producerLock() {
		unique_lock<mutex> locker(mux);
		while (!consumed)
			prod.wait(locker);
	}
	void producerUnlock() {
		lock_guard<mutex> locker(mux);
		consumed = false;
		cons.notify_all();
	}
};

string home_path = getenv("HOME");
string dfout_path = home_path + "/Documents/df/outputs/";

int level;
int tilew;
int tileh;
int type;
PMat ** pmat;
PMat ** pcanny;

void canny_detection(int i) {
	bool finished = false;
	int cvt = CV_BGR2GRAY;
	int thrshold = 100;
	int rtio = 2;
	int kernel_size = 3;
	Mat gray;
	while(!finished) {
		pmat[i]->consumerLock();
		if (!pmat[i]->last) {
			cv::cvtColor(pmat[i]->mat, gray, cvt);
			cv::blur(gray, gray, Size(3,3));
			pcanny[i]->producerLock();  
			cv::Canny(gray, pcanny[i]->mat, thrshold, thrshold*rtio, kernel_size);
			pcanny[i]->producerUnlock();
		}

		finished = pmat[i]->last;
		pmat[i]->consumerUnlock();
	}
	pcanny[i]->producerLock();
	pcanny[i]->last = true;
	pcanny[i]->producerUnlock();	
}

void canny_merge() {
	Mat fout = Mat(tileh*level, tilew*level, CV_32FC1);
	bool finished;
	int stepno = 0;
	string file_out;
	while(!finished) {
		for (int j=0; j<level ; j++) {
			for (int i=0; i<level ; i++) {
				Rect tile(i*tilew, j*tileh, tilew, tileh);
				int idx = j*level+i;
				pcanny[idx]->consumerLock();	
				finished = pcanny[idx]->last;
				pcanny[idx]->mat.copyTo(fout(tile));
				pcanny[idx]->consumerUnlock();
			}
		}
		file_out = dfout_path + to_string(stepno) + ".png";
		cv::imwrite(file_out, fout); 
		stepno++;
	}
}

int main(int argc, char ** argv) {

	string file_name = "pedestrian.mp4";
	if (argc >= 2) {
		file_name = argv[1];
	}
	Mat frame;
	Mat fout;
	int stepno = 0;
	level = 2;
	pmat = new PMat*[level];
	pcanny = new PMat*[level];
	thread * th[level];
	thread * thmerge;
	clock_t start, end;
	string file_in =  home_path + "/Documents/df/" + file_name;

	start = clock();

	VideoCapture * cap = new VideoCapture(file_in);

	if(!cap->isOpened()){
    		cout << "Error opening video stream or file" << endl;
  	}

	*cap >> frame;
	
	tilew = frame.cols / level;
        tileh = frame.rows / level;
	type = frame.type();
	
	for (int i=0; i<level*level; i++) {
		pmat[i] = new PMat();
		pcanny[i] = new PMat();
		th[i] = new thread(&canny_detection, i);
	}

	thmerge = new thread(&canny_merge);
	
	while (!frame.empty()) {
	
		for (int j=0; j<level ; j++) {
			for (int i=0; i<level ; i++) {
				Rect tile(i*tilew, j*tileh, tilew, tileh);
				int idx = j*level+i;
			
				pmat[idx]->producerLock();	
				pmat[idx]->mat = (frame(tile)).clone();
				pmat[idx]->producerUnlock();
			}
		}

		*cap >> frame;
		cout << "[" << stepno << "]" << " processing frame " << "\n";
		stepno++;
	}

	for (int i=0; i<level*level ; i++) {
		pmat[i]->producerLock();
		pmat[i]->last = true;
		pmat[i]->producerUnlock();
	}
		
	for (int i=0; i<level*level; i++) {
		th[i]->join();
	}
	thmerge->join();
	end = clock();
	cout << "Execution time = "+to_string(double(end - start)/CLOCKS_PER_SEC) << "\n"; 
	return 0;
}
