#pragma once

#include <string>
#include <vector>

//计算余弦相似度
float dotProduct(const std::vector<float>& v1, const std::vector<float>& v2);
float module(const std::vector<float>& v);
float cosine(const std::vector<float>& v1, const std::vector<float>& v2);


//递归遍历文件夹，获取文件夹下的所有文件名
void getAllFiles(std::string path, std::vector<std::string>& files);

//字符串分割
std::vector<std::string> split(const std::string &s, const std::string &seperator);

//字符串组合
std::string combineFromVector(std::vector<float> feature, const std::string &seperator);
