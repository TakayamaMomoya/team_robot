//*****************************************************
//
// プレイヤーコントローラー[playercontroller.cpp]
// Author:��山桃也
//
//*****************************************************
#ifndef _PLAYERCONTROLLER_H_
#define _PLAYERCONTROLLER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayer;

//*****************************************************
// クラスの定義
//*****************************************************
class CPlayerController : public CObject
{
public:
	CPlayerController();	// コンストラクタ
	~CPlayerController();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新

private:
	// メンバ関数
	void InputMove(void);	// 移動の入力
	void InputAttack(void);	// 攻撃の入力
};

#endif