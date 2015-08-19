#include <iostream>
#include <fstream>
#include <sstream>

#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2\ml\ml.hpp>

#define DEBUG


int main(int argc, char **argv) {
	const int imgNo = 8;
	//CvSVM::train Ҫ���������ݴ洢��float ���͵�Mat��	
	cv::Mat feature;

	for (int i = 0; i < imgNo; i++) {
		std::stringstream index;
		index << i;

		std::string fileName = "hogData/" + index.str() + ".png";

#ifdef DEBUG
		std::cout << fileName << std::endl;
#endif

		cv::Mat img = cv::imread(fileName);
		if (img.empty()) {
			std::cerr << "no image..." << std::endl;
			exit(1);
		}

		cv::Mat gray;
		cv::cvtColor(img, gray, CV_RGB2GRAY);

		//cv::HOGDescriptor hog(cv::Size(64, 64), cv::Size(16, 16), cv::Size(8,8), cv::Size(8, 8), 9);

		cv::HOGDescriptor hog;
		std::vector<float> desc;
		std::vector<cv::Point> loc;

		hog.compute(gray, desc, cv::Size(0, 0), cv::Size(0, 0), loc);

#ifdef DEBUG
		std::cout << "hog descriptor size is " << hog.getDescriptorSize() << std::endl;
#endif
			
		cv::Mat row = cv::Mat(desc, true).reshape(1, 1);

		if (feature.empty()) {
			feature = row.clone();
		}
		else {
			feature.push_back(row);
		}

	}

#ifdef DEBUG
	std::cout << "feature size: " << feature.size() << std::endl;
#endif

	float lb[imgNo] = { 1, 1, 1, 1, 1, 1, 0, 0 };
	cv::Mat labels = cv::Mat(imgNo, 1, CV_32FC1, lb);

	cv::SVMParams params;
	params.svm_type = cv::SVM::C_SVC;
	params.kernel_type = cv::SVM::LINEAR;
	params.term_crit = cv::TermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

	cv::SVM svm;
	svm.train(feature, labels, cv::Mat(), cv::Mat(), params);
		
	std::cout << svm.predict(feature.row(7)) << std::endl;

	return 0;
}