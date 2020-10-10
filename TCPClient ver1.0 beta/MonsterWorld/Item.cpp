#include "PCH.h"

Item::Item(const string& shape, float x, float y)
	: GameObject(shape, x, y) {}

Item::Item(const string& shape, const Position& pos)
	: GameObject(shape, pos) {}

Item::~Item() {}

void Item::Update() {}

void Item::Drop()
{
	this->Disable();
}