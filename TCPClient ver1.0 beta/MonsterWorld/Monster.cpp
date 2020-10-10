#include "PCH.h"

Monster::Monster(const string& shape, float x, float y, const string& name, vector<GameObject*>& objects)
	: GameObject(shape, x, y), speed(1), nItems(0), moveCount(0), objects(objects), name(name) {}

Monster::Monster(const string& shape, const Position& pos, const string& name, vector<GameObject*>& objects)
	: GameObject(shape, pos), speed(1), nItems(0), moveCount(0), objects(objects), name(name) {}

Monster::~Monster() {}

void Monster::Move()
{
	int dirX = (rand() % 3) - 1;
	int dirY = (rand() % 3) - 1;
	Position movePos = this->GetPosition() + Position(dirX * speed, dirY * speed);
	this->SetPosition(movePos);
	moveCount++;
}

void Monster::CheckRange()
{
	Position pos = this->GetPosition();
	int row = Renderer::Instance().GetRow() - 1;
	int col = Renderer::Instance().GetCol() - 1;

	if (pos.x < 0) pos.x = 0;
	else if (pos.x >= col) pos.x = col;
	if (pos.y < 0) pos.y = 0;
	else if (pos.y >= row) pos.y = row;

	for (auto obj : objects)
	{
		Item* item = dynamic_cast<Item*>(obj);
		if (item == nullptr) continue;

		if (item->GetPosition() == pos)
		{
			item->Drop();
			nItems++;
			break;
		}
	}

	this->SetPosition(pos);
}

void Monster::Update() 
{
	Move();
	CheckRange();
}