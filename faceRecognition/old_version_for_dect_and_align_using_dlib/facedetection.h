#ifndef FACEDETECTION_H
#define FACEDETECTION_H

/**
* @brief ʹ��MTCNN�����������
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
	
	//��������������������ģ��
	void loadModel(const std::string detectModelDir, const std::string alignModelPath, const int gpuid = 0);
	//������⣬��������
	std::vector<cv::Rect> faceDetect(cv::Mat img, double minFaceSize = 10);
	//����һ�������к��������cropSizeΪ���к�Ĵ�С
	std::vector<cv::Mat> getFaces(const cv::Mat img, const std::vector<cv::Rect> rects, const int cropSize = 224);
	//����һ�Ų��ж�����������cropSizeΪ���к�Ĵ�С
	cv::Mat getFace(const cv::Mat imgdata, const cv::Rect rect, const int cropSize = 224);

private:

	MTCNNFaceDetection::MTCNN *cascade;
	FaceAlignment *facealign;
};



#endif //FACEDETECTION_H