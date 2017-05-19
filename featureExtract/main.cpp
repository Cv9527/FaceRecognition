#include <featureextractor.h>
#include <iostream>
#include <opencv2\opencv.hpp>
#include "facedetection.h"

#include <mysql.h>

#pragma comment(lib, "libmysql.lib")

/**
* @brief ����caffe_api_new��������ʶ��ģ��,��ȡͼ������������������,���ݿ���� MySQl 5.7.17
* @author wujiyang
* @date 18:34 2017/3/24
*/
int main()
{
	std::string modelFile = "../models/center_loss/center_loss_deploy_new.prototxt";
	std::string trainedFile = "../models/center_loss/center_loss.caffemodel";
	
	FeatureExtractor *fe = new FeatureExtractor();
	fe->loadModel(modelFile, trainedFile, "");

	FaceDetection *fd = new FaceDetection();
	fd->loadModel("../models/detectionModel/", 0);

	

	std::string imgname[4] = { "../images/3.jpg", "../images/7.jpg", "../images/9.jpg", "../images/10.jpg"};
	for (size_t j = 0; j < 4; j++)
	{
		cv::Mat img = cv::imread(imgname[j]);
		if (img.empty())
		{
			std::cout << "ͼ��" << imgname[j] << "��ȡʧ�ܣ�" << std::endl;
			return 1;
		}

		std::cout << "processing image: " << j + 1 << std::endl;
		std::vector<std::vector<cv::Point2d>> points;
		std::vector<cv::Rect> rects = fd->faceDetect(img, 30, true, points);
		std::vector<cv::Mat> faces = fd->getAlignFaces(img, rects, points);

		for (int i = 0; i < faces.size(); i++)
		{

			/*cv::imshow("face", faces[i]);
			cv::waitKey();*/
			long t0 = cv::getTickCount();
			std::vector<float> feature = fe->getFeature(faces[i], "fc5");
			long t1 = cv::getTickCount();
			std::cout << "����" << i + 1 << ".jpg ��ȡ������ʱ�� " << (t1 - t0) / cv::getTickFrequency() << "s" << std::endl;

			std::cout << "feature.size: " << feature.size() << std::endl;
		}
	}

	

	////���ݿ����
	//MYSQL mydata;
	////��ʼ�����ݿ�
	//if (0 != mysql_library_init(0, NULL, NULL))
	//{
	//	std::cout << "mysql_library_init() failed!" << std::endl;
	//	return -1;
	//}
	////��ʼ�����ݽṹ
	//if (NULL == mysql_init(&mydata))
	//{
	//	std::cout << "mysql_init() failed!" << std::endl;
	//	return -1;
	//}
	////���ݿ�����֮ǰ�ĸ��Ի������������ַ���
	//if (0 != mysql_options(&mydata, MYSQL_SET_CHARSET_NAME, "gbk"))
	//{
	//	std::cout << "mysql_options() failed !" << std::endl;
	//}
	//
	////�������ݿ�
	//if (NULL != mysql_real_connect(&mydata, "localhost", "root", "120629", "facedatabase", 3306, NULL, 0))
	//{
	//	std::cout << "connect succeed!" << std::endl;
	//}
	//else
	//{
	//	std::cout << "connect failed!" << std::endl;
	//	return -1;
	//}

	////����в�������
	//for (int i = 0; i < 5; i++)
	//{
	//	std::string sqlstr;
	//	sqlstr = "insert into picdata(path, feature) values('d portition', 'this is feature')";
	//	if (0 != mysql_query(&mydata, sqlstr.c_str()))
	//	{
	//		std::cout << "insert data failed !" << std::endl;
	//		mysql_close(&mydata);
	//		return -1;
	//	}
	//}

	////��ʾ�ղŲ��������
	//std::string sqlstr = "SELECT * FROM picdata";
	//MYSQL_RES *result = NULL;
	//if (0 == mysql_query(&mydata, sqlstr.c_str())) 
	//{
	//	std::cout << "mysql_query() select data succeed !" << std::endl;

	//	//һ����ȡ�����ݼ�
	//	result = mysql_store_result(&mydata);
	//	//ȡ�ò���ӡ����
	//	int rowcount = mysql_num_rows(result);
	//	std::cout << "row count: " << rowcount << std::endl;

	//	//ȡ�ò���ӡ���ֶε�����
	//	unsigned int fieldcount = mysql_num_fields(result);
	//	MYSQL_FIELD *field = NULL;
	//	for (unsigned int i = 0; i < fieldcount; i++) {
	//		field = mysql_fetch_field_direct(result, i);
	//		std::cout << field->name << "\t\t";
	//	}
	//	std::cout << std::endl;

	//	//��ӡ����
	//	MYSQL_ROW row = NULL;
	//	row = mysql_fetch_row(result);
	//	while (NULL != row) {
	//		for (int i = 0; i < fieldcount; i++) {
	//			std::cout << row[i] << "\t\t";
	//		}
	//		std::cout << std::endl;
	//		row = mysql_fetch_row(result);
	//	}

	//}
	//else {
	//	std::cout << "mysql_query() select data failed" << std::endl;
	//	mysql_close(&mydata);
	//	return -1;
	//}

	//mysql_free_result(result);
	//mysql_close(&mydata);
	//mysql_server_end();

	return 0;
}