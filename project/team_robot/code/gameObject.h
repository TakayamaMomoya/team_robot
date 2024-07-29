//*****************************************************
//
// ゲームオブジェクト[gameObject.cpp]
// Author:髙山桃也
//
//*****************************************************
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラス定義
//*****************************************************
class CGameObject : public CObject
{
public:
	CGameObject(int nPriority) : CObject(nPriority), m_pos(), m_rot() {};	// コンストラクタ
	~CGameObject() {};	// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void) = 0;	// 初期化
	virtual void Uninit(void) { CObject::Release(); }	// 終了
	virtual void Update(void) = 0;	// 更新
	virtual void Draw(void) = 0;	// 描画

	// 変数取得・設定関数
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 位置
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }	// 向き
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }

private:
	// メンバ変数
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
};

#endif