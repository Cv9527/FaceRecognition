#include "facedetection.h"
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
	fd->loadModel(detectionModelFile, 0);

	//构造特征提取对象
	std::string modelFile = "../models/center_loss_happynear/center_loss_happynear_new.prototxt";
	std::string trainedFile = "../models/center_loss_happynear/center_loss_happynear.caffemodel";
	FeatureExtractor *fe = new FeatureExtractor();
	fe->loadModel(modelFile, trainedFile, "");

	std::cout << "models load finished..." << std::endl;

	//检测并显示对齐后的图像
	/*cv::Mat img = cv::imread("../images/6.jpg");
	double min_face_size = 30;
	std::vector<std::vector<cv::Point2d>> points;
	std::vector<cv::Rect> faceRects = fd->faceDetect(img, min_face_size, true, points);
	std::vector<cv::Mat> alignFaces = fd->getAlignFaces(img, faceRects, points);
	for (int i = 0; i < alignFaces.size(); i++)
	{
		cv::imshow("aligned", alignFaces[i]);
		cv::waitKey();
		cv::destroyWindow("aligned");
		std::cout << alignFaces[i].rows << " x " << alignFaces[i].cols << std::endl;
	}*/

	//构造数据库连接对象
	MysqlOperation *mysql = new MysqlOperation();
	mysql->connect2database();

	//遍历文件夹
	std::vector<std::string> fileNames;
	std::string fileDirPath = "E:/FaceDataBase";
	getAllFiles(fileDirPath, fileNames);
	long t0 = cv::getTickCount();
	
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		//对一张图进行人脸检测，人脸对齐，特征提取以及特征入库
		cv::Mat img = cv::imread(fileNames[i]);

		double min_face_size = 30;
		std::vector<std::vector<cv::Point2d>> points;
		std::vector<cv::Rect> faceRects = fd->faceDetect(img, min_face_size, true, points);
		std::vector<cv::Mat> alignFaces = fd->getAlignFaces(img, faceRects, points);
		
		for (int j = 0; j < alignFaces.size(); j++)
		{
			std::vector<float> feature = fe->getFeature(alignFaces[j], "eltmax_fc5");
			
			std::string featureStr = combineFromVector(feature, "+");

			//mysql自动将反斜杠转义，需要再加一个反斜杠
			//std::cout << fileNames[i] << std::endl;
			std::string fnd = "\\";
			std::string rep = "\\\\";
			fileNames[i] = fileNames[i].replace(fileNames[i].find(fnd), fnd.length(), rep);
			//std::cout << fileNames[i] << std::endl;
			std::string sqlStr = "insert into picdata(path, feature)values('" + fileNames[i] + "', '" + featureStr + "')";
			
			mysql->insertOperation(sqlStr, i);
		}
		
		if ((i + 1) % 100 == 0)
			std::cout << "processed: " << i + 1 << "张图片！" << std::endl;
		if ((i == fileNames.size() - 1) && (i + 1) % 1000 != 0)
		{
			mysql_query(&mysql->mydata, "COMMIT");
		}
	}
	long t1 = cv::getTickCount();
	std::cout << "1500张图片 特征入库耗时： " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;

	delete fd;
	delete fe;

	return 0;
}

