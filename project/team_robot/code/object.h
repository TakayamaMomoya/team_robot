//*****************************************************
//
// �I�u�W�F�N�g�̏���[object.h]
// Author:���R����
//
//*****************************************************

#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPlayer;
class CBlock;

//*****************************************************
// �}�N����`
//*****************************************************
#define NUM_OBJECT	(4096)	//	�I�u�W�F�N�g�̐�
#define NUM_PRIORITY	(8)	// �D�揇�ʂ̍ő吔

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CObject
{
public:
	typedef enum
	{
		TYPE_NONE = 0,	// ���ł��Ȃ�
		TYPE_PLAYER,	// �v���C���[
		TYPE_ENEMY,	// �G
		TYPE_BLOCK,	// �u���b�N
		TYPE_BULLET,	// �e
		TYPE_PARTICLE,	// �p�[�e�B�N��
		TYPE_MAX
	}TYPE;

	CObject(int nPriority = 4);	// �R���X�g���N�^
	virtual ~CObject();	//	�f�X�g���N�^

	virtual HRESULT Init(void) = 0;	// ����������
	virtual void Uninit(void) = 0;	// �I������
	virtual void Update(void) = 0;	// �X�V����
	virtual void Draw(void) {};	// �`�揈��
	static void ReleaseAll(void);	// �S�ă����[�X
	static void UpdateAll(void);	// �S�čX�V
	static void UpdateNotStop(void);	// �����Ȃ��I�u�W�F�N�g�̍X�V
	static void DeleteAll(void);	// �S�폜����
	static void DrawAll(void);	// �S�ĕ`��
	static void DrawObject(bool bBlur);	// �I�u�W�F�N�g�̕`��
	virtual void Hit(float fDamage){}	// �q�b�g����
	void SetType(TYPE type);
	TYPE GetType(void) { return m_type; }
	virtual void SetPosition(D3DXVECTOR3 pos) {};	// �ݒ菈��
	virtual D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }	// �ʒu�擾����
	virtual D3DXVECTOR3 GetPositionOld(void) { return D3DXVECTOR3(); }	// �ʒu�擾����
	virtual float GetWidth(void) { return 0.0f; };	// �T�C�Y�擾
	virtual float GetHeight(void) { return 0.0f; }	// �T�C�Y�擾
	void EnableWire(bool bWire) { m_bWire = bWire; }
	static int GetNumAll(void) { return m_nNumAll; }
	bool IsDeath(void) { return m_bDeath; }
	void EnableZtest(bool bZtest) { m_bZtest = bZtest; }
	void EnableLighting(bool bLighting) { m_bLighting = bLighting; }
	void EnableAdd(bool bAdd) { m_bAdd = bAdd; }
	void EnableNotStop(bool bNotStop);
	void EnableFog(bool bFog) { m_bFog = bFog; }
	void EnableCull(bool bCull) { m_bCull = bCull; }
	void SetAlphaTest(DWORD dValue) { m_dAlpha = dValue; }
	DWORD GetAlpha(void) { return m_dAlpha; }
	void EnableBlur(bool bBlur) { m_bBlur = bBlur; }

protected:
	void Release(void);	// �ʃ����[�X����

private:
	void Delete(void);	// �ʍ폜����

	static int m_nNumAll;	// ����
	int m_nID;	// ������ID
	TYPE m_type;	// ���
	int m_nPriority;	// �`��̗D�揇��
	static CObject *m_apTop[NUM_PRIORITY];	// �擪�̃I�u�W�F�N�g�̃A�h���X
	static CObject *m_apCur[NUM_PRIORITY];	// �Ō���̃I�u�W�F�N�g�̃A�h���X
	CObject *m_pPrev;	// �O�̃I�u�W�F�N�g�̃A�h���X
	CObject *m_pNext;	// ���̃I�u�W�F�N�g�̃A�h���X
	bool m_bDeath;	// ���S�t���O
	bool m_bWire;	// ���C���[�t���[���ŕ\�����邩�ǂ���
	bool m_bZtest;	// Z�e�X�g�őO�ɏo�����ǂ���
	bool m_bLighting;	// ���C�e�B���O��L�������邩�ǂ���
	bool m_bNotStop;	// �~�܂�Ȃ��I�u�W�F�N�g���ǂ���
	bool m_bAdd;	// ���Z�������邩�ǂ���
	bool m_bFog;	// �t�H�O�������邩�ǂ���
	bool m_bCull;	// �J�����O���邩�ǂ���
	DWORD m_dAlpha;	// �A���t�@�e�X�g�̒l
	bool m_bBlur;	// �u���[�������邩�ǂ���
	static CObject *m_apNotStop[NUM_OBJECT];	// ��~���ɂ������I�u�W�F�N�g�̔z��
};

namespace Object
{
void DeleteObject(CObject **ppObject, int nSize = 1);
}


#endif