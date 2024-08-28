//*****************************************************
//
// 3Dオブジェクト[object3D.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "gameObject.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CObject3D	: public CGameObject
{
public:
	CObject3D(int nPriority = 4);	// コンストラクタ
	~CObject3D();	//	デストラクタ

	// メンバ関数
	virtual HRESULT Init(void);	// 初期化
	virtual void Uninit(void);	// 終了
	virtual void Update(void);	// 更新
	virtual void Draw(void);	// 描画
	void ResetMtxParent(void);	// 親マトリックスのリセット
	D3DXVECTOR3 GetForward(void);	// マトリックスから前方ベクトルを取得
	D3DXVECTOR3 GetUp(void);	// マトリックスから上方ベクトルを取得
	D3DXVECTOR3 GetRight(void);	// マトリックスから右方ベクトルを取得

	// 変数取得・設定関数
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld; }	// ワールドマトリックス
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }
	D3DXMATRIX GetMatrixParent(void) { return m_mtxParent; }	// 親マトリックス
	void SetMatrixParent(D3DXMATRIX mtx) { m_mtxParent = mtx; }
	D3DXQUATERNION GetQuaternion(void) { return m_quat; }	// 累積クォータニオン
	void SetQuaternion(D3DXQUATERNION quat) { m_quat = quat; }

private:
	// メンバ変数
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXMATRIX m_mtxParent;	// 親マトリックス
	D3DXQUATERNION m_quat;	// 累積クォータニオン
};

#endif