//*****************************************************
//
// �g���l���̏���[tunnel.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "tunnel.h"
#include "texture.h"
#include "effect3D.h"
#include "player.h"
#include "debugproc.h"
#include "blur.h"
#include "player.h"
#include "fan3D.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
const int MESH_U = 32;	// ���̕�����
const float MESH_HEIGHT = 50.0f;	// ���b�V���̍���
const char* TEX_PATH = "data\\TEXTURE\\MATERIAL\\concrete.jpg";	// �e�N�X�`���p�X
const int NUM_VTX_IN_EDGE = 5;	// �ӂ̒��ɂ��钸�_��
const int PRIORITY_FAN = 5;	// ��|���S���̕`��v���C�I���e�B
const char* DOOR_TEX_PATH = "data\\TEXTURE\\MATERIAL\\potal00.png";
const float TEX_SCROLL_X = 0.001f;  // �e�N�X�`�����W�̈ړ���
const float TEX_SCROLL_Y = 0.01f;   // �e�N�X�`�����W�̈ړ���
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CTunnel::CTunnel(int nPriority) : CMeshCylinder(nPriority),m_bInPlayer(false),m_pFanEnter(nullptr)
{
	m_bInPlayer = false;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CTunnel::~CTunnel()
{

}

//=====================================================
// ��������
//=====================================================
CTunnel *CTunnel::Create(std::vector<CMeshRoad::SInfoRoadPoint>::iterator itStart, std::vector<CMeshRoad::SInfoRoadPoint>::iterator itEnd)
{
	CTunnel *pTunnel = nullptr;

	pTunnel = new CTunnel;

	if (pTunnel != nullptr)
	{
		pTunnel->m_itStart = itStart;
		pTunnel->m_itEnd = itEnd;

		pTunnel->Init();
	}

	return pTunnel;
}

//=====================================================
// ����������
//=====================================================
HRESULT CTunnel::Init(void)
{
	// ���_�̐���
	CreateVtx();

	// ���_�𓹂ɉ��킹��
	VtxFollowRoad();

	// ��|���S���̐���
	CreateFan();

	return S_OK;
}

//=====================================================
// ���_�̐���
//=====================================================
void CTunnel::CreateVtx(void)
{
	int nDistStart = std::distance(m_itStart, m_itEnd);

	// �������ݒ�
	int nMeshV = nDistStart * MeshRoad::NUM_EDGE_IN_ROADPOINT;

	SetNumMeshU(MESH_U);
	SetNumMeshV(nMeshV);

	// �p���N���X�̏�����
	CMeshCylinder::Init();

	// ���_�ʒu�ݒ�
	SetVtx();

	// �e�N�X�`���̓Ǎ�
	CTexture *pTexture = CTexture::GetInstance();

	if (pTexture != nullptr)
	{
		int nIdx = pTexture->Regist(TEX_PATH);
		SetIdxTexture(nIdx);
	}
}

//=====================================================
// ���_�𓹂ɉ��킹��
//=====================================================
void CTunnel::VtxFollowRoad(void)
{
	// �J�n�ƏI���܂ł̒��_�ʒu��ۑ�============================
	CMeshRoad *pMesh = CMeshRoad::GetInstance();

	std::vector<D3DXVECTOR3> vectorPosEdge;

	if (pMesh != nullptr)
	{
		// �J�n�n�_���Z�o
		std::vector<CMeshRoad::SInfoRoadPoint> *pList = pMesh->GetArrayRP();

		int nDistStart = std::distance(pList->begin(), m_itStart) + 1;

		LPDIRECT3DVERTEXBUFFER9 pVtxBuffRoad = pMesh->GetVtxBuff();

		// ���_���̃|�C���^
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		pVtxBuffRoad->Lock(0, 0, (void**)&pVtx, 0);

		// �J�n�n�_�܂Ń|�C���^��i�߂�
		pVtx += nDistStart * MeshRoad::NUM_EDGE_IN_ROADPOINT * MeshRoad::NUM_VTX_IN_EDGE;

		// ���_���̌v�Z
		int nDistBetween = std::distance(m_itStart, m_itEnd);
		nDistBetween *= MeshRoad::NUM_EDGE_IN_ROADPOINT * MeshRoad::NUM_VTX_IN_EDGE;

		for (int i = 0; i < nDistBetween; i++)
		{
			vectorPosEdge.push_back(pVtx[0].pos);

			pVtx++;
		}

		// ���_�o�b�t�@���A�����b�N
		pVtxBuffRoad->Unlock();
	}
	else
	{
		return;
	}

	// ���_�𓹂ɉ��킹��========================================
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;
	
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nMeshV = GetMeshCylinder()->nMeshV;

	for (int i = 0; i < nMeshV + 1; i++)
	{
		int nIdx = i * 2;

		D3DXVECTOR3 pos1;
		D3DXVECTOR3 pos2;

		if (i == nMeshV)
		{
			pos1 = vectorPosEdge[nMeshV * 2 - 2];
			pos2 = vectorPosEdge[nMeshV * 2 - 1];
		}
		else
		{
			pos1 = vectorPosEdge[nIdx];
			pos2 = vectorPosEdge[nIdx + 1];
		}

		D3DXVECTOR3 vecDiff = pos2 - pos1;

		float fWidthRoad = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z) * 0.5f;

		for (int j = 0; j <= MESH_U / 2; j++)
		{
			float fRate = (float)j / (MESH_U / 2);
			
			pVtx[j].pos = pos1 + vecDiff * 0.5f * cosf(D3DX_PI * fRate) + vecDiff * 0.5f;

			float fHeight = sinf(D3DX_PI * fRate) * fWidthRoad;
			pVtx[j].pos.y += fHeight;

			pVtx[MESH_U - j].pos = pVtx[j].pos;
			pVtx[MESH_U - j].pos.y -= fHeight * 2;
		}

		pVtx += MESH_U + 1;
	}

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();
}

