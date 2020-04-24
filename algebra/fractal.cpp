#include "fractal.h"
#include "assert.h"
#include "some_algebra_expression_conversions.h"
#include "number.h"
#include "algorithm"
#include "degree.h"
#include "polynomial.h"
#include "exception.h"
#include <cmath>
#include "cosinus.h"
#include "sinus.h"
#include "abstractexpression.h"
#include "logarithm.h"
#include "absolutevalue.h"
#include "tangent.h"
#include "cotangent.h"
#include "variablesdistributor.h"
#include "arctangent.h"
Fractal::Fractal() : coefficient(1)
{

}
AlgebraExpression Fractal::getId() const
{
    return FRACTAL;
}
Fractal::Fractal(AbstractExpression * num, AbstractExpression * denum, Number coe) : coefficient(coe)
{
    this->pushBackToNumerator(makeAbstractExpression(num->getId(), num));
    this->pushBackToDenominator(makeAbstractExpression(denum->getId(), denum));
    this->simplify();
}
Fractal::Fractal(const fractal_argument * num, const fractal_argument * denum, Number coe) : coefficient(coe)
{
    for (auto &it : *num)
    {
        this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    }
    for (auto &it : *denum)
    {
        this->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    }
    this->simplify();
}

Fractal::Fractal(const fractal_argument &num, const fractal_argument &denum, Number coe) : coefficient(coe)
{
    for (auto &it : num)
    {
        this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    }
    for (auto &it : denum)
    {
        this->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    }
    this->simplify();
}
Fractal::Fractal(fractal_argument && num, fractal_argument && denum, Number coe) : coefficient(coe)
{
    for (auto &it : num)
        this->pushBackToNumerator(std::move(it));
    for (auto &it : denum)
        this->pushBackToDenominator(std::move(it));
    this->simplify();
}

Fractal::Fractal(fractal_argument &&num, Number coe) : coefficient(coe)
{
    for (auto &it : num)
        this->pushBackToNumerator(std::move(it));
    this->simplify();
}

Fractal::Fractal(abs_ex &&num, abs_ex &&denum, Number coe)
{
    this->pushBackToNumerator(std::move(num));
    this->pushBackToDenominator(std::move(denum));
    this->coefficient = coe;
    this->simplify();
}
Fractal::Fractal(fractal_argument * num, Number coe) : coefficient(coe)
{
    for (auto &it : *num)
    {
        this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    }
    this->simplify();
}

Fractal::Fractal(const fractal_argument &num, Number coe) : coefficient(coe)
{
    for (auto &it : num)
    {
        this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    }
    this->simplify();
}
Fractal::Fractal(AbstractExpression * num, Number coe) : coefficient(coe)
{
    this->pushBackToNumerator(makeAbstractExpression(num->getId(), num));
    this->simplify();
}
Fractal::Fractal(Number coe) : coefficient(coe)
{

}
bool Fractal::operator<(const AbstractExpression &right) const
{
    assert(right.getId() == FRACTAL);
    Fractal * frac = static_cast<Fractal*>(const_cast<AbstractExpression*>(&right));
    if (less(&this->coefficient, &frac->coefficient))
        return true;
    if (less(&frac->coefficient, &this->coefficient))
        return false;
    if (this->numerator.size() < frac->numerator.size())
        return true;
    if (this->numerator.size() > frac->numerator.size())
        return false;
    auto it1 = frac->numerator.begin();
    for (auto &it : this->numerator)
    {
        if (less(it.get(), it1->get()))
            return true;
        if (less(it1->get(), it.get()))
            return false;
        ++it1;
    }
    if (this->denominator.size() < frac->denominator.size())
        return true;
    if (this->denominator.size() > frac->denominator.size())
        return false;
    auto it2 = frac->denominator.begin();
    for (auto &it : this->denominator)
    {
        if (less(it.get(), it2->get()))
            return true;
        if (less(it2->get(), it.get()))
            return false;
        ++it2;
    }
    return false;
}
bool Fractal::operator==(AbstractExpression &right)
{
    if (right.getId() != FRACTAL)
        return false;
    Fractal * frac = static_cast<Fractal*>(&right);
    if (this->coefficient != frac->coefficient)
        return false;
    if (this->numerator.size() != frac->numerator.size() || this->denominator.size() != frac->denominator.size())
        return false;
    auto it1 = frac->numerator.begin();
    for (auto &it : this->numerator)
    {
        if (*it1->get() != *it.get())
            return false;
        ++it1;
    }
    auto it2 = frac->denominator.begin();
    for (auto &it : this->denominator)
    {
        if (*it.get() != *it2->get())
            return false;
        ++it2;
    }
    return true;
}

bool Fractal::isLetterPartEquallTo(Fractal *frac)
{
    if (this->numerator.size() != frac->numerator.size() ||
            this->denominator.size() != frac->denominator.size())
        return false;
    auto it1 = frac->numerator.begin();
    for (auto &it : this->numerator)
    {
        if (*it1->get() != *it.get())
            return false;
        ++it1;
    }
    auto it2 = frac->denominator.begin();
    for (auto &it : this->denominator)
    {
        if (*it.get() != *it2->get())
            return false;
        ++it2;
    }
    return true;
}
Fractal::Fractal(const Fractal & right) : coefficient(right.coefficient)
{
    for (auto &it : right.numerator)
        this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : right.denominator)
        this->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    this->simplified = right.simplified;
}
Fractal::Fractal(Fractal && expr) : coefficient(expr.coefficient)
{
    for (auto &it : expr.numerator)
        this->pushBackToNumerator(std::move(it));
    for (auto &it : expr.denominator)
        this->pushBackToDenominator(std::move(it));
    this->simplified = expr.simplified;
}
std::pair<fractal_argument *, fractal_argument *> Fractal::getFractal() const
{
    *const_cast<bool*>(&this->simplified) = false;
    return std::pair<fractal_argument *, fractal_argument *>(const_cast<fractal_argument*>(&this->numerator), const_cast<fractal_argument*>(&this->denominator));
}
Number Fractal::getCoefficient()
{
    return this->coefficient;
}
bool Fractal::canDowncastTo(AlgebraExpression expr)
{
    return (this->denominator.empty() && this->coefficient.getDenominator() == 1 && this->coefficient.getNumerator() == 1 && this->numerator.size() == 1 &&
            (this->numerator.begin()->get()->getId() == expr || (this->numerator.begin()->get()->getId() > 0 && expr > 0))) ||
            (expr == NUMBER && this->numerator.empty() && this->denominator.empty()) ||
            (expr == NUMBER && this->coefficient.getNumerator() == 0);
}
std::unique_ptr<AbstractExpression> Fractal::downcastTo(AlgebraExpression expr)
{
    assert(this->canDowncastTo(expr));
    if (expr == NUMBER)
    {
        return makeAbstractExpression(NUMBER, &this->coefficient);
    }
    return makeAbstractExpression(expr, this->numerator.begin()->get());
}
bool Fractal::isZero() const
{
    return this->coefficient.getNumerator() == 0;
}
void Fractal::setCoefficinet(Number num)
{
    NONCONST
    this->coefficient = num;
}
fractal_argument operator*(fractal_argument & fmult, fractal_argument & smult)
{
    fractal_argument result;
    for (auto &it : fmult)
        result.push_back(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : smult)
        result.push_back(makeAbstractExpression(it->getId(), it.get()));
    //std::sort(result.begin(), result.end(), &AbstractExpression::less);
    Fractal frac(&result);
    result = std::move(*frac.getFractal().first);
    return result;
}

