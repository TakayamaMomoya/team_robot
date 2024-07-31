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
#include "meshCube.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CSkybox : public CMeshCube
{
public:
	CSkybox(int nPriority = 2);	// コンストラクタ
	~CSkybox();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CSkybox *Create();
	static CSkybox *GetInstance() { return m_pSkybox; };	// インスタンスの取得

private:
	// メンバ関数
	void FollowPlayer(void);	// プレイヤーの追従
	void DetectionChangeTexture(void);	// テクスチャ変更の検知

	static CSkybox *m_pSkybox;	// 自身のポインタ
};

#endif

