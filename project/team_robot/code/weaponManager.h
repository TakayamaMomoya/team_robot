//*****************************************************
//
// 武器の管理[weaponManager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _WEAPONMANAGER_H_
#define _WEAPONMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CWeaponManager : public CObject
{
public:
	CWeaponManager();	// コンストラクタ
	~CWeaponManager();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void Draw(void);	// 描画

	// 静的メンバ関数
	static CWeaponManager *Create(void);

private:
	// メンバ変数
	//vector<CMob*> m_aAlly;	// 味方のリスト

	// 静的メンバ変数
	static CWeaponManager *s_pMobFactory;	// 自身のポインタ
};

#endif