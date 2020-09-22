#pragma once
#include "IOperation.h"
#include "IShape.h"

class CAddOperation : public IOperation
{
public:
    CAddOperation(IShape* pShape, CList<IShape*>* pListShapes);
    ~CAddOperation();

    virtual void Undo();
    virtual void Redo();

private:
    IShape* m_pShape;
    CList<IShape*>* m_pListShapes;
};

