#ifndef FACEALIGNMENT_H
#define FACEALIGNMENT_H

#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <iostream>

class FaceAlignment
{
public: 

	//加载人脸对齐模型
	FaceAlignment();

	void loadModel(const std::string &modelPath);
	
	//输入：一张图片和一个人脸位置
	//返回：这个人脸位置对齐后的人脸数据
	cv::Mat getFaceChip(cv::Mat imgdata, cv::Rect rect);
	
	//输入：一张图片和一组人脸位置
	//返回：这组人脸位置对齐后的人脸数据
	std::vector<cv::Mat> getFaceChips(cv::Mat imgdata, std::vector<cv::Rect> rects);

private:

	//人脸位置形状预测器
	dlib::shape_predictor sp;
};

#endif //FACEALIGNMENT_H