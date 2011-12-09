// SulJson.cpp

#include "stdafx.h"
#include "SulJson.h"
#include "SulTypes.h"
#include "SulParser.h"
#include "SulString.h"
#include "SulJsonObject.h"
#include "SulJsonArray.h"

CSulJson::CSulJson()
{
}

CSulJson::CSulJson( CSulJson* pParent )
{
	m_pParent = pParent;
}

CSulJson* CSulJson::getParent() const
{
	return m_pParent;
}

