#pragma once
#include <iostream>
using namespace std;
//ģ����
template<class T>
class StackArray {
public:
	StackArray();
	~StackArray();
	void Push(T t);//��ջ�����һ��tԪ��
	T Peek();//��ջ��ȡ��һ��Ԫ��
	T Pop();//��ջ��ɾ��һ��Ԫ��
	int Size();//��С
	int IsEmpty();//�ж��Ƿ�Ϊ��
private:
	T *arr;//���飿
	int count;
};
// ������ջ����Ĭ�ϴ�С��12
template<class T>
StackArray<T>::StackArray() //��������Ǳ���ͨ��
{
	arr = new T[12];
	if (!arr)
	{
		cout << "arr malloc error!" << endl;
	}
}
// ���١�ջ��
template<class T>
StackArray<T>::~StackArray()
{
	if (arr)
	{
		delete[] arr;
		arr = NULL;
	}
}
// ��val��ӵ�ջ��
//��ջ�����һ��Ԫ��
template<class T>
void StackArray<T>::Push(T t)
{
	//arr[count++] = val;
	arr[count++] = t;
}
// ���ء�ջ��Ԫ��ֵ��
template<class T>
T StackArray<T>::Peek()
{
	return arr[count - 1];
}
// ���ء�ջ��Ԫ��ֵ������ɾ����ջ��Ԫ�ء�
template<class T>
T StackArray<T>::Pop()
{
	int ret = arr[count - 1];
	count--;
	return ret;
}
// ���ء�ջ���Ĵ�С
template<class T>
int StackArray<T>::Size()
{
	return count;
}
// ���ء�ջ���Ƿ�Ϊ��
template<class T>
int StackArray<T>::IsEmpty()
{
	return Size() == 0;
}
