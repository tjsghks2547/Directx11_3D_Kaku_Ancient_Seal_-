#include "pch.h" 
#include "..\Public\Loader.h"
#include "Body_Player.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "BackGround.h"
#include "Terrain.h"
#include "Monster.h"
#include "ForkLift.h"
#include "Weapon.h"
#include "Player.h"
#include "Sky.h"	
#include "Water.h"

#pragma region Temple 관련 오브젝트 
#include "SC_Elemental_Boss_02.h"	
#include "Temple_Ground.h"
#include "AlterBase.h"
#include "SM_TempleBrick_04.h"
#include "TempleBlcok.h"
#include "SC_Steps_A_Mod.h"
#include "SC_Corner_D_Mod.h"
#include "M_GuardianP_POSE_A_SC_Rock_01.h"
#include "M_GuardianP_POSE_A_SC_Rock_02.h"
#include "M_GuardianP_POSE_A_SC_Rock_01_Weapon.h"	
#include "SC_Brick_v1.h"	
#include "SM_TempleBrick_10.h"	
#include "SM_Brick_W_B_Mod2.h"		
#include "M_Guardian_Model_C.h"	
#include "SM_LotusFlower_07a1.h"
#include "ST_LiftTable_a2_sui_00.h"
#include "SM_TempleBrick_01.h"	
#include "SC_Brick_W_B_Mod.h"
#include "SC_Brick_W_B_Mod_10.h"
#include "M_Guardian_Model_D.h"	
#include "SM_Templelamp05.h"	
#include "SM_Templelamp06.h"
#include "SC_FlapTrap_03.h"
#include "TurnTable_Body.h"
#include "TurnTable_Head.h"
#include "SC_Stone_HeavyBOX_A_Mod.h"
#include "Box_Lift.h"
#include "LeverButton.h"
#include "SC_TempleDoor_05.h"
#include "SM_TempleFloor_02.h"
#include "Moving_Floor.h"
#include "M_Guardian_Model_A.h"
#include "SC_Object_StoneBall.h"
#include "SC_StepBox.h"
#include "SC_TotemB.h"
#include "SM_Pillar_01.h"
#include "Wind_EleTower_09.h"
#include "Wind_Elementak_Boss.h"
#include "SC_Elemental_Boss_05.h"
#include "SC_Elemental_Boss_04.h"
#include "SC_RuneStatue.h"
#include "SC_WaterTemple_Parts_b.h"
#include "ItemBag.h"
#include "S_HpFlower.h"
#include "SM_MushroomA_L.h"
#pragma endregion 

#pragma region 인스턴싱 관련
#include "Particle_Snow.h"
#include "Particle_Explosion.h"
#include "HitParticle.h"
#include "ItemGainParticle.h"
#include "FireBreath.h"
#include "BulletGainEffectParticle.h"
#pragma endregion 


#pragma region UI 관련 
#include "HpBar.h"
#include "Stamina_Bar.h"
#include "Button_1.h"
#include "Button_2.h"
#include "Button_3.h"
#include "Button_4.h"
#include "S_HpFlower_Ui.h"
#include "SM_MushroomA_L_Ui.h"
#include "QuestUI.h"
#include "QuestMaskUi.h"	
#include "BossMaskUi.h"
#include "BossHpUi.h"	
#include "FadeInOut.h"	
#pragma endregion 

#pragma region Effect 관련
#include "SwordTrail_Buffer.h"
#pragma endregion 


#pragma region NPC 관련
#include "StrongMan.h"
#pragma endregion 


#pragma region 보스 
#include "Boss.h"
#pragma endregion 




CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

_uint APIENTRY Thread_Main(void* pArg)
{
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	pLoader->Start_Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{


	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	// m_eNextLevelID가 필요로하는 자원을 로드해주기위한 스레드를 만들어낸다. 
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, 0);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_Level_GamePlay();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

void CLoader::SetUp_WindowText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Loading_For_Level_Logo()
{

#pragma region Logo 
	lstrcpyW(m_szLoadingText, TEXT("텍스쳐 원형을 생성한다."));
	/* For.Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("셰이더 원형을 생성한다."));
	//D3D11_INPUT_ELEMENT_DESC	VertexElements[] = {
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA , 0 },			
	//};

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;


	lstrcpyW(m_szLoadingText, TEXT("모델 원형을 생성한다."));
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("객체 원형을 생성한다."));

	/* For.Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;	

	/* 로딩이 완료되었습ㄴ미다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));
	m_isFinished = true;

	return S_OK;
#pragma endregion 
}

HRESULT CLoader::Loading_For_Level_GamePlay()
{
	/* 게임플레이레벨을 위한 자원을 로드합니다.  */
	lstrcpyW(m_szLoadingText, TEXT("게임플레이레벨을 위한 자원을 로딩 중입니다."));

	// 초기 설정을 위한 트랜스폼 
	_matrix PreTransformMatrix = XMMatrixIdentity();	


#pragma region CLAW_EFFECT

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_CLAW_RIGHT_EFFECT"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/T_FX_zhuahen_0002_Right.png"), 1))))
		return E_FAIL;

