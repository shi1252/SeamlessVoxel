#include "Defined.h"
#include "SeamlessVoxelEngine.h"

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow)
{
	SVEngine* engine = new SVEngine(hInst);
	if (!engine)
	{
		return -1;
	}

	if (engine->Initialize())
	{
		engine->Run();
	}

	delete engine;
	engine = nullptr;

	return 0;
}