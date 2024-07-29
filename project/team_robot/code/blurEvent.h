//*****************************************************
//
// ブラーイベントの処理[blurEvent.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _BLUREVENT_H_    // 二重インクルードの防止
#define _BLUREVENT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CBlurEvent : public CObject
{
public:
    CBlurEvent();	// コンストラクタ
	~CBlurEvent();	// デストラクタ

    // メンバ関数
    HRESULT Init(void); // 初期化
    void Uninit(void);  // 終了
    void Update(void);  // 更新
    void Draw(void);  // 描画

    // 静的メンバ関数
    static CBlurEvent *Create(float fTime,float fDensity,float fSize);    // 生成処理
    static CBlurEvent *GetInstance(void) { return s_pBlurEvent; }    // インスタンスの取得

private:
    // メンバ変数
    float m_fTimerBlur; // ブラーをかけるタイマー
    float m_fTimerInitial; // 初期タイマー
    float m_fDensityBlur; // ブラーの濃さ
    float m_fSizeBlur; // ブラーのサイズ

    // 静的メンバ変数
    static CBlurEvent *s_pBlurEvent;   // 自身のポインタ
};

#endif
