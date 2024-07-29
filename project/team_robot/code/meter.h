//*****************************************************
//
// メーター[meter.h]
// Author:森川駿弥
//
//*****************************************************

#ifndef _METER_H_
#define _METER_H_

//*****************************************************
// 前方宣言
//*****************************************************
class CNumber;
class CUI;

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CMeter : public CObject
{
public:
	CMeter(int nPriority = 0);	// コンストラクタ
	~CMeter();	//	デストラクタ

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void Update();

	// 静的メンバ関数
	static CMeter* Create();	// 生成
	static CMeter* GetInstance() { return m_pMeter; }	// インスタンス取得

private:
	// メンバ関数
	void Acceleration();
	void Needle();

	//メンバ変数
	CNumber* m_pNumber;	// ナンバーのポインタ
	CUI* m_pNeedle;	// 針UIのポインタ
	CUI* m_pBackMeter;	// メーター背景のポインタ

	// 静的メンバ変数
	static CMeter* m_pMeter;	// 自身のポインタ
};

#endif
