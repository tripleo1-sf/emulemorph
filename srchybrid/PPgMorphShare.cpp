// PpgMorph.cpp : implementation file
//

#include "stdafx.h"
#include "emule.h"
#include "PPgMorphShare.h"
#include "emuledlg.h"
#include "serverWnd.h" //MORPH - Added by SiRoB
#include "OtherFunctions.h"
#include "Scheduler.h" //MORPH - Added by SiRoB, Fix for Param used in scheduler
#include "searchDlg.h"
#include "sharedfilelist.h" //MORPH - Added by SiRoB, POWERSHARE Limit
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


///////////////////////////////////////////////////////////////////////////////
// CPPgMorphShare dialog

IMPLEMENT_DYNAMIC(CPPgMorphShare, CPropertyPage)

BEGIN_MESSAGE_MAP(CPPgMorphShare, CPropertyPage)
	ON_WM_HSCROLL()
    ON_WM_DESTROY()
	ON_MESSAGE(WM_TREEOPTSCTRL_NOTIFY, OnTreeOptsCtrlNotify)
END_MESSAGE_MAP()

CPPgMorphShare::CPPgMorphShare()
	: CPropertyPage(CPPgMorphShare::IDD)
	, m_ctrlTreeOptions(theApp.m_iDfltImageListColorFlags)
{
	m_bInitializedTreeOpts = false;
	m_htiHideOS = NULL;	//MORPH - Added by SiRoB, SLUGFILLER: hideOS
	m_htiSelectiveShare = NULL;	//MORPH - Added by SiRoB, SLUGFILLER: hideOS
	m_htiShareOnlyTheNeed = NULL; //MORPH - Added by SiRoB, SHARE_ONLY_THE_NEED
	m_htiPowerShareLimit = NULL; //MORPH - Added by SiRoB, POWERSHARE Limit
	//MORPH START - Added by SiRoB, Avoid misusing of powersharing	
	m_htiPowershareMode = NULL;
	m_htiPowershareDisabled = NULL;
	m_htiPowershareActivated = NULL;
	m_htiPowershareAuto = NULL;
	m_htiPowershareLimited = NULL;
	//MORPH END   - Added by SiRoB, Avoid misusing of powersharing	
	m_htiPowershareInternalPrio = NULL; //Morph - added by AndCyle, selective PS internal Prio
	//MORPH START - Added by SiRoB, Show Permission
	m_htiPermissions = NULL;
	m_htiPermAll = NULL;
	m_htiPermFriend = NULL;
	m_htiPermNone = NULL;
	// Mighty Knife: Community visible filelist
	m_htiPermCommunity = NULL;
	// [end] Mighty Knife
	//MORPH END   - Added by SiRoB, Show Permission
	m_htiFolderIcons = NULL;
	m_htiDisplay = NULL;
}

CPPgMorphShare::~CPPgMorphShare()
{
}

