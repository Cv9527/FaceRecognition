#include "facedetection.h"

/**
* @brief 使用MTCNN进行人脸检测
* @author wujiyang
* @date 10:36 2017/3/26
*/


FaceDetection::FaceDetection()
{

}

FaceDetection::~FaceDetection()
{
	if (cascade)
		delete cascade;
}

void FaceDetection::loadModel(std::string modelDir, int gpuid)
{
	cascade = new MTCNN(modelDir, gpuid);
}


std::vector<cv::Rect> FaceDetection::faceDetect(cv::Mat img, double minFaceSize, bool output_points, vector<vector<Point2d>>& points)
{
	std::vector<cv::Rect> rects;

	//auto result = cascade->GetDetectionRect(img, 12 / minFaceSize);
	auto result = cascade->GetDetection(img, 12 / minFaceSize, output_points, points);
	for (int i = 0; i < result.size(); i++) {
		//rectangle(img, result[i].first, Scalar(255, 0, 0), 2);
		//MTCNN检测到的人脸框太小，外扩一下0.1倍
		result[i].first.x = result[i].first.x - result[i].first.width * 0.1;
		result[i].first.y = result[i].first.y - result[i].first.height * 0.05;
		result[i].first.width = result[i].first.width * 1.2;
		result[i].first.height = result[i].first.height * 1.1;
		//判断有没有越界
		if (result[i].first.x  < 0) result[i].first.x = 0;
		if (result[i].first.y  < 0) result[i].first.y = 0;
		if (result[i].first.x + result[i].first.width > img.cols)
			result[i].first.width = img.cols - result[i].first.x;
		if (result[i].first.y + result[i].first.height > img.rows)
			result[i].first.height = img.rows - result[i].first.y;

		rects.push_back(result[i].first);
	}

	return rects;
}

std::vector<cv::Mat> FaceDetection::getAlignFaces(cv::Mat img, std::vector<cv::Rect> rects, vector<vector<Point2d>>& points)
{
	std::vector<cv::Mat> faceImgs;
	
	std::vector<cv::Point2d> targrtCoordinate;
	targrtCoordinate.push_back(cv::Point2d(30.2946, 51.6963));
	targrtCoordinate.push_back(cv::Point2d(65.5318, 51.5014));
	targrtCoordinate.push_back(cv::Point2d(48.0252, 71.7366));
	targrtCoordinate.push_back(cv::Point2d(33.5493, 92.3655));
	targrtCoordinate.push_back(cv::Point2d(62.7299, 92.2041));

	for (size_t i = 0; i < points.size(); i++)
	{
		cv::Mat alignImg;
		cv::Mat transform = cv::estimateRigidTransform(points[i], targrtCoordinate, 0);
		if (((transform.type() == CV_32F || transform.type() == CV_64F) && transform.rows == 2 && transform.cols == 3))
		{
			//必须要能正确的进行刚体转换才行
			cv::warpAffine(img, alignImg, transform, cv::Mat::zeros(112, 96, img.type()).size());
			faceImgs.push_back(alignImg);
		}
		else
		{
			//直接根据检测坐标进行裁切
			cv::Mat crop = img(Rect(rects[i].x, rects[i].y, rects[i].width, rects[i].height));
			cv::Mat tmp;
			cv::resize(crop, tmp, cv::Size(96, 112), 0, 0, CV_INTER_LINEAR);
			faceImgs.push_back(tmp);
			//std::cout << "crop_img_size:" << tmp.rows << ":" << tmp.cols << std::endl;
		}
		
	}
	return faceImgs;
}