//*****************************************************
//
// ブラーの処理[blur.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BLUR_H_    // 二重インクルードの防止
#define _BLUR_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"

namespace Blur
{
//*****************************************************
// 定数定義
//*****************************************************
const int NUM_RENDER = 3;   // ターゲットレンダリング用のインターフェースの数

//*****************************************************
// ショートカット関数
//*****************************************************
void SetBlur(float fSize, float fDensity);  // パラメーターの設定
void ResetBlur(void);   // パラメーターのリセット
void AddParameter(float fAddSize, float fAddDensity,float fMaxSize = FLT_MAX, float fMinSize = -FLT_MAX,float fMaxDesity = 1.0f,float fMinDensity = 0.0f); // パラメーターの加算
}

//*****************************************************
// クラスの定義
//*****************************************************
class CBlur
{
public:
	CBlur();	// コンストラクタ
	~CBlur();	// デストラクタ

    // メンバ関数
    void Init(void); // 初期化
    void Uninit(void);  // 終了
    void SaveRenderInfo(void);  // 描画に必要な要素を保存する
    void ChangeTarget(void);    // レンダーターゲットの変更
    void OverlapLastTexture(void);  // 前回のテクスチャを重ねる
    void RestoreTarget(void);   // レンダーターゲットの復元
    void DrawBuckBuffer(void);   // バックバッファへの描画
    void SwapBuffer(void);  // バッファーの入れ替え
    void SetRenderToNotBlur(void);  // ブラーしないレンダーへの変更
    void ClearNotBlur(void);    // ブラーしないレンダーのクリア
    void RestoreTargetBlur(void); // ブラーするレンダーへの変更

    // 変数取得・設定関数
    D3DXCOLOR GetPolygonColor(void) { return m_colPolygon; }    // ポリゴンの色
    void SetPolygonColor(D3DXCOLOR col) { m_colPolygon = col; }
    float GetAddSizePolygon(void) { return m_fAddSizePolygon; }    // ポリゴンの追加サイズ
    void SetAddSizePolygon(float fScale) { m_fAddSizePolygon = fScale; }
    D3DXMATRIX GetViwDef(void) { return m_mtxViewDef; }    // 保存用のビューマトリックス
    D3DXMATRIX GetProjDef(void) { return m_mtxProjDef; }    // 保存用のプロジェクションマトリックス

    // 静的メンバ関数
    static CBlur *Create(void);   // 生成
    static CBlur *GetInstance(void) { return m_pBlur; }    // 取得処理

private:
    // メンバ関数
    void CreatePolygon(void);

    // メンバ変数
    LPDIRECT3DTEXTURE9 m_apTextureMT[Blur::NUM_RENDER];    // レンダリングターゲット用テクスチャ
    LPDIRECT3DSURFACE9 m_apRenderMT[Blur::NUM_RENDER]; // ターゲットレンダリング用インターフェース
    LPDIRECT3DSURFACE9 m_pZBuffMT;  // ターゲットレンダリング用Zバッファ
    D3DVIEWPORT9 m_viewpotrMT;  // ターゲットレンダリング用ビューポート
    LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;   // フィードバック用ポリゴンの頂点バッファ
    LPDIRECT3DSURFACE9 m_pRenderDef;    // 保存用のインターフェイス
    LPDIRECT3DSURFACE9 m_pZBuffDef; // 保存用のZバッファ
    D3DVIEWPORT9 m_viewportDef; // 保存用のビューポート
    D3DXMATRIX m_mtxViewDef;    // 保存用のビューマトリックス
    D3DXMATRIX m_mtxProjDef;    // 保存用のプロジェクションマトリックス
    float m_fAddSizePolygon;   // ポリゴンの差分の大きさ
    D3DXCOLOR m_colPolygon; // ポリゴンの色

    // 静的メンバ変数
    static CBlur *m_pBlur;  // 自身のポインタ
};

#endif
