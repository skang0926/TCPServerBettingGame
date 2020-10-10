template<typename T>
class Container
{
private:
	const int MaxItems;
	int nItems;
	T* items;
public:
	Container(int size) : MaxItems(size), nItems(0), items(new T[size])
	{
		for (int i = 0; i < size; i++) items[i] = nullptr;
	}
	~Container()
	{
		RemoveAll();
		delete[] items;
	}
public:
	int Capacity() const { return MaxItems; }
	int Count() const { return nItems; }

	void Add(T item)
	{
		if (!item) return;
		if (nItems >= MaxItems)
		{
			delete item;
			return;
		}

		for (int i = 0; i < MaxItems; i++)
		{
			if (items[i] != nullptr) continue;

			items[i] = item;
			nItems++;
			return;
		}
		delete item;
	}

	void Remove(int idx)
	{
		if (idx == -1) return;

		delete items[idx];
		items[idx] = nullptr;
		nItems--;
	}

	void Remove(T item)
	{
		int idx = IndexOf(item);
		if (idx == -1)
		{
			if (item != nullptr) delete item;
			return;
		}
		Remove(idx);
	}

	void RemoveAll()
	{
		for (int i = 0; i < nItems; i++)
		{
			delete items[i];
			items[i] = nullptr;
			nItems--;
		}
	}

	int IndexOf(T obj)
	{
		if (obj == nullptr) return -1;

		for (int i = 0; i < MaxItems; i++)
		{
			if (items[i] == obj) return i;
		}
		return -1;
	}

	T At(int idx)
	{
		if (idx < 0 || idx >= nItems) return nullptr;
		return items[idx];
	}

	T operator[](int idx)
	{
		return items[idx];
	}
};

struct Position
{
	float x;
	float y;
	Position() : x(0), y(0) {}
	Position(float x, float y) : x(x), y(y) {}

	Position& operator+(const Position& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	Position& operator-(const Position& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	Position& operator+=(const Position& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	Position& operator-=(const Position& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	bool operator==(const Position& rhs)
	{
		return (this->x == rhs.x && this->y == rhs.y);
	}
};

class Borland
{
public:
	static int WhereX()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.X;
	}
	static int WhereY()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.Y;
	}
	static void Gotoxy(int x, int y)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)x, (SHORT)y });
	}
	static void Gotoxy(const Position& pos)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)pos.x, (SHORT)pos.y });
	}
};

template<typename TYPE>
class Singleton
{
private:
	static TYPE* instance;
protected:
	Singleton() {}
	virtual ~Singleton()
	{
		delete instance;
		instance = nullptr;
	}
public:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
public:
	static TYPE& Instance() 
	{
		if (instance == nullptr) instance = new TYPE;
		return *instance;
	}

	static void Release()
	{
		if (instance == nullptr) return;
		delete instance;
		instance = nullptr;
	}
};

template<typename TYPE> TYPE* Singleton<TYPE>::instance = nullptr;