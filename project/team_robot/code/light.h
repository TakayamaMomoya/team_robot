//*****************************************************
//
// ���C�g�̏���[light.h]
// Author:���R����
//
//*****************************************************

#ifndef _LIGHT_H_
#define _LIGHT_H_

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_LIGHT				(3)			//���C�g�̍ő吔

//*****************************************************
// �N���X��`
//*****************************************************
class CLight
{
public:
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetColDest(int nIdx, D3DXCOLOR col);
	void ResetColDest(void);

private:
	struct SInfo
	{
		D3DXCOLOR col;	// ���݂̐F
		D3DXCOLOR colDest;	// �ڕW�̐F
	};

	D3DLIGHT9 m_aLight[MAX_LIGHT];
	SInfo m_aInfo[MAX_LIGHT];
};

#endif