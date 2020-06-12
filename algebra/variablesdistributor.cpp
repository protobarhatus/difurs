#include "variablesdistributor.h"
#include "variablesnamedistributor.h"
#include "exception.h"
VariablesDistributor::VariablesDistributor()
{
    this->system_var_def = new VariablesDefinition;
    Variable::system_id_counter = this->first_system_num;
    Variable::integrating_constant_id_counter = this->first_integrate_constant;
}
VariablesDistributor& VariablesDistributor::get()
{
    static VariablesDistributor inst;
    return inst;
}
QString makeVariablesName(int id)
{
    //функция создавалась с расчетом на то, что нумерация идет с 0, но я забыл, что она должна идти с 1. поэтому мне проще сделать так
    if (id >= VariablesDistributor::firstSystemNum())
    {
        id = id - VariablesDistributor::firstSystemNum();
        QString name = "$system";
        while (id != 0)
        {
            name += QChar(id % 26 + 97);
            id /= 26;
        }
        return name;
    }
    id -= 1;
    if (id == 0)
        return "x";
    if (id == 1)
        return "y";
    if (id == 2)
        return "z";
    if (id >= 3 && id <= 25)
        return QChar(id + 94);
    if (id >= 26 && id <= 51)
        return QChar(id + 39);
    QString name = "$";
    while (id != 0)
    {
        name += QChar(id % 26 + 97);
        id /= 26;
    }
    return name;
}
Variable VariablesDistributor::createVariable(VariablesDefinition definition)
{
    if (Variable::id_counter >= get().first_system_num - 1)
        throw Exception();
    ++Variable::id_counter;
    QString name = makeVariablesName(Variable::id_counter);
    VariablesNameDistributor::addVariable(Variable::id_counter, name);
    VariablesDistributor& distr = VariablesDistributor::get();
    distr.variables.push_back(new VariablesDefinition(definition));
    return Variable(Variable::id_counter, distr.variables[Variable::id_counter - 1]);
}
void VariablesDistributor::deleteVariables()
{
    for (auto &it : this->variables)
        delete it;
    this->variables.clear();
}
void deleteVariables()
{
    VariablesDistributor::get().deleteVariables();
    Variable::id_counter = 0;
}
VariablesDefinition * VariablesDistributor::getVariablesDefinition(int id)
{

    VariablesDistributor & ref = VariablesDistributor::get();
    //if (id > ref.variables.size())
      //  return ref.system_var_def;
    if (id >= ref.first_integrate_constant)
        return VariablesDistributor::get().system_var_def;
    if (id >= ref.first_system_num)
        return ref.system_variables[id - ref.first_system_num];
    assert(id <= ref.variables.size());
    return ref.variables[id - 1];
}
Variable getVariable(int id)
{
    if (id >= VariablesDistributor::get().first_system_num)
        return systemVar(id);
    return Variable(id, VariablesDistributor::getVariablesDefinition(id));
}
int VariablesDistributor::firstSystemNum()
{
    return VariablesDistributor::get().first_system_num;
}

VariablesDistributor::~VariablesDistributor()
{
    for (auto &it : this->variables)
        delete it;
}
Variable systemVar(int num)
{
    //return Variable(num + VariablesDistributor::firstSystemNum(), makeVariablesName(num + VariablesDistributor::firstSystemNum()));
    return Variable(num, makeVariablesName(num));
}

Variable systemVar()
{
    ++Variable::system_id_counter;

    VariablesDistributor::get().system_variables.push_back(new VariablesDefinition(*VariablesDistributor::get().system_var_def));
    return Variable(Variable::system_id_counter - 1, makeVariablesName(Variable::system_id_counter - 1));
}

Variable integratingConstant()
{
    ++Variable::integrating_constant_id_counter;
    return Variable(Variable::integrating_constant_id_counter - 1, "C");
}

Variable systemVar(int min, int max)
{
    ++Variable::system_id_counter;
    VariablesDefinition * new_def = new VariablesDefinition;
    new_def->setMaxValue(max);
    new_def->setMinValue(min);
    VariablesDistributor::get().system_variables.push_back(new_def);

    Variable new_var = Variable(Variable::system_id_counter - 1, makeVariablesName(Variable::system_id_counter - 1));
    return new_var;
}

std::unique_ptr<AbstractExpression> integratingConstantExpr()
{
    return abs_ex(new Variable(integratingConstant()));
}

std::unique_ptr<AbstractExpression> systemVarExpr()
{
    return abs_ex(new Variable(systemVar()));
}

std::unique_ptr<AbstractExpression> getVariableExpr(int id)
{
    return abs_ex (new Variable(getVariable(id)));
}
