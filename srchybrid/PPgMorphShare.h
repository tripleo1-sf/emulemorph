#pragma once
#include "TreeOptionsCtrlEx.h"
// CPPgMorphShare dialog

class CPPgMorphShare : public CPropertyPage
{
	DECLARE_DYNAMIC(CPPgMorphShare)

public:
	CPPgMorphShare();
	virtual ~CPPgMorphShare();

// Dialog Data
	enum { IDD = IDD_PPG_MORPH_SHARE };
protected:

	int m_iPowershareMode; //MORPH - Added by SiRoB, Avoid misusing of powersharing
	int m_iHideOS;	//MORPH - Added by SiRoB, SLUGFILLER: hideOS
	int m_iSelectiveShare;  //MORPH - Added by SiRoB, SLUGFILLER: hideOS
	int m_iShareOnlyTheNeed; //MORPH - Added by SiRoB, SHARE_ONLY_THE_NEED
	int m_iPowerShareLimit; //MORPH - Added by SiRoB, POWERSHARE Limit
	int m_iPermissions; //MORPH - Added by SiRoB, Show Permissions
	int m_iPowershareInternalPrio; //Morph - added by AndCyle, selective PS internal Prio
	int m_iFolderIcons;
	
	CTreeOptionsCtrlEx m_ctrlTreeOptions;
	bool m_bInitializedTreeOpts;
	HTREEITEM m_htiSFM;
	//MORPH START - Added by SiRoB, Avoid misusing of powersharing
	HTREEITEM m_htiPowershareMode;
	HTREEITEM m_htiPowershareDisabled;
	HTREEITEM m_htiPowershareActivated;
	HTREEITEM m_htiPowershareAuto;
	HTREEITEM m_htiPowershareLimited;
	//MORPH END   - Added by SiRoB, Avoid misusing of powersharing
	HTREEITEM m_htiHideOS;	//MORPH - Added by SiRoB, SLUGFILLER: hideOS
	HTREEITEM m_htiSelectiveShare; //MORPH - Added by SiRoB, SLUGFILLER: hideOS
	HTREEITEM m_htiShareOnlyTheNeed; //MORPH - Added by SiRoB, SHARE_ONLY_THE_NEED
	HTREEITEM m_htiPowerShareLimit; //MORPH - Added by SiRoB, POWERSHARE Limit
	HTREEITEM m_htiPowershareInternalPrio; //Morph - added by AndCyle, selective PS internal Prio
	//MORPH START - Added by SiRoB, Show Permission
	HTREEITEM m_htiPermissions;
	HTREEITEM m_htiPermAll;
	HTREEITEM m_htiPermFriend;
	HTREEITEM m_htiPermNone;
	// Mighty Knife: Community visible filelist
	HTREEITEM m_htiPermCommunity;
	// [end] Mighty Knife
	//MORPH END   - Added by SiRoB, Show Permission
	HTREEITEM m_htiDisplay;
	HTREEITEM m_htiFolderIcons;
	
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnTreeOptsCtrlNotify(WPARAM wParam, LPARAM lParam);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void Localize(void);	
	void LoadSettings(void);
	virtual BOOL OnApply();
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnSettingsChange()			{ SetModified(); }
};