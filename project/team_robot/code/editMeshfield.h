//*****************************************************
//
// ���b�V���t�B�[���h�G�f�B�^�[�̏���[editMeshfield.h]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "edit.h"

//*****************************************************
// �O���錾
//*****************************************************
class CMeshCylinder;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEditMeshfield : public CEdit
{
public:
	CEditMeshfield();	// �R���X�g���N�^
	~CEditMeshfield();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;

private:
	// �����o�֐�
	void MoveCursor(void);	// �J�[�\���𓮂�������
	void DentMeshField(void);	// ���b�V���t�B�[���h���ւ��܂�����

	// �����o�ϐ�
	float m_fDepthDent;	// 1�t���[���Ԃɂւ��܂��[��
	float m_fRadiusDent;	// �ւ��܂��͈�
	CMeshCylinder *m_pMeshCylinder;	// �͈͕\���̃��b�V���V�����_�[
};