#pragma endregion 


#pragma region Decal_Ground_Crack_01

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Crack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/body-shock1.dds"), 1))))
		return E_FAIL;

#pragma endregion 

#pragma region FadeInOut		
	/* For.Prototype_Component_Texture_Hit_Effect0 */	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_FadeInOut"),	
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/FadeInOut.dds"), 1))))	
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FadeInOut"),
		CFadeInOut::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	

#pragma endregion 


#pragma region Bullet	
	/* For.Prototype_Component_Texture_Hit_Effect0 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Bullet_Circle"),	
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Particle_Skill_HitAG.png"), 1))))	
		return E_FAIL;
#pragma endregion 



#pragma region BossHpUi
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_BossHpUi"),
		CBossHpUi::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 
	
	
#pragma region BOSS_Ui_Mask	
	/* For.Prototype_Component_Texture_S_HpFlower_Ui */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_BossMaskUi"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/BOSS.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_BossMaskUi"),
		CBossMaskUi::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
#pragma endregion


#pragma region Quest_Ui_Mask	
	/* For.Prototype_Component_Texture_S_HpFlower_Ui */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_QuestMaskUi"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/QuestMaskUi.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_QuestMaskUI"),	
		CQuestMaskUi::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion



#pragma region Quest_Ui	
	/* For.Prototype_Component_Texture_S_HpFlower_Ui */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_QuestUi"),	
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/QuestUi.png"), 1))))	
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_QuestUI"),	
		CQuestUI::Create(m_pDevice, m_pContext))))		
		return E_FAIL;		
	
#pragma endregion



#pragma region NpcDialog_Ui
	/* For.Prototype_Component_Texture_S_HpFlower_Ui */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_NpcDialog_Ui"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NpcDialog.png"), 1))))
		return E_FAIL;
#pragma endregion


#pragma region Question_Ui
	/* For.Prototype_Component_Texture_S_HpFlower_Ui */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Question_Ui"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Question_Ui.png"), 1))))
		return E_FAIL;
#pragma endregion 

#pragma region F_UI
	/* For.Prototype_Component_Texture_S_HpFlower_Ui */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_F"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/F.png"), 1))))
		return E_FAIL;
#pragma	endregion 
	
#pragma region SM_MushroomA_L_Ui

	/* For.Prototype_Component_Texture_SM_MushroomA_L_Ui */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_SM_MushroomA_L_Ui"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/SM_MushroomA_L_Ui.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_MushroomA_L_Ui"),
		CSM_MushroomA_L_Ui::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 


#pragma region S_HpFlower_Ui

	/* For.Prototype_Component_Texture_S_HpFlower_Ui */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_S_HpFlower_Ui"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/S_HpFlower_Ui.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_S_HpFlower_Ui"),
		CS_HpFlower_Ui::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 