void CPPgMorphShare::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MORPH_OPTS, m_ctrlTreeOptions);
	if (!m_bInitializedTreeOpts)
	{
		int iImgSFM = 8;
		int iImgPerm = 8;
		int iImgPS = 8;
		int iImgDisp = 8;

		CImageList* piml = m_ctrlTreeOptions.GetImageList(TVSIL_NORMAL);
		if (piml){
			iImgSFM = piml->Add(CTempIconLoader(_T("SHAREDFILES")));
			iImgPS = piml->Add(CTempIconLoader(_T("PREF_FILES"))); //MORPH - Added by SiRoB, POWERSHARE Limit
			iImgPerm = piml->Add(CTempIconLoader(_T("ClientCompatible"))); //MORPH - Added by SiRoB, Show Permission
			iImgDisp = piml->Add(CTempIconLoader(_T("PREF_DISPLAY")));
		}
		
		CString Buffer;
		m_htiSFM = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_SFM), iImgSFM, TVI_ROOT);
		//MORPH START - Added by SiRoB, SLUGFILLER: hideOS
		m_htiHideOS = m_ctrlTreeOptions.InsertItem(GetResString(IDS_HIDEOVERSHARES), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiSFM);
		m_ctrlTreeOptions.AddEditBox(m_htiHideOS, RUNTIME_CLASS(CNumTreeOptionsEdit));
		m_htiSelectiveShare = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SELECTIVESHARE), m_htiHideOS, m_iSelectiveShare);
		//MORPH END   - Added by SiRoB, SLUGFILLER: hideOS
		//MORPH START - Added by SiRoB, SHARE_ONLY_THE_NEED
		m_htiShareOnlyTheNeed = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_SHAREONLYTHENEED), m_htiSFM, m_iShareOnlyTheNeed);
		//MORPH END   - Added by SiRoB, SHARE_ONLY_THE_NEED

		//MORPH START - Added by SiRoB, Avoid misusing of powersharing
		m_htiPowershareMode = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_POWERSHARE), iImgPS, m_htiSFM);
		m_htiPowershareDisabled = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_POWERSHARE_DISABLED), m_htiPowershareMode, m_iPowershareMode == 0);
		m_htiPowershareActivated =  m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_POWERSHARE_ACTIVATED), m_htiPowershareMode, m_iPowershareMode == 1);
		m_htiPowershareAuto =  m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_POWERSHARE_AUTO), m_htiPowershareMode, m_iPowershareMode == 2);
		//MORPH END   - Added by SiRoB, Avoid misusing of powersharing
		//MORPH START - Added by SiRoB, POWERSHARE Limit
		m_htiPowershareLimited =  m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_POWERSHARE_LIMITED), m_htiPowershareMode, m_iPowershareMode == 3);
		m_htiPowerShareLimit = m_ctrlTreeOptions.InsertItem(GetResString(IDS_POWERSHARE_LIMIT), TREEOPTSCTRLIMG_EDIT, TREEOPTSCTRLIMG_EDIT, m_htiPowershareLimited );
		m_ctrlTreeOptions.AddEditBox(m_htiPowerShareLimit, RUNTIME_CLASS(CNumTreeOptionsEdit));
		//MORPH END   - Added by SiRoB, POWERSHARE Limit
		//Morph Start - added by AndCyle, selective PS internal Prio
		m_htiPowershareInternalPrio = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_POWERSHAER_INTERPRIO), m_htiPowershareMode, m_iPowershareInternalPrio);
		//Morph End - added by AndCyle, selective PS internal Prio
		
		//MORPH START - Added by SiRoB, Show Permission
		m_htiPermissions = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_PERMISSION), iImgPerm, m_htiSFM);
		m_htiPermAll = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_FSTATUS_PUBLIC), m_htiPermissions, m_iPermissions == 0);
		m_htiPermFriend = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_FSTATUS_FRIENDSONLY), m_htiPermissions, m_iPermissions == 1);
		m_htiPermNone = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_HIDDEN), m_htiPermissions, m_iPermissions == 2);
		// Mighty Knife: Community visible filelist
		m_htiPermCommunity = m_ctrlTreeOptions.InsertRadioButton(GetResString(IDS_COMMUNITY), m_htiPermissions, m_iPermissions == 3);
		// [end] Mighty Knife
		//MORPH END   - Added by SiRoB, Show Permission

		m_htiDisplay = m_ctrlTreeOptions.InsertGroup(GetResString(IDS_PW_DISPLAY), iImgDisp, TVI_ROOT);
		m_htiFolderIcons = m_ctrlTreeOptions.InsertCheckBox(GetResString(IDS_FOLDERICONS),m_htiDisplay, m_iFolderIcons);

		m_ctrlTreeOptions.Expand(m_htiSFM, TVE_EXPAND);
		m_ctrlTreeOptions.Expand(m_htiHideOS, TVE_EXPAND);
		m_ctrlTreeOptions.Expand(m_htiPermissions, TVE_EXPAND);
		m_ctrlTreeOptions.Expand(m_htiPowershareMode, TVE_EXPAND);
		m_ctrlTreeOptions.Expand(m_htiDisplay, TVE_EXPAND);
		m_ctrlTreeOptions.SendMessage(WM_VSCROLL, SB_TOP);
		m_bInitializedTreeOpts = true;
	}
	//MORPH START - Added by SiRoB, SLUGFILLER: hideOS
	DDX_TreeEdit(pDX, IDC_MORPH_OPTS, m_htiHideOS, m_iHideOS);
	DDV_MinMaxInt(pDX, m_iHideOS, 0, INT_MAX);
	DDX_TreeCheck(pDX, IDC_MORPH_OPTS, m_htiSelectiveShare, m_iSelectiveShare);
	//MORPH END - Added by SiRoB, SLUGFILLER: hideOS
	//MORPH START - Added by SiRoB, SHARE_ONLY_THE_NEED
	DDX_TreeCheck(pDX, IDC_MORPH_OPTS, m_htiShareOnlyTheNeed, m_iShareOnlyTheNeed);
	//MORPH END   - Added by SiRoB, SHARE_ONLY_THE_NEED
	//MORPH START - Added by SiRoB, POWERSHARE Limit
	DDX_TreeEdit(pDX, IDC_MORPH_OPTS, m_htiPowerShareLimit, m_iPowerShareLimit);
	DDV_MinMaxInt(pDX, m_iShareOnlyTheNeed, 0, INT_MAX);
	//MORPH END   - Added by SiRoB, POWERSHARE Limit
	//MORPH START - Added by SiRoB, Avoid misusing of powersharing
	DDX_TreeRadio(pDX, IDC_MORPH_OPTS, m_htiPowershareMode, m_iPowershareMode);
	//MORPH END   - Added by SiRoB, Avoid misusing of powersharing
	//Morph Start - added by AndCyle, selective PS internal Prio
	DDX_TreeCheck(pDX, IDC_MORPH_OPTS, m_htiPowershareInternalPrio, m_iPowershareInternalPrio);
	//Morph End - added by AndCyle, selective PS internal Prio
	//MORPH START - Added by SiRoB, Show Permission
	DDX_TreeRadio(pDX, IDC_MORPH_OPTS, m_htiPermissions, m_iPermissions);
	//MORPH END   - Added by SiRoB, Show Permission
	DDX_TreeCheck(pDX, IDC_MORPH_OPTS, m_htiFolderIcons, m_iFolderIcons);
}


