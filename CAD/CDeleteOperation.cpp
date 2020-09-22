#include "pch.h"
#include "CDeleteOperation.h"

CDeleteOperation::CDeleteOperation(IShape* pShape, CList<IShape*>* pListShapes, int nIdx)
{
    m_pShape = pShape;
    m_pListShapes = pListShapes;
    m_nIdx = nIdx;
}

void CDeleteOperation::Undo()
{
    int i = 0;
    POSITION position = m_pListShapes->GetHeadPosition();
    while (i < m_nIdx)
    {
        m_pListShapes->GetNext(position);
        i++;
    }

    if (position == NULL)
    {
        m_pListShapes->AddTail(m_pShape);
    }
    else
    {
        m_pListShapes->InsertBefore(position, m_pShape);
    }
}

void CDeleteOperation::Redo()
{
    POSITION position = m_pListShapes->Find(m_pShape);
    m_pListShapes->RemoveAt(position);
}
