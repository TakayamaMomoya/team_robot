//*****************************************************
//
// 敵スポナー[enemySpawner.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _ENEMYSPAWNER_H_
#define _ENEMYSPAWNER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object3D.h"
#include "polygon3D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemySpawner : public CObject3D
{
public:
	CEnemySpawner(int nPriority = 7);	// コンストラクタ
	~CEnemySpawner();	//	デストラクタ

	// メンバ関数
	HRESULT Init();	// 初期化
	void Uninit();	// 終了
	void Update();	// 更新
	void Draw();	// 描画

	// 変数取得・設定関数
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }	// 位置
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetRotation(float fRot) { m_fRot = fRot; }	// 向き取得設定
	float GetRotation() { return m_fRot; }
	void SetIdxSpline(int nIdx) { m_nIdxSpline = nIdx; }	// スプラインの番号
	int GetIdxSpline(void) { return m_nIdxSpline; }

	// 静的メンバ関数
	static CEnemySpawner* Create(int nIdxSpline = 0, float fRot = 0.0f, float fLength = 1000.0f);	// 生成

private:
	// メンバ関数
	void SetPositionSpline(void);	// スプラインに対応した位置設定
	void SetPolygon(void);
	void CreateEnemy(void);	// 敵の生成

	// メンバ変数
	int m_nTransitionTime;		// 遷移時間
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posStart;	// ゴールの始点
	D3DXVECTOR3 m_posEnd;	// ゴールの終点
	float m_fRot;		// 向き
	float m_fLength;	// 長さ
	int m_nIdxSpline;	// スプラインの番号
	CPolygon3D* m_pObj3D;	// オブジェクト3Dのポインタ
};

#endif
