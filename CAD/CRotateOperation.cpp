#include "pch.h"
#include "CRotateOperation.h"

CRotateOperation::CRotateOperation(IShape* pShape)
{
    m_pShape = pShape;
}

void CRotateOperation::Undo()
{
    m_pShape->Rotate();
}

void CRotateOperation::Redo()
{
    m_pShape->Rotate();
}