//=====================================================
// ��|���S���̐���
//=====================================================
void CTunnel::CreateFan(void)
{
	if (m_pFanEnter != nullptr)
	{
		m_pFanEnter->Uninit();
		m_pFanEnter = nullptr;
	}

	// ��|���S���̐���
	m_pFanEnter = CFan3D::Create(PRIORITY_FAN, MESH_U);

	if (m_pFanEnter == nullptr)
		return;

	int nIdx = Texture::GetIdx(DOOR_TEX_PATH);
	m_pFanEnter->SetIdxTexture(nIdx);

	// �g���l���̓����̒��_���W�擾==================================
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	std::vector<D3DXVECTOR3> vPos;

	for (int i = 0; i <= MESH_U; i++)
	{
		vPos.push_back(pVtx[i].pos);
	}

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	// �g���l���̓����̒��_���W�擾==================================
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffFan = *m_pFanEnter->GetVtxBuff();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuffFan->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i <= MESH_U; i++)
	{
		pVtx[i].pos = vPos[i];
	}

	// ���_�o�b�t�@���A�����b�N
	pVtxBuffFan->Unlock();

	m_pFanEnter->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//=====================================================
// �I������
//=====================================================
void CTunnel::Uninit(void)
{
	if (m_pFanEnter != nullptr)
	{
		m_pFanEnter->Uninit();
		m_pFanEnter = nullptr;
	}

	CMeshCylinder::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CTunnel::Update(void)
{
	// �v���C���[�̌��m
	DetectionPlayer();

	CMeshCylinder::Update();

	if (m_bInPlayer)
		StayPlayer();	// �v���C���[�������ɂ���Ƃ��̏���

	// �e�N�X�`�����W�ݒ菈��
	if (m_pFanEnter != nullptr)
		m_pFanEnter->SetTex(TEX_SCROLL_X, TEX_SCROLL_Y);
}

//=====================================================
// �v���C���[�̌��m
//=====================================================
void CTunnel::DetectionPlayer(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer == nullptr)
		return;

	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (!m_bInPlayer)
	{// �v���C���[�������Ă�������
		CollidePlayerEnter(pVtx);
	}
	else
	{// �v���C���[���o�Ă���������
		CollidePlayerExit(pVtx);
	}

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();
}

