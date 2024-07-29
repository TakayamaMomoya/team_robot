//*****************************************************
//
// ブラーの処理[blur.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "blur.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const D3DXCOLOR COL_POLYGON_INITIAL = { 1.0f,1.0f,1.0f,0.1f };  // 初期の重ねるポリゴンの色
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CBlur *CBlur::m_pBlur = nullptr;    // 自身のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CBlur::CBlur() : m_colPolygon{}, m_apRenderMT{}, m_apTextureMT{}, m_viewpotrMT(), m_pZBuffMT(nullptr), m_pVtxBuffMT(nullptr), m_fAddSizePolygon(0.0f),
                    m_pRenderDef(nullptr), m_pZBuffDef(nullptr),m_viewportDef(),m_mtxProjDef(),m_mtxViewDef()
{

}

//=====================================================
// デストラクタ
//=====================================================
CBlur::~CBlur()
{

}

//=====================================================
// 生成処理
//=====================================================
CBlur *CBlur::Create(void)
{
    if (m_pBlur == nullptr)
    {
        m_pBlur = new CBlur;

        if (m_pBlur != nullptr)
        {
            m_pBlur->Init();
        }
    }

    return m_pBlur;
}

//=====================================================
// 初期化
//=====================================================
void CBlur::Init(void)
{
    m_colPolygon = COL_POLYGON_INITIAL;

    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    for (int i = 0; i < Blur::NUM_RENDER - 1; i++)
    {
        // レンダーターゲット用のテクスチャ生成
        if (SUCCEEDED(pDevice->CreateTexture(SCREEN_WIDTH,
            SCREEN_HEIGHT,
            1,
            D3DUSAGE_RENDERTARGET,
            D3DFMT_X8R8G8B8,
            D3DPOOL_DEFAULT,
            &m_apTextureMT[i],
            nullptr)))
        {
            // ターゲットレンダリング用インターフェイス生成
            if (FAILED(m_apTextureMT[i]->GetSurfaceLevel(0, &m_apRenderMT[i])))
            {
                assert(("ターゲットレンダリング用インターフェース生成に失敗", false));
            }
        }
        else
        {
            assert(("レンダーターゲット用のテクスチャ生成に失敗", false));
        }
    }

    // レンダーターゲット用のテクスチャ生成
    if (SUCCEEDED(pDevice->CreateTexture(SCREEN_WIDTH,
        SCREEN_HEIGHT,
        1,
        D3DUSAGE_RENDERTARGET,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &m_apTextureMT[Blur::NUM_RENDER - 1],
        nullptr)))
    {
        // ターゲットレンダリング用インターフェイス生成
        if (FAILED(m_apTextureMT[Blur::NUM_RENDER - 1]->GetSurfaceLevel(0, &m_apRenderMT[Blur::NUM_RENDER - 1])))
        {
            assert(("ターゲットレンダリング用インターフェース生成に失敗", false));
        }
    }
    else
    {
        assert(("レンダーターゲット用のテクスチャ生成に失敗", false));
    }

    // ターゲットレンダリング用Zバッファ生成
    if (FAILED(pDevice->CreateDepthStencilSurface(SCREEN_WIDTH,
        SCREEN_HEIGHT,
        D3DFMT_D16,
        D3DMULTISAMPLE_NONE,
        0,
        TRUE,
        &m_pZBuffMT,
        nullptr)))
    {
        assert(("ターゲットレンダリング用Zバッファ生成に失敗", false));
    }

    // レンダリングターゲットとZバッファを保存
    LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

    pDevice->GetRenderTarget(0, &pRenderDef);
    pDevice->GetDepthStencilSurface(&pZBuffDef);

    // Zバッファの設定
    pDevice->SetDepthStencilSurface(m_pZBuffMT);

    for (int i = 0; i < Blur::NUM_RENDER; i++)
    {
        // レンダリングターゲットを設定
        pDevice->SetRenderTarget(0, m_apRenderMT[i]);

        // クリアする
        pDevice->Clear(0, nullptr,
            (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
            D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
    }

    // レンダリングターゲットとZバッファを元に戻す
    pDevice->SetRenderTarget(0, pRenderDef);
    pDevice->SetDepthStencilSurface(pZBuffDef);

    // ビューポート初期化
    m_viewpotrMT.X = 0;
    m_viewpotrMT.Y = 0;
    m_viewpotrMT.Width = SCREEN_WIDTH;
    m_viewpotrMT.Height = SCREEN_HEIGHT;
    m_viewpotrMT.MinZ = 0.0f;
    m_viewpotrMT.MaxZ = 1.0f;

    // ポリゴンの生成
    CreatePolygon();
}

//=====================================================
// 終了
//=====================================================
void CBlur::Uninit(void)
{
    m_pBlur = nullptr;

    for (int i = 0; i < Blur::NUM_RENDER; i++)
    {
        if (m_apRenderMT[i] != nullptr)
        {
            m_apRenderMT[i]->Release();
            m_apRenderMT[i] = nullptr;
        }

        if (m_apTextureMT[i] != nullptr)
        {
            m_apTextureMT[i]->Release();
            m_apTextureMT[i] = nullptr;
        }
    }

    if (m_pZBuffMT != nullptr)
    {
        m_pZBuffMT->Release();
        m_pZBuffMT = nullptr;
    }

    if (m_pVtxBuffMT != nullptr)
    {
        m_pVtxBuffMT->Release();
        m_pVtxBuffMT = nullptr;
    }

    if (m_pRenderDef != nullptr)
    {
        m_pRenderDef->Release();
        m_pRenderDef = nullptr;
    }

    if (m_pZBuffDef != nullptr)
    {
        m_pZBuffDef->Release();
        m_pZBuffDef = nullptr;
    }
}

//=====================================================
// ポリゴンの生成
//=====================================================
void CBlur::CreatePolygon(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    // 頂点バッファの生成
    if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,
        D3DPOOL_MANAGED,
        &m_pVtxBuffMT,
        NULL)))
    {
        assert(("頂点バッファの生成に失敗", false));
    }

    // 頂点情報のポインタ
    VERTEX_2D *pVtx;
    
    // 頂点バッファをロックし、頂点情報へのポインタを取得
    m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

    // 頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    // 法線ベクトルの設定
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    // 頂点カラーの設定
    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

    // テクスチャ座標
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    // 頂点バッファをアンロック
    m_pVtxBuffMT->Unlock();
}