std::unique_ptr<Fractal> Fractal::makeZeroFractal()
{
    return std::make_unique<Fractal>((Number(0)));
}
bool operator==(fractal_argument & left, fractal_argument & right)
{
    if (left.size() != right.size())
        return false;
    fractal_argument::iterator it1 = right.begin();
    for (auto &it : left)
    {
        if (*it1->get() != *it.get())
            return false;
    }
    return true;
}
bool Fractal::isDenominatorEqualToOne()
{
    return this->denominator.empty() && this->coefficient.getDenominator() == 1;
}
std::unique_ptr<Fractal> Fractal::operator+(const std::unique_ptr<Fractal> & sec_sum)
{
    if (this->denominator == sec_sum->denominator && this->coefficient == sec_sum->coefficient)
    {
        std::unique_ptr<AbstractExpression> newnum_ptr = *static_cast<AbstractExpression*>(new Fractal(&this->numerator, this->coefficient.getNumerator())) +
                *static_cast<AbstractExpression*>(new Fractal(&sec_sum->numerator, sec_sum->coefficient.getNumerator()));
        fractal_argument newnum;
        newnum.push_back(std::move(newnum_ptr));
        std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&newnum, &this->denominator, Number(1, this->coefficient.getDenominator())/*, this->coefficient * sec_sum->coefficient*/));
        result->simplify();
        return result;
    }
    /*
    fractal_argument left_sum = this->numerator * (sec_sum->denominator / this->denominator);
    fractal_argument right_sum = sec_sum->numerator * this->denominator;
    std::unique_ptr<AbstractExpression> newnum_ptr = *static_cast<AbstractExpression*>(new Fractal(&left_sum)) +
            *static_cast<AbstractExpression*>(new Fractal(&right_sum));
    fractal_argument newnum;
    newnum.push_back(std::move(newnum_ptr));
    fractal_argument newdenum = this->denominator * sec_sum->denominator;
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&newnum, &newdenum, this->coefficient * sec_sum->coefficient));
    result->simplify();*/

    std::unique_ptr<AbstractExpression> this_denum = std::unique_ptr<AbstractExpression>(new Fractal(&this->denominator));
    std::unique_ptr<AbstractExpression> this_num_denum = std::unique_ptr<AbstractExpression>(new Number(this->coefficient.getDenominator()));
    this_denum = this_denum * this_num_denum;
    std::unique_ptr<AbstractExpression> sec_sum_denum = std:: unique_ptr<AbstractExpression>(new Fractal(&sec_sum->denominator));
    std::unique_ptr<AbstractExpression> sec_sum_num_denum = std::unique_ptr<AbstractExpression>(new Number(sec_sum->coefficient.getDenominator()));
    sec_sum_denum = sec_sum_denum * sec_sum_num_denum;

    std::unique_ptr<AbstractExpression> this_num = std::unique_ptr<AbstractExpression>(new Fractal(&this->numerator));
    std::unique_ptr<AbstractExpression> this_num_num = std::unique_ptr<AbstractExpression>(new Number(this->coefficient.getNumerator()));
    this_num = this_num * this_num_num;
    std::unique_ptr<AbstractExpression> sec_sum_num = std::unique_ptr<AbstractExpression>(new Fractal(&sec_sum->numerator));
    std::unique_ptr<AbstractExpression> sec_sum_num_num = std::unique_ptr<AbstractExpression>(new Number(sec_sum->coefficient.getNumerator()));
    sec_sum_num = sec_sum_num * sec_sum_num_num;
    std::unique_ptr<AbstractExpression> left_sum = this_num * sec_sum_denum;
    std::unique_ptr<AbstractExpression> right_sum = sec_sum_num * this_denum;

    return std::unique_ptr<Fractal>(new Fractal((left_sum + right_sum).get(), (this_denum * sec_sum_denum).get()));
}
std::unique_ptr<Fractal> Fractal::operator-(const std::unique_ptr<Fractal> &subtrahend)
{
    return *this + subtrahend * std::unique_ptr<Fractal>(new Fractal(-1));
}
void Fractal::simplify()
{
    SIM_IF_NEED
    for (auto &it: this->numerator)
    {
        it->simplify();
        it = it->downcast();
    }
    for (auto &it: this->denominator)
    {
        it->simplify();
        it = it->downcast();
    }
    this->takeCommonPartOfPolynomials();
    for (fractal_argument::iterator it = this->numerator.begin(); it != this->numerator.end();)
    {
        if (it->get()->getId() == FRACTAL)
        {
            this->addFractal(static_cast<Fractal*>(it->get()));
            it = this->numerator.erase(it);
        }
        else
            ++it;
    }
    for (fractal_argument::iterator it = this->denominator.begin(); it != this->denominator.end();)
    {
        if (it->get()->getId() == FRACTAL)
        {
            this->addFractal(static_cast<Fractal*>(it->get()), true);
            it = this->denominator.erase(it);
        }
        else
            ++it;
    }
    this->coefficient.simplify();
    //std::sort(this->numerator.begin(), this->numerator.end(), &AbstractExpression::less);
   // std::sort(this->denominator.begin(), this->denominator.end(), &AbstractExpression::less);

    this->numerator.sort(&AbstractExpression::lessToSort);
    this->denominator.sort(&AbstractExpression::lessToSort);
    this->multiplyIrrationalSums();
    this->getRidOfIrrationalityInDenominator();
    this->reduceDegrees();
    this->reduceSameMembers();
    this->reduceMembersWithDowngradingDegree();
    this->reduceComparibleDegrees();
//    this->getRidOfRootsInDenominator();
    this->turnDegreesIntoList();
    this->reducePolynomialsCoefficient();
    //ye, its extra, but it is unimportant becouse fractals in requlary geometric are small. it's maden for reduced polynomial.
    this->reduceSameMembers();
    this->reducePolynomials();
    this->takeCommonPartOfPolynomials();
    for (auto &it: this->numerator)
        it = it->downcast();
    for (auto &it: this->denominator)
        it = it->downcast();
    this->reduceSameMembers();
    this->pullNumbersIntoCoefficient();
    this->setSameMembersIntoDegree();
    this->castTrigonometry();
    this->castTrigonometryArguments();
    this->simplified = true;
}
void Fractal::reduceDegrees()
{
    NONCONST
    for (auto &it : this->numerator)
    {
        if (it->getId() == DEGREE)
            this->setCoefficinet( this->coefficient * static_cast<Degree*>(it.get())->reduce());
    }
    for (auto &it : this->denominator)
    {
        if (it->getId() == DEGREE)
            this->setCoefficinet( this->coefficient / static_cast<Degree*>(it.get())->reduce());
    }
}
void Fractal::pullNumbersIntoCoefficient()
{
    NONCONST
    for (auto it = this->numerator.begin(); it != this->numerator.end();)
    {
        if (it->get()->getId() == NUMBER)
        {
            this->setCoefficinet( this->coefficient * *static_cast<Number*>(it->get()));
            it = this->numerator.erase(it);
        }
        else
            ++it;
    }
    for (auto it = this->denominator.begin(); it != this->denominator.end();)
    {
        if (it->get()->getId() == NUMBER)
        {
            this->setCoefficinet( this->coefficient / *static_cast<Number*>(it->get()));
            it = this->denominator.erase(it);
        }
        else
            ++it;
    }
    this->coefficient.simplify();
}
void Fractal::reduceSameMembers()
{
    NONCONST
    //numerator and denominator must be sorted
    fractal_argument::iterator to_start_in_denominator = this->denominator.begin();
    bool has_erased = false;
    for (fractal_argument::iterator it1 = this->numerator.begin(); it1 != this->numerator.end();)
    {
        fractal_argument::iterator it2 = to_start_in_denominator;
        while (it2 != this->denominator.end() && !has_erased)
        {
            if (*it1->get() == *it2->get())
            {
                it1 = this->numerator.erase(it1);
                has_erased = true;
                it2 = this->denominator.erase(it2);
                to_start_in_denominator = it2;
            }
            else
                ++it2;
        }
        if (has_erased)
            has_erased = false;
        else
            ++it1;
    }
}

void Fractal::reduceMembersWithDowngradingDegree()
{
    NONCONST
    //this method assumes that reduceSameMembers was called before it, so one of reducing member should be degree and they won't dissapeare both
    //(except case that might happen if there can be situation when the same value is written in differen ways)
    //numerator and denominator must be sorted
   // fractal_argument::iterator to_start_in_denominator = this->denominator.begin();
    bool has_erased = false;
    for (fractal_argument::iterator it1 = this->numerator.begin(); it1 != this->numerator.end();)
    {
        fractal_argument::iterator it2 = this->denominator.begin();
        while (it2 != this->denominator.end() && it1 != this->numerator.end())
        {
           if (canReduceWithDowngradingDegree(it1->get(), it2->get()))
           {
               std::unique_ptr<AbstractExpression> newDegree = *Degree::getDegreeOfExpression(it1->get()) - *Degree::getDegreeOfExpression(it2->get());
               std::unique_ptr<AbstractExpression> arg = makeAbstractExpression(Degree::getArgumentOfDegree(it1->get())->getId(), Degree::getArgumentOfDegree(it1->get()));
               has_erased = true;
               it1 = this->numerator.erase(it1);
               it2 = this->denominator.erase(it2);

               if (newDegree->getId() == NUMBER && (static_cast<Number*>(newDegree.get()))->compareWith(0) < 0)
               {
                   std::unique_ptr<AbstractExpression> multiplier(new Number(-1));
                   this->pushBackToDenominator(std::unique_ptr<AbstractExpression>(new Degree(std::move(arg), newDegree * multiplier)));
               }
               else
                   this->pushBackToNumerator(std::unique_ptr<AbstractExpression>(new Degree(std::move(arg), std::move(newDegree))));
           }
           else
               ++it2;
        }
        if (has_erased)
            has_erased = false;
        else
            ++it1;
    }
}
void Fractal::reduceComparibleDegrees()
{
    NONCONST
    for (auto it = this->numerator.begin(); it != this->numerator.end(); )
    {
        if (it->get()->getId() == DEGREE && static_cast<Degree*>(it->get())->canDowncastTo(NUMBER))
        {
            auto deg = it->get()->downcastTo(NUMBER);
            Number * num = static_cast<Number*>(deg.get());

                this->setCoefficinet( this->coefficient * *num);
                it = this->numerator.erase(it);

        }
        else
            ++it;
    }
    for (auto it = this->denominator.begin(); it != this->denominator.end(); )
    {
        if (it->get()->getId() == DEGREE && static_cast<Degree*>(it->get())->canDowncastTo(NUMBER))
        {
            auto deg = it->get()->downcastTo(NUMBER);
            Number * num = static_cast<Number*>(deg.get());
            if (num->isZero())
                throw Exception();
            else
            {
                this->setCoefficinet( this->coefficient * *num);
                it = this->denominator.erase(it);
            }
        }
        else
            ++it;
    }
}
void Fractal::turnDegreesIntoList()
{
    NONCONST
    for (fractal_argument::iterator it = this->numerator.begin(); it != this->numerator.end();)
    {
        if (it->get()->getId() == DEGREE)
        {
            Degree * it_deg = static_cast<Degree*>(it->get());
            if (it_deg->canGetListOfArguments())
            {
                auto list = std::move(it_deg->getListOfArguments());
                if (!list.second)
                {
                    for (auto &it1 : *list.first)
                        this->pushBackToNumerator(std::move(it1));
                }
                else
                {
                    for (auto &it1 : *list.first)
                        this->pushBackToDenominator(std::move(it1));
                }
                it = this->numerator.erase(it);
            }
            else
                ++it;
        }
        else
            ++it;
    }
    for (fractal_argument::iterator it = this->denominator.begin(); it != this->denominator.end();)
    {
        if (it->get()->getId() == DEGREE)
        {
            Degree * it_deg = static_cast<Degree*>(it->get());
            if (it_deg->canGetListOfArguments())
            {
                auto list = it_deg->getListOfArguments();
                if (list.second)
                {
                    for (auto &it1 : *list.first)
                        this->pushBackToNumerator(std::move(it1));
                }
                else
                {
                    for (auto &it1 : *list.first)
                        this->pushBackToDenominator(std::move(it1));
                }
                it = this->denominator.erase(it);
            }
            else
                ++it;
        }
        else
            ++it;
    }
}
bool Fractal::isPolynomial()
{
    return this->denominator.empty() && this->numerator.size() == 1 && this->numerator.begin()->get()->getId() == POLYNOMIAL;
}
void Fractal::addFractal(Fractal *fractal, bool as_denominator)
{
    NONCONST
    if (as_denominator)
    {
        this->setCoefficinet( this->coefficient / fractal->coefficient);
        for (auto &it : fractal->numerator)
            this->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
        for (auto &it : fractal->denominator)
            this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    }
    else
    {
        this->setCoefficinet( this->coefficient * fractal->coefficient);
        for (auto &it : fractal->numerator)
            this->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
        for (auto &it : fractal->denominator)
            this->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    }
}
void Fractal::reducePolynomialsCoefficient()
{
    NONCONST
    for (auto &it : this->numerator)
    {
        if (it->getId() == POLYNOMIAL)
            this->setCoefficinet( this->coefficient * static_cast<Polynomial*>(it.get())->reduce());

    }
    for (auto &it : this->denominator)
    {
        if (it->getId() == POLYNOMIAL)
            this->setCoefficinet( this->coefficient / static_cast<Polynomial*>(it.get())->reduce());
    }
}
std::set<int> Fractal::getSetOfPolyVariables() const
{
    std::set<int> set;
    for (auto &it : this->numerator)
    {
        std::set<int> s = it->getSetOfPolyVariables();
        set.insert(s.begin(), s.end());
    }
    for (auto &it : this->denominator)
    {
        std::set<int> s = it->getSetOfPolyVariables();
        set.insert(s.begin(), s.end());
    }

    return set;
}
std::set<int> Fractal::getSetOfVariables() const
{
    std::set<int> set;
    for (auto &it : this->numerator)
    {
        std::set<int> s = it->getSetOfVariables();
        set.insert(s.begin(), s.end());
    }
    for (auto &it : this->denominator)
    {
        std::set<int> s = it->getSetOfVariables();
        set.insert(s.begin(), s.end());
    }

    return set;
}

