#pragma once
#include "PhmsDataType.h"
#include <string>

using namespace::std;
class CSessionIdManager
{
public:
	static int SetSessionId(string stringSessionId);
	static int GetSessionId(string& stringSessionId);
	static void HandleSessionSync(string stringSessionId);

	static int SetGWSessionId(string stringSessionId);
	static int GetGWSessionId(string& stringSessionId);
	static void HandleGWSessionSync(string stringSessionId);
};
