#pragma once

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
