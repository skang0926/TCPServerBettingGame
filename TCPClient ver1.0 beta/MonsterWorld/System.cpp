#include "PCH.h"

map<KeyCode, pair<int, int>> System::keyStates = {
	{ KeyCode::Enter, { 13, 0} }
};

int System::major = -1;
int System::minor = 0;
string System::str = "";
short System::strIdx = 0;

System::System() {}

System::System(const System& other) {}

System::~System() {}

bool System::GetKeyDown(KeyCode keyCode)
{
	bool retval = false;
	
	if (kbhit()) {

		major = _getch();
		if (major == 0 || major == 224) {
			minor = _getch();
		}

		if (minor != 0) return retval;
		
		if (major == 8) {
			if (strIdx > 0) {
				strIdx--;
				Borland::Gotoxy(strIdx, 20);
				printf(" ");
				str.erase(str.end() - 1);				
				Borland::Gotoxy(strIdx, 20);
			}
		}
		else {
			if (strIdx < STRMAX) {
				Borland::Gotoxy(strIdx, 20);
				printf("%c", major);
				str += major;
				++strIdx;
			}
		}
		auto res = keyStates[keyCode];
		retval = res.first == major && res.second == minor;
	}
	

	return retval;
}

void System::EndOfFrame()
{
	major = -1;
	minor = 0;
}

void System::Clear()
{
	while (strIdx > 0) {
		--strIdx;
		Borland::Gotoxy(strIdx, 20);
		printf(" ");		
		Borland::Gotoxy(strIdx, 20);
	}
	str.clear();
}