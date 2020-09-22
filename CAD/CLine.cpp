#include "pch.h"
#include "CLine.h"

void CLine::Draw(CDC* pDC)
{
    IShape::Draw(pDC);

    pDC->MoveTo(m_ptBegin);
    pDC->LineTo(m_ptEnd);
}

bool CLine::IsSelected(const CPoint& point)
{
    POINT aryPts[4] = { 0 };
    const int nDelta = max(m_logPen.lopnWidth.x, 5);   //±ß¾à

    aryPts[0].x = m_ptBegin.x + nDelta;
    aryPts[0].y = m_ptBegin.y - nDelta;

    aryPts[1].x = m_ptEnd.x + nDelta;
    aryPts[1].y = m_ptEnd.y - nDelta;

    aryPts[2].x = m_ptEnd.x - nDelta;
    aryPts[2].y = m_ptEnd.y + nDelta;

    aryPts[3].x = m_ptBegin.x - nDelta;
    aryPts[3].y = m_ptBegin.y + nDelta;

    CRgn rgn;
    rgn.CreatePolygonRgn(aryPts, 4, ALTERNATE);

    return rgn.PtInRegion(point);
}

void CLine::DrawSelected(CDC* pDC)
{
    IShape::DrawSelected(pDC);

    pDC->MoveTo(m_ptBegin);
    pDC->LineTo(m_ptEnd);
}

IMPLEMENT_SERIAL(CLine, IShape, 1)
void CLine::Serialize(CArchive& archive)
{
    IShape::Serialize(archive);
}

IShape* CLineFactory::Create()
{
    return new CLine();
}