//=====================================================
// 描画に必要な情報の保存
//=====================================================
void CBlur::SaveRenderInfo(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    // もろもろ保存
    pDevice->GetRenderTarget(0, &m_pRenderDef);
    pDevice->GetDepthStencilSurface(&m_pZBuffDef);
    pDevice->GetViewport(&m_viewportDef);
    pDevice->GetTransform(D3DTS_VIEW, &m_mtxViewDef);
    pDevice->GetTransform(D3DTS_PROJECTION, &m_mtxProjDef);
}

//=====================================================
// レンダーターゲットの変更
//=====================================================
void CBlur::ChangeTarget(void)
{
    CCamera *pCamera = CManager::GetCamera();

    if (pCamera == nullptr)
        return;

    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    D3DXMATRIX mtxView, mtxProj;
    float fAspect;

    // レンダリングターゲットとZバッファを設定
    pDevice->SetRenderTarget(0, m_apRenderMT[0]);
    pDevice->SetDepthStencilSurface(m_pZBuffMT);

    // テクスチャ用ビューポートに設定
    pDevice->SetViewport(&m_viewpotrMT);

    //プロジェクションマトリックス初期化
    D3DXMatrixIdentity(&mtxProj);

    fAspect = (float)m_viewpotrMT.Width / (float)m_viewpotrMT.Height;

    //プロジェクションマトリックス作成
    D3DXMatrixPerspectiveFovLH(&mtxProj,
        D3DXToRadian(45.0f),
        fAspect,
        MIN_DRAW,
        MAX_DRAW);

    //マトリックス設定
    pDevice->SetTransform(D3DTS_PROJECTION, &mtxProj);

    //ビューマトリックス初期化
    D3DXMatrixIdentity(&mtxView);

    CCamera::Camera *pInfoCamera = pCamera->GetCamera();

    //ビューマトリックス作成
    D3DXMatrixLookAtLH(&mtxView,
        &pInfoCamera->posV,
        &pInfoCamera->posR,
        &pInfoCamera->vecU);

    //ビューマトリックス設定
    pDevice->SetTransform(D3DTS_VIEW, &pInfoCamera->mtxView);
}