#pragma region NpcInterAction_UI	
	/* For.Prototype_Component_Texture_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ItemInteraction_Ui"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/E.png"), 1))))
		return E_FAIL;

#pragma endregion 

#pragma region SM_MushroomA_L
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_MushroomA_L"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_MushroomA_L/SM_MushroomA_L.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_MushroomA_L"),
		CSM_MushroomA_L::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region S_HpFlower
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_S_HpFlower"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/S_HpFlower/S_HpFlower.fbx", CModel::MODEL_NONANIM))))	
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_S_HpFlower"),		
		CS_HpFlower::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 

#pragma region Item_UI
	/* For.Prototype_Component_Texture_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Button_1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/1.png"), 1))))	
		return E_FAIL;
	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Button_1"),	
		CButton_1::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Button_2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/2.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Button_2"),
		CButton_2::Create(m_pDevice, m_pContext))))	
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Button_3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/3.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Button_3"),
		CButton_3::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Button_4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/4.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Button_4"),
		CButton_4::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_ItemBag"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/ItemBag.png"), 1))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ItemBag"),	
		CItemBag::Create(m_pDevice, m_pContext))))	
		return E_FAIL;


#pragma endregion 
#pragma region Boss 

	PreTransformMatrix = XMMatrixScaling(0.35f, 0.35f, 0.35f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Boss/Boss.fbx", CModel::MODEL_ANIM,PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss"),	
		CBoss::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	

	PreTransformMatrix = XMMatrixIdentity();	
#pragma endregion 
#pragma region SC_WaterTemple_Parts_b

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_WaterTemple_Parts_b"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_WaterTemple_Parts_b/SC_WaterTemple_Parts_b.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_WaterTemple_Parts_b"),
		CSC_WaterTemple_Parts_b::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 

#pragma region SC_RuneStatue

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_RuneStatue"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_RuneStatue/SC_RuneStatue.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_RuneStatue"),
		CSC_RuneStatue::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region SC_Elemental_Boss_04

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Elemental_Boss_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Elemental_Boss_04/SC_Elemental_Boss_04.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Elemental_Boss_04"),
		CSC_Elemental_Boss_04::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region SC_Elemental_Boss_05

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Elemental_Boss_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Elemental_Boss_05/SC_Elemental_Boss_05.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Elemental_Boss_05"),
		CSC_Elemental_Boss_05::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 


#pragma region Wind_Elementak_Boss

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wind_Elementak_Boss"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/Wind_Elementak_Boss/Wind_Elementak_Boss.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Wind_Elementak_Boss"),	
		CWind_Elementak_Boss::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	

#pragma endregion 


#pragma region Wind_EleTower_09

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wind_EleTower_09"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/Wind_EleTower_09/Wind_EleTower_09.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Wind_EleTower_09"),
		CWind_EleTower_09::Create(m_pDevice, m_pContext))))		
		return E_FAIL;

#pragma endregion 
	
#pragma region SM_Pillar_01		

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Pillar_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_Pillar_01/SM_Pillar_01.fbx", CModel::MODEL_NONANIM))))	
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Pillar_01"),
		CSM_Pillar_01::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 
#pragma region 	PortalTexture
	/* For.Prototype_Component_Texture_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Portal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/glow04.dds"), 1))))
		return E_FAIL;

#pragma endregion 



#pragma region TotemAction
	/* For.Prototype_Component_Texture_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_New_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/Dissolve_masks_01.png"), 1))))
		return E_FAIL;

#pragma endregion 


#pragma region TotemActionUi	
	/* For.Prototype_Component_Texture_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_TotemB_Ui"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/TotemB_Ui.png"), 1))))
		return E_FAIL;

#pragma endregion 

#pragma region SC_TotemB		

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_TotemB"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_TotemB/SC_TotemB.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_TotemB"),
		CSC_TotemB::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	

#pragma endregion 

#pragma region SC_StepBox	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_StepBox"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_StepBox/SC_StepBox.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_StepBox"),
		CSC_StepBox::Create(m_pDevice, m_pContext))))		
		return E_FAIL;

#pragma endregion 


#pragma region NpcInterAction_UI	
	/* For.Prototype_Component_Texture_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_NpcInteraction_Ui"),		
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/NpcInteraction_Ui.png"), 1))))	
		return E_FAIL;

#pragma endregion 
#pragma region Strong_Man 

	PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_StrongMan"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Npc/StrongMan/StrongMan.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))	
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_StrongMan"),			
		CStrongMan::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
#pragma endregion 



#pragma region SC_Object_StoneBall

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Object_StoneBall"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Object_StoneBall/SC_Object_StoneBall2.fbx", CModel::MODEL_NONANIM))))	
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Object_StoneBall"),			
		CSC_Object_StoneBall::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	

#pragma endregion 


#pragma region M_Guardian_Model_A

	PreTransformMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(180.f));	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_Guardian_Model_A"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/M_Guardian_Model_A/M_Guardian_Model_A7.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))	
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_Guardian_Model_A"),	
		CM_Guardian_Model_A::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();		
#pragma endregion 


#pragma region Slash Effect 
	
	PreTransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Slash"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Slash/newSlash_7.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))	
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();	

#pragma endregion 


#pragma region CLAW Effect 

	PreTransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Claw"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Claw/Claw_Effect.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();

#pragma endregion 


#pragma region Slash Effect2 

	PreTransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Slash2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Effect/Slash/newSlash_8.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;
	
	PreTransformMatrix = XMMatrixIdentity();	

#pragma endregion 

#pragma region Hit Effect Texture

	/* For.Prototype_Component_Texture_Hit_Effect0 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HitEffect0"),		
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/T_GUANGSHAN_A.dds"), 1))))		
		return E_FAIL;		

	/* For.Prototype_Component_Texture_Hit_Effect1 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HitEffect1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/HitEffect1.dds"), 1))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Hit_Effect2 */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_HitEffect2"),	
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/HitEffect2.png"), 1))))	
		return E_FAIL;	

