#pragma once

#include<iostream>
using namespace std;

#include<assert.h>
#include<string>

#include"Huffman.h"

//存储字符信息，以方便构造HuffmanTree
struct CharInfo
{
	char _ch;
	string _code;//存储Huffman编码
	long long _count;//ch出现的次数
	bool operator !=(const CharInfo& info)
	{
		return _count!= info._count;
	}
	bool operator <(const CharInfo& info)
	{
		return _count < info._count;
	}
	CharInfo operator +(const CharInfo& info)
	{
		CharInfo ret;
		ret._count = _count+info._count;
		return ret;
	}
};

class FileCompress
{
	typedef HuffmanNode<CharInfo> Node;
	//配置信息
	struct ConfInfo
	{
		char _ch;
		long long _count;
	};
public:
	//在构造函数中初始化_info[]
	FileCompress()
	{
		for (int i = 0; i < 256; i++)
		{
			_info[i]._ch = i;
			_info[i]._count = 0;
		}
	}
	//文件压缩
	void Compress(char* file)
	{
		//1.读取文件，统计字符个数
		FILE* fout = fopen(file, "rb");
		assert(fout);
		char ch = getc(fout);
		while (!feof(fout))
		{
			_info[(unsigned char)ch]._count++;
			ch = getc(fout);
		}
		//2.构建HuffmanTree
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_info, 256, invalid);

		//3.生成Huffman编码
		string code;
		Node* root = tree.GetRoot();
		GetHuffmanCode(root, code);
		//4.读取文件，写入配置信息;以方便解压缩时构建HuffmanTree
		string Comfile = file;
		Comfile += ".Compress";
		FILE* fin = fopen(Comfile.c_str(), "wb");
		ConfInfo info;
		for (int i = 0; i < 256; i++)
		{
			if (_info[i]._count != 0)
			{
				info._ch = _info[i]._ch;
				info._count = _info[i]._count;
				fwrite(&info, sizeof(ConfInfo), 1, fin);
			}
		}
		
		//写入最后一个为配置文件标记位
		info._count = 0;
		fwrite(&info, sizeof(ConfInfo), 1, fin);
		//5.压缩
		fseek(fout, 0, SEEK_SET);
		char value = 0;
		int pos = 0;
		Node* cur = root;
		ch = fgetc(fout);
		while (!feof(fout))
		{
			string code = _info[(unsigned char)ch]._code;
			for (int i = 0; i < code.size(); i++)
			{
				if (code[i] == '1')
					value |= (1 << pos);
				else if (code[i] == '0')
					value &= ~(1 << pos);
				else
					assert(false);
				pos++;
				if (pos == 8)
				{
					putc(value, fin);
					pos = 0;
					value = 0;
				}
			}
			ch = getc(fout);
		}
		if (pos != 0)
			putc(value, fin);
		//关闭打开的文件
		fclose(fout);
		fclose(fin);
	}
	void UnCompress(char* file)
	{
		//1.读取配置文件
		assert(file);
		FILE* fout = fopen(file, "rb");
		assert(fout);
		while (1)
		{
			ConfInfo info;
			fread(&info, sizeof(ConfInfo), 1, fout);
			if (info._count == 0)
				break;
			_info[(unsigned char)info._ch]._count = info._count;
		}
		//2.重建huffmanTree
		CharInfo invalid;
		invalid._count = 0;
		HuffmanTree<CharInfo> tree(_info,256,invalid);
		//3.解压缩
		string UnCompress = file;
		size_t find = UnCompress.rfind('.');
		UnCompress.erase(find, UnCompress.size() - find);
		UnCompress += ".UnCompress";
		FILE* fin = fopen(UnCompress.c_str(), "wb");
		Node* root = tree.GetRoot();
		Node* cur = root;
		long long count = root->_w._count;
		char value = fgetc(fout);
		while (!feof(fout))
		{
			for (size_t pos = 0; pos < 8; pos++)
			{
				if (value & (1 << pos))
					cur = cur->_left;
				else
					cur = cur->_right;
				if (cur->_left == NULL && cur->_right == NULL)
				{
					putc(cur->_w._ch, fin);
					cur = root;
					count--;
					if (count == 0)
						break;
				}
			}
			value = getc(fout);
		}
		fclose(fout);
		fclose(fin);
	}
private:
	void GetHuffmanCode(Node* root, string code)//获取HuffmanCode
	{
		if (root == NULL)
			return;
		if (root->_left == NULL && root->_right == NULL)
		{
			_info[(unsigned char)root->_w._ch]._code = code;
			return;
		}
		GetHuffmanCode(root->_left, code + '1');
		GetHuffmanCode(root->_right, code + '0');
	}
	CharInfo _info[256];
};