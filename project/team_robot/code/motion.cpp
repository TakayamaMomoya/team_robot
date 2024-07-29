//*****************************************************
//
// ���[�V�����̏���[motion.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "motion.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "particle.h"
#include "sound.h"
#include "universal.h"
#include "slow.h"
#include "effect3D.h"
#include "afterimage.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_STRING	(256)	// ������̍ő吔

//=====================================================
// �R���X�g���N�^
//=====================================================
CMotion::CMotion(int nPriority) : CObject3D(nPriority)
{
	ZeroMemory(&m_aMotionInfo, sizeof(m_aMotionInfo));
	ZeroMemory(&m_aKeyOld, sizeof(m_aKeyOld));
	ZeroMemory(&m_apParts[0], sizeof(m_apParts));
	ZeroMemory(&m_abMotion[0], sizeof(m_abMotion));
	m_bLoopMotion = false;
	m_motionType = 0;
	m_motionTypeOld = 0;
	m_fCounterMotion = 0;
	m_nKey = 0;
	m_nFrame = 0;
	m_nNumKey = 0;
	m_nNumMotion = 0;
	m_nNumParts = 0;
	m_bFinish = false;
	m_bShadow = false;
	m_posOld = { 0.0f,0.0f,0.0f };
	m_posShadow = { 0.0f,0.0f,0.0f };
	m_move = { 0.0f,0.0f,0.0f };
	m_col = { 1.0f,1.0f,1.0f,1.0f };
	m_bInde = false;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMotion::~CMotion()
{

}

//=====================================================
// ��������
//=====================================================
CMotion *CMotion::Create(char *pPath)
{
	CMotion *pMotion = nullptr;

	if (pMotion == nullptr)
	{
		// �C���X�^���X����
		pMotion = new CMotion;

		if (pMotion != nullptr)
		{
			// �Ǎ�����
			pMotion->Load(pPath);

			// ����������
			pMotion->Init();
		}
	}

	return pMotion;
}

//=====================================================
// ����������
//=====================================================
HRESULT CMotion::Init(void)
{
	// �������[�V�����ݒ�
	SetMotion(0);
	InitPose(0);

	// ���p�[�c���Z�b�g
	ResetEnableMotion();

	CObject3D::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CMotion::Uninit(void)
{
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{// �p�[�e�B�N�����̔j��
		if (m_aMotionInfo[nCntMotion].pEvent != nullptr)
		{
			delete m_aMotionInfo[nCntMotion].pEvent;
			m_aMotionInfo[nCntMotion].pEvent = nullptr;
		}
	}

	CObject3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CMotion::Update(void)
{
	if (m_apParts[0] == nullptr)
	{
		return;
	}

	// ���݂̈ʒu��ۑ�
	m_posOld = GetPosition();

	//�ϐ��錾
	int nNextKey;

	// �p�[�c�̃g�����X�t�H�[��
	D3DXVECTOR3 pos, rot;

	// ���[�V�����̃g�����X�t�H�[���Ǘ�
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_abMotion[nCntParts] == false)
		{
			continue;
		}

		if (m_nKey >= m_aMotionInfo[m_motionType].nNumKey - 1)
		{
			break;
		}

		// �p�[�c�̃g�����X�t�H�[���擾
		pos = m_apParts[nCntParts]->pParts->GetPosOrg();
		rot = m_apParts[nCntParts]->pParts->GetRotation();

		if (m_nKey < m_aMotionInfo[m_motionType].nNumKey - 1)
		{
			nNextKey = m_nKey + 1;
		}
		else
		{
			nNextKey = m_aMotionInfo[m_motionType].nNumKey - 1;
		}

		//�O��Ǝ��̃L�[�̍����擾
		float DiffPosX = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosX -
			m_aKeyOld[nCntParts].fPosX;

		float DiffPosY = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosY -
			m_aKeyOld[nCntParts].fPosY;

		float DiffPosZ = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fPosZ -
			m_aKeyOld[nCntParts].fPosZ;

		float DiffRotX = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotX -
			m_aKeyOld[nCntParts].fRotX;

		float DiffRotY = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotY -
			m_aKeyOld[nCntParts].fRotY;

		float DiffRotZ = m_aMotionInfo[m_motionType].aKeyInfo[nNextKey].aKey[nCntParts].fRotZ -
			m_aKeyOld[nCntParts].fRotZ;

		universal::LimitRot(&DiffRotX);
		universal::LimitRot(&DiffRotY);
		universal::LimitRot(&DiffRotZ);

		//�ړI�̒l=======================================================================================================
		float DestPosX = pos.x + m_aKeyOld[nCntParts].fPosX +
			DiffPosX * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		float DestPosY = pos.y + m_aKeyOld[nCntParts].fPosY +
			DiffPosY * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		float DestPosZ = pos.z + m_aKeyOld[nCntParts].fPosZ +
			DiffPosZ * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		float DestRotX = m_aKeyOld[nCntParts].fRotX +
			DiffRotX * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		float DestRotY = m_aKeyOld[nCntParts].fRotY +
			DiffRotY * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		float DestRotZ = m_aKeyOld[nCntParts].fRotZ +
			DiffRotZ * (float)(1.0f / (float)m_nFrame) * m_fCounterMotion;

		//�p�[�c�̌����E�ʒu�ݒ�
		m_apParts[nCntParts]->pParts->SetPosition(D3DXVECTOR3(DestPosX, DestPosY, DestPosZ));

		universal::LimitRot(&DestRotX);
		universal::LimitRot(&DestRotY);
		universal::LimitRot(&DestRotZ);

		rot = D3DXVECTOR3(DestRotX, DestRotY, DestRotZ);

		m_apParts[nCntParts]->pParts->SetRotation(rot);
	}

	float fFrameOld = m_fCounterMotion;;

	CSlow *pSlow = CSlow::GetInstance();

	if (pSlow != nullptr)
	{
		float fScale = pSlow->GetScale();

		m_fCounterMotion += fScale;
	}
	else
	{
		m_fCounterMotion += 1.0f;
	}

	// �C�x���g�̊Ǘ�
	if (m_aMotionInfo[m_motionType].pEvent != nullptr &&
		m_aMotionInfo[m_motionType].nNumEvent > 0)
	{
		for (int nCntEvent = 0; nCntEvent < m_aMotionInfo[m_motionType].nNumEvent; nCntEvent++)
		{
			if (m_nKey == m_aMotionInfo[m_motionType].pEvent[nCntEvent].nKey &&
				m_fCounterMotion >= m_aMotionInfo[m_motionType].pEvent[nCntEvent].nFrame &&
				fFrameOld < m_aMotionInfo[m_motionType].pEvent[nCntEvent].nFrame)
			{// �C�x���g�̊J�n
				m_aMotionInfo[m_motionType].pEvent[nCntEvent].fTimer = 0.0f;
			}

			if (m_aMotionInfo[m_motionType].pEvent[nCntEvent].fTimer <= m_aMotionInfo[m_motionType].pEvent[nCntEvent].fNumFrame)
			{// �C�x���g�̌Ăяo��
				Event(&m_aMotionInfo[m_motionType].pEvent[nCntEvent]);

				D3DXMATRIX mtxParent = GetParts(m_aMotionInfo[m_motionType].pEvent[nCntEvent].nIdxParent)->pParts->GetMatrix();
				D3DXMATRIX mtx;

				universal::SetOffSet(&mtx, mtxParent, m_aMotionInfo[m_motionType].pEvent[nCntEvent].offset);

				D3DXVECTOR3 pos =
				{
					mtx._41,
					mtx._42,
					mtx._43
				};

				float fRadius = m_aMotionInfo[m_motionType].pEvent[nCntEvent].fRadius;

				CEffect3D::Create(pos, fRadius, 10, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

				m_aMotionInfo[m_motionType].pEvent[nCntEvent].fTimer += 1.0f;
			}
		}
	}

	if (m_nKey >= m_aMotionInfo[m_motionType].nNumKey - 1)
	{// ���[�V�������I�����Ă�����
		m_motionTypeOld = m_motionType;

		// �I��������Ԃɂ���
		m_bFinish = true;

		if (m_aMotionInfo[m_motionType].bLoop)
		{// ���[�v������͍̂Đݒ�
			SetMotion(m_motionType);
		}
	}

	if (m_fCounterMotion > m_nFrame)
	{//�L�[�̃t���[�����ɒB������
		if (m_nKey < m_aMotionInfo[m_motionType].nNumKey)
		{
			m_nKey++;

			m_nFrame = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;

			m_fCounterMotion = 0;

			SetKeyOld();
		}
	}
}

//=====================================================
// ���[�V�����ݒ�
//=====================================================
void CMotion::SetMotion(int nMotionType)
{
	// �I����Ԃ̉���
	m_bFinish = false;

	// �I�����郂�[�V������ۑ�
	m_motionTypeOld = m_motionType;
	SetKeyOld();

	if (m_nKey != -1)
	{
		m_nFrame = m_aMotionInfo[m_motionType].aKeyInfo[m_nKey].nFrame;
	}

	if (m_motionType != nMotionType)
	{
		m_nFrame = 10;
	}

	// ���[�V�������̐ݒ�
	m_motionType = nMotionType;
	m_nKey = -1;
	m_fCounterMotion = 0;
}

//=====================================================
// �I������L�[�̏��ۑ�
//=====================================================
void CMotion::SetKeyOld(void)
{
	for (int nCntPart = 0;nCntPart < m_nNumParts;nCntPart++)
	{
		m_aKeyOld[nCntPart].fPosX = m_apParts[nCntPart]->pParts->GetPosition().x - m_apParts[nCntPart]->pParts->GetPosOrg().x;
		m_aKeyOld[nCntPart].fPosY = m_apParts[nCntPart]->pParts->GetPosition().y - m_apParts[nCntPart]->pParts->GetPosOrg().y;
		m_aKeyOld[nCntPart].fPosZ = m_apParts[nCntPart]->pParts->GetPosition().z - m_apParts[nCntPart]->pParts->GetPosOrg().z;

		m_aKeyOld[nCntPart].fRotX = m_apParts[nCntPart]->pParts->GetRotation().x;
		m_aKeyOld[nCntPart].fRotY = m_apParts[nCntPart]->pParts->GetRotation().y;
		m_aKeyOld[nCntPart].fRotZ = m_apParts[nCntPart]->pParts->GetRotation().z;
	}
}

//=====================================================
// �|�[�Y�̏����ݒ�
//=====================================================
void CMotion::InitPose(int nMotion)
{
	for (int i = 0; i < m_aMotionInfo[nMotion].nNumKey;i++)
	{
		for (int nCntPart = 0; nCntPart < m_nNumParts; nCntPart++)
		{
			m_aKeyOld[nCntPart] = m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart];

			D3DXVECTOR3 rot =
			{
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fRotX,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fRotY,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fRotZ
			};

			D3DXVECTOR3 pos =
			{
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fPosX,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fPosY,
				m_aMotionInfo[nMotion].aKeyInfo[i].aKey[nCntPart].fPosZ
			};

			m_apParts[nCntPart]->pParts->SetRotation(rot);
			//m_apParts[nCntPart]->pParts->SetPosition(pos);
		}
	}
}

//=====================================================
// �����̃}�g���b�N�X�̐ݒ�
//=====================================================
void CMotion::CalcMatrix(void)
{
	CObject3D::Draw();
}

//=====================================================
// �}�g���b�N�X���������킹�鏈��
//=====================================================
void CMotion::MultiplyMtx(bool bDrawParts)
{
	if (m_bInde == false)
	{
		// �e�̃}�g���b�N�X
		CalcMatrix();
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//���[���h�}�g���b�N�X�ݒ�
	D3DXMATRIX mtxWorld = GetMatrix();
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	D3DXMATRIX mtxRotModel, mtxTransModel;
	D3DXMATRIX *pMtxParent;
	D3DXMATRIX mtx;

	for (int nCntParts = 0;nCntParts < m_nNumParts;nCntParts++)
	{
		// �}�g���b�N�X�̎擾
		mtx = m_apParts[nCntParts]->pParts->GetMatrix();

		//���[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&mtx);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			m_apParts[nCntParts]->pParts->GetRotation().y, m_apParts[nCntParts]->pParts->GetRotation().x, m_apParts[nCntParts]->pParts->GetRotation().z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxRotModel);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTransModel,
			m_apParts[nCntParts]->pParts->GetPosition().x, m_apParts[nCntParts]->pParts->GetPosition().y, m_apParts[nCntParts]->pParts->GetPosition().z);
		D3DXMatrixMultiply(&mtx, &mtx, &mtxTransModel);
		
		if (m_apParts[nCntParts]->nIdxParent != -1)
		{//�e�p�[�c������ꍇ
			// �e�}�g���b�N�X�̎擾
			pMtxParent = &m_apParts[m_apParts[nCntParts]->nIdxParent]->pParts->GetMatrix();
		}
		else
		{
			pMtxParent = &mtxWorld;
		}

		//�e�p�[�c�ƃp�[�c�̃��[���h�}�g���b�N�X���������킹��
		D3DXMatrixMultiply(&mtx, &mtx, pMtxParent);

		m_apParts[nCntParts]->pParts->SetMatrix(mtx);

		//���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &mtx);

		if(bDrawParts)
			m_apParts[nCntParts]->pParts->JustDraw();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CMotion::Draw(void)
{
	// �}�g���b�N�X���������킹�鏈��
	MultiplyMtx();
}

//=====================================================
// �Ǎ�����
//=====================================================
void CMotion::Load(char *pPath)
{
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{// �p�[�e�B�N�����̔j��
		if (m_aMotionInfo[nCntMotion].pEvent != nullptr)
		{
			delete m_aMotionInfo[nCntMotion].pEvent;
			m_aMotionInfo[nCntMotion].pEvent = nullptr;
		}
	}

	//ZeroMemory(&m_aMotionInfo, sizeof(m_aMotionInfo));
	ZeroMemory(&m_aKeyOld, sizeof(m_aKeyOld));
	ZeroMemory(&m_apParts[0], sizeof(m_apParts));
	//ZeroMemory(&m_abMotion[0], sizeof(m_abMotion));
	m_nKey = 0;
	m_nNumMotion = 0;

	//�ϐ��錾
	char cTemp[MAX_STRING];
	int nCntMotion = 0;
	int nCntModel = 0;

	//�t�@�C������ǂݍ���
	FILE *pFile = fopen(pPath, "r");

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ
		while(true)
		{
			//�����ǂݍ���
			(void)fscanf(pFile, "%s", &cTemp[0]);

			//�t�@�C�����ǂݍ���=========================================
			if (strcmp(cTemp, "NUM_MODEL") == 0)
			{
				//"="�ǂݍ���
				(void)fscanf(pFile, "%s", &cTemp[0]);

				//���f�����ǂݍ���
				(void)fscanf(pFile, "%d", &m_nNumParts);

				for (int nCntFile = 0; nCntFile < m_nNumParts;)
				{//�t�@�C�����ǂݍ���

					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "MODEL_FILENAME") == 0)
					{//�t�@�C�����ǂݍ���
						(void)fscanf(pFile, "%s", &cTemp[0]);

						char aPath[MAX_STRING];

						// ���f���p�X�Ǎ�
						(void)fscanf(pFile, "%s", &aPath[0]);

						m_apParts[nCntFile] = new Parts;

						m_apParts[nCntFile]->pParts = CParts::Create();

						int nIdx = CModel::Load(&aPath[0]);

						// ���f���Ǎ�
						m_apParts[nCntFile]->pParts->SetIdxModel(nIdx);
						m_apParts[nCntFile]->pParts->BindModel(m_apParts[nCntFile]->pParts->GetIdxModel());

						nCntFile++;
					}
				}
			}
			//===========================================================

			//�L�����N�^�[�ݒ�===========================================
			if (strcmp(cTemp, "CHARACTERSET") == 0)
			{//�L�����N�^�[�X�^�[�g
				nCntModel = 0;

				while (strcmp(cTemp, "END_CHARACTERSET") != 0)
				{//�I���܂ŃL�����N�^�[�ݒ�
					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "PARTSSET") == 0)
					{//�p�[�c�X�^�[�g
						while (strcmp(cTemp, "END_PARTSSET") != 0)
						{//�I���܂Ńp�[�c�ݒ�
							(void)fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "INDEX") == 0)
							{//�ԍ��ǂݍ���
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdx);
							}

							if (strcmp(cTemp, "PARENT") == 0)
							{//�e�ԍ��ǂݍ���
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_apParts[nCntModel]->nIdxParent);
							}

							if (strcmp(cTemp, "POS") == 0)
							{//�ʒu�ǂݍ���
								D3DXVECTOR3 pos;

								(void)fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
								{
									(void)fscanf(pFile, "%f", &pos[nCntPos]);
								}

								m_apParts[nCntModel]->pParts->SetPosition(pos);

								m_apParts[nCntModel]->pParts->SetPosOrg(pos);
							}

							if (strcmp(cTemp, "ROT") == 0)
							{//�����ǂݍ���
								D3DXVECTOR3 rot;

								(void)fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntRot = 0; nCntRot < 3; nCntRot++)
								{
									(void)fscanf(pFile, "%f", &rot[nCntRot]);
								}

								m_apParts[nCntModel]->pParts->SetRotation(rot);
							}

						}//END_PART

						nCntModel++;
					}
				}//END_CHAR
			}
			//===========================================================

			//�ϐ��錾
			int nCntKey = 0;
			int nCntPart = 0;
			int nCntEvent = 0;

			//���[�V�����ݒ�===========================================
			if (strcmp(cTemp, "MOTIONSET") == 0)
			{//���[�V�����X�^�[�g
				while (strcmp(cTemp, "END_MOTIONSET") != 0)
				{//�I���܂ŃL�����N�^�[�ݒ�

					(void)fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "LOOP") == 0)
					{//���[�v���f
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", (int*)&m_aMotionInfo[nCntMotion].bLoop);
					}

					if (strcmp(cTemp, "NUM_KEY") == 0)
					{//�L�[�����f
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNumKey);
					}

					if (strcmp(cTemp, "NUM_EVENT") == 0)
					{// �C�x���g�����f
						(void)fscanf(pFile, "%s", &cTemp[0]);

						(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].nNumEvent);

						if (m_aMotionInfo[m_nNumMotion].nNumEvent != 0)
						{// �C�x���g���𐶐�
							m_aMotionInfo[m_nNumMotion].pEvent = new EVENT_INFO[m_aMotionInfo[m_nNumMotion].nNumEvent];

							ZeroMemory(m_aMotionInfo[m_nNumMotion].pEvent, sizeof(EVENT_INFO) * m_aMotionInfo[m_nNumMotion].nNumEvent);

							for (int i = 0; i < m_aMotionInfo[m_nNumMotion].nNumEvent;i++)
							{
								m_aMotionInfo[m_nNumMotion].pEvent->fTimer = FLT_MAX;
							}
						}
					}

					if (strcmp(cTemp, "EVENTSET") == 0 && m_aMotionInfo[m_nNumMotion].pEvent != 0)
					{// �C�x���g���ݒ�
						while (strcmp(cTemp, "END_EVENTSET") != 0)
						{// �I���܂ŃC�x���g�ݒ�
							(void)fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "KEY") == 0)
							{// �Đ��L�[�擾
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nKey);
							}

							if (strcmp(cTemp, "FRAME") == 0)
							{// �Đ��t���[���擾
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nFrame);
							}

							if (strcmp(cTemp, "NUM_FRAME") == 0)
							{// �Đ��t���[�����擾
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fNumFrame);

								m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fTimer = FLT_MAX;
							}

							if (strcmp(cTemp, "RADIUS") == 0)
							{// ���a
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%f", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].fRadius);
							}

							if (strcmp(cTemp, "POS") == 0)
							{//�ʒu�ǂݍ���
								D3DXVECTOR3 pos;

								(void)fscanf(pFile, "%s", &cTemp[0]);

								for (int nCntPos = 0; nCntPos < 3; nCntPos++)
								{
									(void)fscanf(pFile, "%f", &pos[nCntPos]);
								}

								m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].offset = pos;
							}

							if (strcmp(cTemp, "PARENT") == 0)
							{// �e�p�[�c�ԍ��擾
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[m_nNumMotion].pEvent[nCntEvent].nIdxParent);
							}
						}

						nCntEvent++;
					}

					if (strcmp(cTemp, "KEYSET") == 0)
					{//�L�[�X�^�[�g
						while (strcmp(cTemp, "END_KEYSET") != 0)
						{//�I���܂ŃL�[�ݒ�

							(void)fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "FRAME") == 0)
							{//�t���[�����擾
								(void)fscanf(pFile, "%s", &cTemp[0]);

								(void)fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
							}

							if (strcmp(cTemp, "KEY") == 0)
							{//�t���[�����擾
								while (strcmp(cTemp, "END_KEY") != 0)
								{//�I���܂ŃL�[�ݒ�
									(void)fscanf(pFile, "%s", &cTemp[0]);

									if (strcmp(cTemp, "POS") == 0)
									{//�ʒu�擾
										(void)fscanf(pFile, "%s", &cTemp[0]);

										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosX);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosY);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fPosZ);
									}

									if (strcmp(cTemp, "ROT") == 0)
									{//�����擾
										(void)fscanf(pFile, "%s", &cTemp[0]);

										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotX);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotY);
										(void)fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPart].fRotZ);
									}
								}

								nCntPart++;

							}//KEYif
						}//ENDKEYwhile

						nCntKey++;
						nCntPart = 0;
					}
				}//MOTIONwhile

				nCntMotion++;
				m_nNumMotion++;
			}
			//===========================================================

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{
				break;
			}
		}//while��

		fclose(pFile);
	}
	else
	{
		assert(("���[�V�����f�[�^�ǂݍ��݂Ɏ��s", false));
	}
}

