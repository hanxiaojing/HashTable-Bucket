#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
using namespace std;

#include<vector>

template<class K,class V>
struct HashTableNode
{
	K _key;
	V _value;
	HashTableNode* _next;
	HashTableNode(const K& key,const V& value)
		:_key(key)
		, _value(value)
		, _next(NULL)
	{}
};

template<class K,class V>
class HashTable
{
public:
	typedef HashTableNode<K,V> Node;

	HashTable()
		:_table(NULL)
		, _size()
	{}


	size_t _HashFunc(const K& key)
	{
		//_table.size()��ʾ��ϣͰ�Ŀռ��С
		return key % _table.size();
	}
	
	
	//��������
	HashTable(const HashTable& ht)
	{
		//����ϣ��ht������this
		this->_table.resize(ht._table.size());
		for (int i = 0; i < ht._table.size(); i++)
		{
			Node* cur = ht._table[i];
			while (cur)
			{
				Node* tmp = new Node(cur->_key, cur->_value);
				tmp->_next = _table[i];
				_table[i] = tmp;
				this->_size++;

				cur = cur->_next;
			}
		}	
	}


	HashTable<K, V> operator=(const HashTable<K, V>& ht)
	{	
		if (&ht != this)
		{
			//ɾ����ϣ��this
			for (int i = 0; i < this->_table.size(); i++)
			{
				Node* cur = _table[i];
				while (cur)
				{
					Node* del = cur;
					cur = cur->_next;
					/*delete del;
					del = NULL;*/
					Remove(del->_key);
				}
			}

			//����ϣ��ht������this
			this->_table.resize(ht._table.size());
			for (int i = 0; i < ht._table.size(); i++)
			{
				Node* cur = ht._table[i];
				while (cur)
				{
					Node* tmp = new Node(cur->_key, cur->_value);
					tmp->_next = _table[i];
					_table[i] = tmp;
					this->_size++;

					cur = cur->_next;
				}
			}		
		}
		return *this;
	}


	//��ֵ��������ص��ִ�д��
	HashTable<K, V> operator=(HashTable<K, V> ht)
	{
		if (&ht != this)
		{
			swap(_table, ht._table);
			swap(_size, ht._size);
		}	
		return *this;
	}


	~HashTable()
	{
		//ɾ����ϣ��ht
		if (this->_table.size() !=0)
		{
			for (int i = 0; i < this->_table.size(); i++)
			{
				Node* cur = _table[i];
				while (cur)
				{
					Node* del = cur;
					cur = cur->_next;
					delete del;
					del = NULL;
				}
			}
		}
	}


	//��ȡ�µĹ�ϣ��������С
	size_t _GetnewSize()
	{
		static const int _PrimeSize = 28;
		static const unsigned long _PrimeList[_PrimeSize] =
		{
			53ul, 97ul, 193ul, 389ul, 769ul,
			1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
			49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
			1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
			50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
			1610612741ul, 3221225473ul, 4294967291ul
		};

		for (int i = 0; i < _PrimeSize; i++)
		{
			if (_PrimeList[i]> _table.size())
			{
				return _PrimeList[i];
			}
		}
		return _PrimeList[_PrimeSize - 1];
	}


	//����ϣͰ����
	void _ExpandCapacity()
	{		
		//�����µĸ��������Ĺ�ϣ��
		size_t newSize = _GetnewSize();
		vector<Node*> newTable;
		newTable.resize(newSize);

		//��ÿ��˳����ϵĵ�����Ԫ��ժ�������뵽�µ�˳�����
		for (int i = 0; i < _table.size(); i++)
		{
			Node* cur = _table[i];
			while (cur)
			{
				Node* tmp = cur;
				cur = cur->_next;
				int index = _HashFunc(tmp->_key);
				//ͷ�巨���ڵ�
				tmp->_next = newTable[index];
				newTable[index] = tmp;
			}
			_table[i] = NULL;
		}
		_table.swap(newTable);
	}


	//����ؼ���
	bool Insert(const K& key,const V& value)
	{
		//����غ����ӣ������Ƿ�����
		//��ϣͰ���غ���������Ϊ1
		if (_size == _table.size())
		{
			_ExpandCapacity();
		}

		//��˳����index������ڵ�
		size_t index = _HashFunc(key);
		Node* begin = _table[index];
		while (begin)
		{
			//��Ƴɲ��ɳ����ظ�Ԫ��
			if (begin->_key == key)
			{
				return false;
			}

			begin = begin->_next;
		}

		//���ǵ�ͬһ���������ϣ�����νԪ�ش��˳���ҽ�β��򵥡�--��ͷ��
		Node* tmp = new Node(key, value);
		tmp->_next =_table[index];
		_table[index] = tmp;
		_size++;
		return true;
	}


	//���ҹؼ���
	Node* Find(const K& key)
	{
		int index = _HashFunc(key);
		Node* cur = _table[index];
		while (cur)
		{
			if (cur->_key == key)
				return cur;
			cur = cur->_next;
		}
		return NULL;
	}


	//ɾ���ؼ���
	bool Remove(const K& key)
	{
		int index = _HashFunc(key);
		Node* cur = _table[index];
		Node* prev = NULL;
		while (cur)
		{
			if (cur->_key == key)
				break;
			prev = cur;
			cur = cur->_next;
		}

		if (cur)
		{
			if (cur == _table[index])
			{			
				_table[index] = cur->_next;
			}
			else
			{
				Node* next = cur->_next;
				prev->_next = next;
			}
			delete cur;
			cur = NULL;
			--_size;
			return true;		
		}
		return false;
	}


	//��ӡ��ϣͰ
	void PrintHashTable()
	{
		for (int i = 0; i < _table.size(); i++)
		{
			Node* cur = _table[i];
			cout << i<<":" ;
			while (cur)
			{
				cout << cur->_key << "->";
				cur = cur->_next;
			}
			cout << "NULL" << endl;
		}
		cout << endl;
	}
	
private:
	vector<Node*> _table;
	size_t _size;//���ݸ���
};


void TestHashTableBucket()
{
	typedef HashTableNode<int, char> Node;

	HashTable<int, char> ht;
	ht.Insert(1, 'a');
	ht.Insert(2, 'b');
	ht.Insert(3, 'c');
	ht.Insert(4, 'd');
	ht.Insert(5, 'd');
	ht.Insert(54, 'x');
	ht.Insert(55, 'y');
	ht.Insert(56, 'z');

	ht.PrintHashTable();


	/*Node* ret = ht.Find(5);
	cout << ret->_value << endl;

	ht.Remove(1);
	ht.Remove(6);
	ht.PrintHashTable();*/

	/*HashTable<int, char> ht1(ht);
	ht1.PrintHashTable();*/

	HashTable<int, char> ht2;
	ht2.Insert(54, 'x');
	ht2.Insert(55, 'y');
	ht2.Insert(56, 'z');
	ht2.Insert(1, 'a');
	ht2.Insert(2, 'b');
	ht2.Insert(3, 'c');
	ht2.Insert(4, 'd');
	ht2.Insert(5, 'd');

	ht2.PrintHashTable();

	ht = ht2;
	ht.PrintHashTable();

}


int main()
{
	TestHashTableBucket();
	system("pause");
	return 0;
}