std::set<QString> Fractal::getSetOfFunctions() const
{
    std::set<QString> set;
    for (auto &it : this->numerator)
    {
        auto itset = it->getSetOfFunctions();
        set.merge(itset);
    }
    for (auto &it : this->denominator)
    {
        auto itset = it->getSetOfFunctions();
        set.merge(itset);
    }
    return set;
}
Number Fractal::getMaxDegreeOfVariable(int id)
{
    Number deg(std::numeric_limits<long long int>::min() + 1);
    bool has_var = false;
    for (auto &it : this->numerator)
    {
        Number it_deg = it->getMaxDegreeOfVariable(id);
        if (it_deg.isCorrect())
        {
            has_var = true;
            if (it_deg.compareWith(deg) > 0)
                deg = it_deg;
        }
        else
            return Number::makeErrorNumber();
    }
    for (auto &it : this->denominator)
    {
        Number it_deg = it->getMaxDegreeOfVariable(id) * -1;
        if (it_deg.isCorrect())
        {
            has_var = true;
            if (it_deg.compareWith(deg) > 0)
                deg = it_deg;
        }
        else
            return Number::makeErrorNumber();
    }
    if (has_var)
        return deg;
    return 0;
}

std::unique_ptr<Fractal> Fractal::getFractalWithoutVariable(int id) const
{
    std::unique_ptr<Fractal> result = std::make_unique<Fractal>(this);
    for (auto it = result->numerator.begin(); it != result->numerator.end();)
    {
        if (Degree::getArgumentOfDegree(it->get())->getId() == id)
            it = result->numerator.erase(it);
        else
            ++it;
    }
    for (auto it = result->denominator.begin(); it != result->denominator.end();)
    {
        if (Degree::getArgumentOfDegree(it->get())->getId() == id)
            it = result->denominator.erase(it);
        else
            ++it;
    }
    return result;
}

void Fractal::reducePolynomials()
{
    NONCONST
    for (auto it1 = this->numerator.begin(); it1 != this->numerator.end(); ++it1)
    {
        for (auto it2 = this->denominator.begin(); it2 != this->denominator.end(); ++it2)
        {

            if (it1->get()->getId() == POLYNOMIAL && it2->get()->getId() == POLYNOMIAL)
            {
                auto gcf = gcd(static_cast<Polynomial*>(it1->get()), static_cast<Polynomial*>(it2->get()));
               // if (gcf == nullptr)
               //     continue;
                //qDebug() << gcf->makeStringOfExpression();
                auto it1_pol = static_cast<Polynomial*>(it1->get())->divide(gcf.get()).first;
                if (it1_pol == nullptr)
                    continue;
                auto it1_expr = makeAbstractExpression(POLYNOMIAL, it1_pol.get());
                it1->swap(it1_expr);
                *it1 = it1->get()->downcast();
                auto it2_pol = static_cast<Polynomial*>(it2->get())->divide(gcf.get()).first;
                auto it2_expr = makeAbstractExpression(POLYNOMIAL, it2_pol.get());
                it2->swap(it2_expr);
                *it2 = it2->get()->downcast();
            }
        }
    }
    this->reduceSameMembers();
}
std::unique_ptr<Fractal> Fractal::operator*(const std::unique_ptr<Fractal> & right)
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient * right->coefficient);
    for (auto &it : right->numerator)
        result->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : right->denominator)
        result->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    result->simplify();
    return result;
}
std::unique_ptr<Fractal> Fractal::operator*(const Fractal & right)
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient * right.coefficient);

    for (auto &it : right.numerator)
        result->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : right.denominator)
        result->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    result->simplify();
    return result;
}
std::unique_ptr<Fractal> Fractal::operator/(const std::unique_ptr<Fractal> & right)
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient / right->coefficient);
    for (auto &it : right->numerator)
        result->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : right->denominator)
        result->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    result->simplify();
    return result;
}
std::unique_ptr<Fractal> Fractal::operator/(const Fractal & right) const
{
    std::unique_ptr<Fractal> result = std::unique_ptr<Fractal>(new Fractal(&this->numerator, &this->denominator));
    result->setCoefficinet( this->coefficient / right.coefficient);
    for (auto &it : right.numerator)
        result->pushBackToDenominator(makeAbstractExpression(it->getId(), it.get()));
    for (auto &it : right.denominator)
        result->pushBackToNumerator(makeAbstractExpression(it->getId(), it.get()));
    result->simplify();
    return result;
}
bool Fractal::canTurnIntoPolynomWithOpeningParentheses()
{
    if (!this->denominator.empty() || this->coefficient.getDenominator() != 1)
        return false;
    for (auto &it : this->numerator)
    {
        if (it->getId() == POLYNOMIAL || (it->getId() == DEGREE && static_cast<Degree*>(it.get())->canTurnIntoPolynomial()))
            return true;
    }
    return false;
}