//=====================================================
// �p�[�c�̍ő唼�a�擾
//=====================================================
float CMotion::GetRadiusMax(void)
{
	float fRadiusMax = 0.0f;
	float fTemp;

	for (int nCntParts = 0;nCntParts < m_nNumParts;nCntParts++)
	{
		if (m_apParts[nCntParts] != nullptr)
		{// �p�[�c�̔��a�擾
			fTemp = m_apParts[nCntParts]->pParts->GetRadius();

			if (fTemp > fRadiusMax)
			{// �ő唼�a�ݒ�
				fRadiusMax = fTemp;
			}
		}
	}

	return fRadiusMax;
}

//=====================================================
// �p�[�c�̐�Έʒu�擾
//=====================================================
D3DXVECTOR3 CMotion::GetMtxPos(int nIdx)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (nIdx < 0 && 
		nIdx >= m_nNumParts)
	{// �͈͊O����
		return pos;
	}

	if (m_apParts[nIdx] != nullptr)
	{
		if (m_apParts[nIdx]->pParts != nullptr)
		{
			D3DXMATRIX mtx = m_apParts[nIdx]->pParts->GetMatrix();

			pos = 
			{
				mtx._41,
				mtx._42,
				mtx._43,
			};
		}
	}

	return pos;
}

//=====================================================
// �c���̐ݒ�
//=====================================================
void CMotion::SetAfterImage(D3DXCOLOR col, int m_nLife)
{
	for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
	{
		if (m_apParts[nCntParts] != nullptr)
		{// �c���ݒ�
			D3DXMATRIX *pMtx = &m_apParts[nCntParts]->pParts->GetMatrix();
			//CModel::Model *model = m_apParts[nCntParts]->pParts->GetModel();

			CAfterImage::Create(m_apParts[nCntParts]->pParts->GetMatrix(), m_apParts[nCntParts]->pParts->GetIdxModel(),col,m_nLife);
		}
	}
}

