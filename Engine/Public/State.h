#pragma once
#include "Engine_Defines.h"	
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CState abstract : public CBase
{
protected:
	CState() = default;		
	virtual ~CState() = default; 

private:
	_uint		m_iState = { 0 };		

public:
	void        ChangeState(_uint _State);	

protected: 
	virtual void State_Enter() = 0;	 // ������ ������Ʈ���� �˱� ���� ���� ������Ʈ �־���ҵ�
	virtual void State_Update() = 0;	
	virtual void State_Exit() = 0;

public:
	virtual void Free();
};
END