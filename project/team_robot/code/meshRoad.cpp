//*****************************************************
//
// メッシュ道の処理[meshRoad.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "meshRoad.h"
#include "renderer.h"
#include "texture.h"
#include "effect3D.h"
#include "myLib.h"
#include "tunnel.h"
#include "inputkeyboard.h"
#include "guardRail.h"
#include <fstream>
#include "debugproc.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const UINT NUMVTX_NOTDRAW = 4;	// この頂点数未満の場合、描画しない
const float WIDTH_DEFAULT = 200.0f;	// デフォルトの幅
const float LENGTH_DEFAULT = 200.0f;	// デフォルトの長さ
const char* PATH_TEXTURE = "data\\TEXTURE\\MATERIAL\\road.jpg";	// テクスチャパス
const float DIST_DEFAULT = 200.0f;	// デフォルトの辺間の距離
const float WIDTH_ROAD = 600.0f;	// 道の幅
const float HEIGHT_LIMIT = 200.0f;	// 判定する高さの限界
}

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CMeshRoad *CMeshRoad::m_pMeshRoad = nullptr;	// 自身のポインタ
std::list<CMeshRoad*> CMeshRoad::s_aRoad;	// メッシュロードのコンテナ

//=====================================================
// コンストラクタ
//=====================================================
CMeshRoad::CMeshRoad(int nPriority) : CPolygon3D(nPriority), m_nNumVtx(0), m_pSpline(nullptr), m_pSplineL(nullptr), m_pSplineR(nullptr)
{
	s_aRoad.push_back(this);
}

//=====================================================
// デストラクタ
//=====================================================
CMeshRoad::~CMeshRoad()
{
	s_aRoad.remove(this);
}

