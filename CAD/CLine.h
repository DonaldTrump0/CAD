#pragma once
#include "IShape.h"

class CLine : public IShape
{
public:
    CLine() {}

    virtual void Draw(CDC* pDC);
    virtual bool IsSelected(const CPoint& point);
    virtual void DrawSelected(CDC* pDC);

    DECLARE_SERIAL(CLine)
    void Serialize(CArchive& archive);
};

class CLineFactory : public IShpaeFactory
{
public:
    virtual IShape* Create();
};