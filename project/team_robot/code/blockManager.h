//*****************************************************
//
// �u���b�N�Ǘ�[blockManager.h]
// Author:���R����
//
//*****************************************************

#ifndef _BLOCKMANAGER_H_
#define _BLOCKMANAGER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "block.h"
#include <list>

//*****************************************************
// �O���錾
//*****************************************************
class CModel;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CBlockManager : public CObject
{
public:
	struct SInfoBlock
	{// �u���b�N�̏��
		char aTag[256];	// �^�O
		int nIdxModel;	// ���f���ԍ�
		bool bSnag;	// ��Q���ɂȂ邩�ǂ���
	};

	CBlockManager();	// �R���X�g���N�^
	~CBlockManager();	// �f�X�g���N�^

	static CBlockManager *Create(void);
	static CBlockManager *GetInstance(void) { return m_pBlockManager; }
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	int GetNumBlock(void) { return m_nNumInfoBlock; }
	SInfoBlock *GetInfoBlock(void) { return m_pInfoBlock; }
	void Save(char *pPath);
	void DeleteAll(void);
	std::list<CBlockGrab*> *GetListGrab(void) { return &m_listGrab; }
	void AddGrabList(CBlockGrab *pBlock);
	void RemoveGrabList(CBlockGrab *pBlock);
	CBlock *GetHead(void) { return m_pHead; }
	CBlock *GetTail(void) { return m_pTail; }
	void SetHead(CBlock *pBlock) { m_pHead = pBlock; }
	void SetTail(CBlock *pBlock) { m_pTail = pBlock; }

private:
	void Load(void);
	void LoadMap(FILE *pFile,char *pTemp,CBlock *pBlock);

	SInfoBlock *m_pInfoBlock;	// �u���b�N���̃|�C���^
	int m_nNumInfoBlock;	// �u���b�N���̐�

	CBlock *m_pHead;	// �擪�̃A�h���X
	CBlock *m_pTail;	// �Ō���̃A�h���X
	std::list<CBlockGrab*> m_listGrab;	// �͂߂�u���b�N�̃��X�g
	static CBlockManager *m_pBlockManager;	// ���g�̃|�C���^
};

namespace BlockManager
{
CBlockManager *GetInstance(void);
}

#endif