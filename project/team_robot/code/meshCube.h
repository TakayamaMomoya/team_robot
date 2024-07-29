//*****************************************************
//
// ���b�V���L���[�u[meshCube.h]
// Author:���R����
//
//*****************************************************
#ifndef _MESHCUBE_H_
#define _MESHCUBE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "gameObject.h"

//*****************************************************
// �O����`
//*****************************************************
class CPolygon3D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CMeshCube : public CGameObject
{
public:
	CMeshCube(int nPriority = 6);	// �R���X�g���N�^
	~CMeshCube();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);	// ����������
	void Uninit(void);	// �I������
	void Update(void);	// �X�V����
	void Draw(void);	// �`�揈��

	// �ÓI�����o�֐�
	static CMeshCube *Create(void);	// ��������

private:
	// �����o�ϐ�
	D3DXVECTOR3 m_size;	// �T�C�Y
	std::vector<CPolygon3D*> m_apPolygon3D;	// 3D�|���S���̃|�C���^
};

#endif