#include<iostream>
#include<fstream>
#include<string>
#include<iterator>
#include<vector>
#include<map>
using namespace std;
vector<int>result;
void compress(string &content)
{
	//string result;
	map<string, int>dictionary;			//创建关联容器,字典库
	int size = 256;
	for (int i = 0; i < size; i++)
		dictionary[string(1, i)] = i;	//将ASCII码256内的单个字符加入库
	char c;								//current
	string p;							//previous
	//int k = 0;
	for (string::const_iterator it = content.begin(); it != content.end(); it++)	//创建迭代器
	{
		c = *it;						//取出当前字符
		if (dictionary.count(p + c))	//在库中查询是否存在当前串,若存在则当前串扩增，否则将当前串入库
			p += c;
		else
		{
			result.push_back(dictionary[p]);	//记录对应数值
			dictionary[p + c] = size++;	//入库
			p = c;						//更新previous
		}
	}
	if (!p.empty())						//对于最后一个字符串处理输入result
		result.push_back(dictionary[p]);
}

void decompress()						//与压缩类似方法进行解压
{
	map<int, string>de_dictionary;		//建立初始库
	int size = 256;
	for (int i = 0; i < size; i++)
		de_dictionary[i] = string(1, i);
	//	char c;
	int k;
	string p, s;
	ofstream fp("comp_dep.log", ios::binary);		//确定输出文件地址及方式
	size--;												//事实上第255的字符被改变了，但是真正起作用的字符串是从256开始						
	for (vector<int>::iterator it = result.begin(); it != result.end(); it++)		//对result内数据进行读取
	{
		k = *it;
		if (de_dictionary.count(k))					//若在库中则找出对应字符串
			p = de_dictionary[k];
		else if (k == size)							//若不在库中则需要
			p = s + s[0];
		else
			exit(1);					//抛出异常
		fp << p;						//找到后即将其输出
		//if (size != 255)
		de_dictionary[size++] = s + p[0];	//任意字符串均需入库
		s = p;
	}
	fp.close();
}

int main(int argc, char* argv[])
{
	cout << "Zipper 0.001! Author: zhangbo" << endl;
	if (argc != 4) {
		cerr << "Please make sure the number of parameters is correct." << endl;
		return -1;
	}

	if (strcmp(argv[3], "zip")) {
		cerr << "Unknown parameter!\nCommand list:\nzip" << endl;
		return -1;
	}

	ifstream fin(argv[1], ios::binary);               // 以二进制方式打开文件
	if (!fin) {
		cerr << "Can not open the input file!" << endl;   // 输出错误信息并退出
		return -1;
	}
	map<string, int>dictionary;
	istreambuf_iterator<char> beg(fin), end;              // 设置两个文件指针，指向开始和结束，以char(一字节)为步长
	string content(beg, end);                             // 将文件全部读入string字符串
	fin.close();                                          // 操作完文件后关闭文件句柄是一个好习惯
	compress(content);									 //对文件进行压缩并将压缩后数据传入容器result
	ofstream out_com(argv[2], ios::binary);				//将result内的数据输出到指定位置的文件中
	for (unsigned int i = 0; i < result.size(); i++)
		out_com << result[i];
	out_com.close();
	decompress();										//解压程序，用于将压缩后的程序解压到指定位置，此处选择解压至当前目录

	return 0;
}