BOOL CPPgMorphShare::OnInitDialog()
{
	m_iPowershareMode = thePrefs.m_iPowershareMode;//MORPH - Added by SiRoB, Avoid misusing of powersharing
	m_iHideOS = thePrefs.hideOS; //MORPH - Added by SiRoB, SLUGFILLER: hideOS
	m_iSelectiveShare = thePrefs.selectiveShare; //MORPH - Added by SiRoB, SLUGFILLER: hideOS
	m_iShareOnlyTheNeed = thePrefs.ShareOnlyTheNeed; //MORPH - Added by SiRoB, SHARE_ONLY_THE_NEED
	m_iPowerShareLimit = thePrefs.PowerShareLimit; //MORPH - Added by SiRoB, POWERSHARE Limit
	m_iPowershareInternalPrio = thePrefs.m_bPowershareInternalPrio; //Morph - added by AndCyle, selective PS internal Prio
	m_iPermissions = thePrefs.permissions; //MORPH - Added by SiRoB, Show Permission
	m_iFolderIcons = thePrefs.m_bShowFolderIcons;
	
	CPropertyPage::OnInitDialog();
	Localize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPPgMorphShare::OnKillActive()
{
	// if prop page is closed by pressing VK_ENTER we have to explicitly commit any possibly pending
	// data from an open edit control
	m_ctrlTreeOptions.HandleChildControlLosingFocus();
	return CPropertyPage::OnKillActive();
}

BOOL CPPgMorphShare::OnApply()
{
	// if prop page is closed by pressing VK_ENTER we have to explicitly commit any possibly pending
	// data from an open edit control
	m_ctrlTreeOptions.HandleChildControlLosingFocus();
	
	if (!UpdateData())
		return FALSE;
	thePrefs.m_iPowershareMode = m_iPowershareMode;//MORPH - Added by SiRoB, Avoid misusing of powersharing
	thePrefs.hideOS = m_iHideOS;	//MORPH - Added by SiRoB, SLUGFILLER: hideOS
	thePrefs.selectiveShare = m_iSelectiveShare; //MORPH - Added by SiRoB, SLUGFILLER: hideOS
	thePrefs.ShareOnlyTheNeed = m_iShareOnlyTheNeed; //MORPH - Added by SiRoB, SHARE_ONLY_THE_NEED
	//MORPH START - Added by SiRoB, POWERSHARE Limit
	thePrefs.PowerShareLimit = m_iPowerShareLimit;
	theApp.sharedfiles->UpdatePartsInfo();
	//MORPH END   - Added by SiRoB, POWERSHARE Limit
	thePrefs.permissions = m_iPermissions; //MORPH - Added by SiRoB, Show Permission
	thePrefs.m_bPowershareInternalPrio = m_iPowershareInternalPrio; //Morph - added by AndCyle, selective PS internal Prio
	if(thePrefs.m_bShowFolderIcons != m_iFolderIcons)
	{
		if(m_iFolderIcons)
		{
			theApp.AddIncomingFolderIcon();
			theApp.AddTempFolderIcon();
		}
		else
		{
			theApp.RemoveIncomingFolderIcon();
			theApp.RemoveTempFolderIcon();
		}
	}
	thePrefs.m_bShowFolderIcons = m_iFolderIcons;
	
	//theApp.scheduler->SaveOriginals(); //Removed by SiRoB, no scheduler param in this ppg //Added by SiRoB, Fix for Param used in scheduler

	SetModified(FALSE);


	return CPropertyPage::OnApply();
}

void CPPgMorphShare::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	SetModified(TRUE);
	CSliderCtrl* slider =(CSliderCtrl*)pScrollBar;
	CString temp;
}


