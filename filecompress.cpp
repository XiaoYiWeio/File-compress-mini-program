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
	map<string, int>dictionary;			//������������,�ֵ��
	int size = 256;
	for (int i = 0; i < size; i++)
		dictionary[string(1, i)] = i;	//��ASCII��256�ڵĵ����ַ������
	char c;								//current
	string p;							//previous
	//int k = 0;
	for (string::const_iterator it = content.begin(); it != content.end(); it++)	//����������
	{
		c = *it;						//ȡ����ǰ�ַ�
		if (dictionary.count(p + c))	//�ڿ��в�ѯ�Ƿ���ڵ�ǰ��,��������ǰ�����������򽫵�ǰ�����
			p += c;
		else
		{
			result.push_back(dictionary[p]);	//��¼��Ӧ��ֵ
			dictionary[p + c] = size++;	//���
			p = c;						//����previous
		}
	}
	if (!p.empty())						//�������һ���ַ�����������result
		result.push_back(dictionary[p]);
}

void decompress()						//��ѹ�����Ʒ������н�ѹ
{
	map<int, string>de_dictionary;		//������ʼ��
	int size = 256;
	for (int i = 0; i < size; i++)
		de_dictionary[i] = string(1, i);
	//	char c;
	int k;
	string p, s;
	ofstream fp("comp_dep.log", ios::binary);		//ȷ������ļ���ַ����ʽ
	size--;												//��ʵ�ϵ�255���ַ����ı��ˣ��������������õ��ַ����Ǵ�256��ʼ						
	for (vector<int>::iterator it = result.begin(); it != result.end(); it++)		//��result�����ݽ��ж�ȡ
	{
		k = *it;
		if (de_dictionary.count(k))					//���ڿ������ҳ���Ӧ�ַ���
			p = de_dictionary[k];
		else if (k == size)							//�����ڿ�������Ҫ
			p = s + s[0];
		else
			exit(1);					//�׳��쳣
		fp << p;						//�ҵ��󼴽������
		//if (size != 255)
		de_dictionary[size++] = s + p[0];	//�����ַ����������
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

	ifstream fin(argv[1], ios::binary);               // �Զ����Ʒ�ʽ���ļ�
	if (!fin) {
		cerr << "Can not open the input file!" << endl;   // ���������Ϣ���˳�
		return -1;
	}
	map<string, int>dictionary;
	istreambuf_iterator<char> beg(fin), end;              // ���������ļ�ָ�룬ָ��ʼ�ͽ�������char(һ�ֽ�)Ϊ����
	string content(beg, end);                             // ���ļ�ȫ������string�ַ���
	fin.close();                                          // �������ļ���ر��ļ������һ����ϰ��
	compress(content);									 //���ļ�����ѹ������ѹ�������ݴ�������result
	ofstream out_com(argv[2], ios::binary);				//��result�ڵ����������ָ��λ�õ��ļ���
	for (unsigned int i = 0; i < result.size(); i++)
		out_com << result[i];
	out_com.close();
	decompress();										//��ѹ�������ڽ�ѹ����ĳ����ѹ��ָ��λ�ã��˴�ѡ���ѹ����ǰĿ¼

	return 0;
}