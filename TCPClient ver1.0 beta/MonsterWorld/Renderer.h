class Renderer : public Singleton<Renderer>
{
	friend class Singleton<Renderer>;
private:
	Renderer();
	~Renderer();

private:
	char** screen;
	const int Row;
	const int Col;
	const Position origin;

public:
	int GetRow() const { return Row; }
	int GetCol() const { return Col; }

	void Clear();
	void Draw(const GameObject& obj);
	void Render() const;

	void ShowStatus(GameObjectManager& manager) const;
	void GameOver() const;

	void SetScreen(const char* other);
	string GetScreen() const;
};