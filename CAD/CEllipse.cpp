#include "pch.h"
#include "CEllipse.h"

void CEllipse::Draw(CDC* pDC)
{
    IShape::Draw(pDC);

    CRect rect = CRect(m_ptBegin, m_ptEnd);
    pDC->Ellipse(rect);
}

bool CEllipse::IsSelected(const CPoint& point)
{
    CPoint pt1 = { min(m_ptBegin.x, m_ptEnd.x), min(m_ptBegin.y, m_ptEnd.y) };
    CPoint pt2 = { max(m_ptBegin.x, m_ptEnd.x), max(m_ptBegin.y, m_ptEnd.y) };

    CRect rect(pt1, pt2);
    return rect.PtInRect(point);
}

void CEllipse::DrawSelected(CDC* pDC)
{
    IShape::DrawSelected(pDC);

    CRect rc(m_ptBegin, m_ptEnd);
    pDC->Ellipse(&rc);
}

IMPLEMENT_SERIAL(CEllipse, IShape, 1)
void CEllipse::Serialize(CArchive& archive)
{
    IShape::Serialize(archive);
}

IShape* CEllipseFactory::Create()
{
    return new CEllipse();
}