#pragma endregion Hit Effect Texture;	

#pragma region Noise Texture

	/* For.Prototype_Component_Texture_Noise */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Noise"),	
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Noise/Dissolve_Noise_Texture.png"), 1))))
		return E_FAIL;	

#pragma endregion NoiseTexture;



#pragma region Sword_Trail_Buffer
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Sword_Trail_Buffer"),
		CSwordTrail_Buffer::Create(m_pDevice, m_pContext))))	
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxPosTexAge*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTexAge"),	 
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTexAge.hlsl"), VTXPOSTEXAGE::Elements, VTXPOSTEXAGE::iNumElements))))
		return E_FAIL;	

	/* For.Prototype_Component_Texture_Sword_Trail */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sword_Trail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/HeatmapGradient.png"), 1))))
		return E_FAIL;


#pragma endregion 


#pragma region Moving_Floor

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Moving_Floor"),
		CMoving_Floor::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
	
#pragma endregion 


	
#pragma region SM_TempleFloor_02

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_TempleFloor_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_TempleFloor_02/SM_TempleFloor_02.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleFloor_02"),
		CSM_TempleFloor_02::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region SC_TempleDoor_05

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_TempleDoor_05"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_TempleDoor_05/SC_TempleDoor_05.fbx", CModel::MODEL_NONANIM))))				
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_TempleDoor_05"),	
		CSC_TempleDoor_05::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 



#pragma region LeverButton
	PreTransformMatrix = PreTransformMatrix * XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(90.f));;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_LeverButton"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/LeverButton/LeverButton.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))	
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_LeverButton"),	
		CLeverButton::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
#pragma endregion 

#pragma region Box_Lift

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Box_Lift"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/Box_Lift/Box_Lift.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Box_Lift"),	
		CBox_Lift::Create(m_pDevice, m_pContext))))	
		return E_FAIL;


#pragma endregion 


#pragma region SC_Stone_HeavyBOX_A_Mod

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Stone_HeavyBOX_A_Mod"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Stone_HeavyBOX_A_Mod/SC_Stone_HeavyBOX_A_Mod.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Stone_HeavyBOX_A_Mod"),	
		CSC_Stone_HeavyBOX_A_Mod::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	

#pragma endregion 



#pragma region TurnTable_Head

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TurnTable_Head"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/TurnTable_Head/TurnTable_Head.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TurnTable_Head"),
		CTurnTable_Head::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

	

#pragma region TurnTable_Body

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_TurnTable_Body"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/TurnTable_Body/TurnTable_Body.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_TurnTable_Body"),	
		CTurnTable_Body::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 




#pragma region ST_LiftTable_a2_sui_02

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ST_LiftTable_a2_sui_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/ST_LiftTable_a2_sui_02/ST_LiftTable_a2_sui_02.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

#pragma endregion 


#pragma region ST_LiftTable_a2_sui_01

	//PreTransformMatrix = XMMatrixScaling(1.f,1.f,1.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ST_LiftTable_a2_sui_01"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/ST_LiftTable_a2_sui_01/LiftTableRealNoAnim.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))	
		return E_FAIL;

	//PreTransformMatrix = XMMatrixIdentity();
#pragma endregion 


#pragma region SC_FlapTrap_03

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_FlapTrap_03"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_FlapTrap_03/SC_FlapTrap_03.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_FlapTrap_03"),
		CSC_FlapTrap_03::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 


#pragma region SM_Templelamp06

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Templelamp06"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_Templelamp06/SM_Templelamp06.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Templelamp06"),
		CSM_Templelamp06::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 


#pragma region SM_Templelamp05

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Templelamp05"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_Templelamp05/SM_Templelamp05.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Templelamp05"),
		CSM_Templelamp05::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	

