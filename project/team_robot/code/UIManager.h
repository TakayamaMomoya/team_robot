//*****************************************************
//
// UI�}�l�[�W���[[UIManager.h]
// Author:���R����
//
//*****************************************************

#ifndef _UIMANAGER_H_
#define _UIMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CUI;

//*****************************************************
// �萔��`
//*****************************************************

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CUIManager : public CObject
{
public:

	CUIManager();	// �R���X�g���N�^
	~CUIManager();	// �f�X�g���N�^

	static CUIManager *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CUIManager *GetInstance(void) { return m_pUIManager; }
	bool IsDisp(void) { return m_bDisp; }
	void EnableDisp(bool bDisp) { m_bDisp = bDisp; }

private:
	float m_fCntFrame;	// �t���[���̃J�E���^�[
	bool m_bDisp;	// �\�����邩�ǂ���
	static CUIManager *m_pUIManager;	// ���g�̃|�C���^
};

#endif
