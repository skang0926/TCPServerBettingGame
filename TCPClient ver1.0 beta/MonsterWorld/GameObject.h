class GameObject
{
private:
	string shape;
	bool isEnable;
	Position pos;
	Renderer& renderer;
public:
	GameObject(const string& shape, float x, float y) : shape(shape), pos(x, y), isEnable(true), renderer(Renderer::Instance()) {}
	GameObject(const string& shape, const Position& pos) : shape(shape), pos(pos), isEnable(true), renderer(Renderer::Instance()) {}
	virtual ~GameObject() {}
public:
	float GetX() const { return pos.x; }
	float GetY() const { return pos.y; }
	Position GetPosition() const { return pos; }
	void SetPosition(float x, float y) { pos.x = x; pos.y = y; }
	void SetPosition(const Position& pos) { this->pos = pos; }

	bool IsEnable() const { return isEnable; }
	void Enable() { isEnable = true; }
	void Disable() { isEnable = false; }

	int GetShapeSize() const { return shape.size(); }
	const string& GetShape() const { return shape; }
	void SetShape(const string& shape) { this->shape = shape; }
public:
	virtual void Update() {}
	virtual void Draw() { renderer.Draw(*this); }
};
