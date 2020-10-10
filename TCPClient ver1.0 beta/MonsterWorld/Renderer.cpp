#include "PCH.h"

Renderer::Renderer() : Row(8), Col(8), origin(0, 0)
{
	screen = new char*[Row];
	for (int i = 0; i < Row; i++) screen[i] = new char[Col * 2 + 2];
	srand((unsigned int)time(nullptr));
}

Renderer::~Renderer()
{
	for (int i = 0; i < Row; i++) delete[] screen[i];
	delete[] screen;
}

void Renderer::Clear()
{
	for (int i = 0; i < Row; i++)
	{
		memset(screen[i], '.', Col * 2);
		screen[i][Col * 2] = '\n';
		screen[i][Col * 2 + 1] = '\0';
	}
}

void Renderer::Draw(const GameObject& obj)
{
	if (obj.IsEnable() == false) return;

	int x = obj.GetX();
	int y = obj.GetY();
	
	strncpy(screen[y] + x * 2, obj.GetShape().c_str(), obj.GetShapeSize());
}


void Renderer::Render() const
{
	Borland::Gotoxy(origin);
	for (int i = 0; i < Row; i++) cout << screen[i];
	
	for (int i = 0; i < 127; ++i) Sleep(1);
}

void Renderer::ShowStatus(GameObjectManager& manager) const
{
	Borland::Gotoxy(0, Row);
	const vector<GameObject*>& objects = *manager.GetObjects();

	for (auto obj : objects)
	{
		Monster* monster = dynamic_cast<Monster*>(obj);
		if (monster == nullptr) continue;
		cout << monster->GetName() << " ";
		cout << monster->GetShape() << " : ";
		cout << monster->GetNumOfItem() << endl;
	}
}

void Renderer::GameOver() const
{
	Borland::Gotoxy(0, Row + 8);
	for (int i = 0; i < Col / 2; i++) cout << "-";
	cout << "게임 종료";
	for (int i = 0; i < Col / 2; i++) cout << "-";
	cout << endl;
}

void Renderer::SetScreen(const char* other)
{
	int col = Col * 2;
	for (int row = 0; row < Row; ++row) {

		strncpy(screen[row], &other[row * (col +1)], col + 1);
		screen[row][col + 1] = '\0';
	}
}

string Renderer::GetScreen() const
{
	string map = "";
	for (int i = 0; i < Row; ++i) {
		map += screen[i];
	}

	return map;
}