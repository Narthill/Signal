#pragma once


// CMyStatic

class CMyStatic : public CStatic
{
	DECLARE_DYNAMIC(CMyStatic)

public:
	CMyStatic();
	virtual ~CMyStatic();

	bool m_windsizetype;//false С���ڣ�true �󴰿�

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


