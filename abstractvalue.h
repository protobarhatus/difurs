#ifndef ABSTRACTVALUE_H
#define ABSTRACTVALUE_H
#include "QString"
enum AbstractValueType
{
    VALUE_INCORRECT = -1,
    VALUE_ALGEBRAIC_EXPRESSION,
    VALUE_VECTOR,
    VALUE_MATRIX,
    VALUE_FUNCTION_TOKEN
};
class AlgebraicExpressionValue;
class MatrixValue;
class VectorValue;
class AbstractValue
{
public:
    AbstractValue();
    virtual QString toString() const = 0;
    virtual AbstractValueType getId() const = 0;

    virtual std::unique_ptr<AbstractValue> operator+(const std::unique_ptr<AbstractValue> & b) const = 0;
    virtual std::unique_ptr<AbstractValue> operator-(const std::unique_ptr<AbstractValue> & b) const = 0;
    virtual std::unique_ptr<AbstractValue> operator*(const std::unique_ptr<AbstractValue> & b) const = 0;
    virtual std::unique_ptr<AbstractValue> operator/(const std::unique_ptr<AbstractValue> & b) const = 0;
};

std::unique_ptr<AbstractValue> copy(const std::unique_ptr<AbstractValue> & val);
const AlgebraicExpressionValue* toAlgPtr(const std::unique_ptr<AbstractValue> &b);
const MatrixValue* toMatrixPtr(const std::unique_ptr<AbstractValue> &b);
const VectorValue* toVectorPtr(const std::unique_ptr<AbstractValue> &b);
#endif // ABSTRACTVALUE_H
