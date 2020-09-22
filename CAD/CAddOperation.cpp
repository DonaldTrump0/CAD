#include "pch.h"
#include "CAddOperation.h"

CAddOperation::CAddOperation(IShape* pShape, CList<IShape*>* pListShapes)
{
    m_pShape = pShape;
    m_pListShapes = pListShapes;
}

CAddOperation::~CAddOperation()
{
    delete m_pShape;
}

void CAddOperation::Undo()
{
    m_pListShapes->RemoveTail();
}

void CAddOperation::Redo()
{
    m_pListShapes->AddTail(m_pShape);
}