bool Fractal::canTurnIntoPolynomWithOpeningParentheses(bool is_fractional_coefficient_allowed)
{
    if (!this->denominator.empty() || (this->coefficient.getDenominator() != 1 && !is_fractional_coefficient_allowed))
        return false;
    for (auto &it : this->numerator)
    {
        if (it->getId() == POLYNOMIAL || (it->getId() == DEGREE && static_cast<Degree*>(it.get())->canTurnIntoPolynomial()))
            return true;
    }
    return false;
}
std::unique_ptr<Polynomial> Fractal::turnIntoPolynomWithOpeningParentheses()
{
    //in this function, if fractal has several polynoms, it has to do those operations with 1. another will turning is polynom->simplify() which call this function
    //assert(canTurnIntoPolynomWithOpeningParentheses());
  //  qDebug() << this->makeStringOfExpression();
    std::unique_ptr<Fractal> frac_without_polynom = std::unique_ptr<Fractal>(new Fractal(this));
    frac_without_polynom->turnDegreesIntoList();
    std::unique_ptr<Polynomial> polynom;
    for (auto it = frac_without_polynom->numerator.begin(); it != frac_without_polynom->numerator.end(); ++it)
    {
        if (it->get()->getId() == POLYNOMIAL)
        {
            polynom = std::unique_ptr<Polynomial>(new Polynomial(*static_cast<Polynomial*>(it->get())));
            frac_without_polynom->numerator.erase(it);
            break;
        }

    }
 //   qDebug() << frac_without_polynom->makeStringOfExpression();
  //  qDebug() << "POLYNOMS_MONOMS: ";
 //   for (auto &it : polynom->getMonomialsPointers())
  //      qDebug() << "    " << it->makeStringOfExpression();
    bool fractional_coefficients = polynom->isFractionalCoefficientsAllowed();
    std::list<Fractal*> polynoms_monoms = polynom->getMonomialsPointers();
    polynom.release();
    polynom = std::unique_ptr<Polynomial>(new Polynomial());
    polynom->setFractionalCoefficientsAllowed(fractional_coefficients);
    for (auto &it : polynoms_monoms)
    {
        polynom->addMonomial(*it * frac_without_polynom);
    }
   // qDebug() << "RES_POL: " << polynom->makeStringOfExpression();

    return polynom;
}
void Fractal::setSameMembersIntoDegree()
{
    NONCONST
    for (auto it = this->numerator.begin(); it != this->numerator.end(); ++it)
    {
        for (auto it1 = it; it1 != this->numerator.end();)
        {
            if (it1 == it)
                ++it1;
            else
            {
                if (*Degree::getArgumentOfDegree(it->get()) == *Degree::getArgumentOfDegree(it1->get()))
                {
                    AbstractExpression * degree = Degree::getArgumentOfDegree(it->get());
                     auto degr = std::unique_ptr<AbstractExpression>(new Degree(std::unique_ptr<AbstractExpression>(makeAbstractExpression(degree->getId(), degree)),
                        Degree::getDegreeOfExpression(it->get()) + Degree::getDegreeOfExpression(it1->get())));
                     degr = degr->downcast();
                     it->swap(degr);
                     it1 = this->numerator.erase(it1);
                }
                else
                    ++it1;
            }
        }
    }
    for (auto it = this->denominator.begin(); it != this->denominator.end(); ++it)
    {
        for (auto it1 = it; it1 != this->denominator.end();)
        {
            if (it1 == it)
                ++it1;
            else
            {
                if (*Degree::getArgumentOfDegree(it->get()) == *Degree::getArgumentOfDegree(it1->get()))
                {
                     AbstractExpression * degree = Degree::getArgumentOfDegree(it->get());
                     auto degr = std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(degree->getId(), degree),
                        Degree::getDegreeOfExpression(it->get()) + Degree::getDegreeOfExpression(it1->get())));
                     degr = degr->downcast();
                     it->swap(degr);
                     it1 = this->denominator.erase(it1);
                }
                else
                    ++it1;
            }
        }
    }
    this->pullNumbersIntoCoefficient();
}
void Fractal::_qDebugOut()
{
    qDebug() << "FRACTAL:";
    qDebug() << "Coefficient:";
    this->coefficient._qDebugOut();
    qDebug() << "End of coefficient;";
    qDebug() << "Numerator: <list of members>";
    for (auto &it : this->numerator)
    {
        it->_qDebugOut();
        qDebug() << "#####";
    }
    qDebug() << "End of numerator;";
    qDebug() << "Denominator: <list of members>";
    for (auto &it : this->denominator)
    {
        it->_qDebugOut();
        qDebug() << "#####";
    }
    qDebug() << "End of denominator;";
    qDebug() << "END OF FRACTAL;";
}
bool Fractal::lessFrac(const std::unique_ptr<Fractal> &left, const std::unique_ptr<Fractal> &right)
{

    if (left->getId() != right->getId())
    {
        if ((left->getId() < 0 && right->getId() < 0) || (left->getId() > 0 && right->getId() > 0))
            return left->getId() < right->getId();
        else
        {
            return left->getId() > right->getId();
        }
    }
    else
        return *left < *right;

}
Fractal::Fractal(std::unique_ptr<AbstractExpression> && num, Number coe) : coefficient(coe)
{
    this->pushBackToNumerator(std::move(num));
    this->simplify();
}
QString Fractal::makeStringOfExpression() const
{
    QString result = "(";
    if (!this->coefficient.isOne())
        result += this->coefficient.makeStringOfExpression();
    for (auto &it : this->numerator)
        result += "*" + it->makeStringOfExpression();
    if (result.size() == 1)
        result += "1";
    for (auto &it : this->denominator)
        result += "/" + it->makeStringOfExpression();
    result += ")";
    if (result[1] == "*")
        result = result.remove(1, 1);
    return result;
}
//i need this prefix becouse for some reasons in definition of Fractal::findCommonPart this overloading is not seen
fractal_argument _findCommonPart(fractal_argument & first, fractal_argument & second)
{
    fractal_argument result;
    for (auto &it1 : first)
    {
        for (auto &it2 : second)
        {
            if (*Degree::getArgumentOfDegree(it1.get()) == *Degree::getArgumentOfDegree(it2.get()))
            {
                auto deg1 = Degree::getDegreeOfExpression(it1.get());
                auto deg2 = Degree::getDegreeOfExpression(it2.get());
                std::unique_ptr<AbstractExpression> deg_to_set;
                if (*deg1 == *deg2)
                    deg_to_set = makeAbstractExpression(deg1->getId(), deg1.get());
                else if (deg1->getId() == NUMBER && deg2->getId() == NUMBER)
                    deg_to_set = std::unique_ptr<AbstractExpression>(new Number(static_cast<Number*>(deg1.get())->compareWith(*static_cast<Number*>(deg2.get())) < 0 ?
                                                                                    *static_cast<Number*>(deg1.get()) : *static_cast<Number*>(deg2.get())));
                else if (deg1->getId() == NUMBER)
                    deg_to_set = makeAbstractExpression(deg1->getId(), deg1.get());
                else if (deg2->getId() == NUMBER)
                    deg_to_set = makeAbstractExpression(deg2->getId(), deg2.get());
                else
                {
                    auto sub_result = deg1 - deg2;
                    if (sub_result->getId() == NUMBER)
                    {
                        if (static_cast<Number*>(sub_result.get())->compareWith(0) > 0)
                            deg_to_set = makeAbstractExpression(deg1->getId(), deg1.get());
                        else
                            deg_to_set = makeAbstractExpression(deg2->getId(), deg2.get());
                    }
                    else
                    {
                        auto div_result = deg1 / deg2;
                        if (div_result->getId() == NUMBER)
                        {
                            if (static_cast<Number*>(div_result.get())->compareWith(1) > 0)
                                deg_to_set = makeAbstractExpression(deg1->getId(), deg1.get());
                            else
                                deg_to_set = makeAbstractExpression(deg2->getId(), deg2.get());
                        }
                        else //здесь уже пофиг
                            deg_to_set = makeAbstractExpression(deg1->getId(), deg1.get());
                    }
                }
                 if (deg_to_set->getId() == NUMBER && static_cast<Number*>(deg_to_set.get())->isOne())
                    result.push_back(makeAbstractExpression(Degree::getArgumentOfDegree(it1.get())->getId(), Degree::getArgumentOfDegree(it1.get())));
                else
                    result.push_back(std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(Degree::getArgumentOfDegree(it1.get())->getId(),
                                                                                                                 Degree::getArgumentOfDegree(it1.get())), deg_to_set)));
            }
        }
    }
    return result;

}
void Fractal::takeCommonPartOfPolynomials()
{
   NONCONST
    for (auto &it : this->numerator)
        if (it->getId() == POLYNOMIAL)
            this->pushBackToNumerator(static_cast<Polynomial*>(it.get())->reduceCommonPart()->downcast());
    for (auto &it : this->denominator)
        if (it->getId() == POLYNOMIAL)
            this->pushBackToDenominator(static_cast<Polynomial*>(it.get())->reduceCommonPart()->downcast());
}
std::unique_ptr<AbstractExpression> Fractal::findCommonPart(Fractal *frac)
{
    std::unique_ptr<AbstractExpression> result(new Fractal);
    Fractal * fr_res = static_cast<Fractal*>(result.get());
    fr_res->setCoefficinet(Number(gcd(this->coefficient.getNumerator(), frac->coefficient.getNumerator()), gcd(this->coefficient.getDenominator(), frac->coefficient.getDenominator())));
    if (this->coefficient.compareWith(0) < 0 && frac->coefficient.compareWith(0) < 0)
        fr_res->setCoefficinet( fr_res->coefficient * -1);
    fr_res->numerator = _findCommonPart(this->numerator, frac->numerator);
    fr_res->denominator = _findCommonPart(this->denominator, frac->denominator);
    return result;
}
void multiplyIrrationalSumsInFractal(fractal_argument * arg)
{
    std::unique_ptr<AbstractExpression> result(nullptr);
    bool has_found_first = false;
    fractal_argument::iterator first;

    for (auto it = arg->begin(); it != arg->end();)
    {
        bool has_erased_it = false;
        if (it->get()->getId() == POLYNOMIAL && static_cast<Polynomial*>(it->get())->isIrrationalSum())
        {
            if (!has_found_first)
            {
                first = it;
                has_found_first = true;
            }
            else
            {
                std::unique_ptr<AbstractExpression> zero(new Number(0));
                if (result == nullptr)
                {
                    result = (*first * *it) + zero;
                    arg->erase(first);
                }
                else
                    result = result * *it + zero;
                it = arg->erase(it);
                has_erased_it = true;
            }
        }
        if (!has_erased_it)
            ++it;
    }
    if (result != nullptr)
        arg->push_back(std::move(result));
}
void Fractal::multiplyIrrationalSums()
{
    NONCONST
    //the problem is that multiplyIrrationalSumsInFractal() calls operator * that will call this function again, so, this condition predicts recursion
    if (this->denominator.empty() && this->coefficient.getDenominator() == 1)
        return;
    multiplyIrrationalSumsInFractal(&this->numerator);
    multiplyIrrationalSumsInFractal(&this->denominator);
    this->takeCommonPartOfPolynomials();
}
void Fractal::getRidOfIrrationalityInDenominator()
{
    NONCONST
    //after multipliing irrational sums there should be only one such sum in denom
    bool has_done_multipliing = true;
    while (has_done_multipliing)
    {
        has_done_multipliing = false;
        for (auto it = this->denominator.begin(); it != this->denominator.end(); ++it)
            if (it->get()->getId() == POLYNOMIAL && static_cast<Polynomial*>(it->get())->isIrrationalSum())
            {
                auto res = static_cast<Polynomial*>(it->get())->multiplyIrrationalSumOnAppropriateFormula();
              //  qDebug()<<"F: " << res.first->makeStringOfExpression();
              //  qDebug() <<"S: " << res.second->makeStringOfExpression();
                this->pushBackToNumerator(std::move(res.second));
                *it = std::move(res.first);
                has_done_multipliing = true;
                break;
            }

    }

    this->multiplyIrrationalSums();

}
std::unique_ptr<Fractal> operator*(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right)
{
    return *left * right;
}
std::unique_ptr<Fractal> operator/(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right)
{
    return *left / right;
}
void Fractal::sortVariablesInIncreasingOfTheirId()
{
    NONCONST
    this->numerator.sort([](const std::unique_ptr<AbstractExpression> & a, const std::unique_ptr<AbstractExpression> & b)->bool{
        return Degree::getArgumentOfDegree(a.get())->getId() <  Degree::getArgumentOfDegree(b.get())->getId();
    });
}
void Fractal::pushBackToNumerator(std::unique_ptr<AbstractExpression> && expr)
{
    NONCONST
    this->numerator.push_back(std::move(expr));
}
void Fractal::pushBackToDenominator(std::unique_ptr<AbstractExpression> && expr)
{
    NONCONST
    this->denominator.push_back(std::move(expr));
}
//функция пока не прижилась, наверно уберу.
void Fractal::getRidOfRootsInDenominator()
{
    NONCONST
    bool done_something = false;
    for (auto &it : this->denominator)
    {
        if (it->getId() == DEGREE && Degree::getDegreeOfExpression(it.get())->getId() == NUMBER &&
                static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->getDenominator() != 1)
        {
            done_something = true;
            Number  deg = *static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get());
            long long int new_den_deg = deg.getNumerator() / deg.getDenominator() + 1;
            std::unique_ptr<AbstractExpression> mult_deg_pt(new Number(Number(new_den_deg)-deg));
            std::unique_ptr<AbstractExpression> den_deg(new Number(new_den_deg));
           // it = std::unique_ptr<AbstractExpression> (new Degree(std::unique_ptr<AbstractExpression>(
          //                                                            Degree::getArgumentOfDegree(it.get())),
         //                                                        std::move(den_deg)));
            AbstractExpression * arg = Degree::getArgumentOfDegree(it.get());
            it = std::unique_ptr<AbstractExpression> (new Degree(makeAbstractExpression(arg->getId(), arg),
                                                                std::move(den_deg)));
             arg = Degree::getArgumentOfDegree(it.get());
            this->numerator.push_back(std::unique_ptr<AbstractExpression>(new Degree(makeAbstractExpression(arg->getId(), arg),
                                                                                     std::move(mult_deg_pt))));
        }
    }
    if (done_something)
        this->simplify();
}
double Fractal::getApproximateValue()
{
    assert(this->getSetOfVariables().empty());
    double value = this->coefficient.getApproximateValue();
    for (auto &it : this->numerator)
        value *= it->getApproximateValue();
    for (auto &it : this->denominator)
        value /= it->getApproximateValue();
    return value;
}
int Fractal::getPositionRelativelyZeroIfHasVariables()
{
    bool is_negative = this->coefficient.compareWith(0) < 0;
    for (auto &it : this->numerator)
    {
        int pos = it->getPositionRelativelyZero();
        if (pos == 0)
            return 0;
        if (pos < 0)
            is_negative = !is_negative;
    }
    for (auto &it : this->denominator)
    {
        int pos = it->getPositionRelativelyZero();
        if (pos == 0)
            return 0;
        if (pos < 0)
            is_negative = !is_negative;
    }
    return is_negative ? -1 : 1;
}
double Fractal::getApproximateValue(const std::function<double (VariablesDefinition *)> & choosing_value_rule)
{
    double value = this->coefficient.getApproximateValue();
    for (auto &it : this->numerator)
        value *= it->getApproximateValue(choosing_value_rule);
    for (auto &it : this->denominator)
    {
        double ret_val = it->getApproximateValue(choosing_value_rule);
        if (ret_val == 0.0)
            return std::nan("1");
        value /= ret_val;
    }
    return value;
}

