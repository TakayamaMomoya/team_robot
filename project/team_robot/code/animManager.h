//*****************************************************
//
// アニメーションマネージャー[animManager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _ANIMMANAGER_H_	// 二重インクルード防止
#define _ANIMMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CAnim3D;

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_STRING	(256)	// 最大文字数

//*****************************************************
// クラスの定義
//*****************************************************
class CAnimManager : public CObject
{
public:
	// 列挙型定義
	enum E_TYPE
	{// 種類
		TYPE_EXPLOSION = 0,	// 爆発
		TYPE_FLASH,	// 火花
		TYPE_JUMP,	// 血しぶき
		TYPE_SLASH,	// 斬撃
		TYPE_AIRSLASH,	// 空中斬撃
		TYPE_MAX
	};

	CAnimManager(int nPriority = 4);	// コンストラクタ
	~CAnimManager();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理
	CAnim3D *CreateEffect(D3DXVECTOR3 pos, E_TYPE type);	// エフェクトの生成

	// 静的メンバ関数
	static CAnimManager *Create(void);	// 生成処理
	static CAnimManager *GetInstance(void) { return s_pAnimEffect3D; }	//	インスタンスの取得

private:
	// 構造体定義
	struct S_InfoAnimEffect
	{// アニメーションエフェクト情報
		char acPath[MAX_STRING];	// パス
		int nNumAnim;	// アニメーション数
		int nSpeedAnim;	// アニメーション速度
		float fSize;	// サイズ
		D3DXCOLOR col;	// 色
		bool bAdd;	// 加算合成するかどうか
		bool bLoop;	// ループするかどうか
		bool bBillboard;	// ビルボードにするかどうか
	};
	
	// メンバ関数
	void Load(void);
	
	// メンバ変数
	S_InfoAnimEffect *m_apAnimEffect[TYPE_MAX];	// 情報のポインタ

	// 静的メンバ変数
	static CAnimManager *s_pAnimEffect3D;	// 自身のポインタ
};

namespace Anim3D
{
//*****************************************************
// ショートカット関数
//*****************************************************
CAnim3D *CreateAnim(D3DXVECTOR3 pos, CAnimManager::E_TYPE type);	// アニメーションの生成
}

#endif