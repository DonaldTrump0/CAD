#pragma once
#include "IOperation.h"
#include "IShape.h"

class CRotateOperation : public IOperation
{
public:
    CRotateOperation(IShape* pShape);

    virtual void Undo();
    virtual void Redo();

private:
    IShape* m_pShape;
};

