//*****************************************************
//
// メッシュフィールドエディターの処理[editMeshfield.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "editMeshfield.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "inputmouse.h"
#include "inputkeyboard.h"

//*****************************************************
// 定数定義
//*****************************************************
namespace
{
const float DEFAULT_DEPTH = 100.0f;	// デフォルトの深さ
const float DEFAULT_RADIUS = 1000.0f;	// デフォルトの範囲
const float HEIGHT_CYLINDER = 50000.0f;	// シリンダーの高さ
}

//=====================================================
// コンストラクタ
//=====================================================
CEditMeshfield::CEditMeshfield() : m_fDepthDent(0.0f), m_fRadiusDent(0.0f), m_pMeshCylinder(nullptr)
{

}

//=====================================================
// デストラクタ
//=====================================================
CEditMeshfield::~CEditMeshfield()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEditMeshfield::Init(void)
{
	CEdit::Init();

	m_fDepthDent = DEFAULT_DEPTH;
	m_fRadiusDent = DEFAULT_RADIUS;

	// メッシュシリンダー生成
	m_pMeshCylinder = CMeshCylinder::Create();

	if (m_pMeshCylinder != nullptr)
	{
		m_pMeshCylinder->SetIdxTexture(-1);
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEditMeshfield::Uninit(void)
{
	if (m_pMeshCylinder != nullptr)
	{
		m_pMeshCylinder->Uninit();
		m_pMeshCylinder = nullptr;
	}

	CEdit::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEditMeshfield::Update(void)
{
	CEdit::Update();

	// カーソルを動かす処理
	MoveCursor();

	// メッシュフィールドをへこます処理
	DentMeshField();
}

//=====================================================
// カーソルを動かす処理
//=====================================================
void CEditMeshfield::MoveCursor(void)
{
	D3DXVECTOR3 posNear;
	D3DXVECTOR3 posFar;
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 posHit;

	universal::ConvertScreenPosTo3D(&posNear, &posFar, &vecDiff);

	universal::CalcRayFlat(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), posNear, posFar, &posHit);

	SetPosition(posHit);

	if (m_pMeshCylinder != nullptr)
	{
		m_pMeshCylinder->SetPosition(posHit);
		m_pMeshCylinder->SetHeight(HEIGHT_CYLINDER);
		m_pMeshCylinder->SetRadius(m_fRadiusDent);
		m_pMeshCylinder->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		m_pMeshCylinder->SetVtx();
	}
}

//=====================================================
// メッシュフィールドをへこます処理
//=====================================================
void CEditMeshfield::DentMeshField(void)
{
	CMeshField *pMeshField = CMeshField::GetInstance();

	if (pMeshField == nullptr)
		return;

	D3DXVECTOR3 pos = GetPosition();

	pMeshField->Dent(pos, m_fRadiusDent, m_fDepthDent);
}