//=====================================================
// �v���C���[�����Ă��锻��
//=====================================================
void CTunnel::CollidePlayerEnter(VERTEX_3D *pVtx)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pVtx == nullptr || pPlayer == nullptr)
		return;

	// ����
	D3DXVECTOR3 pos1 = pVtx[0].pos;
	D3DXVECTOR3 pos2 = pVtx[MESH_U / 2].pos;

	CEffect3D::Create(pos1, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CEffect3D::Create(pos2, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// �o��
	int nNumVtx = GetNumVtx();

	D3DXVECTOR3 pos3 = pVtx[nNumVtx - MESH_U / 2 - 1].pos;
	D3DXVECTOR3 pos4 = pVtx[nNumVtx - 1].pos;

	CEffect3D::Create(pos3, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CEffect3D::Create(pos4, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// �O�ςł̃g���K�[����
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 movePlayer = pPlayer->GetMove();
	D3DXVECTOR3 posNext = posPlayer + movePlayer;
	float fRate;

	bool bHit1 = universal::IsCross(posPlayer, pos2, pos1, &fRate, posNext);
	bool bHit1Next = universal::IsCross(posNext, pos2, pos1, &fRate, posNext + movePlayer);

	if (fRate <= 1.0f && fRate >= 0.0f)
	{
		if (!m_bInPlayer && (!bHit1 && bHit1Next))
		{// �����������������
			EnterPlayer();
		}
	}

	CDebugProc::GetInstance()->Print("\nInfRate1[%f]", fRate);

	bool bHit2 = universal::IsCross(posPlayer, pos4, pos3, &fRate, posNext);
	bool bHit2Next = universal::IsCross(posNext, pos4, pos3, &fRate, posNext + movePlayer);

	if (fRate <= 1.0f && fRate >= 0.0f)
	{
		if (!m_bInPlayer && (!bHit2 && bHit2Next))
		{// �o���������������
			EnterPlayer();
		}
	}

	CDebugProc::GetInstance()->Print("\nInfRate2[%f]", fRate);
}

//=====================================================
// �v���C���[�o�Ă�������
//=====================================================
void CTunnel::CollidePlayerExit(VERTEX_3D *pVtx)
{
	if (pVtx == nullptr)
		return;

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pVtx == nullptr || pPlayer == nullptr)
		return;

	// ����
	D3DXVECTOR3 pos1 = pVtx[0].pos;
	D3DXVECTOR3 pos2 = pVtx[MESH_U / 2].pos;

	CEffect3D::Create(pos1, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CEffect3D::Create(pos2, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// �o��
	int nNumVtx = GetNumVtx();

	D3DXVECTOR3 pos3 = pVtx[nNumVtx - MESH_U / 2 - 1].pos;
	D3DXVECTOR3 pos4 = pVtx[nNumVtx - 1].pos;

	CEffect3D::Create(pos3, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	CEffect3D::Create(pos4, 50.0f, 5, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// �O�ςł̃g���K�[����
	D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
	D3DXVECTOR3 movePlayer = pPlayer->GetMove();
	D3DXVECTOR3 posNext = posPlayer + movePlayer;
	float fRate;

	bool bHit1 = universal::IsCross(posPlayer, pos2, pos1, &fRate, posNext);
	bool bHit1Next = universal::IsCross(posNext, pos2, pos1, &fRate, posNext + movePlayer);

	if (fRate <= 1.0f && fRate >= 0.0f)
	{
		if (m_bInPlayer && (bHit1 && !bHit1Next))
		{// ��������o������
			ExitPlayer();
		}
	}

	CDebugProc::GetInstance()->Print("\nOutfRate1[%f]", fRate);

	bool bHit2 = universal::IsCross(posPlayer, pos4, pos3, &fRate, movePlayer);
	bool bHit2Next = universal::IsCross(posNext, pos4, pos3, &fRate, movePlayer);

	if (fRate <= 1.0f && fRate >= 0.0f)
	{
		if (m_bInPlayer && (bHit2 && !bHit2Next))
		{// �o������o������
			ExitPlayer();
		}
	}

	CDebugProc::GetInstance()->Print("\nOutfRate2[%f]", fRate);
}

//=====================================================
// �v���C���[�������Ă����Ƃ��̏���
//=====================================================
void CTunnel::EnterPlayer(void)
{
	m_bInPlayer = true;

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		CPlayer::SParam param = pPlayer->GetParam();

		param.fSpeedMax = param.fSpeedMaxInitial * 4.0f;

		pPlayer->SetParam(param);
	}
}

//=====================================================
// �v���C���[���o���Ƃ��̏���
//=====================================================
void CTunnel::ExitPlayer(void)
{
	m_bInPlayer = false;

	CPlayer *pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		CPlayer::SParam param = pPlayer->GetParam();

		param.fSpeedMax = param.fSpeedMaxInitial;

		pPlayer->SetParam(param);
	}

	Blur::ResetBlur();
}

//=====================================================
// �v���C���[�������ɂ���Ƃ��̏���
//=====================================================
void CTunnel::StayPlayer(void)
{
	CPlayer *pPlayer = CPlayer::GetInstance();

	// �u���[��������
	Blur::SetBlur(10.0f, 0.5f);

	CPlayer::SParam param = pPlayer->GetParam();

	param.fSpeedMax = param.fSpeedMaxInitial * 4.0f;

	pPlayer->SetParam(param);
}

//=====================================================
// �`�揈��
//=====================================================
void CTunnel::Draw(void)
{
	CMeshCylinder::Draw();
}