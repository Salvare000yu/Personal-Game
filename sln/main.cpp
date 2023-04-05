#include <vector>

#include "MyDirectXGame.h"

#include "FbxLoader.h"

#include "fbxsdk.h"//fbxsdk関数呼び出し

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	// メッセージで分岐
	switch (msg) {
	case WM_DESTROY: // ウィンドウが破棄された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // 標準の処理を行う
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameBase* game = new MyDirectXGame;

	game->Run();

	delete game;

	//FbxLoader::GetInstance()->Finalize();

	return 0;
}