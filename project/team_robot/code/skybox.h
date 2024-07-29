//*****************************************************
//
// スカイボックスの処理[skybox.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SKYBOX_H_
#define _SKYBOX_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CSkybox : public CObjectX
{
public:
	CSkybox(int nPriority = 2);	// コンストラクタ
	~CSkybox();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSkybox *Create();
	static CSkybox *GetInstance();	// インスタンスの取得

private:
	// メンバ関数
	void FollowPlayer(void);	// プレイヤーの追従
	void DetectionChangeTexture(void);	// テクスチャ変更の検知

	static CSkybox *m_pSkybox;	// 自身のポインタ
};

#endif

