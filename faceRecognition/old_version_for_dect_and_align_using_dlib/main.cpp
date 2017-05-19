#include "facedetection.h"
#include "facealignment.h"
#include "mysqloperation.h"
#include "utils.h"
#include <featureextractor.h>
#include <opencv2\opencv.hpp>

/**
* @brief 集成人脸检测，人脸对齐与人脸特征提取
* @author wujiyang
* @date 10:39 2017/4/5
*/


int main(int argc, char* argv[])
{
	//构造人脸检测及对齐对象
	FaceDetection *fd = new FaceDetection();
	std::string detectionModelFile = "../models/detectionModel/";
	std::string alignmentFile = "../models/shape_predictor_68_face_landmarks.dat";
	fd->loadModel(detectionModelFile, alignmentFile, 0);

	//构造特征提取对象
	std::string modelFile = "../models/center_loss_happynear/center_loss_happynear_new.prototxt";
	std::string trainedFile = "../models/center_loss_happynear/center_loss_happynear.caffemodel";
	FeatureExtractor *fe = new FeatureExtractor();
	fe->loadModel(modelFile, trainedFile, "");

	std::cout << "models load finished..." << std::endl;

	//检测并显示对齐后的图像
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
	//构造数据库连接对象
	//MysqlOperation *mysql = new MysqlOperation();
	//mysql->connect2database();

	//遍历文件夹
	//std::vector<std::string> fileNames;
	//std::string fileDirPath = "E:/FaceDataBase";
	//getAllFiles(fileDirPath, fileNames);
	//long t0 = cv::getTickCount();
	//for (size_t i = 0; i < fileNames.size(); i++)
	//{
	//	//对一张图进行人脸检测，人脸对齐，特征提取以及特征入库
	//	cv::Mat img = cv::imread(fileNames[i]);

	//	double min_face_size = 20;
	//	//long t0 = cv::getTickCount();
	//	std::vector<cv::Rect> faceRects = fd->faceDetect(img, min_face_size);
	//	//long t1 = cv::getTickCount();
	//	//std::cout << "图片" << i+1 << ".jpg 检测耗时： " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;

	//	for (int j = 0; j < faceRects.size(); j++) 
	//	{
	//		rectangle(img, faceRects[j], Scalar(255, 0, 0), 2);
	//		//long t0 = cv::getTickCount();
	//		cv::Mat alignedImg = fd->getFace(img, faceRects[j]);
	//		std::vector<float> feature = fe->getFeature(alignedImg);
	//		//long t1 = cv::getTickCount();
	//		//std::cout << "人脸片段" << j + 1 << ".jpg 对齐+特征提取耗时： " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;

	//		std::string featureStr = combineFromVector(feature, "+");
	//		
	//		//插入到数据库中
	//		//std::string sqlStr = "insert into picdata(path, feature)values('" + fileNames[i] + "', '" + featureStr + "')";
	//		//mysql->insertOperation(sqlStr);
	//	}
	//	/*cv::imshow("original", img);
	//	cv::waitKey();
	//	cv::destroyWindow("original");*/
	//	//if ((i + 1) % 100 == 0)
	//		//std::cout << "processed: " << i + 1 << "张图片！" << std::endl;
	//}
	////long t1 = cv::getTickCount();
	////std::cout << "1500张图片 特征入库耗时： " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;

	delete fd;
	delete fe;

	return 0;
}

/**
*
* 我觉得把检测和对齐放在一起 好像不是很好，对齐模型相比较检测而言比较大，开多线程的时候可能比较占用资源
*
**/