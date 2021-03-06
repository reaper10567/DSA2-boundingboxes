#include "Main.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	ApplicationClass* App = ApplicationClass::GetInstance(hInstance, lpCmdLine, nCmdShow);

	App->Run();

	App->ReleaseInstance();

	return 0;
}