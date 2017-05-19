#pragma once

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


#include <string>
#include <vector>

//�����������ƶ�
float dotProduct(const std::vector<float>& v1, const std::vector<float>& v2);
float module(const std::vector<float>& v);
float cosine(const std::vector<float>& v1, const std::vector<float>& v2);

//�ݹ�����ļ��У���ȡ�ļ����µ������ļ���
void getAllFiles(std::string path, std::vector<std::string>& files);

//�ַ����ָ�
std::vector<std::string> split(const std::string &s, const std::string &seperator);

//�ַ������
std::string combineFromVector(std::vector<float> feature, const std::string &seperator);

//����ļ����Ƿ����
bool CheckFolderExist(std::string &strPath);

//�����༶Ŀ¼
int CreatDir(const char *pDir);