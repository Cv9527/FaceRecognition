#ifdef _WIN32  
#include <direct.h>  
#include <io.h>  
#include <Windows.h>
#elif _LINUX  
#include <stdarg.h>  
#include <sys/stat.h>  
#endif  

#ifdef _WIN32  
#define ACCESS _access  
#define MKDIR(a) _mkdir((a))  
#elif _LINUX  
#define ACCESS access  
#define MKDIR(a) mkdir((a),0755)  
#endif  

#include <iostream>
#include <stdlib.h>  
#include <string.h>  
#include "utils.h"
#include <opencv2\opencv.hpp>

//�����༶Ŀ¼
//int CreatDir(const char *pDir)
//{
//	int i = 0;
//	int iRet;
//	int iLen;
//	char* pszDir;
//	if (NULL == pDir)
//	{
//		return 0;
//	}
//
//	pszDir = strdup(pDir);
//	iLen = strlen(pszDir);
//
//	// �����м�Ŀ¼  
//	for (i = 0; i < iLen; i++)
//	{
//		if (pszDir[i] == '\\' || pszDir[i] == '/')
//		{
//			pszDir[i] = '\0';
//			//���������,����  
//			iRet = ACCESS(pszDir, 0);
//			if (iRet != 0)
//			{
//				iRet = MKDIR(pszDir);
//				if (iRet != 0)
//				{
//					return -1;
//				}
//			}
//			//֧��linux,������\����/  
//			pszDir[i] = '/';
//		}
//	}
//
//	iRet = MKDIR(pszDir);
//	free(pszDir);
//	return iRet;
//}

//�ж��ļ����Ƿ����
//bool CheckFolderExist(std::string &strPath)
//{
//	WIN32_FIND_DATA  wfd;
//	bool rValue = false;
//	HANDLE hFind = FindFirstFile(strPath.c_str(), &wfd);
//	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
//	{
//		rValue = true;
//	}
//	FindClose(hFind);
//	return rValue;
//}

int test(int argc, char* argv[])
{
	std::vector<std::string> fileNames;
	std::string originalBasePath = "K:/DL-FACEDATASETS/lfw/test";
	std::string alignedBaseDir = "K:/DL-FACEDATASETS/lfw/test-align/";
	getAllFiles(originalBasePath, fileNames);
	for (int i = 0; i < fileNames.size(); i++)
	{
		std::cout << fileNames[i] << std::endl;
		cv::Mat img = cv::imread(fileNames[i]);
		//ֻ��find������ƥ�䣬find_first_of�ȶ���������һ���ַ�ƥ�伴��
		//��ע�⣺5Ϊ�ļ������ֵĳ��ȼ�1!��
		std::string subPath = fileNames[i].substr(fileNames[i].find("test") + 5);
		//std::cout << subPath << std::endl;
		std::string newSubfolder = subPath.substr(0, subPath.find_last_of("\\"));
		//std::cout << newSubfolder << std::endl;
		std::string newFolder = alignedBaseDir + newSubfolder;
		
		if (!CheckFolderExist(newFolder))
		{
			CreatDir(newFolder.c_str());
		}

		std::string newFullPath = alignedBaseDir + subPath;
		//std::cout << newFullPath << std::endl;
		cv::imwrite(newFullPath, img);
	}
	std::cout << "Start,total number of images is: " << fileNames.size() << std::endl;


	system("pause");
	return 0;
}