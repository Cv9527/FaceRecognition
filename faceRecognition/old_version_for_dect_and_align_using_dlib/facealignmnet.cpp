#include "facealignment.h"

FaceAlignment::FaceAlignment()
{
	
}

void FaceAlignment::loadModel(const std::string &modelPath)
{
	//加载人脸对齐模型，shape_predictor_68_face_landmarks.dat
	dlib::deserialize(modelPath) >> sp;
}

void FaceAlignment::setAlignmentModel(const std::string &modelPath)
{
	//加载人脸对齐模型
	dlib::deserialize(modelPath) >> sp;
}

cv::Mat FaceAlignment::getFaceChip(cv::Mat imgdata, cv::Rect rect, int cropSize)
{
	//先将mat类型转换为dlib库支持的类型
	dlib::cv_image<dlib::rgb_pixel> img(imgdata);
	//将cv::Rect类型转换成为 dlib::rectangle类型
	dlib::rectangle det(rect.x, rect.y, rect.x + rect.width, rect.y + rect.height);

	//预测姿势，输入为两个参数，一个是图片，另一个是从该图片检测到的边界框
	dlib::full_object_detection shape = sp(img, det);

	//提取每张对齐剪裁后的人脸的副本，旋转和缩放到一个标准尺寸
	dlib::array2d<dlib::rgb_pixel> facechip;
	extract_image_chip(img, get_face_chip_details(shape, cropSize), facechip);

	//转换成mat图像并返回
	cv::Mat result = dlib::toMat(facechip);

	//------------------------------------------------------------------------------------------------
	//输出检测边界框
	/*cv::rectangle(imgdata, cvPoint(cvRound(det.left()), cvRound(det.top())),
		cvPoint(cvRound(det.left() + det.width()), cvRound(det.top() + det.height())),
		CV_RGB(0, 255, 0), 1, 8, 0);*/

	//输出68个特征点位置
	for (size_t i = 0; i < 68; i++)
	{
		circle(imgdata, cvPoint(shape.part(i).x(), shape.part(i).y()), 1, cv::Scalar(0, 0, 255), -1);
	}
	//------------------------------------------------------------------------------------------------

	return result;
}

std::vector<cv::Mat> FaceAlignment::getFaceChips(cv::Mat imgdata, std::vector<cv::Rect> rects, int cropSize)
{
	//将mat类型转换为dlib库支持的类型
	dlib::cv_image<dlib::rgb_pixel> img(imgdata);
	//将cv::Rect类型转换成为 dlib::rectangle类型
	std::vector<dlib::rectangle> dets;
	for (size_t i = 0; i < rects.size(); i++)
	{
		dets.push_back(dlib::rectangle(rects[i].x, rects[i].y, rects[i].x + rects[i].width, rects[i].y + rects[i].height));
	}

	std::vector<cv::Mat> resultdata;

	//处理每个人脸位置
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