#include "pch.h"
#include "CMoveOperation.h"

CMoveOperation::CMoveOperation(IShape* pShape, CPoint ptBegin, CPoint ptEnd)
{
    m_pShape = pShape;
    m_ptBegin = ptBegin;
    m_ptEnd = ptEnd;
}

void CMoveOperation::Undo()
{
    m_pShape->Move(m_ptEnd, m_ptBegin);
}

void CMoveOperation::Redo()
{
    m_pShape->Move(m_ptBegin, m_ptEnd);
}
