#include "utils.h"
#include <cassert>
#include <io.h>
#include <sstream>

//计算两个向量的余弦相似度
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
	//特征已经在提取的时候归一化过，此处无需再除以模
	return dotProduct(v1, v2) / (module(v1) * module(v2));
}


//遍历文件夹，获取文件夹下的所有文件名，并返回到一个std::vector<string>中去
void getAllFiles(std::string path, std::vector<std::string>& files)
{
	//文件句柄
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
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


//将字符串按照特定的符号分割开来
std::vector<std::string> split(const std::string &s, const std::string &seperator)
{
	std::vector<std::string> result;
	typedef std::string::size_type string_size;
	string_size i = 0;

	while (i != s.size())
	{
		//找到字符串中首个不等于分隔符的字母；
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
		//找到又一个分隔符，将两个分隔符之间的字符串取出；
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


//将vector按照特定的标记符号组合成字符串
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


//创建多级目录
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

	// 创建中间目录  
	for (i = 0; i < iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';
			//如果不存在,创建  
			iRet = ACCESS(pszDir, 0);
			if (iRet != 0)
			{
				iRet = MKDIR(pszDir);
				if (iRet != 0)
				{
					return -1;
				}
			}
			//支持linux,将所有\换成/  
			pszDir[i] = '/';
		}
	}

	iRet = MKDIR(pszDir);
	free(pszDir);
	return iRet;
}


//判断文件夹是否存在
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