#include "facedetection.h"
#include "facealignment.h"
#include "mysqloperation.h"
#include "utils.h"
#include <featureextractor.h>
#include <opencv2\opencv.hpp>

/**
* @brief ����������⣬��������������������ȡ
* @author wujiyang
* @date 10:39 2017/4/5
*/


int main(int argc, char* argv[])
{
	//����������⼰�������
	FaceDetection *fd = new FaceDetection();
	std::string detectionModelFile = "../models/detectionModel/";
	std::string alignmentFile = "../models/shape_predictor_68_face_landmarks.dat";
	fd->loadModel(detectionModelFile, alignmentFile, 0);

	//����������ȡ����
	std::string modelFile = "../models/center_loss_happynear/center_loss_happynear_new.prototxt";
	std::string trainedFile = "../models/center_loss_happynear/center_loss_happynear.caffemodel";
	FeatureExtractor *fe = new FeatureExtractor();
	fe->loadModel(modelFile, trainedFile, "");

	std::cout << "models load finished..." << std::endl;

	//��Ⲣ��ʾ������ͼ��
	cv::Mat img = cv::imread("../images/5.jpg");
	double min_face_size = 30;
	std::vector<cv::Rect> faceRects = fd->faceDetect(img, min_face_size);
	for (int i = 0; i < faceRects.size(); i++)
	{
		cv::Mat alignedImg = fd->getFace(img, faceRects[i]);
		cv::imshow("aligned", alignedImg);
		cv::waitKey();
		cv::destroyWindow("aligned");
		//std::cout << alignedImg.cols << " x " << alignedImg.rows << std::endl;
	}
	//�������ݿ����Ӷ���
	//MysqlOperation *mysql = new MysqlOperation();
	//mysql->connect2database();

	//�����ļ���
	//std::vector<std::string> fileNames;
	//std::string fileDirPath = "E:/FaceDataBase";
	//getAllFiles(fileDirPath, fileNames);
	//long t0 = cv::getTickCount();
	//for (size_t i = 0; i < fileNames.size(); i++)
	//{
	//	//��һ��ͼ����������⣬�������룬������ȡ�Լ��������
	//	cv::Mat img = cv::imread(fileNames[i]);

	//	double min_face_size = 20;
	//	//long t0 = cv::getTickCount();
	//	std::vector<cv::Rect> faceRects = fd->faceDetect(img, min_face_size);
	//	//long t1 = cv::getTickCount();
	//	//std::cout << "ͼƬ" << i+1 << ".jpg ����ʱ�� " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;

	//	for (int j = 0; j < faceRects.size(); j++) 
	//	{
	//		rectangle(img, faceRects[j], Scalar(255, 0, 0), 2);
	//		//long t0 = cv::getTickCount();
	//		cv::Mat alignedImg = fd->getFace(img, faceRects[j]);
	//		std::vector<float> feature = fe->getFeature(alignedImg);
	//		//long t1 = cv::getTickCount();
	//		//std::cout << "����Ƭ��" << j + 1 << ".jpg ����+������ȡ��ʱ�� " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;

	//		std::string featureStr = combineFromVector(feature, "+");
	//		
	//		//���뵽���ݿ���
	//		//std::string sqlStr = "insert into picdata(path, feature)values('" + fileNames[i] + "', '" + featureStr + "')";
	//		//mysql->insertOperation(sqlStr);
	//	}
	//	/*cv::imshow("original", img);
	//	cv::waitKey();
	//	cv::destroyWindow("original");*/
	//	//if ((i + 1) % 100 == 0)
	//		//std::cout << "processed: " << i + 1 << "��ͼƬ��" << std::endl;
	//}
	////long t1 = cv::getTickCount();
	////std::cout << "1500��ͼƬ ��������ʱ�� " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;

	delete fd;
	delete fe;

	return 0;
}

/**
*
* �Ҿ��ðѼ��Ͷ������һ�� �����Ǻܺã�����ģ����Ƚϼ����ԱȽϴ󣬿����̵߳�ʱ����ܱȽ�ռ����Դ
*
**/