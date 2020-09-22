#pragma once
#include "IOperation.h"
#include "IShape.h"

class CDeleteOperation : public IOperation
{
public:
    CDeleteOperation(IShape* pShape, CList<IShape*>* pListShapes, int m_nIdx);

    virtual void Undo();
    virtual void Redo();

private:
    IShape* m_pShape;
    CList<IShape*>* m_pListShapes;
    int m_nIdx;
};