void Fractal::intoAcceptedSignForm()
{
    bool change_sign = false;
    for (auto &it : this->numerator)
        if (it->getId() == POLYNOMIAL)
            if (static_cast<Polynomial*>(it.get())->isNeedToChangeSignToLeadTheAcceptedForm())
            {
                static_cast<Polynomial*>(it.get())->changeSign();
                change_sign = !change_sign;
            }
    for (auto &it : this->denominator)
        if (it->getId() == POLYNOMIAL)
            if (static_cast<Polynomial*>(it.get())->isNeedToChangeSignToLeadTheAcceptedForm())
            {
                static_cast<Polynomial*>(it.get())->changeSign();
                change_sign = !change_sign;
            }
    if (change_sign)
        this->coefficient *= -1;
}

void Fractal::convertTrigonometricalFunctionsByFormulas(const std::map<QString, TrigonometricalFunctionsCastType> &instructions)
{
    NONCONST
    auto convert = [&instructions](abs_ex & convertible)
    {
        if (!isDegreeOfTrigonometricalFunction(convertible))
            return;
        auto convert_type = instructions.find(getStringArgumentOfTrigonometricalFunction(convertible.get()))->second;
        if (convert_type == NONE)
            return;
        if (convert_type == COSINUS_TO_SINUS || convert_type == SINUS_TO_COSINUS || convert_type == TANGENT_TO_COSINUS || convert_type == COSINUS_TO_TANGENT ||
        convert_type == COTANGENT_TO_SINUS || convert_type == SINUS_TO_COTANGENT)
        {
            auto degree = Degree::getDegreeOfExpression(convertible.get());
            if (degree->getId() != NUMBER)
                return;
            Number deg = *static_cast<Number*>(degree.get());
            if (!(deg / 2).isInteger())
                return;
            auto arg = Degree::getArgumentOfDegree(convertible.get());
            abs_ex res;
            switch (convert_type) {
                case COSINUS_TO_SINUS:
                    if (arg->getId() != COSINUS)
                        return;
                    res = cosinusToSinus(std::move(convertible));
                break;
                case SINUS_TO_COSINUS:
                    if (arg->getId() != SINUS)
                        return;
                    res = sinusToCosinus(std::move(convertible));
                break;
                case TANGENT_TO_COSINUS:
                    if (arg->getId() != TANGENT)
                        return;
                    res = tangentToCosinus(std::move(convertible));
                break;
                case COSINUS_TO_TANGENT:
                    if (arg->getId() != COSINUS)
                        return;
                    res = cosinusToTangent(std::move(convertible));
                break;
                case COTANGENT_TO_SINUS:
                    if (arg->getId() != COTANGENT)
                        return;
                    res = cotangentToSinus(std::move(convertible));
                break;
                case SINUS_TO_COTANGENT:
                    if (arg->getId() != SINUS)
                        return;
                    res = sinusToCotangent(std::move(convertible));
                break;
            }
           convertible = takeDegreeOf(res, deg / 2);
        }
        if (convert_type == TANGENTS_AND_COTANGENTS_TO_FRACTAL || convert_type == TANGENT_TO_COTANGENT || convert_type == COTANGENT_TO_TANGENT)
        {
            auto arg = Degree::getArgumentOfDegree(convertible.get());
            if (arg->getId() != TANGENT && arg->getId() != COTANGENT)
                return;
            switch(convert_type)
            {
                case TANGENTS_AND_COTANGENTS_TO_FRACTAL:
                    if (arg->getId() == TANGENT)
                        convertible = tangentToFractal(std::move(convertible));
                    else
                        convertible = cotangentToFractal(std::move(convertible));
                break;
                case TANGENT_TO_COTANGENT:
                    if (arg->getId() != TANGENT)
                        return;
                    convertible = tangentToCotangent(std::move(convertible));
                break;
                case COTANGENT_TO_TANGENT:
                    if (arg->getId() != COTANGENT)
                        return;
                    convertible = cotangentToTangent(std::move(convertible));
                break;

            }
        }
    };
    for (auto &it : this->numerator)
    {
        convert(it);
    }
    for (auto &it : this->denominator)
    {
        convert(it);
    }
}

void Fractal::checkTrigonometricalFunctionsItHas(std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool> > &params)
{
    auto check = [&params](abs_ex & it)->void {
        AbstractExpression * arg = Degree::getArgumentOfDegree(it.get());
        if (it->getId() == DEGREE &&  (Degree::getDegreeOfExpression(it.get())->getId() == NUMBER && *static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get()) == 2))
        {
            Degree * deg = static_cast<Degree*>(it.get());
            if (deg->getArgumentOfDegree(deg)->getId() == SINUS )
                std::get<0>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
            else if (deg->getArgumentOfDegree(deg)->getId() == COSINUS)
                std::get<2>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
            else if (deg->getArgumentOfDegree(deg)->getId() == TANGENT)
                std::get<4>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
            else if (deg->getArgumentOfDegree(deg)->getId() == COTANGENT)
                std::get<6>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
        }
        else if (arg->getId() == SINUS)
            std::get<1>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
        else if (arg->getId() == COSINUS)
            std::get<3>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
        else if (arg->getId() == TANGENT)
            std::get<5>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
        else if (arg->getId() == COTANGENT)
            std::get<7>(params[getStringArgumentOfTrigonometricalFunction(it)]) = true;
    };
    for (auto &it : this->numerator)
    {
        check(it);
    }
    for (auto &it : this->denominator)
    {
        check(it);
    }
}

std::unique_ptr<AbstractExpression> Fractal::changeSomePartOn(QString part, std::unique_ptr<AbstractExpression> &on_what)
{
    abs_ex its_part = nullptr;
    auto add = [&on_what, &its_part](abs_ex & part)->void {
        if (its_part == nullptr)
            its_part = std::move(part);
        part = copy(on_what);
    };
    auto add_r = [&its_part](abs_ex && part)->void {
        if (its_part == nullptr && part != nullptr)
            its_part = std::move(part);
    };
    for (auto &it : this->numerator)
    {
        if (it->makeStringOfExpression() == part)
        {
            add(it);

        }
        else
            add_r(it->changeSomePartOn(part, on_what));

    }
    for (auto &it : this->denominator)
    {
        if (it->makeStringOfExpression() == part)
        {
            add(it);

        }
        else
            add_r(it->changeSomePartOn(part, on_what));

    }
    return its_part;
}

std::vector<std::pair<abs_ex, Number>> Fractal::getTrigonometryMultipliersArgumentsCopyAndItsDegrees()
{
  //  assert(this->denominator.size() == 0);
    std::vector<std::pair<abs_ex, Number>> res;
    for (auto &it : this->numerator)
    {
        if (isDegreeOfTrigonometricalFunction(it) && Degree::getDegreeOfExpression(it.get())->getId() == NUMBER &&
                static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->isInteger())
        {
            res.push_back({getArgumentOfTrigonometricalFunction(it), *static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())});

        }
    }
    for (auto &it : this->denominator)
    {
        if (isDegreeOfTrigonometricalFunction(it) && Degree::getDegreeOfExpression(it.get())->getId() == NUMBER &&
                static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->isInteger())
        {
            res.push_back({getArgumentOfTrigonometricalFunction(it), *static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())});

        }
    }
    return res;
}

