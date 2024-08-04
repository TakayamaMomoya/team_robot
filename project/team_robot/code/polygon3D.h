//*****************************************************
//
// 3Dポリゴンの処理[polygon3D.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _POLYGON3D_H_
#define _POLYGON3D_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object3D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CPolygon3D : public CObject3D
{
public:
	enum MODE
	{
		MODE_NORMAL = 0,	// 通常モード
		MODE_BILLBOARD,	// ビルボード
		MODE_STRETCHBILLBOARD,	// ストレッチビルボード
		MODE_MAX
	};

	CPolygon3D(int nPriority = 5);	// コンストラクタ
	~CPolygon3D();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtx(void);	// 頂点情報の設定
	LPDIRECT3DVERTEXBUFFER9 CreateVtxBuff(int nNumVtx = 4);	// 頂点バッファの生成

	// 変数取得・設定関数
	float GetWidth(void) { return m_width; }	// 幅
	float GetHeight(void) { return m_heigth; }	// 高さ
	void SetSize(float width, float height);
	void SetIdxTexture(int nIdx) { m_nIdxTexture = nIdx; }	// テクスチャ番号
	int GetIdxTexture(void) { return m_nIdxTexture; }
	D3DXCOLOR GetColor(void) { return m_col; }	// 色
	void SetColor(D3DXCOLOR col);
	void SetTex(D3DXVECTOR2 rd, D3DXVECTOR2 lu);	// テクスチャ座標
	void SetFactSB(float fFact) { m_fFactSB = fFact; }	// ストレッチビルボードの補正値
	void SetMode(MODE mode);	// 描画モード
	MODE GetMode(void) { return m_mode; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }	// 頂点バッファ
	void SetNormal(D3DXVECTOR3 nor);	// 法線の設定

	// 静的メンバ関数
	static CPolygon3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });

private:
	// メンバ関数
	void SetVtxNormal(void);
	void SetVtxStretchBillboard(void);
	void SetMtx(void);
	void SetMtxBillboard(void);

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファへのポインタ
	D3DXCOLOR m_col;	// 色
	float m_width;	// 幅
	float m_heigth;	// 高さ
	float m_fFactSB;	// ストレッチビルボードのときの見やすさの補正
	int m_nIdxTexture;	// テクスチャの番号
	int m_nNumVtx;	// 頂点数
	MODE m_mode;	// モード
};

#endif