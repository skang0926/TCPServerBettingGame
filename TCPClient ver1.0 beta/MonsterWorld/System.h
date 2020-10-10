#ifndef _SYSTEM_H
#define _SYSTEM_H

#define STRMAX	30

enum class KeyCode {
	Enter,
};

class System
{
public:
	static bool GetKeyDown(KeyCode keyCode);
	static void EndOfFrame();
	static void Clear();
	static string GetStr() { return str; }

private:
	System();
	System(const System& other);
	~System();

	static map<KeyCode, pair<int, int>> keyStates;
	static int major;
	static int minor;
	static string str;
	static short strIdx;
};
#endif