//========================================
//
//�v���R���p�C��[precompile.h]
//Author�F�X��x��
//
//========================================
#ifndef PRECOMPILE_H__
#define PRECOMPILE_H__

//========================================
// ���O�C���N���[�h
//========================================
#include <windows.h>
#include"d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)						//�r���h���̌x���Ώ��p�}�N��
#include "dinput.h"
#include "Xinput.h"
#include "xaudio2.h"
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <time.h>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <locale>
#include <codecvt>
#include <imgui.h>
#include <imconfig.h>
#include <imgui_impl_win32.h>
#include <backends/imgui_impl_dx9.h>

//========================================
// Using�錾
//========================================
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::vector;
using std::list;
using std::map;

#endif // _PRECOMPILE_H_