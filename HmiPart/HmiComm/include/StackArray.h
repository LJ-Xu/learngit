#pragma once
#include <iostream>
using namespace std;
//模板类
template<class T>
class StackArray {
public:
	StackArray();
	~StackArray();
	void Push(T t);//向栈中添加一个t元素
	T Peek();//向栈中取出一个元素
	T Pop();//在栈中删除一个元素
	int Size();//大小
	int IsEmpty();//判断是否为空
private:
	T *arr;//数组？
	int count;
};
// 创建“栈”，默认大小是12
template<class T>
StackArray<T>::StackArray() //这里面就是比普通的
{
	arr = new T[12];
	if (!arr)
	{
		cout << "arr malloc error!" << endl;
	}
}
// 销毁“栈”
template<class T>
StackArray<T>::~StackArray()
{
	if (arr)
	{
		delete[] arr;
		arr = NULL;
	}
}
// 将val添加到栈中
//向栈中添加一个元素
template<class T>
void StackArray<T>::Push(T t)
{
	//arr[count++] = val;
	arr[count++] = t;
}
// 返回“栈顶元素值”
template<class T>
T StackArray<T>::Peek()
{
	return arr[count - 1];
}
// 返回“栈顶元素值”，并删除“栈顶元素”
template<class T>
T StackArray<T>::Pop()
{
	int ret = arr[count - 1];
	count--;
	return ret;
}
// 返回“栈”的大小
template<class T>
int StackArray<T>::Size()
{
	return count;
}
// 返回“栈”是否为空
template<class T>
int StackArray<T>::IsEmpty()
{
	return Size() == 0;
}
