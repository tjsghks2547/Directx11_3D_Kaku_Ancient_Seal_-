#pragma once
#include "State.h"

class CIdle final : public CState
{
private:
	CIdle();
	virtual ~CIdle();

public:
	HRESULT Initialize();

public:
	virtual void State_Enter();	 // 누구의 스테이트인지 알기 위해 게임 오브젝트 넣어야할듯
	virtual void State_Update();
	virtual void State_Exit();

public:
	static CIdle* Create();
	virtual void  Free() override;	
	
};

