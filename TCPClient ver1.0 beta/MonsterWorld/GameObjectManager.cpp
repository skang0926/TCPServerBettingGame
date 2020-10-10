#include "PCH.h"

GameObjectManager::GameObjectManager(int nEnemies)
	: MaxItems(Renderer::Instance().GetRow() * Renderer::Instance().GetCol()), 
	MaxEnemies(nEnemies), numOfItems(0), numOfEnemies(0) 
{
	int row = Renderer::Instance().GetRow();
	int col = Renderer::Instance().GetCol();

	for (int y = 0; y < row; y++)
	{
		for (int x = 0; x < col; x++)
		{
			objects.push_back(new Item("¡á", x, y));
			numOfItems++;
		}
	}
	
	char monsters[5][3] = { "¡×", "¡Ø", "¡Ý", "¡Ú", "£¦" };
	char name[5][10] = { "Ghost", "Bug", "Circle", "Star", "Bird" };

	for (int i = 0; i < MaxEnemies; i++)
	{
		Position spawnPos(rand() % col, rand() % row);
		Monster* monster = new Monster(monsters[i], spawnPos, name[i], objects);
		objects.push_back(monster);
		counts.push_back(monster->GetNumOfItem());
		numOfEnemies++;
	}
}

GameObjectManager::~GameObjectManager() {}

void GameObjectManager::Update()
{
	for (auto obj : objects)
	{
		obj->Update();
		obj->Draw();
	}
	
	for (int i = 0, j = 0;  i < objects.size(); ++i) 
	{
		Monster* monster = dynamic_cast<Monster*>(objects[i]);
		if (monster == nullptr) continue;

		counts[j++] = monster->GetNumOfItem();
	}

	for (auto it = objects.begin(); it != objects.end();)
	{
		if ((*it)->IsEnable() == false) 
		{
			if (dynamic_cast<Item*>(*it))
			{
				delete *it;
				it = objects.erase(it);
				numOfItems--;
			}
			else it++;
		}
		else it++;
	}
}

void GameObjectManager::SetCounts(int* num)
{
	for (int i = 0, j = 0; i < objects.size(); ++i) {
		Monster* monster = dynamic_cast<Monster*>(objects[i]);		
		if (monster == nullptr) continue;

		counts[j] = num[j];
		monster->SetNumOfItem(counts[j]);
		++j;
	}
}