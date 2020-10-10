class GameObjectManager
{
private:
	vector<GameObject*> objects;
	vector<int>	counts;
	const int MaxItems;
	const int MaxEnemies;
	int numOfItems;
	int numOfEnemies;
public:
	GameObjectManager(int nEnemies);
	~GameObjectManager();

public:
	vector<int>	 GetCounts() const { return counts; }
	void SetCounts(int* num);
	int GetNumOfItems() const { return numOfItems; }
	int GetNumOfEnemies() const { return numOfEnemies; }
	const vector<GameObject*>* GetObjects() { return const_cast<const vector<GameObject*>*>(&objects); }
	
	void Update();
};