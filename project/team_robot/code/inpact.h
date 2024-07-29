//*****************************************************
//
// �Ռ��g�̏���[inpact.h]
// Author:���R����
//
//*****************************************************

#ifndef _INPACT_H_
#define _INPACT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "meshcylinder.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CInpact : public CMeshCylinder
{
public:
	CInpact(int nPriority = 5);	// �R���X�g���N�^
	~CInpact();	// �f�X�g���N�^
	static CInpact *Create(float nLife = 0.4f,D3DXMATRIX *pMtx = nullptr);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetSpeedExpand(float fSpeed) { m_info.fSpeedExpand = fSpeed; }

private:
	struct SInfo
	{
		float fRadiusDiff;	// ���a�ɂǂꂾ���������邩
		float fLife;	// ����
		float fLifeInitial;	// ��������
		float fSpeedExpand;	// �c��ޑ��x
	};

	void SetVtx(void);
	void ManageLife(void);

	SInfo m_info;
};
#endif