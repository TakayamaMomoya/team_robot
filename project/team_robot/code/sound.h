//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ���R����
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CSound
{
public:
	typedef enum
	{
		LABEL_BGM_TITLE,					//�^�C�g��BGM
		LABEL_BGM_GAME00,					// �Q�[��BGM00
		LABEL_BGM_GAME01,					// �Q�[��BGM01
		LABEL_BGM_GAME02,					// �Q�[��BGM02
		LABEL_BGM_GAME03,					// �Q�[��BGM03
		LABEL_BGM_GAME04,					// �Q�[��BGM04
		LABEL_BGM_GAME05,					// �Q�[��BGM05
		LABEL_BGM_GAME06,					// �Q�[��BGM06
		LABEL_BGM_GAME07,					// �Q�[��BGM07
		LABEL_BGM_GAME08,					// �Q�[��BGM08
		LABEL_BGM_GAME09,					// �Q�[��BGM09
		LABEL_BGM_GAME10,					// �Q�[��BGM10
		LABEL_BGM_GAME11,					// �Q�[��BGM11
		LABEL_BGM_GAME12,					// �Q�[��BGM12
		LABEL_BGM_GAME13,					// �Q�[��BGM13
		LABEL_BGM_GAME14,					// �Q�[��BGM14
		LABEL_BGM_GAME15,					// �Q�[��BGM15
		LABEL_BGM_GAME16,					// �Q�[��BGM16
		LABEL_BGM_GAME17,					// �Q�[��BGM17
		LABEL_BGM_GAME18,					// �Q�[��BGM18

		LABEL_SE_ENGIN,					// �G���W����
		LABEL_SE_ACCELERATOR,			// �A�N�Z����
		LABEL_SE_DRIFT,                 // �h���t�g��

		LABEL_SE_PAUSE_MENU,			// �|�[�Y
		LABEL_SE_PAUSE_ARROW,			// �|�[�Y�ړ�
		LABEL_SE_PAUSE_ENTER,			// �|�[�Y�I��
		LABEL_MAX
	} LABEL;
	typedef enum
	{
		FADESTATE_NONE = 0,	//�������Ă��Ȃ����
		FADESTATE_IN,	//�t�F�[�h�C�����
		FADESTATE_OUT,	//�t�F�[�h�A�E�g���
		FADESTATE_MAX
	}FADESTATE;

	CSound();	// �R���X�g���N�^
	~CSound();	// �f�X�g���N�^

	static CSound *Create(HWND hWnd);
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	void Update(void);

	HRESULT Play(LABEL label);
	void Stop(LABEL label);
	void Stop(void);

	void SetVolume(LABEL label, float fVolume);
	void SetFade(LABEL LabelNow, LABEL LabelNext, float fSpeed);

	static CSound *GetInstance(void) { return m_pSound; }

private:
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDINFO;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX];	// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX];	// �I�[�f�B�I�f�[�^�T�C�Y
	FADESTATE m_fadeSound;	// �t�F�[�h���
	LABEL m_SoundLabelNow;	// ���݂̃T�E���h���x��
	LABEL m_SoundLabelNext;	// ���̃T�E���h���x��
	float m_fSpeedFadeSound;	// �T�E���h���t�F�[�h����X�s�[�h
	float m_fVolumeSound;	// �T�E���h�̃{�����[��

	// �T�E���h�̏��
	SOUNDINFO m_aSoundInfo[LABEL_MAX] =
	{
		{ "data/SOUND/BGM/BGM_Game00.wav",-1 },			// �^�C�g�����BGM

		{ "data/SOUND/BGM/BGM_Game00.wav",-1 },			// �Q�[�����BGM00
		{ "data/SOUND/BGM/BGM_Game01.wav",-1 },			// �Q�[�����BGM01
		{ "data/SOUND/BGM/BGM_Game02.wav",-1 },			// �Q�[�����BGM02
		{ "data/SOUND/BGM/BGM_Game03.wav",-1 },			// �Q�[�����BGM03
		{ "data/SOUND/BGM/BGM_Game04.wav",-1 },			// �Q�[�����BGM04
		{ "data/SOUND/BGM/BGM_Game05.wav",-1 },			// �Q�[�����BGM05
		{ "data/SOUND/BGM/BGM_Game06.wav",-1 },			// �Q�[�����BGM06
		{ "data/SOUND/BGM/BGM_Game07.wav",-1 },			// �Q�[�����BGM07
		{ "data/SOUND/BGM/BGM_Game08.wav",-1 },			// �Q�[�����BGM08
		{ "data/SOUND/BGM/BGM_Game09.wav",-1 },			// �Q�[�����BGM09
		{ "data/SOUND/BGM/BGM_Game10.wav",-1 },			// �Q�[�����BGM10
		{ "data/SOUND/BGM/BGM_Game11.wav",-1 },			// �Q�[�����BGM11
		{ "data/SOUND/BGM/BGM_Game12.wav",-1 },			// �Q�[�����BGM12
		{ "data/SOUND/BGM/BGM_Game13.wav",-1 },			// �Q�[�����BGM13
		{ "data/SOUND/BGM/BGM_Game14.wav",-1 },			// �Q�[�����BGM14
		{ "data/SOUND/BGM/BGM_Game15.wav",-1 },			// �Q�[�����BGM15
		{ "data/SOUND/BGM/BGM_Game16.wav",-1 },			// �Q�[�����BGM16
		{ "data/SOUND/BGM/BGM_Game17.wav",-1 },			// �Q�[�����BGM17
		{ "data/SOUND/BGM/BGM_Game18.wav",-1 },			// �Q�[�����BGM18

		{ "data/SOUND/SE/SE_Engin.wav",-1 },			// �G���W����
		{ "data/SOUND/SE/SE_Accelerator.wav",-1 },		// �A�N�Z����
		{ "data/SOUND/SE/SE_Drift.wav",-1 },		    // �h���t�g��

		{ "data/SOUND/SE/pause.wav",0 },				// �|�[�Y
		{ "data/SOUND/SE/pause_arrow.wav",0 },			// �I��
		{ "data/SOUND/SE/pause_enter.wav",0 },			// ����
	};

	static CSound *m_pSound;	// ���g�̃|�C���^
};

namespace Sound
{
void Play(CSound::LABEL label);
}

#endif