#pragma once

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CSoundMgr : public CBase
{
private:
	CSoundMgr();
	virtual ~CSoundMgr() = default;	
private:
    // ���� ���ҽ� ������ ���� ��ü 
    map<const _tchar*, FMOD::Sound*> m_mapSound;    

    // FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
    FMOD::Channel* m_pChannelArr[MAXCHANNEL];
    // ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
    FMOD::System* m_pSystem = { nullptr };
    _tchar* m_CurBGM;

public:
	HRESULT Initialize();	    
	void Release();	    


public:
    void Play_Sound(const _tchar* pSoundKey, CHANNELID eID, float fVolume); 
    void PlayBGM(const _tchar* pSoundKey, float fVolume);
    void StopSound(CHANNELID eID);
    void StopAll();
    void SetChannelVolume(CHANNELID eID, float fVolume);
    bool CheckSoundPlaying(CHANNELID eID);
    _tchar* Get_CurBGM() { return m_CurBGM; };

private:
    void LoadSoundFile();

private:
    bool m_isPlaying = { false };

public:
    static CSoundMgr* Create(); 
    virtual void Free();

};

END
