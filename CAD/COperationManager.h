#pragma once

#include "IOperation.h"
#include <stack>
using namespace std;

class COperationManager
{
public:
    void InsertOperation(IOperation* pOperation);
    void Undo();
    void Redo();

private:
    stack<IOperation*> m_stkToUndo;
    stack<IOperation*> m_stkToRedo;
};

