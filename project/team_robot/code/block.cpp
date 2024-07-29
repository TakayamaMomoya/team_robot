//*****************************************************
//
// �u���b�N�̏���[block.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "block.h"
#include "blockManager.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "effect3D.h"
#include "fan3D.h"
#include "polygon3D.h"
#include "player.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************
// �萔��`
//*****************************************************
namespace
{
	const float SPEED_MOVE(1.0f);	// �ړ����x
	const char* MAP_FILE = "data\\MAP\\map01.bin";	// �}�b�v�̃t�@�C���p�X
	const float RADIUS_DRIFT_DEFAULT = 1000.0f;	// �h���t�g���a�̃f�t�H���g�l
	const float POLYGON_SIZE_X = 100.0f;        // ���|�C���gUI�̃T�C�Y
	const float POLYGON_SIZE_Y = 100.0f;        // ���|�C���gUI�̃T�C�Y
	const float STANDARD_LENGHT = 7000.0f;      // ���|�C���gUI���o�鋗��
}

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CBlock::m_nNumAll = 0;	// ����

//=====================================================
// �R���X�g���N�^
//=====================================================
CBlock::CBlock(int nPriority)
{
	m_fLife = 0.0f;
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_nIdx = -1;

	// �擪�A�Ō���A�h���X�擾
	CBlockManager *pManager = CBlockManager::GetInstance();
	CBlock *pHead = nullptr;
	CBlock *pTail = nullptr;

	if (pManager != nullptr)
	{
		pHead = pManager->GetHead();
		pTail = pManager->GetTail();
	}

	if (pHead == nullptr)
	{// �擪�ƍŌ���A�h���X�̑��
		pManager->SetHead(this);
		pManager->SetTail(this);

		return;
	}

	// �O�̃A�h���X�ɍŌ���̃A�h���X��������
	m_pPrev = pTail;

	// �Ō���̃A�h���X�������ɂ���
	pManager->SetTail(this);

	if (m_pPrev != nullptr)
	{
		// �O�̃I�u�W�F�N�g�̎��̃A�h���X�������ɂ���
		m_pPrev->m_pNext = this;
	}

	m_nNumAll++;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBlock::~CBlock()
{
	// �擪�A�Ō���A�h���X�擾
	CBlockManager *pManager = CBlockManager::GetInstance();
	CBlock *pHead = nullptr;
	CBlock *pTail = nullptr;

	if (pManager != nullptr)
	{
		pHead = pManager->GetHead();
		pTail = pManager->GetTail();
	}

	m_nNumAll--;

	if (pTail != this && pHead != this)
	{// �^�񒆂̃A�h���X�̔j��
		if (m_pPrev != nullptr)
		{
			// �O�̃A�h���X���玟�̃A�h���X���Ȃ�
			m_pPrev->m_pNext = m_pNext;
		}

		if (m_pNext != nullptr)
		{
			// ���̃A�h���X����O�̃A�h���X���Ȃ�
			m_pNext->m_pPrev = m_pPrev;
		}
	}

	if (pHead == this)
	{// �擪�A�h���X�̔j��
		//if (m_pNext != nullptr)
		{// �擪�A�h���X�����̃A�h���X�Ɉ����p��
			pManager->SetHead(m_pNext);

			if (m_pNext != nullptr)
			{
				m_pNext->m_pPrev = nullptr;
			}
		}
	}

	if (pTail == this)
	{// �Ō���A�h���X�̔j��
		if (m_pPrev != nullptr)
		{// �Ō���A�h���X��O�̃A�h���X�Ɉ����p��
			pManager->SetTail(m_pPrev);

			m_pPrev->m_pNext = nullptr;
		}
	}

	m_nNumAll--;
}

//=====================================================
// ��������
//=====================================================
CBlock *CBlock::Create(BEHAVIOUR behaviour)
{
	CBlock *pBlock = nullptr;

	// �C���X�^���X����
	switch (behaviour)
	{
	case BEHAVIOUR_NORMAL:
		pBlock = new CBlock;
		break;
	case BEHAVIOUR_GRAB:
		pBlock = new CBlockGrab;
		break;
	default:
		assert(("�u���b�N�̃r�w�C�r�A�[�ɕs���Ȓl�����͂���܂���", false));
		break;
	}

	if (pBlock != nullptr)
	{
		// ����������
		pBlock->Init();
	}

	return pBlock;
}

//=====================================================
// ����������
//=====================================================
HRESULT CBlock::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �p���N���X�̏�����
	CObjectX::Init();

	// �^�C�v�̐ݒ�
	SetType(TYPE_BLOCK);

	m_fLife = 300.0f;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CBlock::Uninit(void)
{
	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CBlock::Update(void)
{
	// �p���N���X�̍X�V
	CObjectX::Update();
}

//=====================================================
// �ʒu�ݒ�
//=====================================================
void CBlock::SetPosition(D3DXVECTOR3 pos)
{
	CObjectX::SetPosition(pos);
}

//=====================================================
// �`�揈��
//=====================================================
void CBlock::Draw(void)
{
	// �p���N���X�̕`��
	CObjectX::Draw();
}

//=====================================================
// �ۑ�����
//=====================================================
void CBlock::Save(FILE *pFile)
{
	fprintf(pFile, "SETBLOCK\n");

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	int nIdx = GetIdx();

	fprintf(pFile, " IDX = %d\n", nIdx);
	fprintf(pFile, " POS = %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
	fprintf(pFile, " ROT = %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);
}

//=====================================================
// �Ǎ�����
//=====================================================
void CBlock::Load(FILE *pFile, char* pTemp)
{
	if (strcmp(pTemp, "IDX") == 0)
	{// �C���f�b�N�X
		int nIdx;

		CBlockManager *pBlockManager = CBlockManager::GetInstance();

		if (pBlockManager != nullptr)
		{
			(void)fscanf(pFile, "%s", pTemp);

			(void)fscanf(pFile, "%d", &nIdx);

			SetIdx(nIdx);

			CBlockManager::SInfoBlock *InfoBlock = pBlockManager->GetInfoBlock();

			BindModel(InfoBlock[nIdx].nIdxModel);
		}
	}

	if (strcmp(pTemp, "POS") == 0)
	{// �ʒu
		D3DXVECTOR3 pos;

		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &pos.x);
		(void)fscanf(pFile, "%f", &pos.y);
		(void)fscanf(pFile, "%f", &pos.z);

		SetPosition(pos);
	}

	if (strcmp(pTemp, "ROT") == 0)
	{// ����
		D3DXVECTOR3 rot;

		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &rot.x);
		(void)fscanf(pFile, "%f", &rot.y);
		(void)fscanf(pFile, "%f", &rot.z);

		SetRotation(rot);
	}
}

//=====================================================
// �����蔻�菈��
//=====================================================
bool CBlock::Collide(D3DXVECTOR3* pPos, D3DXVECTOR3 posOld)
{
	float fHeight = pPos->y;

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	D3DXVECTOR3 BlockCorner[4];
	D3DXVECTOR3 BlockNor;
	D3DXVECTOR3 BlockMax = GetVtxMax();
	D3DXVECTOR3 BlockMin = GetVtxMin();

	BlockCorner[0] = universal::PosRelativeMtx(pos, rot, D3DXVECTOR3(BlockMax.x, BlockMax.y, BlockMin.z));
	BlockCorner[1] = universal::PosRelativeMtx(pos, rot, D3DXVECTOR3(BlockMin.x, BlockMax.y, BlockMin.z));
	BlockCorner[2] = universal::PosRelativeMtx(pos, rot, D3DXVECTOR3(BlockMin.x, BlockMax.y, BlockMax.z));
	BlockCorner[3] = universal::PosRelativeMtx(pos, rot, D3DXVECTOR3(BlockMax.x, BlockMax.y, BlockMax.z));

	BlockNor = universal::NorRelativeMtx(pos, rot, BlockMax, BlockMin);

	// �|���S���̉��ɓ����Ă��邩���肷��
	if (D3DXVec3Length(&(pos - *pPos)) > 10000.0f)
		return false;

	if (!universal::IsOnSquare(BlockCorner[0], BlockCorner[1], BlockCorner[2], BlockCorner[3], BlockNor, *pPos, posOld, fHeight))
		return false;

	// ���������̍����ȓ������肷��
	if (200.0f < fHeight - pPos->y)
		return false;

	pPos->y = fHeight;

	return true;
}

//============================================================================
// �͂߂�u���b�N�N���X
//============================================================================
//=====================================================
// �R���X�g���N�^
//=====================================================
CBlockGrab::CBlockGrab() : m_bCurrent(false), m_fRadiusOffset(0.0f),m_pFan(nullptr)
{
	for (int i = 0; i < NUM_OFFSET; i++)
	{
		m_afAngleOffset[i] = 0.0f;
	}

	m_pGrabPointUI = nullptr;
}
 
//=====================================================
// �f�X�g���N�^
//=====================================================
CBlockGrab::~CBlockGrab()
{

}

//=====================================================
// ������
//=====================================================
HRESULT CBlockGrab::Init(void)
{
	// �͂݃u���b�N���X�g�ɒǉ�
	CBlockManager *pBlockManager =  BlockManager::GetInstance();

	if (pBlockManager != nullptr)
		pBlockManager->AddGrabList(this);

	// ���N���X�̏�����
	CBlock::Init();

	// �I�t�Z�b�g�̏�����
	m_afAngleOffset[0] = D3DX_PI;
	m_afAngleOffset[1] = D3DX_PI * 0.7f;
	m_fRadiusOffset = 1000.0f;

#ifdef _DEBUG
	// ��������p�̐��
	if (m_pFan == nullptr)
	{
		m_pFan = CFan3D::Create();
		
		if (m_pFan != nullptr)
		{
			m_pFan->SetRotation(D3DXVECTOR3(D3DX_PI * 0.5f, 0.0f, 0.0f));
			m_pFan->SetRadius(400.0f);
			m_pFan->SetVtx();

			SetFan();
		}
	}
#endif

	return S_OK;
}

//=====================================================
// �I��
//=====================================================
void CBlockGrab::Uninit(void)
{
	// �͂݃u���b�N���X�g���珜�O
	CBlockManager *pBlockManager = BlockManager::GetInstance();

	if (pBlockManager != nullptr)
		pBlockManager->RemoveGrabList(this);
	
	// ���N���X�̏I��
	CBlock::Uninit();
}

//=====================================================
// �X�V
//=====================================================
void CBlockGrab::Update(void)
{
	CBlock::Update();

	// ���|�C���g��UI�\��
	GrabPointUI();

#ifdef _DEBUG
	SetFan();	// ��|���S���̐ݒ�
#endif
}

//=====================================================
// �`��
//=====================================================
void CBlockGrab::Draw(void)
{
	if (m_bCurrent)
	{
		CEffect3D::Create(GetPosition(), 200.0f, 5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	CBlock::Draw();
}

//=====================================================
// �͂߂邩�̔���
//=====================================================
bool CBlockGrab::CanGrab(D3DXVECTOR3 pos)
{
	bool bCanGrab1 = true;
	bool bCanGrab2 = true;

	// ����̐ݒu
	D3DXMATRIX mtxVec1;
	D3DXMATRIX mtxVec2;
	D3DXMATRIX mtx = GetMatrix();

	// �I�t�Z�b�g�̐ݒ�
	D3DXVECTOR3 offset1 = { sinf(m_afAngleOffset[0]) * m_fRadiusOffset,0.0f,cosf(m_afAngleOffset[0]) * m_fRadiusOffset };
	D3DXVECTOR3 offset2 = { sinf(m_afAngleOffset[1]) * m_fRadiusOffset,0.0f,cosf(m_afAngleOffset[1]) * m_fRadiusOffset };

	universal::SetOffSet(&mtxVec1, mtx, offset1);
	universal::SetOffSet(&mtxVec2, mtx, offset2);

	D3DXVECTOR3 posMtx1 = { mtxVec1._41,mtxVec1._42 ,mtxVec1._43 };
	D3DXVECTOR3 posMtx2 = { mtxVec2._41,mtxVec2._42 ,mtxVec2._43 };

	bCanGrab1 = universal::IsCross(pos, posMtx1, GetPosition(), nullptr);
	bCanGrab2 = universal::IsCross(pos, GetPosition(), posMtx2, nullptr);

#if 0
	CDebugProc::GetInstance()->Print("\n�͂߂�1[%d]", bCanGrab1);
	CDebugProc::GetInstance()->Print("\n�͂߂�2[%d]", bCanGrab2);
#endif

	bool bOK = bCanGrab1 && bCanGrab2;

#ifdef _DEBUG
	CEffect3D::Create(posMtx1, 100.0f, 3, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	CEffect3D::Create(posMtx2, 100.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	
	if (bCanGrab1)
	{
		posMtx1.y += 100.0f;
		CEffect3D::Create(posMtx1, 100.0f, 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	
	if (bCanGrab2)
	{
		posMtx2.y += 100.0f;
		CEffect3D::Create(posMtx2, 100.0f, 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (bOK)
	{
		CEffect3D::Create(GetPosition(), 150.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
#endif

	return bOK;
}

//=====================================================
// ��|���S���̐ݒ�
//=====================================================
void CBlockGrab::SetFan(void)
{
	// �I�t�Z�b�g�̐ݒ�
	D3DXMATRIX mtxVec1;
	D3DXMATRIX mtxVec2;
	D3DXMATRIX mtx = GetMatrix();

	D3DXVECTOR3 offset1 = { sinf(m_afAngleOffset[0]) * m_fRadiusOffset,0.0f,cosf(m_afAngleOffset[0]) * m_fRadiusOffset };
	D3DXVECTOR3 offset2 = { sinf(m_afAngleOffset[1]) * m_fRadiusOffset,0.0f,cosf(m_afAngleOffset[1]) * m_fRadiusOffset };

	universal::SetOffSet(&mtxVec1, mtx, offset1);
	universal::SetOffSet(&mtxVec2, mtx, offset2);

	D3DXVECTOR3 posMtx1 = { mtxVec1._41,mtxVec1._42 ,mtxVec1._43 };
	D3DXVECTOR3 posMtx2 = { mtxVec2._41,mtxVec2._42 ,mtxVec2._43 };

	CEffect3D::Create(posMtx1, 100.0f, 3, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	CEffect3D::Create(posMtx2, 100.0f, 3, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	// ��̊p�x�̐ݒ�
	if (m_pFan != nullptr)
	{
		D3DXVECTOR3 pos = GetPosition();

		float fAngleOffset1 = atan2f(posMtx1.x - pos.x, posMtx1.z - pos.z);
		float fAngleOffset2 = atan2f(posMtx2.x - pos.x, posMtx2.z - pos.z);

		float fDiff = fAngleOffset1 - fAngleOffset2;

		float fRate = fDiff / (D3DX_PI * 2.0f);

		if (fRate < 0)
			fRate = 1.0f + fRate;

		m_pFan->SetRateAngle(fRate);

		pos.y += 10.0f;
		m_pFan->SetPosition(pos);
		m_pFan->SetRotation(D3DXVECTOR3(D3DX_PI * 0.5f, fAngleOffset2, 0.0f));
		m_pFan->SetVtx();
	}
}

//=====================================================
// �ʒu�ݒ�
//=====================================================
void CBlockGrab::SetPosition(D3DXVECTOR3 pos)
{
	CBlock::SetPosition(pos);

#ifdef _DEBUG
	Draw();

	if (m_pFan != nullptr)
	{
		SetFan();
	}
#endif
}

//=====================================================
// �����̐ݒ�
//=====================================================
void CBlockGrab::SetRotation(D3DXVECTOR3 rot)
{
	CBlock::SetRotation(rot);

#ifdef _DEBUG
	Draw();

	if (m_pFan != nullptr)
	{
		SetFan();
	}
#endif
}

//=====================================================
// ���|�C���gUI�\������
//=====================================================
void CBlockGrab::GrabPointUI(void)
{
	CPlayer* pPlayer = CPlayer::GetInstance();

	if (pPlayer != nullptr)
	{
		D3DXVECTOR3 PlayerPos = pPlayer->GetPosition();
		D3DXVECTOR3 pos = GetPosition();

		// �v���C���[�����|�C���g�̋���
		float fLenght = (pos.x + pos.z) - (PlayerPos.x + PlayerPos.z);
		fLenght = fabs(fLenght);

		if (fLenght < STANDARD_LENGHT)
		{// �͈͓�

			if (m_pGrabPointUI == nullptr)
			{
				// UI�̐ݒ�
				m_pGrabPointUI = CPolygon3D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				int nIdx = CTexture::GetInstance()->Regist("data\\TEXTURE\\UI\\FK.png");
				m_pGrabPointUI->SetIdxTexture(nIdx);
				D3DXVECTOR3 vtxmax = GetVtxMax();
				m_pGrabPointUI->SetPosition(D3DXVECTOR3(pos.x, pos.y + vtxmax.y, pos.z));
				m_pGrabPointUI->SetMode(CPolygon3D::MODE_BILLBOARD);
				m_pGrabPointUI->SetSize(POLYGON_SIZE_X, POLYGON_SIZE_Y);
				m_pGrabPointUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));
				m_pGrabPointUI->SetVtx();
				m_pGrabPointUI->EnableZtest(true);
			}

			if(m_fColA <= 1.0f)
			   m_fColA += 0.1f;

			m_pGrabPointUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));
		}
		else
		{// �͈͊O

			if (m_pGrabPointUI != nullptr)
			{
				if (m_fColA >= 0.0f)
					m_fColA -= 0.1f;

				m_pGrabPointUI->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColA));

				if (m_fColA <= 0.0f)
				{
					m_pGrabPointUI->Uninit();
					m_pGrabPointUI = nullptr;
				}
			}
		}
	}
}

//=====================================================
// �ۑ�����
//=====================================================
void CBlockGrab::Save(FILE *pFile)
{
	fprintf(pFile, "SETGRABBLOCK\n");

	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRotation();
	int nIdx = GetIdx();

	fprintf(pFile, " IDX = %d\n", nIdx);
	fprintf(pFile, " POS = %.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
	fprintf(pFile, " ROT = %.2f %.2f %.2f\n", rot.x, rot.y, rot.z);

	fprintf(pFile, " ANGLE_OFFSET = %.2f %.2f\n", m_afAngleOffset[0], m_afAngleOffset[1]);
	fprintf(pFile, " RADIUS_OFFSET = %.2f\n", m_fRadiusOffset);
}

//=====================================================
// �Ǎ�����
//=====================================================
void CBlockGrab::Load(FILE *pFile, char* pTemp)
{
	// ���ʂ̓Ǎ�
	CBlock::Load(pFile, pTemp);

	if (strcmp(pTemp, "ANGLE_OFFSET") == 0)
	{// �I�t�Z�b�g�̊p�x
		(void)fscanf(pFile, "%s", pTemp);

		for (int i = 0; i < NUM_OFFSET; i++)
		{
			(void)fscanf(pFile, "%f", &m_afAngleOffset[i]);
		}
	}

	if (strcmp(pTemp, "RADIUS_OFFSET") == 0)
	{// �I�t�Z�b�g�̔��a
		(void)fscanf(pFile, "%s", pTemp);

		(void)fscanf(pFile, "%f", &m_fRadiusOffset);
	}
}