#pragma endregion 

#pragma region InterAction_UI
	/* For.Prototype_Component_Texture_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Interaction_Ui"),	
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Interaction_Ui4.png"), 1))))
		return E_FAIL;


#pragma endregion 


#pragma region Stamina_Bar_UI
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Stamina_Bar_UI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Stamina_Bar.png"), 1))))	
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Stamina_Bar"),
		CStamina_Bar::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma region HP_Bar_UI

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	
	
	/* For.Prototype_Component_Texture_Hp_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Hp_Bar_UI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Hp_Bar_Final.png"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_HpBar"),	
		CHpBar::Create(m_pDevice, m_pContext))))		
		return E_FAIL;

#pragma endregion 

#pragma region M_Guardian_Model_D

	PreTransformMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) * XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_Guardian_Model_D"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/M_Guardian_Model_D/M_Guardian_Model_D_real.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_Guardian_Model_D"),
		CM_Guardian_Model_D::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

	//if ((((HRESULT)(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_GameObject_M_Guardian_Model_D", CM_Guardian_Model_C::Create(m_pDevice, m_pContext)))) < 0))
	//	return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
#pragma endregion 

#pragma region SC_Brick_W_B_Mod_10

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Brick_W_B_Mod_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Brick_W_B_Mod_10/SC_Brick_W_B_Mod_10.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Brick_W_B_Mod_10"),
		CSC_Brick_W_B_Mod_10::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 



#pragma region SC_Brick_W_B_Mod

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Brick_W_B_Mod"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Brick_W_B_Mod/SC_Brick_W_B_Mod.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Brick_W_B_Mod"),
		CSC_Brick_W_B_Mod::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region SM_TempleBrick_01

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_TempleBrick_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_TempleBrick_01/SM_TempleBrick_01.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleBrick_01"),
		CSM_TempleBrick_01::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region ST_LiftTable_a2_sui_00
	PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.4f,0.4f,0.5f) * XMMatrixRotationX(90.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ST_LiftTable_a2_sui_00"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/ST_LiftTable_a2_sui_00/LiftTableWithAni.fbx", CModel::MODEL_ANIM,PreTransformMatrix))))	
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ST_LiftTable_a2_sui_00"),
		CST_LiftTable_a2_sui_00::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();	
#pragma endregion 
#pragma region SM_LotusFlower_07a1

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_LotusFlower_07a1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_LotusFlower_07a1/SM_LotusFlower_07a1.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_LotusFlower_07a1"),
		CSM_LotusFlower_07a1::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 
#pragma region M_Guardian_Model_C 

	PreTransformMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f) *XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_Guardian_Model_C"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/M_Guardian_Model_C/M_Guardian_Model_C.fbx", CModel::MODEL_ANIM,PreTransformMatrix))))		
		return E_FAIL;	


	if ((((HRESULT)(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, L"Prototype_GameObject_M_Guardian_Model_C", CM_Guardian_Model_C::Create(m_pDevice, m_pContext)))) < 0))
		return E_FAIL;		

	PreTransformMatrix = XMMatrixIdentity();		
#pragma endregion 

#pragma region Water
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Water"),
		CWater::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
	/* For.Prototype_Component_Shader_Water */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Water"),	
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Water.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Water_Texture"),	
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Water/Water.dds"), 1))))	
		return E_FAIL;	

	///* For.Prototype_Component_VIBuffer_Terrain */
	lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain")	
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 100, 100, 1, nullptr))))	
		return E_FAIL;	
#pragma endregion 

#pragma region Sky
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"),
		CSky::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Sky */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Sky"),	
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))	
		return E_FAIL;	

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),	
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region SM_Brick_W_B_Mod2

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_Brick_W_B_Mod2"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_Brick_W_B_Mod2/SM_Brick_W_B_Mod2.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_Brick_W_B_Mod2"),
		CSM_Brick_W_B_Mod2::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion 
#pragma region SM_TempleBrick_v10

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_TempleBrick_10"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_TempleBrick_10/SM_TempleBrick_10.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleBrick_10"),
		CSM_TempleBrick_10::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion 
#pragma region SC_Brick_v1

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Brick_v1"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Brick_v1/SC_Brick_v1.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Brick_v1"),
		CSC_Brick_v1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 
