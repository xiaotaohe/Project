#pragma once
#include<iostream>
#include<vector>
#include<stdio.h>
#include<assert.h>
using namespace std;

template<class T>
struct Less
{
	bool operator()(const T& l, const T& r)
	{
		return l < r;
	}
};
template<class T>
struct Great
{
	bool operator()(const T& l, const T& r)
	{
		return l>r;
	}
};

template<class T, class compare = Less<T>>
class Heap
{
public:
	Heap()
	{}
	Heap(T* a, size_t size)
	{
		_array.reserve(size);
		for (size_t i = 0; i < size; i++)
		{
			_array.push_back(a[i]);
		}
		//建堆，从最后一个叶子节点，调用向下调整
		for (int i = (size - 2) / 2; i >= 0; --i)
		{
			AdjustDown(i);
		}
	}
	void Push(const T&data)
	{
		_array.push_back(data);
		AdjustUp(_array.size() - 1);
	}
	void Pop()
	{
		if (!_array.empty())
		{
			swap(_array[0], _array[_array.size() - 1]);
			_array.pop_back();
			AdjustDown(0);
		}
	}
	const T& Top()
	{
		assert(!_array.empty());
		return _array[0];
	}
	bool Empty()
	{
		return _array.empty();
	}
	size_t Size()
	{
		return _array.size();
	}
private:
	void AdjustUp(int root)
	{
		int child = root;
		int parent = (child - 1) / 2;
		compare com;
		while (parent >= 0)
		{
			if (com(_array[child], _array[parent]))
			{
				swap(_array[child], _array[parent]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
				break;
		}
	}
	void AdjustDown(int root)
	{
		size_t parent = root;
		size_t child = root * 2 + 1;
		compare com;
		while (child < _array.size())
		{
			if (child + 1 < _array.size() && com(_array[child + 1], _array[child]))
				child++;
			if (com(_array[child], _array[parent]))
			{
				swap(_array[child], _array[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	vector<T> _array;
};
