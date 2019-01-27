/*
    SeriousSamGLWeapons - a free software for rendering Serious Sam weapon models with OpenGL 3.3 shaders
    Copyright (C) 2019 Oleksii Sierov (seriousalexej@gmail.com)
	
    SeriousSamGLWeapons is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    SeriousSamGLWeapons is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.
    You should have received a copy of the GNU Lesser General Public License
    along with SeriousSamGLWeapons.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "StdH.h"
#define MAX_WEAPONS 30
static FLOAT wpn_fH[MAX_WEAPONS + 1];
static FLOAT wpn_fP[MAX_WEAPONS + 1];
static FLOAT wpn_fB[MAX_WEAPONS + 1];
static FLOAT wpn_fX[MAX_WEAPONS + 1];
static FLOAT wpn_fY[MAX_WEAPONS + 1];
static FLOAT wpn_fZ[MAX_WEAPONS + 1];
static FLOAT wpn_fFOV[MAX_WEAPONS + 1];
static FLOAT wpn_fClip[MAX_WEAPONS + 1];
static FLOAT wpn_fFX[MAX_WEAPONS + 1];
static FLOAT wpn_fFY[MAX_WEAPONS + 1];
#ifdef DECL_DLL
#undef DECL_DLL
#define DECL_DLL __declspec(dllimport)
#include <Engine/Ska/ModelInstance.h>
#include <GameMP/PlayerSettings.h>
#include <GameMP/SessionProperties.h>
#include <EntitiesMP/Common/GameInterface.h>
#include <EntitiesMP/Global.h>
#include <EntitiesMP/Common/Common.h>
#include <EntitiesMP/Player.h>
#include <EntitiesMP/PlayerWeapons.h>
#include <Models/Weapons/Laser/Laser.h>
#include <Models/Weapons/Laser/Barrel.h>
#undef DECL_DLL
#define DECL_DLL __declspec(dllexport)
#endif
#include <AlexShaders/Init.h>
#include <map>


extern CEntity* AlexPlayer_New();
extern "C" __declspec(dllimport) CDLLEntityClass CPlayer_DLLClass;
extern "C" __declspec(dllexport) CDLLEntityClass AlexPlayer_DLLClass;

CDLLEntityClass AlexPlayer_DLLClass = {
  CPlayer_DLLClass.dec_aepProperties,
  CPlayer_DLLClass.dec_ctProperties,
  CPlayer_DLLClass.dec_aeheHandlers,
  CPlayer_DLLClass.dec_ctHandlers,
  CPlayer_DLLClass.dec_aecComponents,
  CPlayer_DLLClass.dec_ctComponents,
  CPlayer_DLLClass.dec_strName,
  CPlayer_DLLClass.dec_strIconFileName,
  CPlayer_DLLClass.dec_iID,
  CPlayer_DLLClass.dec_pdecBase,
  &AlexPlayer_New,
  CPlayer_DLLClass.dec_OnInitClass,
  CPlayer_DLLClass.dec_OnEndClass,
  CPlayer_DLLClass.dec_OnPrecache,
  CPlayer_DLLClass.dec_OnWorldInit,
  CPlayer_DLLClass.dec_OnWorldTick,
  CPlayer_DLLClass.dec_OnWorldRender,
  CPlayer_DLLClass.dec_OnWorldEnd
};

#define PLF_INITIALIZED           (1UL<<0)   // set when player entity is ready to function
#define PLF_DONTRENDER            (1UL<<5)   // don't render view (used at end of level)
#define GRV_SHOWEXTRAS            (1L<<0)    // add extra stuff like console, weapon, pause
#define PICKEDREPORT_TIME         (2.0f)
#define SE_COL_BLUE_NEUTRAL_LT    0x6097cc00

static CTimerValue _tvProbingLast;
extern ULONG _ulPlayerRenderingMask = 0;

static bool g_HasOpenGLAndShaders = false;

static int g_PrevWeapon = -1;
static INDEX g_CurrentAnim = -1;
static std::map<long, CModelInstance*> g_SupportedModels;
static std::map<long, int> g_RenderSessions;

static void LoadModel(long type, const char* path)
{
  if (!g_HasOpenGLAndShaders)
  {
    return;
  }
  CModelInstance* newInstance = new CModelInstance();
  try
  {
    ParseSmcFile_t(*newInstance, path);
  }
  catch (const char* errorMessage) {
    delete newInstance;
    g_HasOpenGLAndShaders = false;
    CPrintF("^cff0000Error loading model! %s^C\n", errorMessage);
    return;
  }
  AlexShaders_Precache(*newInstance);
  g_SupportedModels[type] = newInstance;
  g_RenderSessions[type] = AlexShaders_RegisterRenderSession();
}

class DECL_DLL  AlexPlayer : public CPlayer {
public:
  AlexPlayer()
  {
    wpn_fFOV[WEAPON_LASER] = 65.5f;

    wpn_fX[WEAPON_LASER] = 0.125335f;
    wpn_fY[WEAPON_LASER] = -0.367129f;
    wpn_fZ[WEAPON_LASER] = -0.0366111f;

    wpn_fH[WEAPON_LASER] = 0.0f;
    wpn_fP[WEAPON_LASER] = 5.0f;
    wpn_fB[WEAPON_LASER] = 0.0f;

    g_HasOpenGLAndShaders = (_pShell->GetINDEX("sam_iGfxAPI") == 0) && AlexShaders_Initialize();
    if (g_HasOpenGLAndShaders)
    {
      LoadModel(WEAPON_LASER, "SeriousAlexej\\Models\\Weapons\\Laser\\Laser.smc");
    }
  }

  virtual ~AlexPlayer()
  {
    {for (std::map<long, CModelInstance*>::iterator it = g_SupportedModels.begin(); it != g_SupportedModels.end(); ++it)
    {
      delete it->second;
    }}
    g_SupportedModels.clear();
    g_RenderSessions.clear();
    if (g_HasOpenGLAndShaders)
    {
      AlexShaders_Finalize();
    }
  }

  void SyncWeaponAnimations(CModelInstance& wepModel, CPlayerWeapons& weapons)
  {
    if (g_PrevWeapon != weapons.m_iCurrentWeapon)
    {
      g_CurrentAnim = -1;
      g_PrevWeapon = weapons.m_iCurrentWeapon;
    }
    switch (weapons.m_iCurrentWeapon)
    {
    case WEAPON_LASER:
    {
      CModelObject& mo = weapons.m_moWeapon;
      if (mo.ao_iCurrentAnim != g_CurrentAnim)
      {
        g_CurrentAnim = mo.ao_iCurrentAnim;
        switch (mo.ao_iCurrentAnim)
        {
        case LASER_ANIM_WAIT01:
          wepModel.AddAnimation(ska_GetIDFromStringTable("Laser_Wait"), AN_LOOPING, 1.0f, 0);
          break;
        case LASER_ANIM_ACTIVATE:
          wepModel.AddAnimation(ska_GetIDFromStringTable("Laser_Activate"), 0, 1.0f, 0);
          break;
        case LASER_ANIM_DEACTIVATE:
          wepModel.AddAnimation(ska_GetIDFromStringTable("Laser_DeActivate"), 0, 1.0f, 0);
          break;
        case LASER_ANIM_FIRE:
          wepModel.AddAnimation(ska_GetIDFromStringTable("Laser_Fire"), AN_LOOPING, 1.0f, 0);
          break;
        default:
          break;
        }
      }
      if (g_CurrentAnim == LASER_ANIM_FIRE)
      {
        CModelObject* moBarrel = NULL;
        INDEX animId = -1;
        switch (weapons.m_iLaserBarrel) {
        case 0:
          moBarrel = &(mo.GetAttachmentModel(LASER_ATTACHMENT_LEFTUP)->amo_moModelObject);
          animId = ska_GetIDFromStringTable("Laser_BarrelFireLU");
          break;
        case 3:
          moBarrel = &(mo.GetAttachmentModel(LASER_ATTACHMENT_RIGHTDOWN)->amo_moModelObject);
          animId = ska_GetIDFromStringTable("Laser_BarrelFireRD");
          break;
        case 1:
          moBarrel = &(mo.GetAttachmentModel(LASER_ATTACHMENT_LEFTDOWN)->amo_moModelObject);
          animId = ska_GetIDFromStringTable("Laser_BarrelFireLD");
          break;
        case 2:
          moBarrel = &(mo.GetAttachmentModel(LASER_ATTACHMENT_RIGHTUP)->amo_moModelObject);
          animId = ska_GetIDFromStringTable("Laser_BarrelFireRU");
          break;
        }

        if (moBarrel->ao_iCurrentAnim == BARREL_ANIM_FIRE)
        {
          wepModel.AddAnimation(animId, 0, 1.4f, 0);
        }
      }
      break;
    } // WEAPON_LASER
    default:
    {
      break;
    }
    }
  }

  virtual void RenderGameView(CDrawPort* pdp, void* pvUserData)
  {
    const bool initialized = (m_ulFlags&PLF_INITIALIZED) || (m_ulFlags&PLF_DONTRENDER);
    if (!initialized || !g_HasOpenGLAndShaders)
    {
      CPlayer::RenderGameView(pdp, pvUserData);
      return;
    }

    BOOL bShowExtras = (ULONG(pvUserData)&GRV_SHOWEXTRAS);
    pdp->Unlock();

    if (pvUserData != 0) {
      CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
      if ((tvNow - _tvProbingLast).GetSeconds()<0.1) {
        _pGfx->gl_bAllowProbing = TRUE;
      }
      _tvProbingLast = tvNow;
    }

    BOOL bDualHead =
      pdp->IsDualHead() &&
      GetSP()->sp_gmGameMode != CSessionProperties::GM_FLYOVER &&
      m_penActionMarker == NULL;

    if (bDualHead || m_penCamera == NULL) {
      CDrawPort dpView(pdp, TRUE);
      if (dpView.Lock()) {
        if (_pShell->GetINDEX("sam_iGfxAPI") == 0 && g_HasOpenGLAndShaders)
        {
          RenderPlayerViewAlex(&dpView, bShowExtras);
        }
        else {
          g_HasOpenGLAndShaders = false;
          CPrintF("^cff0000Shaders work only on ^cffffffOpenGL^cff0000 API! Switch to it in game settings!^C\n");
          RenderPlayerView(&dpView, bShowExtras);
        }
        dpView.Unlock();
      }
    }

    if (m_penCamera != NULL) {
      CDrawPort dpView(pdp, m_penActionMarker != NULL);
      if (dpView.Lock()) {
        RenderCameraView(&dpView, !bDualHead);
        dpView.Unlock();
      }
    }
    else {
      if (bDualHead) {
        cmp_ppenDHPlayer = this;
      }
    }
    pdp->Lock();
  }

  void RenderPlayerViewAlex(CDrawPort *pdp, BOOL bShowExtras)
  {
    CAnyProjection3D apr;
    CEntity *penViewer;
    CPlacement3D plViewer;
    COLOR colBlend;

    for (INDEX iEye = STEREO_LEFT; iEye <= (Stereo_IsEnabled() ? STEREO_RIGHT : STEREO_LEFT); iEye++) {
      SetupView(pdp, apr, penViewer, plViewer, colBlend, FALSE);

      Stereo_SetBuffer(iEye);
      Stereo_AdjustProjection(*apr, iEye, 1);

      ASSERT(IsValidFloat(plViewer.pl_OrientationAngle(1)) && IsValidFloat(plViewer.pl_OrientationAngle(2)) && IsValidFloat(plViewer.pl_OrientationAngle(3)));
      _ulPlayerRenderingMask = 1 << GetMyPlayerIndex();
      RenderView(*en_pwoWorld, *penViewer, apr, *pdp);
      _ulPlayerRenderingMask = 0;

      if (iEye == STEREO_LEFT) {
        ListenFromEntity(this, plViewer);
      }

      RenderScroll(pdp);
      RenderTextFX(pdp);
      RenderCredits(pdp);
      RenderHudPicFX(pdp);

      if (_pShell->GetINDEX("hud_bShowAll") && bShowExtras) {
        CPlacement3D plLight(_vViewerLightDirection, ANGLE3D(0, 0, 0));
        plLight.AbsoluteToRelative(plViewer);

        CPlayerWeapons& weapons = *((CPlayerWeapons*)m_penWeapons.ep_pen);

        INDEX showWeapon = _pShell->GetINDEX("hud_bShowWeapon");

        if (showWeapon && _pShell->GetINDEX("gfx_bRenderModels") && !weapons.m_bSniping)
        {
          if (g_SupportedModels.find(weapons.m_iCurrentWeapon) == g_SupportedModels.end())
          {
            weapons.RenderWeaponModel(
              *(CPerspectiveProjection3D *)(CProjection3D *)apr,
              pdp,
              plLight.pl_PositionVector,
              _colViewerLight,
              _colViewerAmbient,
              penViewer == this && (GetFlags()&ENF_ALIVE),
              iEye);
          }
          else {
            SyncWeaponAnimations(*g_SupportedModels[weapons.m_iCurrentWeapon], weapons);
            RenderShaderModel(
              weapons,
              *(CPerspectiveProjection3D *)(CProjection3D *)apr,
              pdp,
              plLight.pl_PositionVector,
              _colViewerLight,
              _colViewerAmbient,
              penViewer == this && (GetFlags()&ENF_ALIVE));
          }
        }

        _pShell->SetINDEX("hud_bShowWeapon", 0);

        RenderHUD(*(CPerspectiveProjection3D *)(CProjection3D *)apr, pdp,
          plLight.pl_PositionVector, _colViewerLight, _colViewerAmbient,
          penViewer == this && (GetFlags()&ENF_ALIVE), iEye);

        _pShell->SetINDEX("hud_bShowWeapon", showWeapon);
      }
    }
    Stereo_SetBuffer(STEREO_BOTH);

    PIX pixDPWidth = pdp->GetWidth();
    PIX pixDPHeight = pdp->GetHeight();
    FLOAT fScale = (FLOAT)pixDPWidth / 640.0f;

    if (_pTimer->CurrentTick()<m_tmCenterMessageEnd) {
      pdp->SetFont(_pfdDisplayFont);
      pdp->SetTextScaling(fScale);
      pdp->SetTextAspect(1.0f);
      pdp->PutTextCXY(m_strCenterMessage, pixDPWidth*0.5f, pixDPHeight*0.85f, C_WHITE | 0xDD);
    }
    else if (_pTimer->CurrentTick()<m_tmLastPicked + PICKEDREPORT_TIME) {
      pdp->SetFont(_pfdDisplayFont);
      pdp->SetTextScaling(fScale);
      pdp->SetTextAspect(1.0f);
      CTString strPicked;
      if (m_fPickedAmmount == 0) {
        strPicked = m_strPickedName;
      }
      else {
        strPicked.PrintF("%s +%d", m_strPickedName, int(m_fPickedAmmount));
      }
      pdp->PutTextCXY(strPicked, pixDPWidth*0.5f, pixDPHeight*0.82f, C_WHITE | 0xDD);
      if (!GetSP()->sp_bCooperative && !GetSP()->sp_bUseFrags && m_fPickedMana >= 1) {
        CTString strValue;
        strValue.PrintF("%s +%d", TRANS("Value"), INDEX(m_fPickedMana));
        pdp->PutTextCXY(strValue, pixDPWidth*0.5f, pixDPHeight*0.85f, C_WHITE | 0xDD);
      }
    }

    if (_pTimer->CurrentTick()<m_tmAnalyseEnd) {
      pdp->SetFont(_pfdDisplayFont);
      pdp->SetTextScaling(fScale);
      pdp->SetTextAspect(1.0f);
      UBYTE ubA = int(sin(_pTimer->CurrentTick()*10.0f) * 127 + 128);
      pdp->PutTextCXY(TRANS("Analyzing..."), pixDPWidth*0.5f, pixDPHeight*0.2f, SE_COL_BLUE_NEUTRAL_LT | ubA);
    }
  }

  void RenderShaderModel(
    CPlayerWeapons& weapons,
    CPerspectiveProjection3D& prProjection,
    CDrawPort* pdp,
    FLOAT3D vViewerLightDirection,
    COLOR colViewerLight,
    COLOR colViewerAmbient,
    BOOL bRender)
  {
    _mrpModelRenderPrefs.SetRenderType(RT_TEXTURE | RT_SHADING_PHONG);

    if (!bRender) {
      return;
    }
    // make sure that weapon will be bright enough
    UBYTE ubLR, ubLG, ubLB, ubAR, ubAG, ubAB;
    ColorToRGB(colViewerLight, ubLR, ubLG, ubLB);
    ColorToRGB(colViewerAmbient, ubAR, ubAG, ubAB);
    INDEX iMinDL = Min(Min(ubLR, ubLG), ubLB) - 32;
    INDEX iMinDA = Min(Min(ubAR, ubAG), ubAB) - 32;
    if (iMinDL<0) {
      ubLR = ClampUp(ubLR - iMinDL, (INDEX)255);
      ubLG = ClampUp(ubLG - iMinDL, (INDEX)255);
      ubLB = ClampUp(ubLB - iMinDL, (INDEX)255);
    }
    if (iMinDA<0) {
      ubAR = ClampUp(ubAR - iMinDA, (INDEX)255);
      ubAG = ClampUp(ubAG - iMinDA, (INDEX)255);
      ubAB = ClampUp(ubAB - iMinDA, (INDEX)255);
    }
    const COLOR colLight = RGBToColor(ubLR, ubLG, ubLB);
    const COLOR colAmbient = RGBToColor(ubAR, ubAG, ubAB);

    CPlacement3D plView;
    plView = en_plViewpoint;
    plView.RelativeToAbsolute(GetPlacement());

    prProjection.ViewerPlacementL() = plView;
    prProjection.FrontClipDistanceL() = 0.1f;
    prProjection.DepthBufferNearL() = 0.0f;
    prProjection.DepthBufferFarL() = 0.1f;
    INDEX iWeaponData = weapons.m_iCurrentWeapon;
    AlexShaders_SetRenderSession(g_RenderSessions[iWeaponData]);
    ((CPerspectiveProjection3D&)prProjection).FOVL() = AngleDeg(wpn_fFOV[iWeaponData]);
    CPlacement3D plWeapon(
      FLOAT3D(wpn_fX[iWeaponData],
              wpn_fY[iWeaponData],
              wpn_fZ[iWeaponData]),
      ANGLE3D(AngleDeg(wpn_fH[iWeaponData]),
              AngleDeg(wpn_fP[iWeaponData]),
              AngleDeg(wpn_fB[iWeaponData])));
    weapons.WeaponMovingOffset(plWeapon.pl_PositionVector);
    plWeapon.RelativeToAbsoluteSmooth(plView);
    CAnyProjection3D apr;
    apr = prProjection;

    vViewerLightDirection.Normalize();

    RM_BeginRenderingView(apr, pdp);
    RM_SetObjectPlacement(plWeapon);
    RM_SetLightColor(colAmbient, colLight);
    RM_SetLightDirection(vViewerLightDirection);
    RM_AddFlag(SRMF_WEAPON);
    RM_RenderSKA(*g_SupportedModels[iWeaponData]);
    RM_EndRenderingView();

    AlexShaders_SetRenderSession(-1);
  }
};

CEntity* AlexPlayer_New()
{
  return new AlexPlayer();
};
