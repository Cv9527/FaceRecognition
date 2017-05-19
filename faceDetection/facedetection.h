#ifndef FACEDETECTION_H
#define FACEDETECTION_H

/**
* @brief 使用MTCNN进行人脸检测与对齐
* @author wujiyang
* @date 19:36 2017/5/17
*/

#include <chrono>
#include <memory>
#include <Windows.h>

#include "opencv2\opencv.hpp"
#include "MTCNNFaceDetection.inc.h"

using namespace MTCNNFaceDetection;

class FaceDetection
{
public:
	FaceDetection();
	~FaceDetection();
	void loadModel(std::string modelDir, int gpuid = 0);
	std::vector<cv::Rect> faceDetect(cv::Mat img, double minFaceSize = 30, 
		bool output_points = false, vector<vector<Point2d>>& points = vector<vector<Point2d>>());

	std::vector<cv::Mat> getAlignFaces(cv::Mat img, std::vector<cv::Rect> rects, vector<vector<Point2d>>& points);

private:

	MTCNNFaceDetection::MTCNN *cascade;
};

#endif //FACEDETECTION_H