//=====================================================
// 生成処理
//=====================================================
CMeshRoad *CMeshRoad::Create(const char* pPath)
{
	CMeshRoad *pMeshRoad = nullptr;

	if (pMeshRoad == nullptr)
	{
		// インスタンス生成
		pMeshRoad = new CMeshRoad;

		if (pMeshRoad != nullptr)
		{
			m_pMeshRoad = pMeshRoad;

			// 初期化処理
			pMeshRoad->Init();

			// 読込処理
			pMeshRoad->Load(pPath);
		}
	}

	return pMeshRoad;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMeshRoad::Init(void)
{
	// テクスチャ読み込み
	int nIdx = Texture::GetIdx(PATH_TEXTURE);
	SetIdxTexture(nIdx);

#ifdef _DEBUG
	//EnableWire(true);
#endif // _DEBUG

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CMeshRoad::Uninit(void)
{
	if (m_pSpline != nullptr)
	{
		delete m_pSpline;
		m_pSpline = nullptr;
	}

	if (m_pSplineL != nullptr)
	{
		delete m_pSplineL;
		m_pSplineL = nullptr;
	}

	if (m_pSplineR != nullptr)
	{
		delete m_pSplineR;
		m_pSplineR = nullptr;
	}

	m_pMeshRoad = nullptr;

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CMeshRoad::Update(void)
{
#ifdef _DEBUG
	for (SInfoRoadPoint info : m_aRoadPoint)
		CEffect3D::Create(info.pos, 50.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif // _DEBUG
}

//=====================================================
// 描画処理
//=====================================================
void CMeshRoad::Draw(void)
{
	if (m_nNumVtx < NUMVTX_NOTDRAW)	// 描画制限
		return;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX *pMtx = GetMatrix();

	// ワールドマトリックス初期化
	D3DXMatrixIdentity(pMtx);

	D3DXMATRIX mtxRot, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);

	// ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, pMtx);

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ設定
	int nIdxTexture = GetIdxTexture();
	LPDIRECT3DTEXTURE9 pTexture = Texture::GetTexture(nIdxTexture);
	pDevice->SetTexture(0, pTexture);

	// カリングを無効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

	// カリングを有効化
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=====================================================
// メッシュロードの選択
//=====================================================
void CMeshRoad::SelectMeshRoad(void)
{
	ImGui::Text("[SelectMeshRoad]");

	std::list<CMeshRoad*>::iterator it = std::find(s_aRoad.begin(), s_aRoad.end(), m_pMeshRoad);
	int nDist = std::distance(s_aRoad.begin(), it);
	int nSize = s_aRoad.size();

	if (ImGui::DragInt("RoadIndex", &nDist, 1.0f, 0, nSize - 1))
	{
		// イテレータを再初期化して指定番号の要素に移動
		it = s_aRoad.begin();
		std::advance(it, nDist);

		m_pMeshRoad = (*it);
	}

	if (ImGui::Button("NextRoad", ImVec2(70, 30)))
	{
		if (it != s_aRoad.end() && std::next(it) != s_aRoad.end())
			std::advance(it, 1);
	}
	if (ImGui::Button("PrevRoad", ImVec2(70, 30)))
	{
		if (it != s_aRoad.begin())
			std::advance(it, -1);
	}

	m_pMeshRoad = (*it);
}

//=====================================================
// ロードポイントの選択
//=====================================================
std::vector<CMeshRoad::SInfoRoadPoint>::iterator CMeshRoad::SelectRoadPoint(void)
{
	ImGui::Text("[SelectRoadPoint]");

	int nDist = std::distance(m_aRoadPoint.begin(), m_it);
	int nSize = m_aRoadPoint.size();

	if (ImGui::DragInt("Index", &nDist, 1.0f, 0, nSize - 1))
	{
		// イテレータを再初期化して指定番号の要素に移動
		m_it = m_aRoadPoint.begin();
		std::advance(m_it, nDist);
	}

	if (ImGui::Button("NextEdge", ImVec2(70, 30)))
	{
		if (m_it != m_aRoadPoint.end() && std::next(m_it) != m_aRoadPoint.end())
			std::advance(m_it, 1);
	}
	if (ImGui::Button("PrevEdge", ImVec2(70, 30)))
	{
		if (m_it != m_aRoadPoint.begin())
			std::advance(m_it, -1);
	}

	CEffect3D::Create(m_it->pos, 100.0f, 3, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	return m_it;
}

//=====================================================
// イテレーターからロードポイント番号を指定
//=====================================================
size_t CMeshRoad::GetIdxRoad(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it)
{
	if (m_aRoadPoint.empty())
		return 0;

	return std::distance(m_aRoadPoint.begin(), it);
}

//=====================================================
// ロードポイントの削除
//=====================================================
void CMeshRoad::DeleteRoadPoint(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it)
{
	m_aRoadPoint.erase(it);

	CreateVtxBuffEdge();

	m_it = m_aRoadPoint.begin();
}

//=====================================================
// イテレーターのリセット
//=====================================================
void CMeshRoad::ResetIterator(void)
{
	m_it = m_aRoadPoint.end() - 1;
}

//=====================================================
// ロードポイントの追加
//=====================================================
void CMeshRoad::AddRoadPoint(D3DXVECTOR3 pos, bool bReCreateVtx)
{
	// リストに情報を追加
	SInfoRoadPoint info;
	info.pos = pos;
	info.fWidth = 500.0f;

	// トンネルの終始点を保存
	std::vector<int> vStart;
	std::vector<int> vEnd;
	int nCnt = 0;

	vStart.resize(m_aTunnel.size());
	vEnd.resize(m_aTunnel.size());

	for (auto it : m_aTunnel)
	{
		int nStart = std::distance(m_aRoadPoint.begin(), it->GetItStart());
		int nEnd = std::distance(m_aRoadPoint.begin(), it->GetItEnd());

		vStart[nCnt] = nStart;
		vEnd[nCnt] = nEnd;

		nCnt++;
	}

	// ガードレールの終始点を保存
	nCnt = 0;

	std::vector<int> vStartGR;
	std::vector<int> vEndGR;

	vStartGR.resize(m_aGR.size());
	vEndGR.resize(m_aGR.size());

	for (auto it : m_aGR)
	{
		int nStart = std::distance(m_aRoadPoint.begin(), it->GetItStart());
		int nEnd = std::distance(m_aRoadPoint.begin(), it->GetItEnd());

		vStartGR[nCnt] = nStart;
		vEndGR[nCnt] = nEnd;

		nCnt++;
	}

	m_aRoadPoint.push_back(info);

	if (bReCreateVtx)	// ロードポイントに応じた頂点の再生成
		CreateVtxBuffEdge();

	// 終始点を再割り当て
	for (int i = 0; i < (int)m_aTunnel.size(); i++)
	{
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator it;

		it = m_aRoadPoint.begin();
		std::advance(it, vStart[i]);
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart = it;

		it = m_aRoadPoint.begin();
		std::advance(it, vEnd[i]);
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd = it;

		m_aTunnel[i]->SetStart(itStart);
		m_aTunnel[i]->SetEnd(itEnd);
	}

	// ガードレールの再割り当て
	for (int i = 0; i < (int)m_aGR.size(); i++)
	{
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator it;

		it = m_aRoadPoint.begin();
		std::advance(it, vStartGR[i]);
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart = it;

		it = m_aRoadPoint.begin();
		std::advance(it, vEndGR[i]);
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd = it;

		m_aGR[i]->SetStart(itStart);
		m_aGR[i]->SetEnd(itEnd);
	}
}

//=====================================================
// ロードポイントに応じた頂点の生成
//=====================================================
void CMeshRoad::CreateVtxBuffEdge(void)
{
	// スプラインの再生成
	CreateSpline();

	// 頂点の生成
	m_nNumVtx = m_aRoadPoint.size() * MeshRoad::NUM_VTX_IN_EDGE * MeshRoad::NUM_EDGE_IN_ROADPOINT;
	CreateVtxBuff(m_nNumVtx);

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	std::vector<SInfoRoadPoint>::iterator itRoadPoint;
	int nIdx = 0;

	for (itRoadPoint = m_aRoadPoint.begin(); itRoadPoint != m_aRoadPoint.end(); itRoadPoint++)
	{
		if (itRoadPoint != m_aRoadPoint.begin())
		{
			SInfoRoadPoint *pInfoRoadPointOld = &*std::prev(itRoadPoint);

			// ロードポイント間の頂点設定
			CreateVtxBetweenRoadPoint(*itRoadPoint, pVtx, pInfoRoadPointOld, nIdx);
		}

		pVtx += MeshRoad::NUM_EDGE_IN_ROADPOINT * MeshRoad::NUM_VTX_IN_EDGE;

		nIdx++;
	}

	// 頂点バッファをアンロック
	pVtxBuff->Unlock();
}

//=====================================================
// ロードポイント間の頂点設定
//=====================================================
void CMeshRoad::CreateVtxBetweenRoadPoint(SInfoRoadPoint infoRoadPoint, VERTEX_3D *pVtx, SInfoRoadPoint *pInfoRoadPointOld, int nIdx)
{
	if (pVtx == nullptr)
		assert(("CreateVtxBetweenRoadPointで頂点情報がnullです", false));

	if (m_pSpline == nullptr)
		assert(("CreateVtxBetweenRoadPointでスプラインがnullです", false));

	D3DXVECTOR3 posEdgeOld = {};	// 前回の辺の位置

	if (pInfoRoadPointOld != nullptr)
	{
		posEdgeOld = pInfoRoadPointOld->pos;
	}

	// ロードポイント間で必要な辺
	for (int i = 0; i < MeshRoad::NUM_EDGE_IN_ROADPOINT; i++)
	{
		D3DXVECTOR3 pos = infoRoadPoint.pos;

		if (pInfoRoadPointOld == nullptr)
		{// 前回のロードポイントがない場合、分割は実質無し
			pos = m_pSpline->Interpolate(pos.x, 0);

			// 前回の辺と比べない頂点位置設定
			pVtx[0].pos = pos;
			pVtx[1].pos = pos;

			pVtx[0].pos.z -= WIDTH_ROAD;
			pVtx[1].pos.z += WIDTH_ROAD;

			posEdgeOld = GetPosEdge(pVtx[0].pos, pVtx[1].pos);	// 辺の位置を保存
		}
		else
		{
			float fDiff = infoRoadPoint.pos.x - pInfoRoadPointOld->pos.x;
			float fRate = ((float)i + 1.0f) / MeshRoad::NUM_EDGE_IN_ROADPOINT;

			// 座標の決定
			pos = m_pSpline->Interpolate(fRate, nIdx);

			pVtx[0].pos = m_pSplineL->Interpolate(fRate, nIdx);
			pVtx[1].pos = m_pSplineR->Interpolate(fRate, nIdx);

			posEdgeOld = GetPosEdge(pVtx[0].pos, pVtx[1].pos);	// 辺の位置を保存
		}

		// 法線の設定
		SetNormal(pVtx);

		// テクスチャ座標の設定
		if (i % 2 == 0)
		{
			pVtx[0].tex = { 0.0f,0.0f };
			pVtx[1].tex = { 1.0f,0.0f };
		}
		else
		{
			pVtx[0].tex = { 0.0f,1.0f };
			pVtx[1].tex = { 1.0f,1.0f };
		}

		pVtx += MeshRoad::NUM_VTX_IN_EDGE;	// 辺にある頂点数分ポインタを進める
	}
}

//=====================================================
// 辺の角度を設定
//=====================================================
void CMeshRoad::SetEdgeAngle(VERTEX_3D *pVtx, D3DXVECTOR3 posEdge, D3DXVECTOR3 posEdgeOld)
{
	//if (pVtx == nullptr)
	//	return;

	//// 差分ベクトルから角度を取得
	//D3DXVECTOR3 vecDiff = posEdge - posEdgeOld;
	//float fAngle = atan2f(vecDiff.x, vecDiff.z);
	//
	//fAngle += D3DX_PI * 0.5f;	// 角度を90度傾ける
	//universal::LimitRot(&fAngle);

	//// 角度から極座標で頂点位置を決定
	//D3DXVECTOR3 vecPole = universal::PolarCoordinates(D3DXVECTOR3(D3DX_PI * 0.5f, fAngle, 0.0f));

	//pVtx[0].pos = posEdge + vecPole * WIDTH_ROAD;
	//pVtx[1].pos = posEdge - vecPole * WIDTH_ROAD;
}

//=====================================================
// 辺の位置を取得
//=====================================================
D3DXVECTOR3 CMeshRoad::GetPosEdge(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2)
{
	D3DXVECTOR3 vecDiff = vtx2 - vtx1;

	D3DXVECTOR3 posEdge = vtx1 + vecDiff * 0.5f;

	return posEdge;
}

//=====================================================
// 法線の設定
//=====================================================
void CMeshRoad::SetNormal(VERTEX_3D *pVtx)
{
	if (pVtx == nullptr)
		return;

	// 頂点位置
	D3DXVECTOR3 vtxLu = pVtx[-MeshRoad::NUM_VTX_IN_EDGE].pos;
	D3DXVECTOR3 vtxRu = pVtx[0].pos;
	D3DXVECTOR3 vtxRd = pVtx[1].pos;

	// 頂点どうしの差分ベクトルから辺を算出
	D3DXVECTOR3 edge1 = vtxLu - vtxRu;
	D3DXVECTOR3 edge2 = vtxRd - vtxRu;

	// 二辺の外積から法線を算出
	D3DXVECTOR3 nor;
	D3DXVec3Cross(&nor, &edge1, &edge2);

	D3DXVec3Normalize(&nor, &nor);	// 法線を正規化

	// 法線を適用
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
}

//=====================================================
// スプラインの生成
//=====================================================
void CMeshRoad::CreateSpline(void)
{
	if (m_pSpline != nullptr)
	{// スプラインが既にあったら破棄
		delete m_pSpline;
		m_pSpline = nullptr;
	}

	m_pSpline = new CCutMullSpline;

	if (m_pSpline != nullptr)
	{
		// データ点のベクター用意
		std::vector<D3DXVECTOR3> vPos;

		// ベクターを必要なサイズに調整
		int nSize = m_aRoadPoint.size();
		vPos.resize(nSize);

		for (int i = 0; i < nSize; i++)
		{
			vPos[i] = m_aRoadPoint[i].pos;
		}

		// 中心スプラインの初期化
		m_pSpline->Init(vPos);

		// 左右のスプラインの生成
		CreateSideSpline();
	}
}

//=====================================================
// 左右のスプライン生成
//=====================================================
void CMeshRoad::CreateSideSpline(void)
{
	if (m_pSpline == nullptr)
		return;

	int nSize = m_aRoadPoint.size();

	std::vector<D3DXVECTOR3> aPosL;
	std::vector<D3DXVECTOR3> aPosR;

	for (int i = 0; i < nSize; i++)
	{
		D3DXVECTOR3 posL;
		D3DXVECTOR3 posR;

		if (i == 0)
		{// 最初のスプラインの設定
			// 次回のデータ点との差分ベクトルから側面のデータ点を算出
			D3DXVECTOR3 vecDiff = m_aRoadPoint[i + 1].pos - m_aRoadPoint[i].pos;

			float fAngle = atan2f(vecDiff.x, vecDiff.z);

			fAngle += D3DX_PI * 0.5f;	// 角度を90度傾ける
			universal::LimitRot(&fAngle);

			// 角度から極座標で頂点位置を決定
			D3DXVECTOR3 vecPole = universal::PolarCoordinates(D3DXVECTOR3(D3DX_PI * 0.5f, fAngle, 0.0f));

			posL = m_aRoadPoint[i].pos + vecPole * m_aRoadPoint[i].fWidth;
			posR = m_aRoadPoint[i].pos - vecPole * m_aRoadPoint[i].fWidth;
		}
		else if (i == nSize - 1)
		{// 最後のスプラインの設定
			// 前回のデータ点との差分ベクトルから側面のデータ点を算出
			D3DXVECTOR3 vecDiff = m_aRoadPoint[i].pos - m_aRoadPoint[i - 1].pos;

			float fAngle = atan2f(vecDiff.x, vecDiff.z);

			fAngle += D3DX_PI * 0.5f;	// 角度を90度傾ける
			universal::LimitRot(&fAngle);

			// 角度から極座標で頂点位置を決定
			D3DXVECTOR3 vecPole = universal::PolarCoordinates(D3DXVECTOR3(D3DX_PI * 0.5f, fAngle, 0.0f));

			posL = m_aRoadPoint[i].pos + vecPole * m_aRoadPoint[i].fWidth;
			posR = m_aRoadPoint[i].pos - vecPole * m_aRoadPoint[i].fWidth;
		}
		else
		{// 中間のスプラインの設定
			// 判断用のカーブ角度計算
			D3DXVECTOR3 vecDiffNext = m_aRoadPoint[i + 1].pos - m_aRoadPoint[i].pos;
			D3DXVECTOR3 vecDiffPrev = m_aRoadPoint[i - 1].pos - m_aRoadPoint[i].pos;

			float fDot = universal::Vec3Dot(vecDiffNext, vecDiffPrev);

			if (fDot < m_aRoadPoint[i].fWidth)
			{// 緩やかなカーブの場合
				// 前回のデータ点との差分ベクトルから側面のデータ点を算出
				D3DXVECTOR3 vecDiff = m_aRoadPoint[i].pos - m_aRoadPoint[i - 1].pos;

				float fAngle = atan2f(vecDiff.x, vecDiff.z);

				fAngle += D3DX_PI * 0.5f;	// 角度を90度傾ける
				universal::LimitRot(&fAngle);

				// 角度から極座標で頂点位置を決定
				D3DXVECTOR3 vecPole = universal::PolarCoordinates(D3DXVECTOR3(D3DX_PI * 0.5f, fAngle, 0.0f));

				posL = m_aRoadPoint[i].pos + vecPole * m_aRoadPoint[i].fWidth;
				posR = m_aRoadPoint[i].pos - vecPole * m_aRoadPoint[i].fWidth;
			}
			else
			{// 角度がしきい値を超えた急カーブだったら
				// 前回のデータ点と次回のデータ点と自身の位置からできる三角形の中心座標
				D3DXVECTOR3 posMid = (m_aRoadPoint[i].pos + m_aRoadPoint[i + 1].pos + m_aRoadPoint[i - 1].pos) / 3;

				// 道幅に正規化
				D3DXVECTOR3 vecDiff = posMid - m_aRoadPoint[i].pos;

				D3DXVec3Normalize(&vecDiff, &vecDiff);

				vecDiff *= m_aRoadPoint[i].fWidth;

				bool bCross = universal::IsCross(m_aRoadPoint[i + 1].pos, m_aRoadPoint[i - 1].pos, posMid, nullptr);

				if (bCross)
				{
					posL = m_aRoadPoint[i].pos + vecDiff;
					posR = m_aRoadPoint[i].pos - vecDiff;
					posR.y = m_aRoadPoint[i].pos.y;	// 高さは合わせる
				}
				else
				{
					posL = m_aRoadPoint[i].pos - vecDiff;
					posR = m_aRoadPoint[i].pos + vecDiff;
					posR.y = m_aRoadPoint[i].pos.y;	// 高さは合わせる
				}
			}
		}

		aPosL.push_back(posL);
		aPosR.push_back(posR);
	}

	m_pSplineL = new CCutMullSpline;
	m_pSplineR = new CCutMullSpline;

	if (m_pSplineL != nullptr && m_pSplineR != nullptr)
	{
		m_pSplineL->Init(aPosL);
		m_pSplineR->Init(aPosR);
	}
}

//=====================================================
// 道イベントの調整
//=====================================================
void CMeshRoad::AjustRoadEvent(void)
{
	for (auto it : m_aGR)
	{
		it->VtxFollowRoad();
	}
}

//=====================================================
// スプラインの取得
//=====================================================
CCutMullSpline *CMeshRoad::GetCenterSpline(void)
{
	return m_pSpline;
}

//=====================================================
// 全ロードイベントの削除
//=====================================================
void CMeshRoad::DeleteAllRoadEvent(void)
{
	if (ImGui::Button("DeleteAllRoadEvent", ImVec2(200, 100)))
	{
		for (auto it : m_aGR)
		{
			it->Uninit();
		}

		m_aGR.clear();

		for (auto it : m_aTunnel)
		{
			it->Uninit();
		}

		m_aTunnel.clear();
	}
}

//=====================================================
// 当たり判定処理
//=====================================================
bool CMeshRoad::CollideRoad(D3DXVECTOR3* pPos, D3DXVECTOR3 posOld)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	VERTEX_3D* pVtx;
	float fHeight = pPos->y;
	float fHeightDef = pPos->y;
	bool bColRoad = false;
	D3DXVECTOR3 posOldRoadPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	int effectNum = 0;

	for (auto itRoadPoint : m_aRoadPoint)
	{
		// 道の区分の中にいなければ判定しない
		if (D3DXVec3Dot(&(itRoadPoint.pos - posOldRoadPoint), &(*pPos - posOldRoadPoint)) < 0.0f)
		{
			// 区分判定用変数に代入
			posOldRoadPoint = itRoadPoint.pos;
			pVtx += MeshRoad::NUM_VTX_IN_EDGE * MeshRoad::NUM_EDGE_IN_ROADPOINT;
			continue;
		}
		posOldRoadPoint = itRoadPoint.pos;

		for (int i = 0; i < MeshRoad::NUM_EDGE_IN_ROADPOINT; i++)
		{
			pVtx += MeshRoad::NUM_VTX_IN_EDGE;
			
			// 判定用の位置を設定
			D3DXVECTOR3 posUpPolygon = *pPos;
			posUpPolygon.y = pVtx[2].pos.y;

			// ポリゴンの法線を計算
			D3DXVECTOR3 vtxNor[2];
			D3DXVec3Cross(&vtxNor[0], &(pVtx[1].pos - pVtx[0].pos), &(pVtx[2].pos - pVtx[0].pos));
			D3DXVec3Cross(&vtxNor[1], &(pVtx[2].pos - pVtx[3].pos), &(pVtx[1].pos - pVtx[3].pos));

			// ポリゴンの下に入っているか判定する
			if (!universal::IsOnSquarePolygon(pVtx[0].pos, pVtx[1].pos, pVtx[2].pos, pVtx[3].pos, vtxNor[0], vtxNor[1], *pPos, posUpPolygon, fHeight))
				continue;

			// 高さが一定の高さ以内か判定する
			if (HEIGHT_LIMIT < fHeight - pPos->y)
				continue;

			// 高さが一番高い場所で判定する
			if (fHeightDef < fHeight || !bColRoad)
				fHeightDef = fHeight;

#ifdef _DEBUG

#endif // _DEBUG

			// 判定をtrueにする
			bColRoad = true;
		}
	}

	// 高さを代入する
	pPos->y = fHeightDef;

	// 頂点バッファをアンロック
	pVtxBuff->Unlock();

	return bColRoad;
}

//=====================================================
// 保存処理
//=====================================================
void CMeshRoad::Save(const char* pPath)
{
	// ファイルを開く
	std::ofstream outputFile(pPath, std::ios::binary);

	if (!outputFile.is_open())
		assert(("メッシュロードのファイルを開けませんでした", false));

	// 情報数保存
	size_t size = m_aRoadPoint.size();
	outputFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

	// リストの情報保存
	outputFile.write(reinterpret_cast<char*>(m_aRoadPoint.data()), sizeof(SInfoRoadPoint) * size);

	// トンネル情報保存
	size = m_aTunnel.size();	// トンネル数保存



	outputFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

	// イテレーターの終始端を保存
	for (auto it : m_aTunnel)
	{
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart = it->GetItStart();
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd = it->GetItEnd();

		int nDist = std::distance(m_aRoadPoint.begin(), itStart);
		outputFile.write(reinterpret_cast<char*>(&nDist), sizeof(int));

		nDist = std::distance(m_aRoadPoint.begin(), itEnd);
		outputFile.write(reinterpret_cast<char*>(&nDist), sizeof(int));
	}

	// ガードレール情報保存
	size = m_aGR.size();	// 総数保存
	outputFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

	// イテレーターの終始端を保存
	for (auto it : m_aGR)
	{
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart = it->GetItStart();
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd = it->GetItEnd();

		// 始点距離保存
		int nDist = std::distance(m_aRoadPoint.begin(), itStart);
		outputFile.write(reinterpret_cast<char*>(&nDist), sizeof(int));

		// 終点距離保存
		nDist = std::distance(m_aRoadPoint.begin(), itEnd);
		outputFile.write(reinterpret_cast<char*>(&nDist), sizeof(int));

		// 左かどうか保存
		bool bLeft = it->IsLeft();
		outputFile.write(reinterpret_cast<char*>(&bLeft), sizeof(bool));

		// 高さ保存
		float fHeight = it->GetHeight();
		outputFile.write(reinterpret_cast<char*>(&fHeight), sizeof(float));
	}

	outputFile.close();
}

//=====================================================
// 読み込み処理
//=====================================================
void CMeshRoad::Load(const char* pPath)
{
	m_aRoadPoint.clear();

	// ファイルを開く
	std::ifstream inputFile(pPath, std::ios::binary);

	if (!inputFile.is_open())
		assert(("メッシュロードのファイルを開けませんでした", false));

	// データ数読み込み
	size_t size;
	inputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	m_aRoadPoint.resize(size);

	for (auto it : m_aRoadPoint)
	{// 道幅のリセット
		it.fWidth = WIDTH_ROAD;
	}

	// ロードポイントデータ読み込み
	inputFile.read(reinterpret_cast<char*>(m_aRoadPoint.data()), sizeof(SInfoRoadPoint) * size);

	CreateVtxBuffEdge();

	// トンネルの生成
	inputFile.read(reinterpret_cast<char*>(&size), sizeof(size));

	if (inputFile.eof())
		return;	// ファイルの終了チェック

	m_aTunnel.resize(size);

	for (size_t i = 0; i < size; i++)
	{
		int nDistStart;
		int nDistEnd;

		inputFile.read(reinterpret_cast<char*>(&nDistStart), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&nDistEnd), sizeof(int));

		std::vector<CMeshRoad::SInfoRoadPoint>::iterator it;

		it = m_aRoadPoint.begin();
		std::advance(it, nDistStart);
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart = it;

		it = m_aRoadPoint.begin();
		std::advance(it, nDistEnd);
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd = it;

		CTunnel *pTunnel = CTunnel::Create(itStart, itEnd);

		m_aTunnel[i] = pTunnel;
	}

	// ガードレールの生成
	inputFile.read(reinterpret_cast<char*>(&size), sizeof(size));

	if (inputFile.eof())
		return;	// ファイルの終了チェック

	m_aGR.resize(size);

	for (size_t i = 0; i < size; i++)
	{
		int nDistStart;
		int nDistEnd;

		inputFile.read(reinterpret_cast<char*>(&nDistStart), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&nDistEnd), sizeof(int));

		std::vector<CMeshRoad::SInfoRoadPoint>::iterator it;

		// 始点距離取得
		it = m_aRoadPoint.begin();
		std::advance(it, nDistStart);
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart = it;

		// 終点距離取得
		it = m_aRoadPoint.begin();
		std::advance(it, nDistEnd);
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd = it;

		// 左かどうか取得
		bool bLeft = false;
		inputFile.read(reinterpret_cast<char*>(&bLeft), sizeof(bool));

		// 高さ取得
		float fHeight = 0.0f;
		inputFile.read(reinterpret_cast<char*>(&fHeight), sizeof(float));

		CGuardRail *pGR = CGuardRail::Create(itStart, itEnd, bLeft, fHeight);

		m_aGR[i] = pGR;
	}

	inputFile.close();

	m_it = m_aRoadPoint.begin();
}

namespace MeshRoad
{
CMeshRoad *GetInstance(void)
{
	CMeshRoad *pMeshRoad = CMeshRoad::GetInstance();

	if (pMeshRoad == nullptr)
		assert(("meshroadがないよ～～", false));

	return pMeshRoad;
}
}