#pragma region M_GuardianP_POSE_A_SC_Rock_01_Weapon

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_GuardianP_POSE_A_SC_Rock_01_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/M_GuardianP_POSE_A_SC_Rock_01_Weapon/M_GuardianP_POSE_A_SC_Rock_01_weapon.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_01_Weapon"),
		CM_GuardianP_POSE_A_SC_Rock_01_Weapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion	
#pragma region M_GuardianP_POSE_A_SC_Rock_01

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_GuardianP_POSE_A_SC_Rock_01"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/M_GuardianP_POSE_A_SC_Rock_01/M_GuardianP_POSE_A_SC_Rock_01.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_01"),
		CM_GuardianP_POSE_A_SC_Rock_01::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion
#pragma region M_GuardianP_POSE_A_SC_Rock_02

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_M_GuardianP_POSE_A_SC_Rock_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/M_GuardianP_POSE_A_SC_Rock_02/M_GuardianP_POSE_A_SC_Rock_02.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_M_GuardianP_POSE_A_SC_Rock_02"),
		CM_GuardianP_POSE_A_SC_Rock_02::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion
#pragma region SC_Corner_D_Mod
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Corner_D_Mod"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Corner_D_Mod/SC_Corner_D_Mod.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Corner_D_Mod"),
		CSC_Corner_D_Mod::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion
#pragma region SC_Steps_A_Mod

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Steps_A_Mod"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SC_Steps_A_Mod/SC_Steps_A.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Steps_A_Mod"),
		CSC_Steps_A_Mod::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 
#pragma region Temple_Block

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Temple_Block"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/Temple_Block/Temple_Block.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Temple_Block"),
		CTempleBlock::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion
#pragma region SM_TempleBrick_04

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SM_TempleBrick_04"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_TempleBrick_04/SM_TempleBrick_04.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SM_TempleBrick_04"),
		CSM_TempleBrick_04::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 
#pragma region Temple_Ground
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Temple_Ground"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/Temple_Ground/Temple_Ground.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Temple_Ground"),
		CTemple_Ground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 
#pragma region Alter_Base


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_AlterBase"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_AltarBase/SM_AltarBase.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_AlterBase"),
		CAlterBase::Create(m_pDevice, m_pContext))))
		return E_FAIL;


#pragma endregion
#pragma region SC_Elemental_Boss_02	
	PreTransformMatrix = XMMatrixScaling(0.001f, 0.001f, 0.001f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_SC_Elemental_Boss_02"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Temple/SM_Elemental_Boss_02/SC_Elemental_Boss_02.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SC_Elemental_Boss_02"),
		CSC_Elemental_Boss_02::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion

#pragma region 지형

	//lstrcpyW(m_szLoadingText, TEXT("텍스쳐 원형을 생성한다."));
	///* For.Prototype_Component_Texture_Terrain*/
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
	//	return E_FAIL;
	//
	//lstrcpyW(m_szLoadingText, TEXT("모델 원형을 생성한다."));
	///* For.Prototype_Component_VIBuffer_Terrain */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
	//	CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
	//	return E_FAIL;
	//
	///* For.Prototype_GameObject_Terrain */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
	//	CTerrain::Create(m_pDevice, m_pContext))))	
	//	return E_FAIL;

#pragma endregion 

#pragma region 카메라 
	/* For.Prototype_GameObject_Camera_Free */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext))))
		return E_FAIL;
#pragma endregion 

#pragma region 셰이더

	lstrcpyW(m_szLoadingText, TEXT("셰이더 원형을 생성한다."));
	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Shader_VtxMeshNoDefferd */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMeshNoDefferd"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMeshNoDefferd.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))	
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMeshNoDefferd */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMeshNoDefferd"),	
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMeshNoDefferd.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))	
		return E_FAIL;	


	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTexInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosTexInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTexInstance.hlsl"), VTX_RECT_INSTANCE::Elements, VTX_RECT_INSTANCE::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPointInstance */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPointInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPointInstance.hlsl"), VTX_POINT_INSTANCE::Elements, VTX_POINT_INSTANCE::iNumElements))))
		return E_FAIL;

#pragma endregion 

#pragma region 피오나 모델
	//lstrcpyW(m_szLoadingText, TEXT("피오나 모델을 생성한다."));
	///* For.Prototype_Component_Model_Fiona */
	//PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));		
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),	
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Fiona/Fiona.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))	
	//	return E_FAIL;	
	//
	//
	///* For.Prototype_GameObject_Monster */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster"),	
	//	CMonster::Create(m_pDevice, m_pContext))))	
	//	return E_FAIL;	

