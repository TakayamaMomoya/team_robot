//*****************************************************
//
// タイトル処理[title.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CMenu;
class CPolygon2D;
class CFan3D;
class CMotion;
class CTitleBehavior;

//*****************************************************
// クラスの定義
//*****************************************************
class CTitle : public CScene
{
public:
	enum STATE
	{
		STATE_NONE = 0,	// 何もしてない状態
		STATE_OUT,		// フェードアウト状態
		START_MAX
	};

	CTitle();	// コンストラクタ
	~CTitle();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void ChangeBehavior(CTitleBehavior *pBehavior);
	STATE GetState(void) { return m_state; }
	static CMotion *GetPlayer(void) { return m_pPlayer; }
	static CMotion *GetBike(void) { return m_pBike; }
	
private:
	STATE m_state;				// 状態
	CPolygon2D *m_pTitleLogo;   // タイトルロゴのポインタ
	CPolygon2D *m_pTeamLogo;    // チームロゴのポインタ
	CFan3D *m_pFan3D;           // トンネルの扉のポインタ
	static CMotion *m_pPlayer;	// プレイヤーモデル
	static CMotion *m_pBike;   // バイクモデル
	CTitleBehavior *m_pBehavior;	// ビヘイビア
	float m_fTImerSmoke;	// 煙のスポーンタイマー
};

class CTitleBehavior
{// 基本ビヘイビア
public:
	CTitleBehavior();	// コンストラクタ
	virtual ~CTitleBehavior();	// デストラクタ

	virtual void Update(CTitle *pTItle) = 0;

private:
};

class CTitleStart : public CTitleBehavior
{// スタート表示状態
public:
	CTitleStart();	// コンストラクタ
	virtual ~CTitleStart();	// デストラクタ
	void Update(CTitle *pTItle) override;

private:
	CPolygon2D *m_pStart;	// スタート表示のポインタ
	CPolygon2D *m_pAfter;	// スタート表示の残像
};

class CTitleMenu : public CTitleBehavior
{// メニュー
public:
	enum MENU
	{
		MENU_GAME = 0,	// ゲーム
		MENU_TRANING,	// 訓練場
		MENU_MAX
	};

	CTitleMenu();	// コンストラクタ
	virtual ~CTitleMenu();	// デストラクタ
	void Update(CTitle *pTItle) override;

private:
	void Input(void);
	void ManageCursor(void);
	//void Fade(void);
	
	CPolygon2D *m_apMenu[MENU_MAX];	// メニュー項目
	CPolygon2D *m_pCursor;	// カーソル
	MENU m_menu;	// 選択メニュー項目

	bool m_bFade = false;
};

class CTitleMovePlayer : public CTitleBehavior
{
public:
	CTitleMovePlayer();  // コンストラクタ
	virtual ~CTitleMovePlayer();  // デストラクタ
	void Update(CTitle* pTItle) override;

private:
	void Fade(void);

};

#endif