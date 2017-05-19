#include "facealignment.h"

FaceAlignment::FaceAlignment()
{
	
}

void FaceAlignment::loadModel(const std::string &modelPath)
{
	//������������ģ�ͣ�shape_predictor_68_face_landmarks.dat
	dlib::deserialize(modelPath) >> sp;
}

void FaceAlignment::setAlignmentModel(const std::string &modelPath)
{
	//������������ģ��
	dlib::deserialize(modelPath) >> sp;
}

cv::Mat FaceAlignment::getFaceChip(cv::Mat imgdata, cv::Rect rect, int cropSize)
{
	//�Ƚ�mat����ת��Ϊdlib��֧�ֵ�����
	dlib::cv_image<dlib::rgb_pixel> img(imgdata);
	//��cv::Rect����ת����Ϊ dlib::rectangle����
	dlib::rectangle det(rect.x, rect.y, rect.x + rect.width, rect.y + rect.height);

	//Ԥ�����ƣ�����Ϊ����������һ����ͼƬ����һ���ǴӸ�ͼƬ��⵽�ı߽��
	dlib::full_object_detection shape = sp(img, det);

	//��ȡÿ�Ŷ�����ú�������ĸ�������ת�����ŵ�һ����׼�ߴ�
	dlib::array2d<dlib::rgb_pixel> facechip;
	extract_image_chip(img, get_face_chip_details(shape, cropSize), facechip);

	//ת����matͼ�񲢷���
	cv::Mat result = dlib::toMat(facechip);

	//------------------------------------------------------------------------------------------------
	//������߽��
	/*cv::rectangle(imgdata, cvPoint(cvRound(det.left()), cvRound(det.top())),
		cvPoint(cvRound(det.left() + det.width()), cvRound(det.top() + det.height())),
		CV_RGB(0, 255, 0), 1, 8, 0);*/

	//���68��������λ��
	for (size_t i = 0; i < 68; i++)
	{
		circle(imgdata, cvPoint(shape.part(i).x(), shape.part(i).y()), 1, cv::Scalar(0, 0, 255), -1);
	}
	//------------------------------------------------------------------------------------------------

	return result;
}

std::vector<cv::Mat> FaceAlignment::getFaceChips(cv::Mat imgdata, std::vector<cv::Rect> rects, int cropSize)
{
	//��mat����ת��Ϊdlib��֧�ֵ�����
	dlib::cv_image<dlib::rgb_pixel> img(imgdata);
	//��cv::Rect����ת����Ϊ dlib::rectangle����
	std::vector<dlib::rectangle> dets;
	for (size_t i = 0; i < rects.size(); i++)
	{
		dets.push_back(dlib::rectangle(rects[i].x, rects[i].y, rects[i].x + rects[i].width, rects[i].y + rects[i].height));
	}

	std::vector<cv::Mat> resultdata;

	//����ÿ������λ��
	for (size_t i = 0; i < dets.size(); i++)
	{
		dlib::full_object_detection shape = sp(img, dets[i]);
		dlib::array2d<dlib::rgb_pixel> facechip;
		extract_image_chip(img, get_face_chip_details(shape, cropSize), facechip);
		cv::Mat tmp = dlib::toMat(facechip);
		resultdata.push_back(tmp);
	}

	return resultdata;
}