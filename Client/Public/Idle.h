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
	virtual void State_Enter();	 // ������ ������Ʈ���� �˱� ���� ���� ������Ʈ �־���ҵ�
	virtual void State_Update();
	virtual void State_Exit();

public:
	static CIdle* Create();
	virtual void  Free() override;	
	
};