void Fractal::convertTrigonometricalMultipliersToDifferentArgument(const std::map<QString, TrigonometricalFunctionsArgumentsCastType> &instructions)
{

    for (auto &it : numerator)
    {
        if (isDegreeOfTrigonometricalFunction(it) && Degree::getDegreeOfExpression(it.get())->getId() == NUMBER &&
                static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->isInteger())
        {
            NONCONST
            it = convertTrigonometricalFunctionByArgument(std::move(it), instructions.find(getArgumentOfTrigonometricalFunction(it)->makeStringOfExpression())->second);
            //qDebug() << "RES: " << convertTrigonometricalFunctionByArgument(std::move(it), instructions.find(getArgumentOfTrigonometricalFunction(it)->makeStringOfExpression())->second)->makeStringOfExpression();
            //qDebug() << "IN FR: " << it->makeStringOfExpression();
        }

    }
    for (auto &it : denominator)
    {
        if (isDegreeOfTrigonometricalFunction(it) && Degree::getDegreeOfExpression(it.get())->getId() == NUMBER &&
                static_cast<Number*>(Degree::getDegreeOfExpression(it.get()).get())->isInteger())
        {
            NONCONST
            it = convertTrigonometricalFunctionByArgument(std::move(it), instructions.find(getArgumentOfTrigonometricalFunction(it)->makeStringOfExpression())->second);
            //qDebug() << "RES: " << convertTrigonometricalFunctionByArgument(std::move(it), instructions.find(getArgumentOfTrigonometricalFunction(it)->makeStringOfExpression())->second)->makeStringOfExpression();
            //qDebug() << "IN FR: " << it->makeStringOfExpression();
        }

    }
}

