/**
*�˽ű����������ݽ��м��Ͷ���Ԥ����������Ͷ���֮��Ĳ���ͼƬ
**/
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include "utils.h"
#include "facedetection.h"

std::mutex mtx;

bool compare(cv::Rect i, cv::Rect j)
{
	return (i.area() > j.area()); 
}

volatile int count{ 0 };

void imgaeProcess(int num, const std::vector<std::string> &filenames)
{

	std::string alignedBaseDir = "K:/DL-FACEDATASETS/lfw/lfw-deepfunneled-aligned-112-96/";
	FaceDetection *fd = new FaceDetection();
	std::string model_folder = "../models/detectionModel/";
	fd->loadModel(model_folder, 0);
	int tmp = 0;
	int length = filenames.size();
	while (TRUE)
	{
		//��ȡ����
		mtx.lock();
	    tmp = ::count;
		++::count;
		mtx.unlock();
		if (tmp < filenames.size())
		{
			//��ͼƬ���д���
			cv::Mat img = imread(filenames[tmp]);
			double min_face_size = 20;
			std::vector<std::vector<cv::Point2d>> points;
			std::vector<cv::Rect> results = fd->faceDetect(img, min_face_size, true, points);
			if (img.empty() || results.empty())
			{
				//���������ͼƬд��txt�ĵ�,�ֶ�����
				ofstream out("K:/DL-FACEDATASETS/lfw/error.txt", ios::app);
				if (out.is_open())
				{
					out << filenames[tmp] << endl;;
					out.close();
				}
				continue;
			}
			if (results.size() > 1)
			{
				//��������ϴ����һ��,�����·�����ʹ��points�����ֲ�����������
				std::sort(results.begin(), results.end(), ::compare);
			}
			
			std::cout << filenames[tmp] << ":" << tmp << std::endl;
			std::vector<cv::Mat> saveImgs = fd->getAlignFaces(img, results, points);
			//�����µ��ļ���·��
			std::string subPath = filenames[tmp].substr(filenames[tmp].find("lfw-deepfunneled") + 17);
			std::string newSubfolder = subPath.substr(0, subPath.find_last_of("\\"));
			std::string newFolder = alignedBaseDir + newSubfolder;
			if (!CheckFolderExist(newFolder))
			{
				CreatDir(newFolder.c_str());
			}
			std::string newFullPath = alignedBaseDir + subPath;
			cv::imwrite(newFullPath, saveImgs[0]);

			std::cout << "Thread: " << num << " face num: " << results.size() << " Imgae Num:" << tmp << std::endl;
		}
		else
		{
			break;
		}
			
	}
	/*if(fd)
		delete fd;*/
	//����delete�������߳��˳�ʱ����bug
	
}

int main()
{
	
	std::vector<std::string> fileNames;
	std::string fileDirPath = "K:/DL-FACEDATASETS/lfw/lfw-deepfunneled";
	getAllFiles(fileDirPath, fileNames);
	std::cout << "strat,total images: "<< fileNames.size() << endl;

	std::thread threads[4];
	for (int i = 0; i < 4; i++)
	{
		threads[i] = std::thread(imgaeProcess, i, fileNames);
	}
	for (auto& th : threads)
		th.join();

	std::cout << ::count << " success!" << std::endl;
	return 0;
}