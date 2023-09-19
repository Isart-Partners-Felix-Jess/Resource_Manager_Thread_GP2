#include <crtdbg.h>

#include <Application.hpp>

int main()
{
	//Detect memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(160); // Debugger should stop alloc nb
	Log::OpenFile("DebugLog.txt");

	Application app(800, 600);
	app.Update();

	//In App ?
	ResourcesManager::Destroy();
	Log::DeleteInstance();

	std::cout << "Program ended!\n";

	return 0;
}