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

	//������������ģ��
	FaceAlignment();

	void loadModel(const std::string &modelPath);
	
	//���룺һ��ͼƬ��һ������λ��
	//���أ��������λ�ö�������������
	cv::Mat getFaceChip(cv::Mat imgdata, cv::Rect rect);
	
	//���룺һ��ͼƬ��һ������λ��
	//���أ���������λ�ö�������������
	std::vector<cv::Mat> getFaceChips(cv::Mat imgdata, std::vector<cv::Rect> rects);

private:

	//����λ����״Ԥ����
	dlib::shape_predictor sp;
};

#endif //FACEALIGNMENT_H