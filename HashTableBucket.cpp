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
		//_table.size()表示哈希桶的空间大小
		return key % _table.size();
	}
	
	
	//拷贝构造
	HashTable(const HashTable& ht)
	{
		//将哈希表ht拷贝给this
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
			//删除哈希表this
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

			//将哈希表ht拷贝给this
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


	//赋值运算符重载的现代写法
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
		//删除哈希表ht
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


	//获取新的哈希表容量大小
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


	//给哈希桶扩容
	void _ExpandCapacity()
	{		
		//开辟新的更大容量的哈希表
		size_t newSize = _GetnewSize();
		vector<Node*> newTable;
		newTable.resize(newSize);

		//将每处顺序表上的单链表元素摘下来插入到新的顺序表上
		for (int i = 0; i < _table.size(); i++)
		{
			Node* cur = _table[i];
			while (cur)
			{
				Node* tmp = cur;
				cur = cur->_next;
				int index = _HashFunc(tmp->_key);
				//头插法插插节点
				tmp->_next = newTable[index];
				newTable[index] = tmp;
			}
			_table[i] = NULL;
		}
		_table.swap(newTable);
	}


	//插入关键字
	bool Insert(const K& key,const V& value)
	{
		//检查载荷因子，考虑是否扩容
		//哈希桶的载荷因子设置为1
		if (_size == _table.size())
		{
			_ExpandCapacity();
		}

		//往顺序表的index处插入节点
		size_t index = _HashFunc(key);
		Node* begin = _table[index];
		while (begin)
		{
			//设计成不可出现重复元素
			if (begin->_key == key)
			{
				return false;
			}

			begin = begin->_next;
		}

		//考虑到同一条单链表上，无所谓元素存放顺序，且较尾插简单。--》头插
		Node* tmp = new Node(key, value);
		tmp->_next =_table[index];
		_table[index] = tmp;
		_size++;
		return true;
	}


	//查找关键字
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


	//删除关键字
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


	//打印哈希桶
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
	size_t _size;//数据个数
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