//=====================================================
// 前回のテクスチャを重ねる
//=====================================================
void CBlur::OverlapLastTexture(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    // 頂点情報のポインタ
    VERTEX_2D *pVtx;

    // 頂点バッファをロックし、頂点情報へのポインタを取得
    m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

    // 頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(m_fAddSizePolygon, m_fAddSizePolygon, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - m_fAddSizePolygon, m_fAddSizePolygon, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(m_fAddSizePolygon, SCREEN_HEIGHT - m_fAddSizePolygon, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - m_fAddSizePolygon, SCREEN_HEIGHT - m_fAddSizePolygon, 0.0f);

    // 頂点カラーの設定
    pVtx[0].col = m_colPolygon;
    pVtx[1].col = m_colPolygon;
    pVtx[2].col = m_colPolygon;
    pVtx[3].col = m_colPolygon;

    // 頂点バッファをアンロック
    m_pVtxBuffMT->Unlock();

    // 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    LPDIRECT3DTEXTURE9 pTexture = m_apTextureMT[1];

    // テクスチャ設定
    pDevice->SetTexture(0, pTexture);

    // 描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=====================================================
// レンダーターゲットの復元
//=====================================================
void CBlur::RestoreTarget(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    pDevice->SetRenderTarget(0, m_pRenderDef);
    pDevice->SetDepthStencilSurface(m_pZBuffDef);
    pDevice->SetViewport(&m_viewportDef);
    //pDevice->SetTransform(D3DTS_VIEW, &m_mtxViewDef);
    //pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjDef);
}

//=====================================================
// バックバッファへの描画
//=====================================================
void CBlur::DrawBuckBuffer(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    // 頂点情報のポインタ
    VERTEX_2D *pVtx;

    //頂点バッファをロックし、頂点情報へのポインタを取得
    m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

    //頂点座標の設定
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    //頂点カラーの設定
    pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

    //頂点バッファをアンロック
    m_pVtxBuffMT->Unlock();

    // 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

    // 頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    LPDIRECT3DTEXTURE9 pTexture = m_apTextureMT[0];

    // テクスチャ設定
    pDevice->SetTexture(0, pTexture);

    // 描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

    // テクスチャ設定
    pDevice->SetTexture(0, m_apTextureMT[2]);

    // 描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=====================================================
// バッファの入れ替え
//=====================================================
void CBlur::SwapBuffer(void)
{
    // 入れ替え用のバッファ
    LPDIRECT3DSURFACE9 pRednerWk;
    LPDIRECT3DTEXTURE9 pTextureWk;

    pRednerWk = m_apRenderMT[0];
    m_apRenderMT[0] = m_apRenderMT[1];
    m_apRenderMT[1] = pRednerWk;

    pTextureWk = m_apTextureMT[0];
    m_apTextureMT[0] = m_apTextureMT[1];
    m_apTextureMT[1] = pTextureWk;
}

//=====================================================
// ブラーしないレンダーへの変更
//=====================================================
void CBlur::SetRenderToNotBlur(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    pDevice->SetRenderTarget(0, m_apRenderMT[2]);
}

//=====================================================
// ブラーしないレンダーのリセット
//=====================================================
void CBlur::ClearNotBlur(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    LPDIRECT3DSURFACE9 m_pRenderDef;

    pDevice->GetRenderTarget(0, &m_pRenderDef);

    pDevice->SetRenderTarget(0, m_apRenderMT[2]);

    // 画面クリア
    pDevice->Clear(0, nullptr,
        (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
        D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

    pDevice->SetRenderTarget(0, m_pRenderDef);
}

//=====================================================
// ブラーするターゲットに戻す
//=====================================================
void CBlur::RestoreTargetBlur(void)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = Renderer::GetDevice();

    pDevice->SetRenderTarget(0, m_apRenderMT[0]);
}

namespace Blur
{
//=====================================================
// パラメーターの設定
//=====================================================
void SetBlur(float fSize, float fDensity)
{
    CBlur *pBlur = CBlur::GetInstance();

    if (pBlur != nullptr)
    {
        pBlur->SetAddSizePolygon(fSize);
        pBlur->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, fDensity));
    }
}

//=====================================================
// パラメーターのリセット
//=====================================================
void ResetBlur(void)
{
    CBlur *pBlur = CBlur::GetInstance();

    if (pBlur != nullptr)
    {
        pBlur->SetAddSizePolygon(0.0f);
        pBlur->SetPolygonColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
    }
}

//=====================================================
// パラメーターの加算
//=====================================================
void AddParameter(float fAddSize, float fAddDensity, float fMaxSize, float fMinSize, float fMaxDesity, float fMinDensity)
{
    CBlur *pBlur = CBlur::GetInstance();

    if (pBlur != nullptr)
    {
        float fSize = pBlur->GetAddSizePolygon();
        D3DXCOLOR col = pBlur->GetPolygonColor();

        fSize += fAddSize;
        col.a += fAddDensity;

        universal::LimitValuefloat(&fSize, fMaxSize, fMinSize);
        universal::LimitValuefloat(&col.a, fMaxDesity, fMinDensity);

        pBlur->SetAddSizePolygon(fSize);
        pBlur->SetPolygonColor(col);
    }
}
}