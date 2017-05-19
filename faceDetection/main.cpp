/**
* @brief 对封装的人脸检测类进行测试
* @author wujiyang
* @date 11:02 2017/3/26
*/

#include "facedetection.h"

int CaptureDemo(MTCNN cascade);

int main(int argc, char* argv[])
{
	
	FaceDetection *fd = new FaceDetection();

	string model_folder = "../models/detectionModel/";
	fd->loadModel(model_folder, 0);

	string imgname[10] = { "1.jpg", "2.jpg", "3.jpg", "4.jpg", "5.jpg", "6.jpg", "7.jpg", "8.jpg", "9.jpg", "10.jpg"};
	//string imgname[3] = { "2.jpg", "9.jpg", "10.jpg" };
	
	for (size_t i = 0; i < sizeof(imgname) / sizeof(imgname[0]); i++)
	{
		cv::Mat img = imread("../images/" + imgname[i]);
		double min_face_size = 30;
		
		std::vector<std::vector<cv::Point2d>> points;
		long t0 = cv::getTickCount();
		std::vector<cv::Rect> results = fd->faceDetect(img, min_face_size, true, points);
		long t1 = cv::getTickCount();
		std::cout << "face num:" << results.size() << std::endl;
		std::vector<cv::Mat> alignFaces = fd->getAlignFaces(img, results, points);
		for (int i = 0; i < alignFaces.size(); i++)
		{
			/*imshow("align", alignFaces[i]);
			waitKey(0);
			destroyWindow("align");*/
		}
		for (int j = 0; j < results.size(); j++) {
			rectangle(img, results[j], Scalar(255, 0, 0), 2);
		}

		std::cout << "图片" << i+1 << ".jpg 检测耗时： " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;

		imshow("capture", img);
		waitKey(0); 
		destroyWindow("capture");
	}
	
	system("pause");
	return 0;
}

//调用摄像头
int CaptureDemo(MTCNN cascade) {
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		return -1;
	}
	Mat frame;
	Mat edges;

	bool stop = false;
	Rect_<double> bakFaceRect;
	int bak_time = 0;
	while (!stop) {
		cap >> frame;
		if (frame.empty()) {
			cout << "cannot read from camera!" << endl;
			Sleep(100);
			continue;
		}
		//vector<vector<Point2d>> points;
		std::chrono::time_point<std::chrono::system_clock> p0 = std::chrono::system_clock::now();
		double min_face_size = 10;
		//auto result = cascade.GetDetection(frame, 12 / min_face_size, 0.7, true, 0.7, true, points);
		auto result = cascade.GetDetectionRect(frame, 12 / min_face_size, 0.95);
		std::chrono::time_point<std::chrono::system_clock> p1 = std::chrono::system_clock::now();
		cout << "detection time:" << (float)std::chrono::duration_cast<std::chrono::microseconds>(p1 - p0).count() / 1000 << "ms" << endl;
		for (int i = 0; i < result.size(); i++) {
			rectangle(frame, result[i].first, Scalar(255, 0, 0), 4);
			//for (int p = 0; p < 5; p++) {
			//	circle(frame, points[i][p], 2, Scalar(0, 255, 255), -1);
			//}
		}
		//resize(image, image, Size(0, 0), 0.5, 0.5);
		imshow("capture", frame);
		waitKey(1);
	}
}


