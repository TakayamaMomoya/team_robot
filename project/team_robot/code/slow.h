//*****************************************************
//
// �X���[�̏���[slow.h]
// Author:���R����
//
//*****************************************************

#ifndef _SLOW_H_
#define _SLOW_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

namespace Slow
{
float GetScale(void);
}

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSlow : public CObject
{
public:
	CSlow();	// �R���X�g���N�^
	~CSlow();	// �f�X�g���N�^

	static CSlow *Create(void);
	static CSlow *GetInstance(void) { return m_pSlow; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	float GetScale(void) { return m_fScale; }
	void SetScale(float fScale);
	void SetSlowTime(float fTime, float fScale);

private:
	float m_fScale;	// �X���[�̃X�P�[��
	float m_fTimeSlow;	// �X���[�ɂ��鎞��

	static CSlow *m_pSlow;	// ���g�̃|�C���^
};

#endif