#pragma endregion 
	
#pragma region 지게차 모델
	//lstrcpyW(m_szLoadingText, TEXT("지게차 모델을 생성한다."));
	/////* For.Prototype_Component_Model_ForkLift */
	//PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));	
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"),
	//	CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/ForkLift/ForkLift.fbx", CModel::MODEL_NONANIM, PreTransformMatrix))))
	//	return E_FAIL;	
	//
	///* For.Prototype_GameObject_ForkLift */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_ForkLift"),	
	//	CForkLift::Create(m_pDevice, m_pContext))))	
	//	return E_FAIL;	
#pragma endregion 

#pragma region 카쿠 모델
	lstrcpyW(m_szLoadingText, TEXT("카쿠 모델을 생성한다."));
	/* For.Prototype_Component_Model_Kaku*/
	PreTransformMatrix = /*XMMatrixScaling(0.015f, 0.015f, 0.015f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Kaku"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Kaku/2_03_kaku_2.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;

	///* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
		CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	///* For.Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Player"),
		CBody_Player::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region 카쿠 무기 모델 
	lstrcpyW(m_szLoadingText, TEXT("카쿠 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Kaku_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/KakuWeapon/weapon.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;


	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Weapon"),
		CWeapon::Create(m_pDevice, m_pContext))))
		return E_FAIL;	

#pragma endregion 

#pragma region Navigation 
	lstrcpyW(m_szLoadingText, TEXT("네비게이션 원형을 생성한다."));	
	/* For.Prototype_Component_Navigation */	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),	
		CNavigation::Create(m_pDevice, m_pContext, TEXT("../Navigation_File/test49.bin")))))	
		return E_FAIL;	

#pragma endregion 

#pragma region Collider

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		CCollider::Create(m_pDevice, m_pContext,CCollider::TYPE_AABB))))	
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_SPHERE"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))	
		return E_FAIL;


#pragma endregion

#pragma region 인스턴싱 (눈)

	CVIBuffer_Rect_Instancing::PARTICLE_RECT_INSTANCING_DESC		SnowDesc{};

	SnowDesc.iNumInstance = 1000;
	SnowDesc.vCenter = _float3(64.f, 20.f, 64.f);
	SnowDesc.vRange = _float3(128.f, 1.f, 128.f);
	SnowDesc.vSize = _float2(0.2f, 0.5f);
	SnowDesc.vSpeed = _float2(2.f, 5.f);
	SnowDesc.vLifeTime = _float2(3.0f, 5.5f);
	SnowDesc.isLoop = true;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Instancing"),
		CVIBuffer_Rect_Instancing::Create(m_pDevice, m_pContext, &SnowDesc))))	

		return E_FAIL;
	/* For.Prototype_Component_Texture_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Snow"),
		CParticle_Snow::Create(m_pDevice, m_pContext))))		
		return E_FAIL;

#pragma endregion 

#pragma region 인스턴싱 히트 이펙트 

	CVIBuffer_Point_Instancing::PARTICLE_POINT_INSTANCING_DESC		ExploDesc{};

	ExploDesc.iNumInstance = 100;
	ExploDesc.vCenter = _float3(0.f, 0.f, 0.f);		
	ExploDesc.vRange = _float3(100.f, 100.f, 100.f);		
	ExploDesc.vSize = _float2(0.7f, 1.0f);	
	ExploDesc.vSpeed = _float2(700.f, 800.f);		
	ExploDesc.vLifeTime = _float2(2.f, 3.f);		
	ExploDesc.isLoop = true;
	ExploDesc.vPivot = _float3(0.f, 0.f, 0.f);	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instancing"),	
		CVIBuffer_Point_Instancing::Create(m_pDevice, m_pContext, &ExploDesc))))		
		return E_FAIL;	

	/* For.Prototype_GameObject_Particle_Explosion */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Hit"),
		CHitParticle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_Hit */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Hit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/T_GUANGSHAN_A.dds"), 1))))
		return E_FAIL;


#pragma endregion 



