#include "pch.h"
#include "Monster.h"
#include "GameInstance.h"
#include "Player.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CMonster::CMonster(const CMonster& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pPlayer = m_pGameInstance->Get_Player_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player"));
	
	


	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pPlayer);

	if (pPlayer->Get_MonsterActivateOnOff())
	{
		m_bMonsterActive = true;
	}

}

void CMonster::Update(_float fTimeDelta)
{
	
	//m_pModelCom->Play_Animation(fTimeDelta);
}

void CMonster::Late_Update(_float fTimeDelta)
{
	//m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);	
}

HRESULT CMonster::Render()
{
	return S_OK;
}


void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);	

}
