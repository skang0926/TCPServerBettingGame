class Monster : public GameObject
{
private:
	int speed;
	int nItems;
	int moveCount;
	string name;
	vector<GameObject*>& objects;
public:
	Monster(const string& shape, float x, float y, const string& name, vector<GameObject*>& objects);
	Monster(const string& shape, const Position& pos, const string& name, vector<GameObject*>& objects);
	~Monster();
public:
	int GetMoveCount() const { return moveCount; }
	const string& GetName() const { return name; }
	int GetNumOfItem() const { return nItems; }

	void SetNumOfItem(int num) { nItems = num; }

	void Move();
	void CheckRange();
public:
	virtual void Update();
	virtual void Draw() { GameObject::Draw(); }
};