#pragma region 총알 모으기 이펙트 

	ExploDesc.iNumInstance = 500;		
	ExploDesc.vCenter = _float3(0.f, 0.f, 0.f);		
	ExploDesc.vRange = _float3(150.f, 125.f, 150.f);		
	ExploDesc.vSize = _float2(5.f, 6.0f);	
	ExploDesc.vSpeed = _float2(100.f, 300.f);	
	ExploDesc.vLifeTime = _float2(0.5f, 2.f);	
	ExploDesc.isLoop = true;	
	ExploDesc.vPivot = _float3(0.f, 0.f, 0.f);	

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instancing_Bullet"),		
		CVIBuffer_Point_Instancing::Create(m_pDevice, m_pContext, &ExploDesc))))		
		return E_FAIL;		

	/* For.Prototype_GameObject_Particle_Explosion */	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_BulletGainEffect"),			
		CBulletGainEffectParticle::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	

	///* For.Prototype_Component_Texture_Hit */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Hit"),	
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/T_GUANGSHAN_A.dds"), 1))))	
	//	return E_FAIL;	


#pragma endregion 


#pragma region 인스턴싱 아이템 획득 이펙트	

	/* For.Prototype_GameObject_Particle_Explosion */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_ItemGainParticle"),	
		CItemGainParticle::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

	/* For.Prototype_Component_Texture_Gain */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Gain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Particle_Glow02.png"), 1))))
		return E_FAIL;

	
#pragma endregion 


#pragma region 인스턴싱 화염 브레스 

	CVIBuffer_Point_Instancing::PARTICLE_POINT_INSTANCING_DESC		FireDesc{};	

	FireDesc.iNumInstance = 30;		
	FireDesc.vCenter = _float3(0.f, 0.f, 0.f);		
	FireDesc.vRange = _float3(40.f, 40.f, 40.f);		
	FireDesc.vSize = _float2(1.f, 40.0f);		
	FireDesc.vSpeed = _float2(400.f, 500.f);		
	FireDesc.vLifeTime = _float2(5.f, 10.f);	

	FireDesc.isLoop = true;	
	FireDesc.vPivot = _float3(0.f, 0.f, 0.f);	
	FireDesc.vBornTime = _float2(0.f, 0.2f);
	FireDesc.vTexInterval = _float2(1.f / 4.f, 1.f / 6.f);			
	FireDesc.fTexDurationTime = 0.04f;	
	FireDesc.vTexOffSet = _float2(1.f / 4.f, 1.f / 6.f);		
	//FireDesc.iSpawnCount = 10.f;
	//FireDesc.fSpawnTime = 0.2f;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Fire_Instancing"),
		CVIBuffer_Point_Instancing::Create(m_pDevice, m_pContext, &FireDesc))))
		return E_FAIL;	


	/* 2월 9일 이전 작업 */
	/* For.Prototype_Component_Texture_Snow */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Fire"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Particle_Glow02.png"), 1))))	
	//	return E_FAIL;
	//
	/* For.Prototype_Component_Texture_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Alpha_Fire"),	
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/Fire.png"), 1))))			
		return E_FAIL;
	/* ====================================== */

	/* For.Prototype_Component_Texture_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Fire"),	
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effect/P_SUCE_A__1_.png"), 1))))		
		return E_FAIL;

	/* For.Prototype_GameObject_Particle_Snow */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Fire"),
		CFireBreath::Create(m_pDevice, m_pContext))))
		return E_FAIL;

#pragma endregion 

#pragma region 인스턴스 이펙트 포인트 
	//CVIBuffer_Point_Instancing::PARTICLE_POINT_INSTANCING_DESC		ExploDesc{};	

	//ExploDesc.iNumInstance = 600;	
	//ExploDesc.vCenter = _float3(0.f, 0.f, 0.f);		
	//ExploDesc.vRange = _float3(500.f, 500.f, 500.f);			
	//ExploDesc.vSize = _float2(0.3f, 0.6f);			
	//ExploDesc.vSpeed = _float2(300.f, 500.f);				
	//ExploDesc.vLifeTime = _float2(1.f, 2.0f);			
	//ExploDesc.isLoop = true;
	//ExploDesc.vPivot = _float3(0.f, 0.f, 0.f);	
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Point_Instancing"),
	//	CVIBuffer_Point_Instancing::Create(m_pDevice, m_pContext, &ExploDesc))))
	//	return E_FAIL;
	//
	///* For.Prototype_GameObject_Particle_Explosion */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Particle_Explosion"),
	//	CParticle_Explosion::Create(m_pDevice, m_pContext))))	
	//	return E_FAIL;	
#pragma endregion 

	/* 로딩이 완료되었습ㄴ미다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));	
	m_isFinished = true;
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
