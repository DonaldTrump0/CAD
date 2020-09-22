#pragma once
#include "IShape.h"

class CRectangle : public IShape
{
public:
    CRectangle() {}

    virtual void Draw(CDC* pDC);
    virtual bool IsSelected(const CPoint& point);
    virtual void DrawSelected(CDC* pDC);

    DECLARE_SERIAL(CRectangle)
    void Serialize(CArchive& archive);
};

class CRectangleFactory : public IShpaeFactory
{
public:
    virtual IShape* Create();
};