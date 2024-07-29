//*****************************************************
//
// �X�J�C�{�b�N�X�̏���[skybox.h]
// Author:���R����
//
//*****************************************************

#ifndef _SKYBOX_H_
#define _SKYBOX_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "objectX.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSkybox : public CObjectX
{
public:
	CSkybox(int nPriority = 2);	// �R���X�g���N�^
	~CSkybox();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSkybox *Create();
	static CSkybox *GetInstance();	// �C���X�^���X�̎擾

private:
	// �����o�֐�
	void FollowPlayer(void);	// �v���C���[�̒Ǐ]
	void DetectionChangeTexture(void);	// �e�N�X�`���ύX�̌��m

	static CSkybox *m_pSkybox;	// ���g�̃|�C���^
};

#endif

