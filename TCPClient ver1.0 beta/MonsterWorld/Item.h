class Item : public GameObject
{
public:
	Item(const string& shape, float x, float y);
	Item(const string& shape, const Position& pos);
	~Item();
public:
	void Drop();
public:
	virtual void Update();
	virtual void Draw() { GameObject::Draw(); }
};

