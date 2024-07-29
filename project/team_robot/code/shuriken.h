//*****************************************************
//
// 手裏剣の処理[shuriken.h]
// Author:大原怜将
//
//*****************************************************

#ifndef _SHURIKEN_H_
#define _SHURIKEN_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CEffekseerEffect;

//*****************************************************
// クラスの定義
//*****************************************************
class CShuriken : public CObjectX
{
public:
	CShuriken();	// コンストラクタ
	~CShuriken();	                // デストラクタ

	// メンバ関数
	HRESULT Init(void);             // 初期化処理
	void Uninit(void);              // 終了処理
	void Update(void);              // 更新処理
	void Draw(void);                // 描画処理
	void Hit(float fDamage);	// ヒット処理
	static CShuriken* Create(D3DXVECTOR3 pos, D3DXVECTOR3 vecForward);     // 生成処理

	// 設定系
	static std::list<CShuriken*> GetList(void) { return m_aShuriken; }

private:
	// メンバ関数
	void CalcMove(D3DXVECTOR3 vecForward);	// 移動量計算
	void Debug(void);	// デバッグ処理

	// メンバ変数
	float m_fLife;	// 寿命
	D3DXVECTOR3 m_vecDest;	// 目標に向かうベクトル
	D3DXVECTOR3 m_relativePos;	// 敵の位置からの相対座標
	CEffekseerEffect *m_pWindEffect;	// 回転風のエフェクト

	// 静的メンバ関数
	static std::list<CShuriken*> m_aShuriken;  // 手裏剣のリスト
};

#endif