std::unique_ptr<AbstractExpression> Fractal::derivative(int var) const
{
    if (this->coefficient != 1)
    {
        abs_ex fr(new Fractal(*this));
        abs_ex coe(new Number(this->coefficient));
        fr = fr / coe;
        return coe * fr->derivative(var);
    }
    if (!this->denominator.empty())
    {
        abs_ex f(new Fractal(this->numerator, Number(this->coefficient.getNumerator())));
        f = f->downcast();
        abs_ex g(new Fractal(this->denominator, Number(this->coefficient.getDenominator())));
        g = g->downcast();
        return (f->derivative(var) * g - f * g->derivative(var)) / takeDegreeOf(g, 2);

    }
    if (this->numerator.size() == 1)
        return (*this->numerator.begin())->derivative(var);
    if (this->numerator.size() == 0)
        return copy(zero);
    assert(this->numerator.size() > 1);
    abs_ex f = copy(*this->numerator.begin());
    fractal_argument other_mults;
    for (auto it = std::next(this->numerator.begin()); it != this->numerator.end(); ++it)
    {
        other_mults.push_back(copy(*it));
    }
    abs_ex g (new Fractal(std::move(other_mults)));
    g = g->downcast();
    return f->derivative(var) * g + g->derivative(var) * f;
}
//да, это будет очередная большая функция. Здесь нет сложного поведения, только проверка на различные табличные интегралы
//рассчитываем на то, что сработало takeCommonPartOfPolynomial()
//еще рассчитываем на то, что все полиномы которые можно разложить - разложены
std::unique_ptr<AbstractExpression> Fractal::antiderivative(int var) const
{
    auto fr_without_var = this->getFractalWithoutVariable(var);
    if (!fr_without_var->isOne())
        return toAbsEx(fr_without_var) * (*this / *fr_without_var)->antiderivative(var);
    abs_ex x(new Variable(getVariable(var)));
    if (numerator.size() == 0 && denominator.size() == 1)
    {
        // 1/(ax+b)
        auto ln_f = checkIfItsLinearFunction(*denominator.begin(), var);
        if (ln_f.first != nullptr)
            return one / ln_f.first * ln(abs(*denominator.begin()));
        if ((*denominator.begin())->getId() == SINUS)
        { // 1/sin(x)
            ln_f = checkIfItsFunctionOfLinearArgument(*denominator.begin(), var);
            if (ln_f.first == nullptr)
                return nullptr;
            auto arg = static_cast<Sinus*>(denominator.begin()->get())->getArgumentsCopy();
            return one / ln_f.first * minus_one * ln(abs(tan(arg/two)));
        }
        if ((*denominator.begin())->getId() == COSINUS)
        { // 1/cos(x)
            ln_f = checkIfItsFunctionOfLinearArgument(*denominator.begin(), var);
            if (ln_f.first == nullptr)
                return nullptr;
            auto arg = static_cast<Cosinus*>(denominator.begin()->get())->getArgumentsCopy();
            return one / ln_f.first * ln(abs(cot(arg/two)));
        }
        if ((*denominator.begin())->getId() == TANGENT)
        { // 1/tan(x)
            ln_f = checkIfItsFunctionOfLinearArgument(*denominator.begin(), var);
            if (ln_f.first == nullptr)
                return nullptr;
            auto arg = static_cast<Tangent*>(denominator.begin()->get())->getArgumentsCopy();
            return one / ln_f.first * ln(sin(arg));
        }
        if ((*denominator.begin())->getId() == COTANGENT)
        { // 1/ctg(x)
            ln_f = checkIfItsFunctionOfLinearArgument(*denominator.begin(), var);
            if (ln_f.first == nullptr)
                return nullptr;
            auto arg = static_cast<Cotangent*>(denominator.begin()->get())->getArgumentsCopy();
            return minus_one / ln_f.first * ln(cos(arg));
        }
        if ((*denominator.begin())->getId() == DEGREE)
        {
            auto deg = Degree::getDegreeOfExpression(denominator.begin()->get());
            auto arg = Degree::getArgumentOfDegree(denominator.begin()->get());
            if (!arg->hasVariable(var))
            { // 1/a^x
                ln_f = checkIfItsLinearFunction(deg, var);
                if (ln_f.first == nullptr)
                    return nullptr;
                return minus_one/ln_f.first * takeDegreeOf(copy(arg), (*minus_one) * (*deg)) / ln(arg);
            }
            if (deg->hasVariable(var))
                return nullptr;
            if (*deg == *two)
            {
                if (arg->getId() == SINUS)
                {// 1/sin(x)^2
                    ln_f = checkIfItsFunctionOfLinearArgument(arg, var);
                    if (ln_f.first == nullptr)
                        return nullptr;
                    return minus_one / ln_f.first * cot(copy(static_cast<Sinus*>(arg)->getArgumentsCopy()));
                }
                if (arg->getId() == COSINUS)
                {//  1/cos(x)^2
                    ln_f = checkIfItsFunctionOfLinearArgument(arg, var);
                    if (ln_f.first == nullptr)
                        return nullptr;
                    return one/ln_f.first * tan(copy(static_cast<Cosinus*>(arg)->getArgumentsCopy()));
                }
                if (arg->getId() == TANGENT)
                {// 1/tan(x)^2
                    ln_f = checkIfItsFunctionOfLinearArgument(arg, var);
                    if (ln_f.first == nullptr)
                        return nullptr;
                    auto x = static_cast<Tangent*>(arg)->getArgumentsCopy();
                    return one/ln_f.first * minus_one * (x + cot(x));
                }
                if (arg->getId() == COTANGENT)
                {// 1/cot(x)^2
                    ln_f = checkIfItsFunctionOfLinearArgument(arg, var);
                    if (ln_f.first == nullptr)
                        return nullptr;
                    auto x = static_cast<Tangent*>(arg)->getArgumentsCopy();
                    return one/ln_f.first * (tan(x) - x);
                }
                auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                if (qc_f[0] != nullptr)
                {
                    //здесь мы считаем, что если дискриминант положителен, то оно уже разложено на множители(вообще мы и вдругих местах могли бы так считать), но там код
                    //я уже написал(
                    auto D = sqr(qc_f[1]) - four * qc_f[0]*qc_f[2];
                    auto d = arg->derivative(var);
                    return -(d / copy(arg) + four*qc_f[0] * atan(d/sqrt(-D))/sqrt(-D)) / D;
                }


            }
            if (*deg == *three)
            {
                //здесь мы также считаем, что если дискриминант положителен, то оно уже разложено на множители(вообще мы и вдругих местах могли бы так считать), но там код
                //я уже написал(. Ну и в следющих кодах я также из этого предположения исходить буду
                auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                if (qc_f[0] != nullptr)
                {
                    auto D = sqr(qc_f[1]) - four * qc_f[0]*qc_f[2];
                    abs_ex &a = qc_f[0];
                    abs_ex &b = qc_f[1];
                    abs_ex &c = qc_f[2];
                    return (-(((b + two* a* x)* (sqr(b) - numToAbs(6)* a* b* x - two* a* (numToAbs(5)* c + three* a *sqr(x))))/sqr(c +
                                                                                   x* (b + a *x))) + (
                                                                                numToAbs(24)* sqr(a)* atan((b + two* a* x)/sqrt(-D)))/sqrt(-D))/(two* sqr(D));
                }

            }
            ln_f = checkIfItsLinearFunction(arg, var);
            if (ln_f.first != nullptr)
            { // 1/x^n
                return one/ln_f.first * takeDegreeOf(copy(arg), one - copy(deg)) / (one - copy(deg));
            }
            if (deg->getId() != NUMBER)
                return nullptr;

            Number num_d = *static_cast<Number*>(deg.get());
            if (num_d == Number(1, 2))
            {
                auto qc_f = checkIfItsQuadraticFunction(arg, var);
                abs_ex x (new Variable(getVariable(var)));
                return one / takeDegreeOf(qc_f[0], half) * ln(abs(two*qc_f[0]*x + qc_f[1] + two*takeDegreeOf(qc_f[0]*copy(arg), half)));
            }

        }
        if ((*denominator.begin())->getId() == POLYNOMIAL)
        {
            auto arg = denominator.begin()->get();
            auto qc_f = checkIfItsQuadraticFunction(arg, var);
            if (qc_f[0] == nullptr)
                return nullptr;
            auto D = qc_f[1]*qc_f[1] - four*qc_f[0]*qc_f[2];
            abs_ex x (new Variable(getVariable(var)));
            if (::isZero(D))
                return minus_one * two / (two*qc_f[0]*x + qc_f[1]);
            //здесь воспользуемся формулами: dx/(x+w)/(x+u) -> 1/(w-u)*ln(|(x+u)/(x+w)|), ax^2+bx+c = a(x-x1)(x-x2), x1,2 = (-b+-sqrt(D))/2a
            int pos = D->getPositionRelativelyZero();
            if (pos > 0)
            {
                auto w = -(-qc_f[1] + sqrt(D))/(two*qc_f[0]);
                auto u = -(-qc_f[1] - sqrt(D))/(two*qc_f[0]);
                return (one / (w - u) * ln(abs((x + u) / (x + w)))) / qc_f[0];
            }
            if (pos < 0)
                return two * atan((two*qc_f[0]*x + qc_f[1])/sqrt(-D))/sqrt(-D);
            return nullptr;
        }
        return nullptr;
    }
    auto linearDivideQuadraticFormula = [&var](std::pair<abs_ex, abs_ex> & ln_f, std::array<abs_ex, 3> & qc_f2)->abs_ex {
        auto D = qc_f2[1]*qc_f2[1] - four*qc_f2[0]*qc_f2[2];
        abs_ex x (new Variable(getVariable(var)));
        if (::isZero(D)) // (ax+b)/(x+c)^2
        {
            //c это МИНУС КОРЕНЬ
            auto c = qc_f2[1]/two/qc_f2[0];
            return one / qc_f2[0] * ((ln_f.first*c - ln_f.second)/(x + c) + ln_f.first * ln(abs(c + x)));
        }
        //здесь воспользуемся формулами: dx/(x+w)/(x+u) -> 1/(w-u)*ln(|(x+u)/(x+w)|), ax^2+bx+c = a(x-x1)(x-x2), x1,2 = (-b+-sqrt(D))/2a
        int pos = D->getPositionRelativelyZero();
        if (pos > 0)
        {
            auto w = -(-qc_f2[1] + sqrt(D))/(two*qc_f2[0]);
            auto u = -(-qc_f2[1] - sqrt(D))/(two*qc_f2[0]);
            return one / qc_f2[0] * ((ln_f.first * u - ln_f.second) * ln(abs(u + x)) + (ln_f.second - ln_f.first * w) * ln(abs(w + x))) / (u - w);
        }

            return ((two * (two * ln_f.second * qc_f2[0] - ln_f.first * qc_f2[1]) * atan((two*qc_f2[0]*x + qc_f2[1])/sqrt(-D)))/sqrt(-D) + ln_f.first*ln(abs(x*(qc_f2[0]*x + qc_f2[1]) + qc_f2[2]))) / two / qc_f2[0];
    };
    auto quadraticDivideLinearFormula = [&var](std::array<abs_ex, 3> & qc_f, std::pair<abs_ex, abs_ex> & ln_f2)->abs_ex {
       /* auto D = qc_f[1]*qc_f[1] - four*qc_f[0]*qc_f[2];
        abs_ex x (new Variable(getVariable(var)));
        if (::isZero(D)) // (x-c)^2/(ax+b)
        {
            auto c = -qc_f[1]/two/qc_f[0];
            return qc_f[0] * (ln_f2.first * x * (ln_f2.first * (x - four * c) - two * ln_f2.second) + two * sqr(ln_f2.first * c + ln_f2.second) * ln(abs(ln_f2.first * x + ln_f2.second))) / two / takeDegreeOf(ln_f2.first, three);
        }
        //здесь воспользуемся формулами: dx/(x+w)/(x+u) -> 1/(w-u)*ln(|(x+u)/(x+w)|), ax^2+bx+c = a(x-x1)(x-x2), x1,2 = (-b+-sqrt(D))/2a
        if (D->getPositionRelativelyZero() > 0)
        {
            auto w = -(-qc_f[1] + sqrt(D))/(two*qc_f[0]);
            auto u = -(-qc_f[1] - sqrt(D))/(two*qc_f[0]);
            return qc_f[0] * (ln_f2.first * x * (ln_f2.first * (two*u + two*w + x) - two*ln_f2.second) + two * (ln_f2.second - ln_f2.first * w) * (ln_f2.second - ln_f2.first * u) * ln(abs(ln_f2.first * x + ln_f2.second))) / two / pow(ln_f2.first, three);
        }*/
        abs_ex& a = qc_f[0];
        abs_ex& b = qc_f[1];
        abs_ex& c = qc_f[2];
        abs_ex& f = ln_f2.first;
        abs_ex &g = ln_f2.second;
        abs_ex x (new Variable(getVariable(var)));
        return (two * ln(abs(f*x + g))*(g*(a*g - b*f) + c*f*f) + f*x*(a*f*x - two*a*g + two*b*f)) /two / pow(f, three);
    };
    auto quadraticDivideQuadraticFormula = [&var](std::array<abs_ex, 3> && qc_f1, std::array<abs_ex, 3> && qc_f2)->abs_ex {
      // (ax^2+bx+c)/(ex^2+fx+g)
        abs_ex a = std::move(qc_f1[0]), b = std::move(qc_f1[1]), c = std::move(qc_f1[2]), e = std::move(qc_f2[0]), f = std::move(qc_f2[1]), g = std::move(qc_f2[2]);
        abs_ex D1 = sqr(b) - four * a * c;
        abs_ex D2 = sqr(f) - four * e * g;
        abs_ex x(new Variable(getVariable(var)));
        if ((*D1 == *zero || D1->getPositionRelativelyZero() > 0) && *D2 == *zero)
        {//(x-w)(x-u)/(x-t)^2
            abs_ex w = (-b + sqrt(D1))/two/a;
            abs_ex u = (-b - sqrt(D1))/two/a;
            abs_ex t = -f/two/e;
            return qc_f1[0] * qc_f2[0] * ((t - u)*(t - w) / (t - x) - (-two * t + u + w)*ln(abs(t - x)) + x);
        }
        if ((*D1 == *zero || D1->getPositionRelativelyZero() > 0) && (D2->getPositionRelativelyZero() > 0))
        {
            // (x-w)(x-u)/(x-t)/(x-z)
            abs_ex w = (-b + sqrt(D1))/two/a;
            abs_ex u = (-b - sqrt(D1))/two/a;
            abs_ex t = (-f + sqrt(D2))/two/a;
            abs_ex z = (-f - sqrt(D2))/two/a;
            return qc_f1[0] / qc_f2[0] * ((t - u)*(t - w) * ln(abs(x - t)) + x*(t - z) + (u - z)*(z - w)*ln(abs(x - z))) / (t - z);
        }
        return (two*a*e*x + (two*(two*c*sqr(e) -b*e*f + a*(sqr(f) - two*e*g))*atan((f + two*e*x) / sqrt(-D2)))/sqrt(-D2) + (b*e - a*f)*ln(abs(g + x*(f + e*x))))/two/sqr(e);
    };

    if (numerator.size() == 1 && denominator.size() == 1)
    {
        auto num = numerator.begin()->get();
        auto denom = denominator.begin()->get();
        auto ln_f = checkIfItsLinearFunction(num, var);
        auto ln_f2 = checkIfItsLinearFunction(denom, var);
        auto x = abs_ex(new Variable(getVariable(var)));
        if (ln_f.first != nullptr && ln_f2.first != nullptr) // (ax+b)/(cx+d) dx
            return ln_f.first / ln_f2.first * x + (ln_f.second * ln_f2.first - ln_f.first * ln_f2.second) * ln(abs(*denominator.begin())) / sqr(ln_f2.first);
        if (ln_f.first != nullptr && ln_f2.first == nullptr)
        {
            auto qc_f2 = checkIfItsQuadraticFunction(denom, var);
            if (qc_f2[0] != nullptr) // (ax+b)/(cx^2+dx+e)
            {
                return linearDivideQuadraticFormula(ln_f, qc_f2);
            }

            if (denom->getId() == DEGREE)
            {
                //аналогично если дискриминант больше нуля, то трехчлен должен быть уже автоматом разложенный
                auto deg = Degree::getDegreeOfExpression(denom);
                if (*deg == *two)
                {
                    auto arg = Degree::getArgumentOfDegree(denom);
                    auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                    if (qc_f[0] != nullptr)
                    {
                        abs_ex &a = ln_f.first;
                        abs_ex &b = ln_f.second;
                        abs_ex &c = qc_f[0];
                        abs_ex &d = qc_f[1];
                        abs_ex &e = qc_f[2];
                        auto D = sqr(d) - four*c*e;
                        return ((-b *(d + two* c* x) + a* (two* e + d* x))/(e + x* (d + c* x)) + (
                         two* (-two* b* c + a* d)* atan((d + two* c* x)/sqrt(-D)))/sqrt(-D))/(D);
                    }
                }
                if (*deg == *three)
                {
                    auto arg = Degree::getArgumentOfDegree(denom);
                    auto qc_f = ::checkIfItsQuadraticFunction(arg, var);
                    if (qc_f[0] != nullptr)
                    {
                        abs_ex &a = ln_f.first;
                        abs_ex &b = ln_f.second;
                        abs_ex &c = qc_f[0];
                        abs_ex &d = qc_f[1];
                        abs_ex &e = qc_f[2];
                        auto D = sqr(d) - four*c*e;
                        return ((three* (two* b* c - a* d)* (d + two* c* x))/(
                         e + x* (d + c *x)) + ((D)* (-b* (d + two* c* x) +
                            a (2 e + d x)))/(e + x (d + c x))^2 - (
                         12 c (-2 b c + a d) ArcTan[(d + 2 c x)/
                           Sqrt[-d^2 + 4 c e]])/Sqrt[-d^2 + 4 c e])/(2 (d^2 - 4 c e)^2)

                    }
                }
            }
        }
        if (ln_f.first == nullptr && ln_f2.first != nullptr)
        {
            auto qc_f = checkIfItsQuadraticFunction(num, var);
            if (qc_f[0] != nullptr)
            {
                return quadraticDivideLinearFormula(qc_f, ln_f);
            }
        }
        auto qc_f = checkIfItsQuadraticFunction(num, var);
        auto qc_f2 = checkIfItsQuadraticFunction(denom, var);
        if (qc_f[0] != nullptr && qc_f2[0] != nullptr)
            return quadraticDivideQuadraticFormula(std::move(qc_f), std::move(qc_f2));
    }
    if (numerator.size() == 0 && denominator.size() == 2)
    {
        auto ln_f1 = ::checkIfItsLinearFunction(*denominator.begin(), var);
        auto ln_f2 = ::checkIfItsLinearFunction(*next(denominator.begin()), var);
        if (ln_f1.first != nullptr && ln_f2.second != nullptr)
        {// (ax+b)/(cx+d)
            return (ln(abs(*denominator.begin())) - ln(abs(*next(denominator.begin()))))/(ln_f1.first * ln_f2.second - ln_f1.second * ln_f2.first);
        }
        auto den_beg = denominator.begin();
        auto den_next = next(denominator.begin());
        if (ln_f2.first != nullptr && ln_f1.first == nullptr)
        {
            std::swap(ln_f1, ln_f2);
            std::swap(den_beg, den_next);
        }
        if (ln_f1.first != nullptr)
        {//   1/(a*x + b)*(c*x^2 + d*x + e)
            auto qc_f = ::checkIfItsQuadraticFunction(*den_next, var);
            if (qc_f[0] != nullptr)
            {
                abs_ex& a = ln_f1.first;
                abs_ex& b = ln_f1.second;
                abs_ex &c = qc_f[0];
                abs_ex &d = qc_f[1];
                abs_ex &e = qc_f[2];
               // (a x (-2 b c + 2 a d + a c x) +
                // 2 (b^2 c - a b d + a^2 e) Log[b + a x])/(2 a^3)
                return (a* x *(-two* b* c + two* a* d + a* c* x) +
                                 two* (sqr(b)* c - a* b* d + sqr(a)*e) *ln(abs(*den_beg)))/(two*pow(a, 3));
            }
        }
        auto qc_f1 = ::checkIfItsQuadraticFunction(*den_beg, var);
        auto qc_f2 = ::checkIfItsQuadraticFunction(*den_next, var);
        if (qc_f1[0] != nullptr && qc_f2[0] != nullptr)
        {//   1/(a*x^2 + b*x + f)*(c*x^2 + d*x + e)
            abs_ex &c = qc_f2[0];
            abs_ex &d = qc_f2[1];
            abs_ex &e = qc_f2[2];
            abs_ex &a = qc_f1[0];
            abs_ex &b = qc_f1[1];
            abs_ex &f = qc_f1[2];

            abs_ex D1 = sqr(b) - four * a * f;
            abs_ex D2 = sqr(d) - four * c * e;

        }
    }
    if (numerator.size() == 1 && denominator.size() == 2)
    {
        auto ln_f = ::checkIfItsLinearFunction(*numerator.begin(), var);
        auto ln_f1 = ::checkIfItsLinearFunction(*denominator.begin(), var);
        auto ln_f2 = ::checkIfItsLinearFunction(*next(denominator.begin()), var);
        if (ln_f.first != nullptr && ln_f1.first != nullptr && ln_f2.first != nullptr)
        {// (ex+f)/((ax+b)(cx+d))
            return ((-ln_f1.second * ln_f2.first * ln_f.first + ln_f1.first * ln_f2.first * ln_f.second) * ln(abs(*denominator.begin())) +
                    ln_f1.first * (ln_f2.second * ln_f.first - ln_f2.first * ln_f.second) * ln(abs(*next(denominator.begin())))) /
                    (ln_f1.first * ln_f2.first * (ln_f1.first * ln_f2.second - ln_f1.second * ln_f2.first));
        }
    }
    if (numerator.size() == 2 && denominator.size() == 2)
    {
        auto ln_f1 = ::checkIfItsLinearFunction(*numerator.begin(), var);
        auto ln_f2 = ::checkIfItsLinearFunction(*next(numerator.begin()), var);
        auto ln_f3 = ::checkIfItsLinearFunction(*denominator.begin(), var);
        auto ln_f4 = ::checkIfItsLinearFunction(*next(denominator.begin()), var);
        if (ln_f1.first != nullptr && ln_f2.first != nullptr && ln_f3.first != nullptr && ln_f4.first != nullptr)
        {// (e*x + f)*(g*x + h)/((a*x + b)*(c*x + d)
            abs_ex &e = ln_f1.first;
            abs_ex &f = ln_f1.second;
            abs_ex &g = ln_f2.first;
            abs_ex &h = ln_f2.second;
            abs_ex &a = ln_f3.first;
            abs_ex &b = ln_f3.second;
            abs_ex &c = ln_f4.first;
            abs_ex &d = ln_f4.second;
            return (sqr(c) * (b*e - a*f)*(b*g - a*h) * ln(abs(*denominator.begin())) + a*(c*(a*d - b*c)*e*g*x - a*(d*e - c*f)*(d*g - c*h)*ln(abs(*next(denominator.begin()))))) /
                    (sqr(a)*sqr(c) * (a*d - b*c));
        }
    }


}

