#ifndef FACEDETECTION_H
#define FACEDETECTION_H

/**
* @brief 使用MTCNN进行人脸检测
* @author wujiyang
* @date 10:26 2017/3/26
*/

#include <chrono>
#include <memory>
#include <Windows.h>

#include "opencv2\opencv.hpp"
#include "MTCNNFaceDetection.inc.h"
#include "facealignment.h"

using namespace MTCNNFaceDetection;

class FaceDetection
{
public:
	FaceDetection();
	~FaceDetection();
	
	//加载人脸检测和人脸对齐模型
	void loadModel(const std::string detectModelDir, const std::string alignModelPath, const int gpuid = 0);
	//人脸检测，返回坐标
	std::vector<cv::Rect> faceDetect(cv::Mat img, double minFaceSize = 10);
	//返回一组对齐裁切后的人脸，cropSize为裁切后的大小
	std::vector<cv::Mat> getFaces(const cv::Mat img, const std::vector<cv::Rect> rects, const int cropSize = 224);
	//返回一张侧切对齐后的人脸，cropSize为裁切后的大小
	cv::Mat getFace(const cv::Mat imgdata, const cv::Rect rect, const int cropSize = 224);

private:

	MTCNNFaceDetection::MTCNN *cascade;
	FaceAlignment *facealign;
};



#endif //FACEDETECTION_H