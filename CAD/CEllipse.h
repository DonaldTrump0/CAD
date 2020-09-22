#pragma once
#include "IShape.h"

class CEllipse : public IShape
{
public:
    CEllipse() {}

    virtual void Draw(CDC* pDC);
    virtual bool IsSelected(const CPoint& point);
    virtual void DrawSelected(CDC* pDC);

    DECLARE_SERIAL(CEllipse)
    void Serialize(CArchive& archive);
};

class CEllipseFactory : public IShpaeFactory
{
public:
    virtual IShape* Create();
};