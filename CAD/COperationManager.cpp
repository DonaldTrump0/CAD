#include "pch.h"
#include "COperationManager.h"

void COperationManager::InsertOperation(IOperation* pOperation)
{
	m_stkToUndo.push(pOperation);

	while (!m_stkToRedo.empty())
	{
		IOperation* pOperation = m_stkToRedo.top();
		delete pOperation;
		m_stkToRedo.pop();
	}
}

void COperationManager::Undo()
{
	if (!m_stkToUndo.empty())
	{
		IOperation* pOperation = m_stkToUndo.top();
		m_stkToUndo.pop();
		m_stkToRedo.push(pOperation);
		pOperation->Undo();
	}
}

void COperationManager::Redo()
{
	if (!m_stkToRedo.empty())
	{
		IOperation* pOperation = m_stkToRedo.top();
		m_stkToRedo.pop();
		m_stkToUndo.push(pOperation);
		pOperation->Redo();
	}
}