std::pair<abs_ex, abs_ex> Fractal::checkIfItIsLinearFunction(int var) const
{
    bool has_first_deg_var = false;
    bool has_other_deg_var = false;
    for (auto &it : numerator)
    {
        if (Degree::getArgumentOfDegree(it.get())->getId() != var)
            continue;
        auto degr = Degree::getDegreeOfExpression(it.get());
        if (*degr == *one)
            has_first_deg_var = true;
        else
        {
            has_other_deg_var = true;
            break;
        }
    }
    for (auto &it : denominator)
        if (Degree::getArgumentOfDegree(it.get())->getId() == var)
        {
            has_other_deg_var = true;
            break;
        }
    if (has_other_deg_var || !has_first_deg_var)
        return {nullptr, nullptr};
    return {toAbsEx(this->getFractalWithoutVariable(var)), copy(zero)};
}

bool Fractal::isOne() const
{
    return this->numerator.empty() && this->denominator.empty() && this->coefficient.isOne();
}



void Fractal::castTrigonometry()
{

    this->casted_trigonometry = false;
    auto rules = this->getTrigonometricalFunctionToTurnIntoIt();
    bool has_to_cast = false;
    for (auto &it : rules)
    {
        if (it.second != NONE)
        {
            has_to_cast = true;
            break;
        }
    }
    if (!has_to_cast)
        return;
    this->casted_trigonometry = true;
    this->convertTrigonometricalFunctionsByFormulas(rules);
    this->simplified = false;

    this->simplify();
}

void Fractal::castTrigonometryArguments()
{
    //все степени здесь целые
    //первое выражение - аргумент функции, второй - степень функции и то, есть у этой функции нечетная степень, затем в словаре аргументы, которые представляют собой тот
    //аргумент, умноженный на какое-либо число.
    //ключ словаря - то самое число,  число в значении словаря - степени триг. функции с этим самым аргументом и то, есть ли у этой функции нечетная степень,
    //а строка - строковое представление функции с этим аргументом

    std::vector<std::pair<std::pair<abs_ex, std::pair<Number, bool>>, std::map<Number, std::pair<std::pair<Number, bool>, QString>, std::function<bool (const Number & a, const Number & b)>>>>
                                                                                                                                                            arguments_multipliers;

   auto arguments = this->getTrigonometryMultipliersArgumentsCopyAndItsDegrees();
   distributeTrigonometryArgumentsMultipliersRatios(arguments_multipliers, arguments);

   auto res = chooseInstructionsToCastTrigonometryArguments(arguments_multipliers);
   bool need_to_convert = res.second;
   auto instructions = std::move(res.first);
    if (need_to_convert)
        this->convertTrigonometricalMultipliersToDifferentArgument(instructions);
    if (need_to_convert)
    {
        this->simplified = false;
        this->simplify();
    }
}

std::map<QString, TrigonometricalFunctionsCastType> Fractal::getTrigonometricalFunctionToTurnIntoIt()
{
    std::map<QString, std::tuple<bool, bool, bool, bool, bool, bool, bool, bool>> params;
    this->checkTrigonometricalFunctionsItHas(params);
    return chooseConversionRules(params);
}

std::unique_ptr<Fractal> operator+(const std::unique_ptr<Fractal> &left, const std::unique_ptr<Fractal> &right)
{
    return *left + right;
}
std::unique_ptr<Fractal> operator-(const std::unique_ptr<Fractal> & left, const std::unique_ptr<Fractal> & right)
{
    return *left - right;
}

std::unique_ptr<AbstractExpression> toAbsEx(const std::unique_ptr<Fractal> &expr)
{
    return abs_ex(new Fractal(expr.get()));
}

std::unique_ptr<AbstractExpression> toAbsEx(std::unique_ptr<Fractal> &&expr)
{
    return abs_ex(expr.release());
}

std::unique_ptr<Fractal> toFrac(std::unique_ptr<AbstractExpression> &expr)
{
    return std::unique_ptr<Fractal>(new Fractal(expr.get()));
}

std::unique_ptr<Fractal> toFrac(std::unique_ptr<AbstractExpression> &&expr)
{
    return std::unique_ptr<Fractal>(new Fractal(std::move(expr)));
}
