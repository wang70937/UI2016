#pragma once

// ���ڽ����̳й�ϵ�����ݽṹ��TODO: �������ʹ��
template<class T>
class tree
{
public:
	tree()
	{
		parent = NULL;
		first_child = NULL;
		first_neighbour = NULL;
	};

	T         data;
	tree<T>*  parent;
	tree<T>*  first_child;
	tree<T>*  first_neighbour;
};