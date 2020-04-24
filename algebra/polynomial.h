#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <algebra/abstractexpression.h>
#include <memory>
#include <list>
#include <algebra/number.h>
#include <tuple>
#include "trigonometrical_conversions.h"
#include <array>
class Fractal;

class Polynomial : public AbstractExpression
{
public:
    Polynomial();
    Polynomial(std::list<std::unique_ptr<Fractal>> & list);
    Polynomial(std::list<std::unique_ptr<Fractal>> && list);
    Polynomial(const Polynomial & pol);
    Polynomial(Polynomial && pol);
    Polynomial(AbstractExpression * fsum, AbstractExpression * secsum = std::make_unique<Number>(0).get());
    Polynomial& operator=(Polynomial && pol);
    void addMonomial(Fractal* fractal);
    void addMonomial(std::unique_ptr<Fractal> && monom);
    virtual void simplify() override;
    virtual AlgebraExpression getId() const override;
    virtual bool operator==(AbstractExpression & right) override;
    bool isMonomial();
    virtual bool canDowncastTo(AlgebraExpression expr) override;
    virtual std::unique_ptr<AbstractExpression> downcastTo(AlgebraExpression expr) override;
    //assert if cannot
    //i don't remember why do i need this function, maybe should delete?
    std::unique_ptr<AbstractExpression> turnIntoMonomial();
    bool canBecameFractal();
    bool isZero();
    void merge(Polynomial * polynomial);
    void merge(std::unique_ptr<Polynomial> && polynom);
    //makes coefficients's GCF = 1. Returned number is number polynomial has divided on
    Number reduce();
    //returns result and remainder. makes copy of both divider and dividend. both of them must be simplified
    std::pair<std::unique_ptr<Polynomial>, std::unique_ptr<Polynomial>> divide(Polynomial * dividend);
    virtual std::set<int> getSetOfPolyVariables() const override;
    virtual std::set<int> getSetOfVariables() const override;
    virtual std::set<QString> getSetOfFunctions() const override;
    virtual Number getMaxDegreeOfVariable(int id) override;
    std::list<Fractal*> getMonomialsPointers();
    Polynomial getCoefficientOfMaxDegreeOfVariable(int id);
    virtual void _qDebugOut() override;

    Polynomial operator+(AbstractExpression * expr);
    Polynomial operator-(AbstractExpression * expr);
    Polynomial operator*(AbstractExpression * expr);
    Fractal operator/(AbstractExpression * expr);
    virtual QString makeStringOfExpression() const override;
    std::unique_ptr<AbstractExpression> reduceCommonPart();
    // if it is a sum like sqrt(3)+sqrt(5), it returns result on formula (a+b)(a-b)=a^2-b^2. returns result and multiplier
    std::pair<std::unique_ptr<AbstractExpression>, std::unique_ptr<AbstractExpression>> multiplyIrrationalSumOnAppropriateFormula();
    //is sum of rational and irrational numbers without any variables
    bool isIrrationalSum();
    bool canGetRidOfIrrationalytyByMultiplying();
    std::unique_ptr<AbstractExpression> tryToTakeRoot(long long int root);
    std::unique_ptr<AbstractExpression> toDegree(long long int degree);

    virtual double getApproximateValue() override;
    virtual double getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule) override;
    void changeSign();
    //нужно для четных/нечетных функций. проверяет выражения такие как a - b или b - a чтобы привести их к единой форме
    bool isNeedToChangeSignToLeadTheAcceptedForm();

    //имеет ли триг. функцию означает все степени кроме квадрата
    //нулевое - имеет ли синус квадрат, первое - имеет ли синус, второе - имеет ли косинус квадрат, третье - имеет ли косинус, четвертое - имеет ли тангенс квадрат
    //пятое - имеет ли тангенс,
    //шестое - имеет ли котангенс квадрат, седьмое - имеет ли котангенс
    std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool>> checkTrigonometricalFunctions();
    virtual abs_ex changeSomePartOn(QString part, abs_ex & on_what) override;
    std::unique_ptr<Fractal> toCommonDenominator();
    bool isFractionalCoefficientsAllowed();
    void setFractionalCoefficientsAllowed(bool allow);
    std::pair<abs_ex, abs_ex> checkIfItIsLinearFunction(int var) const;

    virtual abs_ex derivative(int var) const override;
    virtual abs_ex antiderivative(int var) const override;
    //проверяет, является ли полином квадратичной функцией и возвращает коэффициенты a, b, c в выражении вида ax^2+bx+c
    std::array<abs_ex, 3> checkQuadraticFunction(int var) const;
private:
    bool casted_trigonometry;
    void castTrigonometry();
    void castTrigonometryArguments();
    std::map<QString, TrigonometricalFunctionsCastType> getTrigonometricalFunctionToTurnIntoIt();
    void castTrigonometricalFunctions();
    virtual int getPositionRelativelyZeroIfHasVariables() override;
    void pushBack(std::unique_ptr<Fractal> && fractal);
    std::unique_ptr<AbstractExpression> tryToTakeRootOfVariablesPolynomial(long long int root);
    //only square root
    std::unique_ptr<AbstractExpression> tryToTakeRootOfNonVariablesPolynomial();
    std::list<std::unique_ptr<Fractal>> monomials;
    //this functions is for situations when merging can make iterator invalid
    void mergeWithoutSimplifiyng(Polynomial * polynomial, Number coe = 1);
    void mergeWithoutSimplifiyng(std::unique_ptr<Polynomial> && polynom, Number coe = 1);
    void leadLikeMembers(std::list<std::unique_ptr<Fractal>>::iterator to_whom);
    void leadLikeMembers();
    void openParentheses();
    bool foldMonomials(std::list<std::unique_ptr<Fractal>>::iterator first, std::list<std::unique_ptr<Fractal>>::iterator second);
    void removeZeroes();
    virtual bool operator<(const AbstractExpression & right) const override;
    //for dividing
    bool is_fractional_coefficients_allowed = false;
};

#endif // POLYNOMIAL_H
