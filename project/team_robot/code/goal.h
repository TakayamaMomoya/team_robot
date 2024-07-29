//*****************************************************
//
// ゴール処理[goal.h]
// Author:森川駿弥
//
//*****************************************************

#ifndef _GOAL_H_
#define _GOAL_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "player.h"
#include "polygon3D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CGoal : public CObject
{
public:
	CGoal(int nPriority = 7);	// コンストラクタ
	~CGoal();	//	デストラクタ

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetGoal(void);	// ゴールの設定

	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }	// 位置取得
	void SetRotation(float fRot) { m_fRot = fRot; }	// 向き取得設定
	float GetRotation() { return m_fRot; }	// 向き取得処理

	// 静的メンバ関数
	static CGoal* Create(D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f }, float fRot = 0.0f, float fLength = 1000.0f);	// 生成
	static CGoal *GetInstance(void) { return m_pGoal; }

private:
	//メンバ変数
	int m_nTransitionTime;		// 遷移時間
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posStart;	// ゴールの始点
	D3DXVECTOR3 m_posEnd;	// ゴールの終点
	float m_fRot;		// 向き
	float m_fLength;	// 長さ
	CPolygon3D* m_pObj3D;	// オブジェクト3Dのポインタ

	// 静的メンバ変数
	static CGoal *m_pGoal;	// 自身のポインタ
};

#endif
