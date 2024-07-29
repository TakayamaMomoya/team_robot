//*****************************************************
//
// メイン処理[main.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "manager.h"
#include "inputkeyboard.h"
#include "object.h"
#include "renderer.h"
#include "debugproc.h"

//*****************************************************
// プロトタイプ宣言
//*****************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************
// グローバル変数宣言
//*****************************************************
int g_nCountFPS;	// FPSカウンター

//=====================================================
// メイン関数
//=====================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	//乱数シード値の設定
	srand((unsigned int)time(0));

	// マネージャーの宣言
	CManager *pManager = nullptr;
	
	DWORD dwCurrentTime;				//現在時刻
	DWORD dwExecLastTime;				//最後に処理した時刻
	DWORD dwFrameCount;					//フレームカウント
	DWORD dwFPSLastTime;				//最後にFPSを計測した時間

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//フォントポインタ・FPSカウンタの初期化
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		nullptr,
		CLASS_NAME,
		LoadIcon(nullptr,IDI_APPLICATION),
	};

	HWND hWnd;
	MSG msg;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域をウィンドウサイズに合わせる
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウを生成
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,  // 調整された幅
		rect.bottom - rect.top,  // 調整された高さ
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	if (pManager == nullptr)
	{
		// インスタンス生成
		pManager = new CManager;

#ifdef _DEBUG
		// マネージャーの初期化
		pManager->Init(hInstance, hWnd, TRUE);
#else 
		// マネージャーの初期化
		pManager->Init(hInstance, hWnd, FALSE);
#endif	
	}

	//RECT size;
	//RECT wndsize;
	//GetClientRect(hWnd, &size);
	//GetWindowRect(hWnd, &wndsize);
	//wndsize.right = wndsize.right - wndsize.left;
	//wndsize.bottom = wndsize.bottom - wndsize.top;
	//SetWindowPos(hWnd, NULL, 0, 0, SCREEN_WIDTH + wndsize.right - size.right, SCREEN_HEIGHT + wndsize.bottom - size.bottom, SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOZORDER);

	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

#ifdef _DEBUG
	// Imguiの初期化
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.DisplaySize = ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT);

	// imgui設定
	ImGui::StyleColorsDark();

	LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

	ImGui_ImplDX9_Init(pDevice);
	ImGui_ImplWin32_Init(hWnd);
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
#endif

	// メッセージループ
	while(true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0)
		{// Windowsの処理
			if (msg.message == WM_QUIT)
			{// WM＿QUITメッセージを受け取ったらループを抜ける
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理
			
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPS計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				//FPSを測定した時間を保存する
				dwFPSLastTime = dwCurrentTime;

				//フレームカウントのクリア
				dwFrameCount = 0;
			}

			//現在時刻の取得
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				float fDeltaTime = (dwCurrentTime - dwExecLastTime) * 0.001f;

				CDebugProc::GetInstance()->Print("\nデルタタイム[%f]", fDeltaTime);
				CDebugProc::GetInstance()->Print("\nCurrentTime[%d]", dwCurrentTime);
				CDebugProc::GetInstance()->Print("\nLastTime[%dw]", dwExecLastTime);

				//処理開始の時刻を計算
				dwExecLastTime = dwCurrentTime;

				if (pManager != nullptr)
				{
#ifdef _DEBUG
					RECT rect;
					GetClientRect(hWnd, &rect);
					float width = static_cast<float>(rect.right - rect.left);
					float height = static_cast<float>(rect.bottom - rect.top);
					io.DisplaySize = ImVec2(width, height);
#endif
					// ティックの設定
					CManager::SetTick(fDeltaTime);

					// 更新処理
					pManager->Update();

					// 描画処理
					pManager->Draw();
				}

				//FPSカウンタ経過
				dwFrameCount++;
			}
		}
	}

	if (pManager != nullptr)
	{
#ifdef _DEBUG
		// imgui終了
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX9_Shutdown();
		ImGui::DestroyContext();
#endif

		// 終了処理
		pManager->Uninit();

		// インスタンスの破棄
		delete pManager;
		pManager = nullptr;
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//=====================================================
// ウィンドウプロシージャ
//=====================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg)
	{
		// ウィンドウ破棄メッセージを受け取った場合
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CLOSE:

		// 確認メッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{// イエスの場合
			// ウィンドウ破棄
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}

	case WM_KEYDOWN:
		switch (wParam)
		{
			// [ESC]キーが押された
		case VK_ESCAPE:

			// 確認メッセージ
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{// イエスの場合
				// ウィンドウ破棄
				DestroyWindow(hWnd);
			}
			break;
		}

		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=====================================================
// FPSの取得
//=====================================================
int GetFPS(void)
{
	return g_nCountFPS;
}