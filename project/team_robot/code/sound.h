//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 高山桃也
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CSound
{
public:
	typedef enum
	{
		LABEL_BGM_TITLE,					//タイトルBGM
		LABEL_BGM_GAME00,					// ゲームBGM00
		LABEL_BGM_GAME01,					// ゲームBGM01
		LABEL_BGM_GAME02,					// ゲームBGM02
		LABEL_BGM_GAME03,					// ゲームBGM03
		LABEL_BGM_GAME04,					// ゲームBGM04
		LABEL_BGM_GAME05,					// ゲームBGM05
		LABEL_BGM_GAME06,					// ゲームBGM06
		LABEL_BGM_GAME07,					// ゲームBGM07
		LABEL_BGM_GAME08,					// ゲームBGM08
		LABEL_BGM_GAME09,					// ゲームBGM09
		LABEL_BGM_GAME10,					// ゲームBGM10
		LABEL_BGM_GAME11,					// ゲームBGM11
		LABEL_BGM_GAME12,					// ゲームBGM12
		LABEL_BGM_GAME13,					// ゲームBGM13
		LABEL_BGM_GAME14,					// ゲームBGM14
		LABEL_BGM_GAME15,					// ゲームBGM15
		LABEL_BGM_GAME16,					// ゲームBGM16
		LABEL_BGM_GAME17,					// ゲームBGM17
		LABEL_BGM_GAME18,					// ゲームBGM18

		LABEL_SE_ENGIN,					// エンジン音
		LABEL_SE_ACCELERATOR,			// アクセル音
		LABEL_SE_DRIFT,                 // ドリフト音

		LABEL_SE_PAUSE_MENU,			// ポーズ
		LABEL_SE_PAUSE_ARROW,			// ポーズ移動
		LABEL_SE_PAUSE_ENTER,			// ポーズ選択
		LABEL_MAX
	} LABEL;
	typedef enum
	{
		FADESTATE_NONE = 0,	//何もしていない状態
		FADESTATE_IN,	//フェードイン状態
		FADESTATE_OUT,	//フェードアウト状態
		FADESTATE_MAX
	}FADESTATE;

	CSound();	// コンストラクタ
	~CSound();	// デストラクタ

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
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDINFO;

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX];	// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];	// オーディオデータサイズ
	FADESTATE m_fadeSound;	// フェード状態
	LABEL m_SoundLabelNow;	// 現在のサウンドラベル
	LABEL m_SoundLabelNext;	// 次のサウンドラベル
	float m_fSpeedFadeSound;	// サウンドがフェードするスピード
	float m_fVolumeSound;	// サウンドのボリューム

	// サウンドの情報
	SOUNDINFO m_aSoundInfo[LABEL_MAX] =
	{
		{ "data/SOUND/BGM/BGM_Game00.wav",-1 },			// タイトル画面BGM

		{ "data/SOUND/BGM/BGM_Game00.wav",-1 },			// ゲーム画面BGM00
		{ "data/SOUND/BGM/BGM_Game01.wav",-1 },			// ゲーム画面BGM01
		{ "data/SOUND/BGM/BGM_Game02.wav",-1 },			// ゲーム画面BGM02
		{ "data/SOUND/BGM/BGM_Game03.wav",-1 },			// ゲーム画面BGM03
		{ "data/SOUND/BGM/BGM_Game04.wav",-1 },			// ゲーム画面BGM04
		{ "data/SOUND/BGM/BGM_Game05.wav",-1 },			// ゲーム画面BGM05
		{ "data/SOUND/BGM/BGM_Game06.wav",-1 },			// ゲーム画面BGM06
		{ "data/SOUND/BGM/BGM_Game07.wav",-1 },			// ゲーム画面BGM07
		{ "data/SOUND/BGM/BGM_Game08.wav",-1 },			// ゲーム画面BGM08
		{ "data/SOUND/BGM/BGM_Game09.wav",-1 },			// ゲーム画面BGM09
		{ "data/SOUND/BGM/BGM_Game10.wav",-1 },			// ゲーム画面BGM10
		{ "data/SOUND/BGM/BGM_Game11.wav",-1 },			// ゲーム画面BGM11
		{ "data/SOUND/BGM/BGM_Game12.wav",-1 },			// ゲーム画面BGM12
		{ "data/SOUND/BGM/BGM_Game13.wav",-1 },			// ゲーム画面BGM13
		{ "data/SOUND/BGM/BGM_Game14.wav",-1 },			// ゲーム画面BGM14
		{ "data/SOUND/BGM/BGM_Game15.wav",-1 },			// ゲーム画面BGM15
		{ "data/SOUND/BGM/BGM_Game16.wav",-1 },			// ゲーム画面BGM16
		{ "data/SOUND/BGM/BGM_Game17.wav",-1 },			// ゲーム画面BGM17
		{ "data/SOUND/BGM/BGM_Game18.wav",-1 },			// ゲーム画面BGM18

		{ "data/SOUND/SE/SE_Engin.wav",-1 },			// エンジン音
		{ "data/SOUND/SE/SE_Accelerator.wav",-1 },		// アクセル音
		{ "data/SOUND/SE/SE_Drift.wav",-1 },		    // ドリフト音

		{ "data/SOUND/SE/pause.wav",0 },				// ポーズ
		{ "data/SOUND/SE/pause_arrow.wav",0 },			// 選択
		{ "data/SOUND/SE/pause_enter.wav",0 },			// 決定
	};

	static CSound *m_pSound;	// 自身のポインタ
};

namespace Sound
{
void Play(CSound::LABEL label);
}

#endif