void CPPgMorphShare::Localize(void)
{	
	if(m_hWnd)
	{
		GetDlgItem(IDC_WARNINGMORPH)->SetWindowText(GetResString(IDS_WARNINGMORPH));
		CString Buffer;
		if (m_htiHideOS) m_ctrlTreeOptions.SetEditLabel(m_htiHideOS, GetResString(IDS_HIDEOVERSHARES));//MORPH - Added by SiRoB, SLUGFILLER: hideOS
		if (m_htiSelectiveShare) m_ctrlTreeOptions.SetItemText(m_htiSelectiveShare, GetResString(IDS_SELECTIVESHARE));//MORPH - Added by SiRoB, SLUGFILLER: hideOS
		if (m_htiShareOnlyTheNeed) m_ctrlTreeOptions.SetItemText(m_htiShareOnlyTheNeed, GetResString(IDS_SHAREONLYTHENEED));//MORPH - Added by SiRoB, SHARE_ONLY_THE_NEED
		//MORPH START - Added by SiRoB, Avoid misusing of powersharing
		if (m_htiPowershareMode) m_ctrlTreeOptions.SetItemText(m_htiPowershareMode, GetResString(IDS_POWERSHARE));
		if (m_htiPowershareDisabled) m_ctrlTreeOptions.SetItemText(m_htiPowershareDisabled, GetResString(IDS_POWERSHARE_DISABLED));
		if (m_htiPowershareActivated) m_ctrlTreeOptions.SetItemText(m_htiPowershareActivated, GetResString(IDS_POWERSHARE_ACTIVATED));
		if (m_htiPowershareAuto) m_ctrlTreeOptions.SetItemText(m_htiPowershareAuto, GetResString(IDS_POWERSHARE_AUTO));
		if (m_htiPowershareLimited) m_ctrlTreeOptions.SetItemText(m_htiPowershareLimited, GetResString(IDS_POWERSHARE_LIMITED));
		//MORPH START - Added by SiRoB, POWERSHARE Limit
		if (m_htiPowerShareLimit) m_ctrlTreeOptions.SetEditLabel(m_htiPowerShareLimit, GetResString(IDS_POWERSHARE_LIMIT));
		//MORPH END   - Added by SiRoB, POWERSHARE Limit
		//Morph Start - added by AndCyle, selective PS internal Prio
		if (m_htiPowershareInternalPrio) m_ctrlTreeOptions.SetItemText(m_htiPowershareInternalPrio, GetResString(IDS_POWERSHAER_INTERPRIO));
		//Morph End - added by AndCyle, selective PS internal Prio
		//MORPH START - Added by SiRoB, Show Permission
		if (m_htiPermissions) m_ctrlTreeOptions.SetItemText(m_htiPermissions, GetResString(IDS_PERMISSION));
		if (m_htiPermAll) m_ctrlTreeOptions.SetItemText(m_htiPermAll, GetResString(IDS_FSTATUS_PUBLIC));
		if (m_htiPermFriend) m_ctrlTreeOptions.SetItemText(m_htiPermFriend, GetResString(IDS_FSTATUS_FRIENDSONLY));
		if (m_htiPermNone) m_ctrlTreeOptions.SetItemText(m_htiPermNone, GetResString(IDS_HIDDEN));
		// Mighty Knife: Community visible filelist
		if (m_htiPermCommunity) m_ctrlTreeOptions.SetItemText(m_htiPermCommunity, GetResString(IDS_COMMUNITY));
		// [end] Mighty Knife
		//MORPH END   - Added by SiRoB, Show Permission
		if (m_htiFolderIcons) m_ctrlTreeOptions.SetItemText(m_htiFolderIcons, GetResString(IDS_FOLDERICONS));

	}

}

