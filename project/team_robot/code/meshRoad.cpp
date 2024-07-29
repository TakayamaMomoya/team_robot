//*****************************************************
//
// ���b�V�����̏���[meshRoad.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
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
// �萔��`
//*****************************************************
namespace
{
const UINT NUMVTX_NOTDRAW = 4;	// ���̒��_�������̏ꍇ�A�`�悵�Ȃ�
const float WIDTH_DEFAULT = 200.0f;	// �f�t�H���g�̕�
const float LENGTH_DEFAULT = 200.0f;	// �f�t�H���g�̒���
const char* PATH_TEXTURE = "data\\TEXTURE\\MATERIAL\\road.jpg";	// �e�N�X�`���p�X
const float DIST_DEFAULT = 200.0f;	// �f�t�H���g�̕ӊԂ̋���
const float WIDTH_ROAD = 600.0f;	// ���̕�
const float HEIGHT_LIMIT = 200.0f;	// ���肷�鍂���̌��E
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CMeshRoad *CMeshRoad::m_pMeshRoad = nullptr;	// ���g�̃|�C���^
std::list<CMeshRoad*> CMeshRoad::s_aRoad;	// ���b�V�����[�h�̃R���e�i

//=====================================================
// �R���X�g���N�^
//=====================================================
CMeshRoad::CMeshRoad(int nPriority) : CPolygon3D(nPriority), m_nNumVtx(0), m_pSpline(nullptr), m_pSplineL(nullptr), m_pSplineR(nullptr)
{
	s_aRoad.push_back(this);
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMeshRoad::~CMeshRoad()
{
	s_aRoad.remove(this);
}

//=====================================================
// ��������
//=====================================================
CMeshRoad *CMeshRoad::Create(const char* pPath)
{
	CMeshRoad *pMeshRoad = nullptr;

	if (pMeshRoad == nullptr)
	{
		// �C���X�^���X����
		pMeshRoad = new CMeshRoad;

		if (pMeshRoad != nullptr)
		{
			m_pMeshRoad = pMeshRoad;

			// ����������
			pMeshRoad->Init();

			// �Ǎ�����
			pMeshRoad->Load(pPath);
		}
	}

	return pMeshRoad;
}

//=====================================================
// ����������
//=====================================================
HRESULT CMeshRoad::Init(void)
{
	// �e�N�X�`���ǂݍ���
	int nIdx = Texture::GetIdx(PATH_TEXTURE);
	SetIdxTexture(nIdx);

#ifdef _DEBUG
	//EnableWire(true);
#endif // _DEBUG

	return S_OK;
}

//=====================================================
// �I������
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
// �X�V����
//=====================================================
void CMeshRoad::Update(void)
{
#ifdef _DEBUG
	for (SInfoRoadPoint info : m_aRoadPoint)
		CEffect3D::Create(info.pos, 50.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
#endif // _DEBUG
}

//=====================================================
// �`�揈��
//=====================================================
void CMeshRoad::Draw(void)
{
	if (m_nNumVtx < NUMVTX_NOTDRAW)	// �`�搧��
		return;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	D3DXMATRIX *pMtx = GetMatrix();

	// ���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(pMtx);

	D3DXMATRIX mtxRot, mtxTrans;
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);

	// ���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, pMtx);

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���ݒ�
	int nIdxTexture = GetIdxTexture();
	LPDIRECT3DTEXTURE9 pTexture = Texture::GetTexture(nIdxTexture);
	pDevice->SetTexture(0, pTexture);

	// �J�����O�𖳌���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);

	// �J�����O��L����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=====================================================
// ���b�V�����[�h�̑I��
//=====================================================
void CMeshRoad::SelectMeshRoad(void)
{
	ImGui::Text("[SelectMeshRoad]");

	std::list<CMeshRoad*>::iterator it = std::find(s_aRoad.begin(), s_aRoad.end(), m_pMeshRoad);
	int nDist = std::distance(s_aRoad.begin(), it);
	int nSize = s_aRoad.size();

	if (ImGui::DragInt("RoadIndex", &nDist, 1.0f, 0, nSize - 1))
	{
		// �C�e���[�^���ď��������Ďw��ԍ��̗v�f�Ɉړ�
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
// ���[�h�|�C���g�̑I��
//=====================================================
std::vector<CMeshRoad::SInfoRoadPoint>::iterator CMeshRoad::SelectRoadPoint(void)
{
	ImGui::Text("[SelectRoadPoint]");

	int nDist = std::distance(m_aRoadPoint.begin(), m_it);
	int nSize = m_aRoadPoint.size();

	if (ImGui::DragInt("Index", &nDist, 1.0f, 0, nSize - 1))
	{
		// �C�e���[�^���ď��������Ďw��ԍ��̗v�f�Ɉړ�
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
// �C�e���[�^�[���烍�[�h�|�C���g�ԍ����w��
//=====================================================
size_t CMeshRoad::GetIdxRoad(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it)
{
	if (m_aRoadPoint.empty())
		return 0;

	return std::distance(m_aRoadPoint.begin(), it);
}

//=====================================================
// ���[�h�|�C���g�̍폜
//=====================================================
void CMeshRoad::DeleteRoadPoint(std::vector<CMeshRoad::SInfoRoadPoint>::iterator it)
{
	m_aRoadPoint.erase(it);

	CreateVtxBuffEdge();

	m_it = m_aRoadPoint.begin();
}

//=====================================================
// �C�e���[�^�[�̃��Z�b�g
//=====================================================
void CMeshRoad::ResetIterator(void)
{
	m_it = m_aRoadPoint.end() - 1;
}

//=====================================================
// ���[�h�|�C���g�̒ǉ�
//=====================================================
void CMeshRoad::AddRoadPoint(D3DXVECTOR3 pos, bool bReCreateVtx)
{
	// ���X�g�ɏ���ǉ�
	SInfoRoadPoint info;
	info.pos = pos;
	info.fWidth = 500.0f;

	// �g���l���̏I�n�_��ۑ�
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

	// �K�[�h���[���̏I�n�_��ۑ�
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

	if (bReCreateVtx)	// ���[�h�|�C���g�ɉ��������_�̍Đ���
		CreateVtxBuffEdge();

	// �I�n�_���Ċ��蓖��
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

	// �K�[�h���[���̍Ċ��蓖��
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
// ���[�h�|�C���g�ɉ��������_�̐���
//=====================================================
void CMeshRoad::CreateVtxBuffEdge(void)
{
	// �X�v���C���̍Đ���
	CreateSpline();

	// ���_�̐���
	m_nNumVtx = m_aRoadPoint.size() * MeshRoad::NUM_VTX_IN_EDGE * MeshRoad::NUM_EDGE_IN_ROADPOINT;
	CreateVtxBuff(m_nNumVtx);

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	std::vector<SInfoRoadPoint>::iterator itRoadPoint;
	int nIdx = 0;

	for (itRoadPoint = m_aRoadPoint.begin(); itRoadPoint != m_aRoadPoint.end(); itRoadPoint++)
	{
		if (itRoadPoint != m_aRoadPoint.begin())
		{
			SInfoRoadPoint *pInfoRoadPointOld = &*std::prev(itRoadPoint);

			// ���[�h�|�C���g�Ԃ̒��_�ݒ�
			CreateVtxBetweenRoadPoint(*itRoadPoint, pVtx, pInfoRoadPointOld, nIdx);
		}

		pVtx += MeshRoad::NUM_EDGE_IN_ROADPOINT * MeshRoad::NUM_VTX_IN_EDGE;

		nIdx++;
	}

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();
}

//=====================================================
// ���[�h�|�C���g�Ԃ̒��_�ݒ�
//=====================================================
void CMeshRoad::CreateVtxBetweenRoadPoint(SInfoRoadPoint infoRoadPoint, VERTEX_3D *pVtx, SInfoRoadPoint *pInfoRoadPointOld, int nIdx)
{
	if (pVtx == nullptr)
		assert(("CreateVtxBetweenRoadPoint�Œ��_���null�ł�", false));

	if (m_pSpline == nullptr)
		assert(("CreateVtxBetweenRoadPoint�ŃX�v���C����null�ł�", false));

	D3DXVECTOR3 posEdgeOld = {};	// �O��̕ӂ̈ʒu

	if (pInfoRoadPointOld != nullptr)
	{
		posEdgeOld = pInfoRoadPointOld->pos;
	}

	// ���[�h�|�C���g�ԂŕK�v�ȕ�
	for (int i = 0; i < MeshRoad::NUM_EDGE_IN_ROADPOINT; i++)
	{
		D3DXVECTOR3 pos = infoRoadPoint.pos;

		if (pInfoRoadPointOld == nullptr)
		{// �O��̃��[�h�|�C���g���Ȃ��ꍇ�A�����͎�������
			pos = m_pSpline->Interpolate(pos.x, 0);

			// �O��̕ӂƔ�ׂȂ����_�ʒu�ݒ�
			pVtx[0].pos = pos;
			pVtx[1].pos = pos;

			pVtx[0].pos.z -= WIDTH_ROAD;
			pVtx[1].pos.z += WIDTH_ROAD;

			posEdgeOld = GetPosEdge(pVtx[0].pos, pVtx[1].pos);	// �ӂ̈ʒu��ۑ�
		}
		else
		{
			float fDiff = infoRoadPoint.pos.x - pInfoRoadPointOld->pos.x;
			float fRate = ((float)i + 1.0f) / MeshRoad::NUM_EDGE_IN_ROADPOINT;

			// ���W�̌���
			pos = m_pSpline->Interpolate(fRate, nIdx);

			pVtx[0].pos = m_pSplineL->Interpolate(fRate, nIdx);
			pVtx[1].pos = m_pSplineR->Interpolate(fRate, nIdx);

			posEdgeOld = GetPosEdge(pVtx[0].pos, pVtx[1].pos);	// �ӂ̈ʒu��ۑ�
		}

		// �@���̐ݒ�
		SetNormal(pVtx);

		// �e�N�X�`�����W�̐ݒ�
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

		pVtx += MeshRoad::NUM_VTX_IN_EDGE;	// �ӂɂ��钸�_�����|�C���^��i�߂�
	}
}

//=====================================================
// �ӂ̊p�x��ݒ�
//=====================================================
void CMeshRoad::SetEdgeAngle(VERTEX_3D *pVtx, D3DXVECTOR3 posEdge, D3DXVECTOR3 posEdgeOld)
{
	//if (pVtx == nullptr)
	//	return;

	//// �����x�N�g������p�x���擾
	//D3DXVECTOR3 vecDiff = posEdge - posEdgeOld;
	//float fAngle = atan2f(vecDiff.x, vecDiff.z);
	//
	//fAngle += D3DX_PI * 0.5f;	// �p�x��90�x�X����
	//universal::LimitRot(&fAngle);

	//// �p�x����ɍ��W�Œ��_�ʒu������
	//D3DXVECTOR3 vecPole = universal::PolarCoordinates(D3DXVECTOR3(D3DX_PI * 0.5f, fAngle, 0.0f));

	//pVtx[0].pos = posEdge + vecPole * WIDTH_ROAD;
	//pVtx[1].pos = posEdge - vecPole * WIDTH_ROAD;
}

//=====================================================
// �ӂ̈ʒu���擾
//=====================================================
D3DXVECTOR3 CMeshRoad::GetPosEdge(D3DXVECTOR3 vtx1, D3DXVECTOR3 vtx2)
{
	D3DXVECTOR3 vecDiff = vtx2 - vtx1;

	D3DXVECTOR3 posEdge = vtx1 + vecDiff * 0.5f;

	return posEdge;
}

//=====================================================
// �@���̐ݒ�
//=====================================================
void CMeshRoad::SetNormal(VERTEX_3D *pVtx)
{
	if (pVtx == nullptr)
		return;

	// ���_�ʒu
	D3DXVECTOR3 vtxLu = pVtx[-MeshRoad::NUM_VTX_IN_EDGE].pos;
	D3DXVECTOR3 vtxRu = pVtx[0].pos;
	D3DXVECTOR3 vtxRd = pVtx[1].pos;

	// ���_�ǂ����̍����x�N�g������ӂ��Z�o
	D3DXVECTOR3 edge1 = vtxLu - vtxRu;
	D3DXVECTOR3 edge2 = vtxRd - vtxRu;

	// ��ӂ̊O�ς���@�����Z�o
	D3DXVECTOR3 nor;
	D3DXVec3Cross(&nor, &edge1, &edge2);

	D3DXVec3Normalize(&nor, &nor);	// �@���𐳋K��

	// �@����K�p
	pVtx[0].nor = nor;
	pVtx[1].nor = nor;
}

//=====================================================
// �X�v���C���̐���
//=====================================================
void CMeshRoad::CreateSpline(void)
{
	if (m_pSpline != nullptr)
	{// �X�v���C�������ɂ�������j��
		delete m_pSpline;
		m_pSpline = nullptr;
	}

	m_pSpline = new CCutMullSpline;

	if (m_pSpline != nullptr)
	{
		// �f�[�^�_�̃x�N�^�[�p��
		std::vector<D3DXVECTOR3> vPos;

		// �x�N�^�[��K�v�ȃT�C�Y�ɒ���
		int nSize = m_aRoadPoint.size();
		vPos.resize(nSize);

		for (int i = 0; i < nSize; i++)
		{
			vPos[i] = m_aRoadPoint[i].pos;
		}

		// ���S�X�v���C���̏�����
		m_pSpline->Init(vPos);

		// ���E�̃X�v���C���̐���
		CreateSideSpline();
	}
}

//=====================================================
// ���E�̃X�v���C������
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
		{// �ŏ��̃X�v���C���̐ݒ�
			// ����̃f�[�^�_�Ƃ̍����x�N�g�����瑤�ʂ̃f�[�^�_���Z�o
			D3DXVECTOR3 vecDiff = m_aRoadPoint[i + 1].pos - m_aRoadPoint[i].pos;

			float fAngle = atan2f(vecDiff.x, vecDiff.z);

			fAngle += D3DX_PI * 0.5f;	// �p�x��90�x�X����
			universal::LimitRot(&fAngle);

			// �p�x����ɍ��W�Œ��_�ʒu������
			D3DXVECTOR3 vecPole = universal::PolarCoordinates(D3DXVECTOR3(D3DX_PI * 0.5f, fAngle, 0.0f));

			posL = m_aRoadPoint[i].pos + vecPole * m_aRoadPoint[i].fWidth;
			posR = m_aRoadPoint[i].pos - vecPole * m_aRoadPoint[i].fWidth;
		}
		else if (i == nSize - 1)
		{// �Ō�̃X�v���C���̐ݒ�
			// �O��̃f�[�^�_�Ƃ̍����x�N�g�����瑤�ʂ̃f�[�^�_���Z�o
			D3DXVECTOR3 vecDiff = m_aRoadPoint[i].pos - m_aRoadPoint[i - 1].pos;

			float fAngle = atan2f(vecDiff.x, vecDiff.z);

			fAngle += D3DX_PI * 0.5f;	// �p�x��90�x�X����
			universal::LimitRot(&fAngle);

			// �p�x����ɍ��W�Œ��_�ʒu������
			D3DXVECTOR3 vecPole = universal::PolarCoordinates(D3DXVECTOR3(D3DX_PI * 0.5f, fAngle, 0.0f));

			posL = m_aRoadPoint[i].pos + vecPole * m_aRoadPoint[i].fWidth;
			posR = m_aRoadPoint[i].pos - vecPole * m_aRoadPoint[i].fWidth;
		}
		else
		{// ���Ԃ̃X�v���C���̐ݒ�
			// ���f�p�̃J�[�u�p�x�v�Z
			D3DXVECTOR3 vecDiffNext = m_aRoadPoint[i + 1].pos - m_aRoadPoint[i].pos;
			D3DXVECTOR3 vecDiffPrev = m_aRoadPoint[i - 1].pos - m_aRoadPoint[i].pos;

			float fDot = universal::Vec3Dot(vecDiffNext, vecDiffPrev);

			if (fDot < m_aRoadPoint[i].fWidth)
			{// �ɂ₩�ȃJ�[�u�̏ꍇ
				// �O��̃f�[�^�_�Ƃ̍����x�N�g�����瑤�ʂ̃f�[�^�_���Z�o
				D3DXVECTOR3 vecDiff = m_aRoadPoint[i].pos - m_aRoadPoint[i - 1].pos;

				float fAngle = atan2f(vecDiff.x, vecDiff.z);

				fAngle += D3DX_PI * 0.5f;	// �p�x��90�x�X����
				universal::LimitRot(&fAngle);

				// �p�x����ɍ��W�Œ��_�ʒu������
				D3DXVECTOR3 vecPole = universal::PolarCoordinates(D3DXVECTOR3(D3DX_PI * 0.5f, fAngle, 0.0f));

				posL = m_aRoadPoint[i].pos + vecPole * m_aRoadPoint[i].fWidth;
				posR = m_aRoadPoint[i].pos - vecPole * m_aRoadPoint[i].fWidth;
			}
			else
			{// �p�x���������l�𒴂����}�J�[�u��������
				// �O��̃f�[�^�_�Ǝ���̃f�[�^�_�Ǝ��g�̈ʒu����ł���O�p�`�̒��S���W
				D3DXVECTOR3 posMid = (m_aRoadPoint[i].pos + m_aRoadPoint[i + 1].pos + m_aRoadPoint[i - 1].pos) / 3;

				// �����ɐ��K��
				D3DXVECTOR3 vecDiff = posMid - m_aRoadPoint[i].pos;

				D3DXVec3Normalize(&vecDiff, &vecDiff);

				vecDiff *= m_aRoadPoint[i].fWidth;

				bool bCross = universal::IsCross(m_aRoadPoint[i + 1].pos, m_aRoadPoint[i - 1].pos, posMid, nullptr);

				if (bCross)
				{
					posL = m_aRoadPoint[i].pos + vecDiff;
					posR = m_aRoadPoint[i].pos - vecDiff;
					posR.y = m_aRoadPoint[i].pos.y;	// �����͍��킹��
				}
				else
				{
					posL = m_aRoadPoint[i].pos - vecDiff;
					posR = m_aRoadPoint[i].pos + vecDiff;
					posR.y = m_aRoadPoint[i].pos.y;	// �����͍��킹��
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
// ���C�x���g�̒���
//=====================================================
void CMeshRoad::AjustRoadEvent(void)
{
	for (auto it : m_aGR)
	{
		it->VtxFollowRoad();
	}
}

//=====================================================
// �X�v���C���̎擾
//=====================================================
CCutMullSpline *CMeshRoad::GetCenterSpline(void)
{
	return m_pSpline;
}

//=====================================================
// �S���[�h�C�x���g�̍폜
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
// �����蔻�菈��
//=====================================================
bool CMeshRoad::CollideRoad(D3DXVECTOR3* pPos, D3DXVECTOR3 posOld)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	VERTEX_3D* pVtx;
	float fHeight = pPos->y;
	float fHeightDef = pPos->y;
	bool bColRoad = false;
	D3DXVECTOR3 posOldRoadPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	int effectNum = 0;

	for (auto itRoadPoint : m_aRoadPoint)
	{
		// ���̋敪�̒��ɂ��Ȃ���Δ��肵�Ȃ�
		if (D3DXVec3Dot(&(itRoadPoint.pos - posOldRoadPoint), &(*pPos - posOldRoadPoint)) < 0.0f)
		{
			// �敪����p�ϐ��ɑ��
			posOldRoadPoint = itRoadPoint.pos;
			pVtx += MeshRoad::NUM_VTX_IN_EDGE * MeshRoad::NUM_EDGE_IN_ROADPOINT;
			continue;
		}
		posOldRoadPoint = itRoadPoint.pos;

		for (int i = 0; i < MeshRoad::NUM_EDGE_IN_ROADPOINT; i++)
		{
			pVtx += MeshRoad::NUM_VTX_IN_EDGE;
			
			// ����p�̈ʒu��ݒ�
			D3DXVECTOR3 posUpPolygon = *pPos;
			posUpPolygon.y = pVtx[2].pos.y;

			// �|���S���̖@�����v�Z
			D3DXVECTOR3 vtxNor[2];
			D3DXVec3Cross(&vtxNor[0], &(pVtx[1].pos - pVtx[0].pos), &(pVtx[2].pos - pVtx[0].pos));
			D3DXVec3Cross(&vtxNor[1], &(pVtx[2].pos - pVtx[3].pos), &(pVtx[1].pos - pVtx[3].pos));

			// �|���S���̉��ɓ����Ă��邩���肷��
			if (!universal::IsOnSquarePolygon(pVtx[0].pos, pVtx[1].pos, pVtx[2].pos, pVtx[3].pos, vtxNor[0], vtxNor[1], *pPos, posUpPolygon, fHeight))
				continue;

			// ���������̍����ȓ������肷��
			if (HEIGHT_LIMIT < fHeight - pPos->y)
				continue;

			// ��������ԍ����ꏊ�Ŕ��肷��
			if (fHeightDef < fHeight || !bColRoad)
				fHeightDef = fHeight;

#ifdef _DEBUG

#endif // _DEBUG

			// �����true�ɂ���
			bColRoad = true;
		}
	}

	// ������������
	pPos->y = fHeightDef;

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	return bColRoad;
}

//=====================================================
// �ۑ�����
//=====================================================
void CMeshRoad::Save(const char* pPath)
{
	// �t�@�C�����J��
	std::ofstream outputFile(pPath, std::ios::binary);

	if (!outputFile.is_open())
		assert(("���b�V�����[�h�̃t�@�C�����J���܂���ł���", false));

	// ��񐔕ۑ�
	size_t size = m_aRoadPoint.size();
	outputFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

	// ���X�g�̏��ۑ�
	outputFile.write(reinterpret_cast<char*>(m_aRoadPoint.data()), sizeof(SInfoRoadPoint) * size);

	// �g���l�����ۑ�
	size = m_aTunnel.size();	// �g���l�����ۑ�



	outputFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

	// �C�e���[�^�[�̏I�n�[��ۑ�
	for (auto it : m_aTunnel)
	{
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart = it->GetItStart();
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd = it->GetItEnd();

		int nDist = std::distance(m_aRoadPoint.begin(), itStart);
		outputFile.write(reinterpret_cast<char*>(&nDist), sizeof(int));

		nDist = std::distance(m_aRoadPoint.begin(), itEnd);
		outputFile.write(reinterpret_cast<char*>(&nDist), sizeof(int));
	}

	// �K�[�h���[�����ۑ�
	size = m_aGR.size();	// �����ۑ�
	outputFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

	// �C�e���[�^�[�̏I�n�[��ۑ�
	for (auto it : m_aGR)
	{
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart = it->GetItStart();
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd = it->GetItEnd();

		// �n�_�����ۑ�
		int nDist = std::distance(m_aRoadPoint.begin(), itStart);
		outputFile.write(reinterpret_cast<char*>(&nDist), sizeof(int));

		// �I�_�����ۑ�
		nDist = std::distance(m_aRoadPoint.begin(), itEnd);
		outputFile.write(reinterpret_cast<char*>(&nDist), sizeof(int));

		// �����ǂ����ۑ�
		bool bLeft = it->IsLeft();
		outputFile.write(reinterpret_cast<char*>(&bLeft), sizeof(bool));

		// �����ۑ�
		float fHeight = it->GetHeight();
		outputFile.write(reinterpret_cast<char*>(&fHeight), sizeof(float));
	}

	outputFile.close();
}

//=====================================================
// �ǂݍ��ݏ���
//=====================================================
void CMeshRoad::Load(const char* pPath)
{
	m_aRoadPoint.clear();

	// �t�@�C�����J��
	std::ifstream inputFile(pPath, std::ios::binary);

	if (!inputFile.is_open())
		assert(("���b�V�����[�h�̃t�@�C�����J���܂���ł���", false));

	// �f�[�^���ǂݍ���
	size_t size;
	inputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	m_aRoadPoint.resize(size);

	for (auto it : m_aRoadPoint)
	{// �����̃��Z�b�g
		it.fWidth = WIDTH_ROAD;
	}

	// ���[�h�|�C���g�f�[�^�ǂݍ���
	inputFile.read(reinterpret_cast<char*>(m_aRoadPoint.data()), sizeof(SInfoRoadPoint) * size);

	CreateVtxBuffEdge();

	// �g���l���̐���
	inputFile.read(reinterpret_cast<char*>(&size), sizeof(size));

	if (inputFile.eof())
		return;	// �t�@�C���̏I���`�F�b�N

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

	// �K�[�h���[���̐���
	inputFile.read(reinterpret_cast<char*>(&size), sizeof(size));

	if (inputFile.eof())
		return;	// �t�@�C���̏I���`�F�b�N

	m_aGR.resize(size);

	for (size_t i = 0; i < size; i++)
	{
		int nDistStart;
		int nDistEnd;

		inputFile.read(reinterpret_cast<char*>(&nDistStart), sizeof(int));
		inputFile.read(reinterpret_cast<char*>(&nDistEnd), sizeof(int));

		std::vector<CMeshRoad::SInfoRoadPoint>::iterator it;

		// �n�_�����擾
		it = m_aRoadPoint.begin();
		std::advance(it, nDistStart);
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart = it;

		// �I�_�����擾
		it = m_aRoadPoint.begin();
		std::advance(it, nDistEnd);
		std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd = it;

		// �����ǂ����擾
		bool bLeft = false;
		inputFile.read(reinterpret_cast<char*>(&bLeft), sizeof(bool));

		// �����擾
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
		assert(("meshroad���Ȃ���`�`", false));

	return pMeshRoad;
}
}