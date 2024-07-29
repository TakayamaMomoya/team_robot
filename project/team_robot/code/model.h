//*****************************************************
//
// X�t�@�C�����f���̏���[model.h]
// Author:���R����
//
//*****************************************************

#ifndef _MODEL_H_
#define _MODEL_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define NUM_MODEL	(256)	// ���f���̐�

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CModel
{
public:
	typedef struct
	{// ���f���̍\����
		int *pIdxTexture;	// �e�N�X�`���ԍ��ւ̃|�C���^
		LPD3DXMESH pMesh;		//���b�V���ւ̃|�C���^
		LPD3DXBUFFER pBuffMat;	//�}�e���A���ւ̃o�b�t�@
		DWORD dwNumMat;	//�}�e���A���̐�
	}Model;

	CModel();	// �R���X�g���N�^
	~CModel();	// �f�X�g���N�^
	
	static int Load(char *pPath);
	static void Unload(void);
	static void Init(int nIdx);
	static Model *GetModel(int nIdx) { return m_apModel[nIdx]; }
	static int GetNumAll(void) { return m_nNumAll; }
	static char *GetPath(int nIdx) { return &m_aPath[nIdx][0]; }

private:
	static Model *m_apModel[NUM_MODEL];	// ���f���ւ̃|�C���^
	static char m_aPath[NUM_MODEL][256];	// ���f���̃t�@�C����
	static int m_nNumAll;	// ����
};

#endif