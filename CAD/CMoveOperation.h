#pragma once
#include "IOperation.h"
#include "IShape.h"

class CMoveOperation : public IOperation
{
public:
    CMoveOperation(IShape* pShape, CPoint ptBegin, CPoint ptEnd);

    virtual void Undo();
    virtual void Redo();

private:
    IShape* m_pShape;
    CPoint m_ptBegin;
    CPoint m_ptEnd;
};