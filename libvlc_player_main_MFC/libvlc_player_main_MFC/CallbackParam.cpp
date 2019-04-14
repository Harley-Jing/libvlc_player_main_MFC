#include "stdafx.h"
#include "CallbackParam.h"

CallbackParam::CallbackParam()
{

}

CallbackParam::CallbackParam(uint8_t ucID, uint32_t width, uint32_t height, void* pParam)
	:m_uiWidth(width)
	, m_uiHeight(height)
	, m_pParam(pParam)
	, m_ucID(ucID)
{
	m_buf = new uint8_t[m_uiWidth*m_uiHeight * 4];
}

CallbackParam::CallbackParam(const CallbackParam& other)
	: m_uiWidth(other.m_uiWidth)
	, m_uiHeight(other.m_uiHeight)
	, m_pParam(other.m_pParam)
	, m_ucID(other.m_ucID)
{
	m_buf = new uint8_t[m_uiWidth*m_uiHeight * 4];
	memcpy(m_buf, other.m_buf, m_uiWidth*m_uiHeight * 4);
}

CallbackParam::~CallbackParam()
{
	delete []m_buf;
}
