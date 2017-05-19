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
	if (facealign)
		delete facealign;
}

void FaceDetection::loadModel(const std::string detectModelDir, const std::string alignModelPath, const int gpuid)
{
	cascade = new MTCNN(detectModelDir, gpuid);
	facealign = new FaceAlignment();
	facealign->loadModel(alignModelPath);
}


std::vector<cv::Rect> FaceDetection::faceDetect(cv::Mat img, const double minFaceSize)
{
	std::vector<cv::Rect> rects;

	auto result = cascade->GetDetectionRect(img, 12 / minFaceSize);
	for (int i = 0; i < result.size(); i++) {
		//rectangle(img, result[i].first, Scalar(255, 0, 0), 2);
		//MTCNN检测到的人脸框太小，外扩一下0.1倍
		result[i].first.x = result[i].first.x - result[i].first.width * 0.2;
		result[i].first.y = result[i].first.y - result[i].first.height * 0.2;
		result[i].first.width = result[i].first.width * 1.4;
		result[i].first.height = result[i].first.height * 1.3;
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

cv::Mat FaceDetection::getFace(cv::Mat imgdata, cv::Rect rect, const int cropSize)
{
	cv::Mat faceImg;
	faceImg = facealign->getFaceChip(imgdata, rect, cropSize);
	return faceImg;
}

std::vector<cv::Mat> FaceDetection::getFaces(cv::Mat img, std::vector<cv::Rect> rects, const int cropSize)
{
	std::vector<cv::Mat> faceImgs;
	/*for (size_t i = 0; i < rects.size(); i++)
	{
		cv::Mat tmp = img(Rect(rects[i].x, rects[i].y, rects[i].width, rects[i].height));
		faceImgs.push_back(tmp);
	}*/
	faceImgs = facealign->getFaceChips(img, rects, cropSize);
	return faceImgs;
}