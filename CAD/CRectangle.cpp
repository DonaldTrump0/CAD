#include "pch.h"
#include "CRectangle.h"

void CRectangle::Draw(CDC* pDC)
{
    IShape::Draw(pDC);

    CRect rect = CRect(m_ptBegin, m_ptEnd);
    pDC->Rectangle(rect);
}

bool CRectangle::IsSelected(const CPoint& point)
{
    CPoint pt1 = { min(m_ptBegin.x, m_ptEnd.x), min(m_ptBegin.y, m_ptEnd.y) };
    CPoint pt2 = { max(m_ptBegin.x, m_ptEnd.x), max(m_ptBegin.y, m_ptEnd.y) };

    CRect rect(pt1, pt2);
    return rect.PtInRect(point);
}

void CRectangle::DrawSelected(CDC* pDC)
{
    IShape::DrawSelected(pDC);

    CRect rc(m_ptBegin, m_ptEnd);
    pDC->Rectangle(&rc);
}

IMPLEMENT_SERIAL(CRectangle, IShape, 1)
void CRectangle::Serialize(CArchive& archive)
{
    IShape::Serialize(archive);
}

IShape* CRectangleFactory::Create()
{
    return new CRectangle();
}
