#pragma once
#include "pch.h"

class IShape : public CObject
{
public:
    IShape() {}
    virtual ~IShape();

    CPoint GetBeginPoint();
    void SetBeginPoint(const CPoint& ptBegin);
    CPoint GetEndPoint();
    void SetEndPoint(const CPoint& ptEnd);
    LOGPEN GetPen();
    void SetPen(LOGPEN& logPen);
    LOGBRUSH GetBrush();
    void SetBrush(LOGBRUSH& logBrush);

    virtual void Draw(CDC* pDC);
    virtual bool IsSelected(const CPoint& point) { return FALSE; }
    virtual void DrawSelected(CDC* pDC);
    void Move(CPoint ptBeginPoint, CPoint ptEndPoints);
    void Rotate();

    DECLARE_SERIAL(IShape)
    void Serialize(CArchive& archive);

private:
    void RotateShape(CPoint& beginPos, CPoint& endPos, double dblR);

protected:
    CPoint m_ptBegin;
    CPoint m_ptEnd;
    
    CPen* m_pPen;
    CBrush* m_pBrush;

    LOGPEN m_logPen;
    LOGBRUSH m_logBrush;
};

class IShpaeFactory
{
public:
    virtual IShape* Create() = 0;
};