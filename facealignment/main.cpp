#include "facealignment.h"


// ----------------------------------------------------------------------------------------
// 为了看出效果，我在 facealignment.cpp中getFaceChip()函数中加入了显示，实际使用的时候可以注释掉
// 
// ----------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	try
	{
		std::cout << "processing image " << "2.jpg" << std::endl;
		cv::Mat img = cv::imread("../images/1.jpg");

		//这里模拟检测到的人脸位置
		std::vector<cv::Rect> faceRects;
		faceRects.push_back(cv::Rect(140, 72, 90, 90));
		faceRects.push_back(cv::Rect(126, 196, 90, 105));
		std::cout << "Number of faces detected: " << faceRects.size() << std::endl;
		
		//进行人脸对齐,构造人脸对齐对象
		//FaceAlignment fa("shape_predictor_68_face_landmarks.dat");
		FaceAlignment *fa = new FaceAlignment();
		fa->loadModel("../models/shape_predictor_68_face_landmarks.dat");
		for (unsigned long i = 0; i < faceRects.size(); ++i)
		{
			long t0 = cv::getTickCount();
			cv::Mat tmp = fa->getFaceChip(img, faceRects[i]);
			long t1 = cv::getTickCount();
			std::cout << "对齐和截取操作耗时： " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;
		
			//显示裁切对齐后的图像
			cv::imshow("facechip", tmp);
			std::cout << tmp.size() << std::endl;
			cv::waitKey();
			cv::destroyWindow("facechip");
		}

		cv::imshow("original", img);
		cv::waitKey();
		cv::destroyWindow("original");
	}
	catch (std::exception& e)
	{
		std::cout << "\nexception thrown!" << std::endl;
		std::cout << e.what() << std::endl;
	}

	return 0;
}


