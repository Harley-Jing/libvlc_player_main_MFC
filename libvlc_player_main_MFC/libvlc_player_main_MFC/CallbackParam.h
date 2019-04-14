#pragma once

class CallbackParam
{
public:
	CallbackParam();
	CallbackParam(uint8_t ucID, uint32_t width = 0, uint32_t height = 0, void* pParam = NULL);
	CallbackParam(const CallbackParam& other);
	~CallbackParam();

	uint8_t *m_buf;
	uint32_t m_uiWidth;
	uint32_t m_uiHeight;
	uint8_t m_ucID;
	CMutex m_mutex;
	void* m_pParam;
};

