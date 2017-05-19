#include "utils.h"
#include <cassert>
#include <io.h>
#include <sstream>

//���������������������ƶ�
float dotProduct(const std::vector<float>& v1, const std::vector<float>& v2)
{
	assert(v1.size() == v2.size());
	float ret = 0.0;
	for (std::vector<float>::size_type i = 0; i != v1.size(); ++i)
	{
		ret += v1[i] * v2[i];
	}
	return ret;
}
float module(const std::vector<float>& v)
{
	float ret = 0.0;
	for (std::vector<float>::size_type i = 0; i != v.size(); ++i)
	{
		ret += v[i] * v[i];
	}
	return sqrt(ret);
}
float cosine(const std::vector<float>& v1, const std::vector<float>& v2)
{

	assert(v1.size() == v2.size());
	//return dotProduct(v1, v2) / (module(v1) * module(v2));
	//�����Ѿ�����ȡ��ʱ���һ�������˴������ٳ���ģ
	return dotProduct(v1, v2) / (module(v1) * module(v2));
}


//�����ļ��У���ȡ�ļ����µ������ļ����������ص�һ��std::vector<string>��ȥ
void getAllFiles(std::string path, std::vector<std::string>& files)
{
	//�ļ����
	intptr_t hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮
			//�������,�����б�
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


//���ַ��������ض��ķ��ŷָ��
std::vector<std::string> split(const std::string &s, const std::string &seperator)
{
	std::vector<std::string> result;
	typedef std::string::size_type string_size;
	string_size i = 0;

	while (i != s.size())
	{
		//�ҵ��ַ������׸������ڷָ�������ĸ��
		int flag = 0;
		while (i != s.size() && flag == 0)
		{
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
			if (s[i] == seperator[x])
			{
				++i;
				flag = 0;
				break;
			}
		}
		//�ҵ���һ���ָ������������ָ���֮����ַ���ȡ����
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0)
		{
			for (string_size x = 0; x < seperator.size(); ++x)
			if (s[j] == seperator[x])
			{
				flag = 1;
				break;
			}
			if (flag == 0)
				++j;
		}
		if (i != j)
		{
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}


//��vector�����ض��ı�Ƿ�����ϳ��ַ���
std::string combineFromVector(std::vector<float> feature, const std::string &seperator)
{
	std::string featurestr;
	std::stringstream ss;
	for (int i = 0; i < feature.size(); i++)
	{
		double n = feature[i];
		std::string s;
		ss << n;
		ss >> s;
		ss.clear();
		featurestr = featurestr + seperator + s;
	}

	return featurestr;
}


//�����༶Ŀ¼
int CreatDir(const char *pDir)
{
	int i = 0;
	int iRet;
	int iLen;
	char* pszDir;
	if (NULL == pDir)
	{
		return 0;
	}

	pszDir = strdup(pDir);
	iLen = strlen(pszDir);

	// �����м�Ŀ¼  
	for (i = 0; i < iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';
			//���������,����  
			iRet = ACCESS(pszDir, 0);
			if (iRet != 0)
			{
				iRet = MKDIR(pszDir);
				if (iRet != 0)
				{
					return -1;
				}
			}
			//֧��linux,������\����/  
			pszDir[i] = '/';
		}
	}

	iRet = MKDIR(pszDir);
	free(pszDir);
	return iRet;
}


//�ж��ļ����Ƿ����
bool CheckFolderExist(std::string &strPath)
{
	WIN32_FIND_DATA  wfd;
	bool rValue = false;
	HANDLE hFind = FindFirstFile(strPath.c_str(), &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = true;
	}
	FindClose(hFind);
	return rValue;
}