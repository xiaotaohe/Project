#pragma once
#include"Heap.h"
template<class W>
struct HuffmanNode
{
	HuffmanNode* _left;
	HuffmanNode* _right;
	HuffmanNode* _parent;
	W _w;//权重
	HuffmanNode(const W& w)
		:_left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _w(w)
	{}
};

template<class W>
class HuffmanTree
{
public:
	typedef HuffmanNode<W> Node;//HuffmanNode 命名为Node
	HuffmanTree()
	{
		root = NULL;
	}
	HuffmanTree(W* w, size_t size, const W& invalid)//创建huffmantree的数组w，数组大小 size，无效元素
	{
		//1.建堆来，以赛选最小值、次小值构造Huffmantree
		//用来作比较的仿函数
		struct NodeCompare
		{
			bool operator()(Node* l,  Node* r)
			{
				return l->_w < r->_w;
			}
		};
		//2.建堆
		Heap<Node*, NodeCompare> minheap;
		for (size_t i = 0; i < size; i++)
		{
			if (w[i] != invalid)
			{
				minheap.Push(new Node(w[i]));
			}
		}
		//3.构建Huffmantree
		while (minheap.Size()>1)
		{
			Node* left = minheap.Top();//最小值
			minheap.Pop();
			Node* right = minheap.Top();//次小值
			minheap.Pop();
			Node* parent = new Node(left->_w + right->_w);
			parent->_left = left;
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;
			minheap.Push(parent);
		}
		root = minheap.Top();
	}
	Node* GetRoot()
	{
		return root;
	}
private:
	//将拷贝构造和复制运算符重载封装为私有
	HuffmanTree(const HuffmanTree<W>& tree);
	HuffmanTree<W>& operator=(const HuffmanTree<W>& tree);
	Node* root;
};
