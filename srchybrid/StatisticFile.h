//this file is part of eMule
//Copyright (C)2002 Merkur ( devs@emule-project.net / http://www.emule-project.net )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#pragma once
#include "BarShader.h" //Spreadbars

class CStatisticFile
{
	friend class CKnownFile;
	friend class CPartFile;
public:
	CStatisticFile()
	{
		requested = 0;
		transferred = 0;
		accepted = 0;
		alltimerequested = 0;
		alltimetransferred = 0;
		alltimeaccepted = 0;
		//MORPH START - Added by SiRoB, Reduce SpreadBar CPU consumption
		InChangedSpreadSortValue = false;
		InChangedFullSpreadCount = false;
		InChangedSpreadBar = false;
		lastSpreadSortValue = 0;;
		lastFullSpreadCount = 0;
		//MORPH END   - Added by SiRoB, Reduce SpreadBar CPU consumption
		//Morph Start - Added by AndCycle, Equal Chance For Each File
		shareStartTime = time(NULL);//this value init will be done in other place
		m_bInChangedEqualChanceValue = false;
		lastCheckEqualChanceSemiValue = time(NULL);
		m_dLastEqualChanceBiasValue = 1;
		m_dLastEqualChanceSemiValue = 0;
		//Morph End - Added by AndCycle, Equal Chance For Each File
	}
	//MORPH START - Added by SiRoB, Reduce SpreadBar CPU consumption
	~CStatisticFile()
	{
		m_bitmapSpreadBar.DeleteObject();
	}
	//MORPH END   - Added by SiRoB, Reduce SpreadBar CPU consumption

	void	MergeFileStats( CStatisticFile* toMerge );
	void	AddRequest();
	void	AddAccepted();
	//MORPH START - Added by IceCream SLUGFILLER: Spreadbars
	/*
	void	AddTransferred(uint64 bytes);
	*/
	void	AddTransferred(uint32 start, uint32 bytes);
	void	AddBlockTransferred(uint32 start, uint32 end, uint32 count);
	void	DrawSpreadBar(CDC* dc, RECT* rect, bool bFlat) /*const*/;
	float	GetSpreadSortValue() /*const*/;
	float	GetFullSpreadCount() /*const*/;
	void	ResetSpreadBar(); //MORPH	- Added by AndCycle, SLUGFILLER: Spreadbars - per file
	//MORPH END - Added by IceCream SLUGFILLER: Spreadbars

	UINT	GetRequests() const				{return requested;}
	UINT	GetAccepts() const				{return accepted;}
	uint64	GetTransferred() const			{return transferred;}
	UINT	GetAllTimeRequests() const		{return alltimerequested;}
	UINT	GetAllTimeAccepts() const		{return alltimeaccepted;}
	uint64	GetAllTimeTransferred() const	{return alltimetransferred;}
	
	CKnownFile* fileParent;
//Morph Start - Added by AndCycle, Equal Chance For Each File
	void	SetSharedTime(uint32 sharedTime)		{ shareStartTime = time(NULL) - sharedTime; }
	uint32	GetSharedTime()							{ return time(NULL) - shareStartTime; }
	uint32	GetShareStartTime()						{ return shareStartTime; }
	double	GetEqualChanceValue();
	CString	GetEqualChanceValueString(bool detail = true);
	//Morph End - Added by AndCycle, Equal Chance For Each File
private:
	//MORPH START - Added by IceCream SLUGFILLER: Spreadbars
	CRBMap<uint32, uint32> spreadlist;
	static CBarShader s_SpreadBar;
	//MORPH - Added by SiRoB, Reduce SpreadBar CPU consumption
	bool	InChangedSpreadSortValue;
	bool	InChangedFullSpreadCount;
	bool	InChangedSpreadBar;
	CBitmap m_bitmapSpreadBar;
	int		lastSize;
	bool	lastbFlat;
	float	lastSpreadSortValue;
	float	lastFullSpreadCount;
	//MORPH - Added by SiRoB, Reduce SpreadBar CPU consumption
	//MORPH END   - Added by IceCream, SLUGFILLER: Spreadbars

private:
	uint32 requested;
	uint32 accepted;
	uint64 transferred;
	uint32 alltimerequested;
	uint64 alltimetransferred;
	uint32 alltimeaccepted;
	//Morph Start - Added by AndCycle, Equal Chance For Each File
	uint32	shareStartTime;
	bool	m_bInChangedEqualChanceValue;
	uint32	lastCheckEqualChanceSemiValue;
	double	m_dLastEqualChanceBiasValue;
	double	m_dLastEqualChanceSemiValue;
	//Morph End - Added by AndCycle, Equal Chance For Each File
};