//=====================================================
// �S�p�[�c�̐F�ݒ�
//=====================================================
void CMotion::SetAllCol(D3DXCOLOR col)
{
	m_col = col;

	for (int i = 0; i < m_nNumParts; i++)
	{
		if (m_apParts[i] != nullptr)
		{
			if (m_apParts[i]->pParts != nullptr)
			{
				m_apParts[i]->pParts->SetEmissiveCol(col);
			}
		}
	}
}

//=====================================================
// �S�p�[�c�̐F���Z�b�g
//=====================================================
void CMotion::ResetAllCol(void)
{
	for (int i = 0; i < m_nNumParts; i++)
	{
		if (m_apParts[i] != nullptr)
		{
			if (m_apParts[i]->pParts != nullptr)
			{
				m_apParts[i]->pParts->ResetColor();
			}
		}
	}
}

//=====================================================
// �p�[�c���ƂɃ��[�V�������邩�ݒ�
//=====================================================
void CMotion::EnableMotion(int nIdx,bool bMotion)
{
	if (nIdx >= 0 &&
		nIdx < MAX_PARTS)
	{
		m_abMotion[nIdx] = bMotion;
	}
	else
	{
		assert(("�s���Ȕԍ��̉��p�[�c��ݒ肵�悤�Ƃ��Ă��܂�", false));
	}
}

//=====================================================
// ���p�[�c�̃��Z�b�g
//=====================================================
void CMotion::ResetEnableMotion(void)
{
	for (int i = 0; i < MAX_PARTS; i++)
	{
		m_abMotion[i] = true;
	}
}

//=====================================================
// �O���x�N�g���̎擾
//=====================================================
D3DXVECTOR3 CMotion::GetForward(void)
{
	D3DXVECTOR3 vecForward;

	vecForward =
	{
		GetMatrix()._31,
		GetMatrix()._32,
		GetMatrix()._33,
	};

	return vecForward;
}