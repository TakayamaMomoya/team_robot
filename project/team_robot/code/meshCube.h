//*****************************************************
//
// メッシュキューブ[meshCube.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _MESHCUBE_H_
#define _MESHCUBE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object3D.h"

//*****************************************************
// 前方定義
//*****************************************************
class CPolygon3D;

//*****************************************************
// クラスの定義
//*****************************************************
class CMeshCube : public CObject3D
{
public:
	CMeshCube(int nPriority = 6);	// コンストラクタ
	~CMeshCube();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理

	// 静的メンバ関数
	static CMeshCube *Create(void);	// 生成処理

private:
	// 列挙型定義
	enum E_Rot
	{// 向き
		ROT_UP = 0,	// 上
		ROT_DOWN,	// 下
		ROT_RIGHT,	// 右
		ROT_LEFT,	// 左
		ROT_FRONT,	// 前
		ROT_BACK,	// 後
		ROT_MAX
	};

	// メンバ関数
	void SetNormal(void);	// 法線の設定

	// メンバ変数
	D3DXVECTOR3 m_size;	// サイズ
	std::vector<CPolygon3D*> m_apPolygon3D;	// 3Dポリゴンのポインタ
};

#endif