void CPPgMorphShare::OnDestroy()
{
	m_ctrlTreeOptions.DeleteAllItems();
	m_ctrlTreeOptions.DestroyWindow();
	m_bInitializedTreeOpts = false;
	m_htiHideOS = NULL;	//MORPH - Added by SiRoB, SLUGFILLER: hideOS
	m_htiSelectiveShare = NULL;	//MORPH - Added by SiRoB, SLUGFILLER: hideOS
	m_htiShareOnlyTheNeed = NULL; //MORPH - Added by SiRoB, SHARE_ONLY_THE_NEED
	m_htiPowerShareLimit = NULL; //MORPH - Added by SiRoB, POWERSHARE Limit
	//MORPH START - Added by SiRoB, Avoid misusing of powersharing	
	m_htiPowershareMode = NULL;
	m_htiPowershareDisabled = NULL;
	m_htiPowershareActivated = NULL;
	m_htiPowershareAuto = NULL;
	m_htiPowershareLimited = NULL;
	//MORPH END   - Added by SiRoB, Avoid misusing of powersharing	
	m_htiPowershareInternalPrio = NULL; //Morph - added by AndCyle, selective PS internal Prio
	//MORPH START - Added by SiRoB, Show Permission
	m_htiPermissions = NULL;
	m_htiPermAll = NULL;
	m_htiPermFriend = NULL;
	m_htiPermNone = NULL;
	m_htiPermCommunity = NULL;
	//MORPH END   - Added by SiRoB, Show Permission
	m_htiFolderIcons = NULL;
	m_htiDisplay = NULL;

	CPropertyPage::OnDestroy();
}
LRESULT CPPgMorphShare::OnTreeOptsCtrlNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == IDC_MORPH_OPTS){
		TREEOPTSCTRLNOTIFY* pton = (TREEOPTSCTRLNOTIFY*)lParam;
		//		if (bCheck && m_ctrlTreeOptions.GetCheckBox(m_htiUSSEnabled,bCheck))
		//			if (bCheck) m_ctrlTreeOptions.SetCheckBox(m_htiUSSEnabled,false);
		//}else if (pton->hItem == m_htiUSSEnabled){
		//	BOOL bCheck;
		//	if (m_ctrlTreeOptions.GetCheckBox(m_htiUSSEnabled, bCheck))
		//		if (bCheck && m_ctrlTreeOptions.GetCheckBox(m_htiSUCEnabled,bCheck))
		//			if (bCheck) m_ctrlTreeOptions.SetCheckBox(m_htiSUCEnabled,false);
		//	
		//}
		SetModified();
	}
	return 0;
}