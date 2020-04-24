#include "differential.h"
#include "some_algebra_expression_conversions.h"
#include "number.h"
Differential::Differential(const std::unique_ptr<AbstractExpression> &arg)
{
    this->argument = copy(arg);
    this->simplify();
}

Differential::Differential(std::unique_ptr<AbstractExpression> &arg)
{
    this->argument = std::move(arg);
    this->simplify();
}

Differential::Differential(const Differential &cop)
{
    this->argument = copy(cop.argument);
}

Differential::Differential(Differential &&mov)
{
    this->argument = std::move(mov.argument);
}

Differential::~Differential()
{

}

AlgebraExpression Differential::getId() const
{
    return DIFFERENTIAL;
}

void Differential::simplify()
{
    SIM_IF_NEED
    this->argument->simplify();
    this->simplified = true;
}

bool Differential::operator==(AbstractExpression &right)
{
    if (right.getId() != DIFFERENTIAL)
        return false;
    return *this->argument == *static_cast<Differential*>(&right);
}

bool Differential::canDowncastTo(AlgebraExpression expr)
{
    if (this->argument->getId() == NUMBER)
        return true;
    //пока не раскрываем дифференциал, т. к. непонятно по какой переменной в общем случае, но потом - будем
    return false;
}

std::unique_ptr<AbstractExpression> Differential::downcastTo(AlgebraExpression expr)
{
    if (this->argument->getId() == NUMBER)
        return copy(zero);
    assert(false);
    return nullptr;
}

std::set<int> Differential::getSetOfPolyVariables() const
{
    return std::set<int>();
}

std::set<int> Differential::getSetOfVariables() const
{
    return this->argument->getSetOfVariables();
}

std::set<QString> Differential::getSetOfFunctions() const
{
    //считать ли его функцией? Вряд ли это что-то сломает, но если не считать, то может и что-то полететь
    std::set<QString> s;
    s.insert(this->makeStringOfExpression());
    return s;
}

Number Differential::getMaxDegreeOfVariable(int id)
{
    return Number::makeErrorNumber();
}

void Differential::_qDebugOut()
{
    //да мне уже и нахрен не нужна эта функция, зачем я ее заполняю?
}

QString Differential::makeStringOfExpression() const
{
    if (this->argument->getId() > 0)
        return "d" + this->argument->makeStringOfExpression();
    return "d(" + this->argument->makeStringOfExpression() + ")";
}

double Differential::getApproximateValue()
{
    assert(false);
    return std::nan("0");
}

double Differential::getApproximateValue(const std::function<double (VariablesDefinition *)> &choosing_value_rule)
{
    //нельзя брать значение у бесконечно маленькой величины
    assert(false);
    return 0;
}

int Differential::getPositionRelativelyZeroIfHasVariables()
{
    return 0;
}

QString Differential::getStringArgument() const
{
    return this->argument->makeStringOfExpression();
}

std::unique_ptr<AbstractExpression> Differential::getArgumentMoved()
{
    return std::move(this->argument);
}

std::unique_ptr<AbstractExpression> Differential::changeSomePartOn(QString part, std::unique_ptr<AbstractExpression> &on_what)
{
    return this->argument->changeSomePartOn(part, on_what);
}

std::unique_ptr<AbstractExpression> Differential::getArgumentsCopy()
{
    return copy(this->argument);
}

std::unique_ptr<AbstractExpression> Differential::derivative(int var) const
{
    //производная от дифференциала? может быть дифференциал более высокого порядка? пока хз
    assert(false);
    return nullptr;
}

AbstractExpression *Differential::getArgument()
{
    return this->argument.get();
}

std::unique_ptr<AbstractExpression> Differential::antiderivative(int var) const
{
    if (this->argument->getId() == var)
        return copy(this->argument);
    assert(false);
    return nullptr;
}

bool Differential::operator<(const AbstractExpression &expr) const
{
    assert(expr.getId() == DIFFERENTIAL);
    return AbstractExpression::less(this->argument.get(), static_cast<Differential*>(const_cast<AbstractExpression*>(&expr))->argument.get());
}
