//*****************************************************
//
// リザルト処理[result.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CUI;
class CStateResult;
class CNumber;

//*****************************************************
// クラスの定義
//*****************************************************
class CResult : public CObject
{// チュートリアルのクラス
public:
	CResult();	// コンストラクタ
	~CResult();	// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void);	// 初期化
	virtual void Uninit(void);	// 終了
	virtual void Update();	// 更新
	virtual void Draw();	// 描画
	void ChangeState(CStateResult *pState);	// ステイトの変更
	void StartFade(void);	// フェード

	// 変数取得・設定関数

	// 静的メンバ関数
	static CResult *Create(void);	// 生成処理
private:
	// メンバ関数
	void SetPlayer(void);	// プレイヤー設定
	void SetCamera(void);	// カメラ設定

	// メンバ変数
	CStateResult *m_pState;
};

class CStateResult
{// ステイトの基底クラス
public:
	CStateResult() {};	// コンストラクタ
	virtual ~CStateResult() {};	 // デストラクタ

	// メンバ関数
	virtual void Init(CResult *pResult) = 0;	// 初期化処理
	virtual void Uninit(CResult *pResult);	// 終了処理
	virtual void Update(CResult *pResult) = 0;	// 更新処理

private:
};

class CStateResultDispTime : public CStateResult
{// タイム表示
public:
	CStateResultDispTime();	// コンストラクタ
	virtual ~CStateResultDispTime();	 // デストラクタ

	// メンバ関数
	void Init(CResult *pResult) override;	// 初期化処理
	void Uninit(CResult *pResult) override;	// 終了処理
	void Update(CResult *pResult) override;	// 更新処理

private:
	enum E_NUMBER
	{
		NUMBER_MINUTE = 0,	// 分表示
		NUMBER_SECOND,	// 秒表示
		NUMBER_MILLISSEC,	// ミリ秒表示
		NUMBER_MAX
	};

	// メンバ関数
	void SetNumber(void);	// 数字の設定

	// メンバ変数
	CNumber *m_apTime[NUMBER_MAX];	// 数字のポインタ配列
};

namespace Tutorial
{
CUI *CreateUIDefault(void);
}

#endif