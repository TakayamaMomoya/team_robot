//*****************************************************
//
// ３Dアニメーションポリゴン[anim3D.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _ANIM3D_H_	// 二重インクルード防止
#define _ANIM3D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "polygon3D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CAnim3D : public CPolygon3D
{
public:
	CAnim3D(int nPriority = 6);	// コンストラクタ
	~CAnim3D();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理
	void RandStart(void);	// アニメーションのスタートをランダムにする処理

	// 静的メンバ変数
	static CAnim3D *Create(D3DXVECTOR3 pos, int nNumAnim, int nTimeAnim, bool bLoop = false);	// 生成処理

private:
	// メンバ関数
	void SetAnim(int nAnim, int nNumAnim);	// アニメーションの設定

	// メンバ変数
	int m_nCounterAnim;	// アニメーションカウンター
	int m_nPatternAnim;	// アニメパターンNO.
	int m_nSpeedAnim;	// アニメーション速度
	int m_nNumAnim;	// アニメーション枚数
	bool m_bLoop;	// ループするかどうか
	bool m_bFinish;	// 終了したかどうか
};

#endif