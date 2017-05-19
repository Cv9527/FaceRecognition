#include "facedetection.h"
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
	fd->loadModel(detectionModelFile, 0);

	//����������ȡ����
	std::string modelFile = "../models/center_loss_happynear/center_loss_happynear_new.prototxt";
	std::string trainedFile = "../models/center_loss_happynear/center_loss_happynear.caffemodel";
	FeatureExtractor *fe = new FeatureExtractor();
	fe->loadModel(modelFile, trainedFile, "");

	std::cout << "models load finished..." << std::endl;

	//��Ⲣ��ʾ������ͼ��
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

	//�������ݿ����Ӷ���
	MysqlOperation *mysql = new MysqlOperation();
	mysql->connect2database();

	//�����ļ���
	std::vector<std::string> fileNames;
	std::string fileDirPath = "E:/FaceDataBase";
	getAllFiles(fileDirPath, fileNames);
	long t0 = cv::getTickCount();
	
	for (size_t i = 0; i < fileNames.size(); i++)
	{
		//��һ��ͼ����������⣬�������룬������ȡ�Լ��������
		cv::Mat img = cv::imread(fileNames[i]);

		double min_face_size = 30;
		std::vector<std::vector<cv::Point2d>> points;
		std::vector<cv::Rect> faceRects = fd->faceDetect(img, min_face_size, true, points);
		std::vector<cv::Mat> alignFaces = fd->getAlignFaces(img, faceRects, points);
		
		for (int j = 0; j < alignFaces.size(); j++)
		{
			std::vector<float> feature = fe->getFeature(alignFaces[j], "eltmax_fc5");
			
			std::string featureStr = combineFromVector(feature, "+");

			//mysql�Զ�����б��ת�壬��Ҫ�ټ�һ����б��
			//std::cout << fileNames[i] << std::endl;
			std::string fnd = "\\";
			std::string rep = "\\\\";
			fileNames[i] = fileNames[i].replace(fileNames[i].find(fnd), fnd.length(), rep);
			//std::cout << fileNames[i] << std::endl;
			std::string sqlStr = "insert into picdata(path, feature)values('" + fileNames[i] + "', '" + featureStr + "')";
			
			mysql->insertOperation(sqlStr, i);
		}
		
		if ((i + 1) % 100 == 0)
			std::cout << "processed: " << i + 1 << "��ͼƬ��" << std::endl;
		if ((i == fileNames.size() - 1) && (i + 1) % 1000 != 0)
		{
			mysql_query(&mysql->mydata, "COMMIT");
		}
	}
	long t1 = cv::getTickCount();
	std::cout << "1500��ͼƬ ��������ʱ�� " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;

	delete fd;
	delete fe;

	return 0;
}

