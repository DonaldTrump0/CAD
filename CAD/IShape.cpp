#include "pch.h"
#include "IShape.h"

IShape::~IShape()
{
    delete m_pPen;
    delete m_pBrush;
}

CPoint IShape::GetBeginPoint()
{
    return m_ptBegin;
}

void IShape::SetBeginPoint(const CPoint& ptBegin)
{
    m_ptBegin = ptBegin;
}

CPoint IShape::GetEndPoint()
{
    return m_ptEnd;
}

void IShape::SetEndPoint(const CPoint& ptEnd)
{
    m_ptEnd = ptEnd;
}

LOGPEN IShape::GetPen()
{
    return m_logPen;
}

void IShape::SetPen(LOGPEN& logPen)
{
    m_logPen = logPen;
}

LOGBRUSH IShape::GetBrush()
{
    return m_logBrush;
}

void IShape::SetBrush(LOGBRUSH& logBrush)
{
    m_logBrush = logBrush;
}

void IShape::Draw(CDC* pDC)
{
    if (m_pPen != NULL)
    {
        delete m_pPen;
    }
    m_pPen = new CPen();
    m_pPen->CreatePenIndirect(&m_logPen);

    if (m_pBrush != NULL)
    {
        delete m_pBrush;
    }
    m_pBrush = new CBrush();
    m_pBrush->CreateBrushIndirect(&m_logBrush);

    pDC->SelectObject(m_pPen);
    pDC->SelectObject(m_pBrush);
}

void IShape::DrawSelected(CDC* pDC)
{
    if (m_pPen != NULL)
    {
        delete m_pPen;
    }
    m_pPen = new CPen();
    m_pPen->CreatePen(m_logPen.lopnStyle, max(5, m_logPen.lopnWidth.x), RGB(128, 64, 64));

    if (m_pBrush != NULL)
    {
        delete m_pBrush;
    }
    m_pBrush = new CBrush();
    m_pBrush->CreateBrushIndirect(&m_logBrush);

    pDC->SelectObject(m_pPen);
    pDC->SelectObject(m_pBrush);
}

void IShape::Move(CPoint ptBeginPoint, CPoint ptEndPoints)
{
    int nOffsetX = ptEndPoints.x - ptBeginPoint.x;
    int nOffsetY = ptEndPoints.y - ptBeginPoint.y;

    m_ptBegin.x += nOffsetX;
    m_ptEnd.x += nOffsetX;
    m_ptBegin.y += nOffsetY;
    m_ptEnd.y += nOffsetY;
}

void IShape::Rotate()
{
    RotateShape(m_ptBegin, m_ptEnd, 3.14 * 90.0 / 180);
}

IMPLEMENT_SERIAL(IShape, CObject, 1)
void IShape::Serialize(CArchive& archive)
{
    CObject::Serialize(archive);

    if (archive.IsStoring())
    {
        archive << m_ptBegin << m_ptEnd
            << m_logPen.lopnColor << m_logPen.lopnStyle << m_logPen.lopnWidth
            << m_logBrush.lbColor << m_logBrush.lbStyle << m_logBrush.lbHatch;
    }
    else
    {
        archive >> m_ptBegin >> m_ptEnd
            >> m_logPen.lopnColor >> m_logPen.lopnStyle >> m_logPen.lopnWidth
            >> m_logBrush.lbColor >> m_logBrush.lbStyle >> m_logBrush.lbHatch;
    }
}

void IShape::RotateShape(CPoint& beginPos, CPoint& endPos, double dblR)
{
    double x = (beginPos.x + endPos.x) / 2.0;
    double y = (beginPos.y + endPos.y) / 2.0;

    CPoint pos;

    pos.x = (long)((beginPos.x - x) * cos(dblR) - (beginPos.y - y) * sin(dblR) + x);
    pos.y = (long)((beginPos.x - x) * sin(dblR) - (beginPos.y - y) * cos(dblR) + y);


    beginPos = pos;

    pos.x = (long)((endPos.x - x) * cos(dblR) - (endPos.y - y) * sin(dblR) + x);
    pos.y = (long)((endPos.x - x) * sin(dblR) - (endPos.y - y) * cos(dblR) + y);

    endPos = pos;
}
