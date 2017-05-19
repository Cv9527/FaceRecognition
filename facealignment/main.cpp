#include "facealignment.h"


// ----------------------------------------------------------------------------------------
// Ϊ�˿���Ч�������� facealignment.cpp��getFaceChip()�����м�������ʾ��ʵ��ʹ�õ�ʱ�����ע�͵�
// 
// ----------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
	try
	{
		std::cout << "processing image " << "2.jpg" << std::endl;
		cv::Mat img = cv::imread("../images/1.jpg");

		//����ģ���⵽������λ��
		std::vector<cv::Rect> faceRects;
		faceRects.push_back(cv::Rect(140, 72, 90, 90));
		faceRects.push_back(cv::Rect(126, 196, 90, 105));
		std::cout << "Number of faces detected: " << faceRects.size() << std::endl;
		
		//������������,���������������
		//FaceAlignment fa("shape_predictor_68_face_landmarks.dat");
		FaceAlignment *fa = new FaceAlignment();
		fa->loadModel("../models/shape_predictor_68_face_landmarks.dat");
		for (unsigned long i = 0; i < faceRects.size(); ++i)
		{
			long t0 = cv::getTickCount();
			cv::Mat tmp = fa->getFaceChip(img, faceRects[i]);
			long t1 = cv::getTickCount();
			std::cout << "����ͽ�ȡ������ʱ�� " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;
		
			//��ʾ���ж�����ͼ��
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


