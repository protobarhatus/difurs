#include <vector>
#include "abstractexpression.h"
#include "number.h"
#include "some_algebra_expression_conversions.h"
//#include "fractal.h"
#include "polynomial.h"
#include "variablesdistributor.h"
#include "Matrix.h"
#include "solving_equations.h"
#include "algexpr.h"
#include "polynom.h"
#include "polynomials_factorization.h"
#include "sinus.h"
#include "cosinus.h"
#include "tangent.h"
#include "cotangent.h"
#include "logarithm.h"
#include "arctangent.h"
#include "arcsinus.h"
#include "constant.h"
#include "mathsets.h"
std::list<abs_ex> factorizePolynomOfOneVariable(const abs_ex & polynom, int var);
std::list<abs_ex> _solveEquation(const abs_ex & equation, int var);
bool isPolynomOfAllVariables(const abs_ex & equation)
{
    auto set = equation->getSetOfVariables();
    for (auto &it : set)
    {
        auto chp = checkIfItsPolynom(equation, it);
        if (chp.size() == 0 || chp[0] == nullptr)
            return false;
    }
    return true;
}
//возвратное уравнение, хз как правильно на english
std::list<abs_ex> checkIfitsReturnEquationAndTryToSolve(const abs_ex & equation, int var)
{
    std::list<abs_ex> res;
    auto pol = checkIfItsPolynom(equation, var);
    //если размер массива четный, то степень уравнения нечетная, а тогда оно само должно разложиться в факторизации
    //ну и если это не полином, то тоже отсюда выйдет
    if (pol.size() % 2 == 0)
        return res;
    int center = pol.size() / 2;
    if (*pol[center - 1] == *zero || *pol[center + 1] == *zero)
        return res;
    abs_ex ratio = pol[center - 1]/pol[center + 1];

    for (int i = 2; i <= pol.size()/2; ++i)
    {
        if (isZero(pol[center + i]) && !isZero(pol[center + i]))
            return res;
        if (!subCompare(pol[center - i]/pol[center + i], pow(ratio, i)))
            return res;

    }
    abs_ex new_equation = std::move(pol[center]);
    abs_ex temp_var = systemVarExpr();
    //построение выражений вида x^n + a^n/x^n, выраженных через t, где t = x + a/x. индекс == n, нулевой индекс не рабочий
    std::vector<abs_ex> coefs_expressions(pol.size() / 2 + 1);
    coefs_expressions[1] = copy(temp_var);
    for (int i = 2; i <= pol.size() / 2; ++i)
    {

        abs_ex sub = copy(zero);
        auto coefs = MathSets::getBinomialCoefs(i);
        for (int j = 1; j < coefs.size()/2; ++j)
        {
            sub = std::move(sub) + numToAbs(coefs[j])*coefs_expressions[coefs.size() - 2*j - 1];
        }
        if (coefs.size() % 2 != 0)
            sub = std::move(sub) + numToAbs(coefs.size() / 2)*pow(ratio, coefs.size()/2);
        coefs_expressions[i] = pow(temp_var, i) - std::move(sub);
    }
    for (int i = 2; i <= pol.size()/2; ++i)
    {
        if (!isZero(pol[center + i]))
        {

            new_equation = std::move(new_equation) + std::move(pol[center + i])*coefs_expressions[i];
        }
    }
    res = _solveEquation(equation, var);
    abs_ex t_expr = getVariableExpr(var) + ratio/getVariableExpr(var);
    std::list<abs_ex> actual_res;
    for (auto &it : res)
    {
        setUpExpressionIntoVariable(it, t_expr, temp_var->getId());
        actual_res.splice(actual_res.end(), solveEquation(it, var));
    }
    return res;

}
//имеются ввиду уравнения по типу 2^2x + 2^x + 2 == a
std::list<abs_ex> checkIfItsExponentialSumEquationAndTryToSolve(abs_ex && equation, abs_ex && right, int var)
{
    std::list<abs_ex> res;
    auto monoms = static_cast<Polynomial*>(equation.get())->getMonomialsPointers();
    auto downcasted_frac = monoms.begin()._Ptr->_Myval->downcast();
    auto basis = copy(Degree::getArgumentOfDegree(downcasted_frac.get()));
    if (basis->hasVariable(var))
        return res;
    if (basis->getPositionRelativelyZero() < 0)
    {
        downcasted_frac = -downcasted_frac;
        basis = -basis;
        equation = -equation;
        right = -right;
    }
    auto deg = Degree::getDegreeOfExpression(downcasted_frac.get());
    abs_ex temp_var = systemVarExpr();
    abs_ex new_equation = copy(temp_var);
    for (auto it = next(monoms.begin()); it != monoms.end(); ++it)
    {
        if (!subCompare(copy(Degree::getArgumentOfDegree(it._Ptr->_Myval)), basis))
            return res;
        if ((Degree::getDegreeOfExpression(it._Ptr->_Myval)/deg )->getId() != NUMBER)
            return res;
        new_equation = std::move(new_equation) + pow(temp_var, (Degree::getDegreeOfExpression(it._Ptr->_Myval)/deg ));
    }
    new_equation = std::move(new_equation) - std::move(right);
    auto temp_res = solveEquation(new_equation, temp_var->getId());
    for (auto &it : temp_res)
    {
        if (it->getPositionRelativelyZero() > 0)
        {
            res.splice(res.end(), solveEquation(downcasted_frac - it, var));
        }
    }
    return res;

}
std::list<abs_ex> checkIfItsLogarithmicSumEquationAndTryToSolve(const abs_ex & equation, const abs_ex & right, int var)
{
    std::list<abs_ex> res;
    auto monoms = static_cast<Polynomial*>(equation.get())->getMonomialsPointers();
    abs_ex new_equation = copy(one);
    for (auto &it : monoms)
    {
        auto fr_without_var = toAbsEx(it->getFractalWithoutVariable(var));
        auto fr_with_var = copy(it)/fr_without_var;
        if (fr_with_var->getId() != LOGARITHM)
            return res;
        //думаю, этот иф бессмысленен, однако я не совсем уверен с тем, как степени там даункастятся
        if (it->getCoefficient().compareWith(0) > 0)
            new_equation = std::move(new_equation) * pow(getArgumentOfFunction(fr_with_var), fr_without_var);
        else
            new_equation = std::move(new_equation) / pow(getArgumentOfFunction(fr_with_var), -fr_without_var);
    }
    return solveEquation(new_equation - pow(getEuler(), right), var);

}
//вещи по типу sqrt(x) = a, или sqrt(x) = f(x)
std::list<abs_ex> solveEquationOfSpecialCases(const abs_ex & var_expr, const abs_ex & right_expr, int var)
{
    std::list<abs_ex> res;
    if (var_expr->getId() == DEGREE)
    {
        auto arg = Degree::getArgumentOfDegree(var_expr.get());
        auto deg = Degree::getDegreeOfExpression(var_expr.get());
        if (arg->hasVariable(var) && !deg->hasVariable(var)) //условия
        {
            if (deg->getId() == NUMBER || deg->getId() == FRACTAL)
            {
                Number num;
                if (deg->getId() == NUMBER)
                    num = *static_cast<Number*>(deg.get());
                else
                    num = static_cast<Fractal*>(deg.get())->getCoefficient();
                if (num.getNumerator() % 2 == 0) //(f(x))^2 == g  <=> [f(x)=sqrt(g), f(x) =-sqrt(g)]
                {
                    if (right_expr->getPositionRelativelyZero() < 0)
                        return res;
                    res.splice(res.end(), solveEquation(copy(arg) - pow(right_expr, one/deg), var,
                                                        RootCondition(var, RootCondition::BIGGER_THAN_ZERO, right_expr)));
                    res.splice(res.end(), solveEquation(copy(arg) + pow(right_expr, one/deg), var,
                                                        RootCondition(var, RootCondition::BIGGER_THAN_ZERO, right_expr)));
                    return res;
                }
                if (num.getDenominator() % 2 == 0)
                {
                    if (right_expr->getPositionRelativelyZero() < 0)
                        return res;
                    return solveEquation(copy(arg) - pow(right_expr, one/deg), var,
                    { RootCondition(var, RootCondition::BIGGER_THAN_ZERO, copy(arg)),
                      RootCondition(var, RootCondition::BIGGER_THAN_ZERO, right_expr)});
                }

            }
            return solveEquation(copy(arg) - pow(right_expr, one/deg), var);
        }
        if (!arg->hasVariable(var) && deg->hasVariable(var))
        {
            if (right_expr->getPositionRelativelyZero() < 0)
                return res;
            //иначе можем упасть в бесконечный цикл возведения в в степень
            if (Degree::getArgumentOfDegree(right_expr.get())->hasVariable(var))
                return res;
            return solveEquation(deg - ln(right_expr)/ln(copy(arg)), var);
        }
        return res;
    }
    if (var_expr->getId() == ABSOLUTE_VALUE)
    {
        if (right_expr->getPositionRelativelyZero() < 0)
            return res;
        auto arg = copy(static_cast<AbsoluteValue*>(var_expr.get())->getExpression());
        res.splice(res.end(), solveEquation(arg - right_expr, var,
        {RootCondition(var, RootCondition::BIGGER_THAN_ZERO, arg),
         RootCondition(var, RootCondition::BIGGER_THAN_ZERO, right_expr)}));

        res.splice(res.end(), solveEquation(arg + right_expr, var,
        {RootCondition(var, RootCondition::LESS_THAN_ZERO, arg),
         RootCondition(var, RootCondition::BIGGER_THAN_ZERO, right_expr)}));
        return res;
    }/*
            SINUS = -6,
            COSINUS = -7,
            TANGENT = -8,
            COTANGENT = -9,
            CONSTANT = -10,
            LOGARITHM = -11,
            DIFFERENTIAL = -19,
            ARCTANGENT = -12,
            ARCSINUS = -13,*/


}
std::list<abs_ex> solveEquationOfPolynom(const std::unique_ptr<Polynomial> & equation, int var)
{
    std::list<abs_ex> res;
    res = checkIfitsReturnEquationAndTryToSolve(copy(equation.get()), var);
    if (res.size() > 0)
        return res;

    abs_ex var_expr = copy(zero);
    abs_ex right_expr = copy(zero);
    auto monoms = equation->getMonomialsPointers();
    for (auto &it : monoms)
    {
        if (it->hasVariable(var))
            var_expr = std::move(var_expr) + copy(it);
        else
            right_expr = std::move(right_expr) - copy(it);
    }
    if (var_expr->getId() == POLYNOMIAL)
    {
        auto common_part = static_cast<Polynomial*>(var_expr.get())->reduceCommonPart();
        var_expr = std::move(common_part) * std::move(var_expr);
    }
    if (var_expr->getId() == FRACTAL)
    {
        auto mult = static_cast<Fractal*>(var_expr.get())->getFractalWithoutVariable(var);
        var_expr = std::move(var_expr) / toAbsEx(mult);
        right_expr = std::move(right_expr) * toAbsEx(mult);
    }
    var_expr = var_expr + zero;
    if (var_expr->getId() == var)
    {
        res.push_back(std::move(right_expr));
        return res;
    }
    if (var_expr->getId() == POLYNOMIAL)
    {
        //если это все еще полином, то это либо уравнение общего вида,
        //либо уравнение по типу sqrt(x) = x + 3, которое будет разобрано после, либо
        //уравнение общего вида, которое хер пойми как решать, либо
        //уравнение по типу 2^2x + 2^x + 2 = a, либо сумма логарифмов, либо тригонометрическое уравнение,
        //либо полином
        res = checkIfItsExponentialSumEquationAndTryToSolve(copy(var_expr), copy(right_expr), var);
        if (res.size() > 0)
            return res;
        res = checkIfItsLogarithmicSumEquationAndTryToSolve(var_expr, right_expr, var);
        if (res.size() > 0)
            return res;
        if (isPolynomOfAllVariables(var_expr))
        {
            auto deg = static_cast<Polynomial*>(var_expr.get())->tryToDistinguishFullDegree();
            if (deg != nullptr)
                var_expr = std::move(deg);
        }
    }

    res = solveEquationOfSpecialCases(var_expr, right_expr, var);
    if (res.size() > 0)
        return


    return res;

}
std::list<abs_ex> _solveEquation(const abs_ex & equation, int var)
{
    std::list<abs_ex> res;
    if (equation->getId() == NUMBER)
    {
        if (static_cast<Number*>(equation.get())->isZero())
            res.push_back(integratingConstantExpr());
        return res;
    }
    if (!equation->hasVariable(var))
        return res;
    if (equation->getId() == var)
    {
        res.push_back(copy(zero));
        return res;
    }
    if (equation->getId() > 0)
        return res;
    if (equation->getId() == DEGREE)
    {
        return _solveEquation(copy(Degree::getArgumentOfDegree(equation.get())), var);
    }
    if (equation->getId() == FRACTAL)
    {
        auto fr = static_cast<Fractal*>(equation.get())->getFractal();
        for (auto &it : *fr.first)
            res.splice(res.end(), _solveEquation(it, var));
    }
    if (equation->getId() == ABSOLUTE_VALUE)
        return _solveEquation(copy(static_cast<AbsoluteValue*>(equation.get())->getExpression()), var);
    if (equation->getId() == SINUS)
        return _solveEquation(static_cast<Sinus*>(equation.get())->getArgumentsCopy() - getPi()*systemVarExpr(), var);
    if (equation->getId() == COSINUS)
        return _solveEquation(static_cast<Cosinus*>(equation.get())->getArgumentsCopy() - getPi()/two - getPi()*systemVarExpr(), var);
    if (equation->getId() == TANGENT)
        return _solveEquation(static_cast<Tangent*>(equation.get())->getArgumentsCopy() - getPi()*systemVarExpr(), var);
    if (equation->getId() == COTANGENT)
        return _solveEquation(static_cast<Cotangent*>(equation.get())->getArgumentsCopy() - getPi()*systemVarExpr(), var);
    if (equation->getId() == LOGARITHM)
        return _solveEquation(static_cast<Logarithm*>(equation.get())->getArgumentsCopy() - one, var);
    if (equation->getId() == CONSTANT)
        return res;
    if (equation->getId() == ARCTANGENT)
        return _solveEquation(static_cast<ArcTangent*>(equation.get())->getArgumentsCopy(), var);
    if (equation->getId() == ARCSINUS)
        return _solveEquation(static_cast<ArcSinus*>(equation.get())->getArgumentsCopy(), var);

    //is linear equation
    auto ln_f = checkIfItsLinearFunction(equation, var);
    if (ln_f.first != nullptr)
    {
        res.push_back( -ln_f.second / ln_f.first);
        return res;
    }
    //is quadratic equation
    auto qc_f = checkIfItsQuadraticFunction(equation, var);
    if (qc_f[0] != nullptr)
    {
        auto D = sqr(qc_f[1]) - four*qc_f[0]*qc_f[2];
        if (*D == *zero)
        {
            res.push_back( -qc_f[1]/two/qc_f[0]);
            return res;
        }
        if (D->getPositionRelativelyZero() < 0)
        {
            return res;
        }
        res.push_back( (-qc_f[1] + sqrt(D))/(two*qc_f[0]));
        res.push_back( (-qc_f[1] - sqrt(D))/(two*qc_f[0]));
        return res;
    }
    if (isPolynomOfAllVariables(equation))
    {
        auto facts = factorizePolynom(equation);
        if (facts.first.size() > 1 || (facts.first.size() == 1 && facts.first.begin()->get()->getId() == DEGREE))
        {
            for (auto &it : facts.first)
            {
                res.splice(res.end(), _solveEquation(copy(Degree::getArgumentOfDegree(it.get())), var));
            }
        }
        return res;
    }
    if (equation->getId() == POLYNOMIAL)
    {
        auto common_part = static_cast<Polynomial*>(equation.get())->reduceCommonPart();
        //qDebug() << common_part->makeStringOfExpression();
        //qDebug() << equation->makeStringOfExpression();
        if (common_part != nullptr && *common_part != *one)
        {
            res.splice(res.end(), _solveEquation(common_part, var));
            res.splice(res.end(), _solveEquation(equation, var));
            return res;
        }
        return solveEquationOfPolynom(toPolynomialPointer(equation), var);
    }
        //return { -ln_f.second / ln_f.first};
    return res;
}
std::list<std::unique_ptr<AbstractExpression> > solveEquation(const std::unique_ptr<AbstractExpression> &equation, int var)
{

    int lcm_of_dens = equation->getLcmOfDenominatorsOfDegreesOfVariable(var);
    if (lcm_of_dens > 1)
    {
        abs_ex temp_var;
        if (lcm_of_dens % 2 == 0)
            temp_var = abs_ex(new Variable(systemVar(0, std::numeric_limits<int>::max())));
        else
            temp_var = systemVarExpr();
       abs_ex repl_equation = copy(equation);
       //сделать условие что t > 0 если lcm_of_dens % 2 == 0
        setUpExpressionIntoVariable(repl_equation, pow(temp_var, lcm_of_dens), var);
        auto res = _solveEquation(repl_equation, temp_var->getId());
        std::list<abs_ex> actual_res;
        for (auto &it : res)
            if (lcm_of_dens % 2 != 0 || it->getPositionRelativelyZero() >= 0)
                actual_res.push_back( pow(it, lcm_of_dens));
        return actual_res;

    }
    int gcd_of_nums = equation->getGcdOfNumeratorsOfDegrees(var);
    if (gcd_of_nums > 1)
    {
        abs_ex temp_var;
        if (gcd_of_nums % 2 == 0)
            temp_var = abs_ex(new Variable(systemVar(0, std::numeric_limits<int>::max())));
        else
            temp_var = systemVarExpr();
        abs_ex repl_equation = copy(equation);
        setUpExpressionIntoVariable(repl_equation, pow(temp_var, Number(1, gcd_of_nums)), var);
        //аналогично условие t > 0 если gcd_of_nums % 2 == 0
        auto res = _solveEquation(repl_equation, temp_var->getId());
        if (gcd_of_nums % 2 == 0)
        {
            std::list<abs_ex> actual_res;
            for (auto &it : res)
                if (it->getPositionRelativelyZero() >= 0)
                {
                    actual_res.push_back(pow(it, one/numToAbs(gcd_of_nums)));
                    actual_res.push_back(-pow(it, one/numToAbs(gcd_of_nums)));
                }
            return actual_res;
        }
        for (auto &it : res)
            it = pow(it, one/numToAbs(gcd_of_nums));
        return res;
    }
    return _solveEquation(equation, var);
}

















//возвращает коэффициент и индекс переменной в векторе, к которой этот коэффициент принадлежит. Если там есть другие переменные из вектора или оно нелинейно относительно
//какой-л переменной то возвразщает {nullptr, }
//индекс vars.size() означает что это свободный член
std::pair<abs_ex, int> getLinearCoeffientOfVariable(AbstractExpression * monom, const std::vector<int> & vars)
{
    assert(monom->getId() != POLYNOMIAL);
    int variable_id_in_monom = -1;
    auto set_of_vars = monom->getSetOfVariables();
    for (int i = 0; i < vars.size(); ++i)
    {
        if (set_of_vars.find(vars[i]) != set_of_vars.end())
        {
            if (variable_id_in_monom == -1)
                variable_id_in_monom = i;
            else
                variable_id_in_monom = -2;
        }
    }
    if (variable_id_in_monom == -2)
        return {nullptr, -1};
    if (variable_id_in_monom == -1)
        return {copy(monom), vars.size()};
    auto ln_f = checkIfItsLinearFunction(monom, vars[variable_id_in_monom]);
    return {std::move(ln_f.first), variable_id_in_monom};

}
//последний коэффициент - свободный член. result[0] == nullptr значит что уравнение не линейно относительно этого набора переменных
std::vector<abs_ex> checkIfEquationIsLinearAnsGetCoefficients(const abs_ex & equation, const std::vector<int> & vars)
{
    std::vector<abs_ex> result(vars.size() + 1);
    for (auto &it : result)
        it = copy(zero);
    if (equation->getId() == POLYNOMIAL)
    {
        auto pol = static_cast<Polynomial*>(equation.get())->getMonomialsPointers();
        for (auto &it : pol)
        {
            auto coefficient = getLinearCoeffientOfVariable(it, vars);
            if (coefficient.first == nullptr)
            {
                result[0] = nullptr;
                return result;
            }
            result[coefficient.second] = std::move(result[coefficient.second]) + std::move(coefficient.first);
        }
        return result;
    }
    auto coefficient = getLinearCoeffientOfVariable(equation.get(), vars);
    if (coefficient.first == nullptr)
    {
        result[0] = nullptr;
        return result;
    }
    result[coefficient.second] =  std::move(coefficient.first);
    return result;
}
//возвращает корни в том же порядке, что и vars. если система нелинейна, то возвращает { , -2}. Если система линейна, но не имеет решений, то возвращает { , -1}
//если система имеет единственное решение, то возвращает { , 0}. если бесконечное множество решений, вернет { , > 0},
//теоретически здесь я бы мог продумать вариант с параметром, но для текущих целей - метода неопределенных
//коэффициентов, это не нужно.
std::pair<std::vector<abs_ex>, int> checkIfSystemOfEquationsLinearAndTryToSolveIt(const std::vector<abs_ex> & system, const std::vector<int>& vars)
{
    Matrix<AlgExpr> system_matrix(system.size(), vars.size() + 1);
    for (int i = 0; i < system.size(); ++i)
    {
        auto coefs = checkIfEquationIsLinearAnsGetCoefficients(system[i], vars);
        if (coefs[0] == nullptr)
            return {std::vector<abs_ex>(0), -2};
        coefs.back() = -coefs.back();
        std::vector<AlgExpr> algexpr_coefs(coefs.size());
        for (int i = 0; i < coefs.size(); ++i)
            algexpr_coefs[i] = AlgExpr(std::move(coefs[i]));
        system_matrix[i] = std::move(algexpr_coefs);
    }
    auto result = gauss(std::move(system_matrix));
    if (result.size() == 0)
        return {std::vector<abs_ex>(0), -2};

    //при решении линейных уравнений в результате может быть пареметр, однако сейчас мы упрощаем и рассматриваем
    //без него
    std::vector<abs_ex> result_vector(result.size());
    for (int i = 0; i < result.size(); ++i)
        result_vector[i] = std::move(result[i].back().getExpr());
    return {std::move(result_vector), 0};
}
std::vector<std::vector<abs_ex>> solveSystemOfEquations(const std::vector<abs_ex>& equations, const std::vector<int>& vars)
{
    //возможность решать это с параметром, если необходимо, введу позже
    assert(equations.size() >= vars.size());
    auto linear_system_res = checkIfSystemOfEquationsLinearAndTryToSolveIt(equations, vars);
    if (linear_system_res.second >= 0)
    {
        std::vector<std::vector<abs_ex>> res(vars.size());
        for (int i = 0; i < linear_system_res.first.size(); ++i)
        {
            res[i].resize(1);
            res[i][0] = std::move(linear_system_res.first[i]);
        }
        return res;
    }
    std::vector<std::vector<abs_ex>> res(0);
    return res;
}
std::list<abs_ex> toFactorOfCoefs(const abs_ex & polynom)
{
    auto set = polynom->getSetOfPolyVariables();

    std::list<abs_ex> result;
    if (set.size() <= 1)
    {
        result.push_back(copy(polynom));
        return result;
    }
    for (auto &it : set)
    {
        auto pol = checkIfItsPolynom(polynom, it);
       // qDebug() << "COEFS: ";
        //for (auto &it : pol)
         //   qDebug() << it->makeStringOfExpression();
        if (pol.size() > 0 && pol[0] != nullptr)
        {
            auto gcf = std::move(pol[0]);
            if (gcf->getSetOfVariables().empty())
            {
                result.push_back(copy(polynom));
                return result;
            }
            for (int i = 1; i < pol.size(); ++i)
            {
                Polynomial gcf_pol(gcf.get());
                Polynomial coef_pol(pol[i].get());
                if (coef_pol.getSetOfPolyVariables().empty())
                {
                    result.push_back(copy(polynom));
                    return result;
                }
                gcf_pol.reduce();
                coef_pol.reduce();
                gcf = gcd(&gcf_pol, &coef_pol);
            }
            if (!gcf->getSetOfVariables().empty())
            {
                auto second = polynom / gcf;
                result.splice(result.end(), toFactorOfCoefs(gcf));
                result.splice(result.end(), toFactorOfCoefs(second));
                return result;
            }
        }
    }
    result.push_back(copy(polynom));
    return result;
}
std::list<abs_ex> factorizeAsQuadraticFunction(const abs_ex & polynom)
{
    auto set = polynom->getSetOfVariables();
    std::list<abs_ex> result;
    for (auto &it : set)
    {
        auto qc_f = checkIfItsQuadraticFunction(polynom, it);
        if (qc_f[0] != nullptr)
        {
            auto D = sqr(qc_f[1]) - four*qc_f[0]*qc_f[2];
            //полная степень должна выделиться раньше
            if (::isZero(D))
            {
                assert(false);
            }
            if (D->getPositionRelativelyZero() > 0)
            {

                auto w = (-qc_f[1] + sqrt(D))/two/qc_f[0];
                auto u = (-qc_f[1] - sqrt(D))/two/qc_f[0];
                abs_ex x (new Variable(getVariable(it)));
                auto res = qc_f[0]*(x - w)*(x - u);
                if (res->getId() == FRACTAL)
                {
                    auto fr = static_cast<Fractal*>(res.get());
                    if (fr->getCoefficient().isOne() && fr->getFractal().second->size() == 0 &&
                            fr->getFractal().first->size() == 2 && fr->getFractal().first->begin()->get()->getId() == POLYNOMIAL &&
                            next(fr->getFractal().first->begin())->get()->getId() == POLYNOMIAL)
                    {
                        result.push_back(std::move(*fr->getFractal().first->begin()));
                        result.push_back(std::move(*next(fr->getFractal().first->begin())));
                        return result;
                    }
                }
            }
        }
    }
    result.push_back(copy(polynom));
    return result;
}
//это можно расширять, но пока только так
std::list<abs_ex> factorizePolynomOfSeveralVariablesByAlgorithmOfCoefsFacts(const abs_ex & polynom)
{
    std::list<abs_ex> res = toFactorOfCoefs(polynom);
    if (res.size() == 1)
        return res;
    for (auto it = res.begin(); it != res.end(); ++it)
    {
        if (it->get()->getSetOfVariables().size() == 1)
        {
            res.splice(res.end(), factorizePolynomOfOneVariable(*it, *it->get()->getSetOfVariables().begin()));
            it = res.erase(it);
        }
        else
        {
            if (it->get()->getId() == POLYNOMIAL)
            {
                auto degr = static_cast<Polynomial*>(it->get())->tryToDistinguishFullDegree();
                if (degr != nullptr)
                    *it = std::move(degr);
            }
        }
    }
    return res;
}
std::list<abs_ex> factorizePolynomOfSeveralVariables(const abs_ex & polynom)
{
    auto full_deg_res = static_cast<Polynomial*>(polynom.get())->tryToDistinguishFullDegree();
    if (full_deg_res != nullptr && full_deg_res->getId() == DEGREE)
    {
        auto deg = Degree::getDegreeOfExpression(full_deg_res.get());
        auto res = factorizePolynomOfSeveralVariables(copy(Degree::getArgumentOfDegree(full_deg_res.get())));
        for (auto &it : res)
            it = pow(std::move(it), deg);
        return res;
    }
    auto res = factorizePolynomOfSeveralVariablesByAlgorithmOfCoefsFacts(polynom);
    for (auto it = res.begin(); it != res.end(); )
    {
        auto facts = factorizeAsQuadraticFunction(*it);
        if (facts.size() > 1)
        {
            it = res.erase(it);
            res.splice(res.begin(), std::move(facts));
        }
        else
            ++it;
    }
    if (res.size() == 1)
        return res;
    for (auto it = res.begin(); it != res.end(); )
    {
        auto facts = factorizePolynomOfSeveralVariables(*it);
        if (facts.size() > 1)
        {
            it = res.erase(it);
            res.splice(res.begin(), std::move(facts));

        }
        else
            ++it;
    }
    return res;
}
std::list<abs_ex> factorizePolynomOfOneVariable(const abs_ex & polynom, int var)
{
    auto pol = checkIfItsIntegerPolynom(polynom, var);
    if (pol.empty())
    {
        std::list<abs_ex> res;
        res.push_back(copy(polynom));
        return res;
    }
    Polynom polynom_to_factor(pol.size() - 1);
    for (int i = 0; i < pol.size(); ++i)
        polynom_to_factor[i] = static_cast<Number*>(pol[i].get())->getNumerator();
    auto res = factorize(polynom_to_factor);

    std::list<abs_ex> result;
    abs_ex x(new Variable(getVariable(var)));
    for (auto &it : res)
    {
        abs_ex expr = copy(zero);
        for (int i = 0; i < it.size(); ++i)
            expr = expr + abs_ex(new Number(it[i].toInt())) * pow(x, i);
        result.push_back(std::move(expr));
    }
    return result;
}
std::pair<std::list<std::unique_ptr<AbstractExpression> >, Number> factorizePolynom(const std::unique_ptr<AbstractExpression> &polynom)
{
    //в полиноме уже должно было произойти reduce и takeCommonPart
    auto set = polynom->getSetOfVariables();
    std::list<abs_ex> res;
    if (set.size() == 1)
    {
        res = factorizePolynomOfOneVariable(polynom, *set.begin());
    }
    else
        res = factorizePolynomOfSeveralVariables(polynom);

    //это для того, чтобы одинаковые множители свернуть в степень
    Fractal fr(res);
    res = std::move(*fr.getFractal().first);

    return {std::move(res), fr.getCoefficient()};

    //return {std::move(res), 1};
}



std::list<abs_ex> solveEquation(const abs_ex &equation, int var, const EquationRootsConditions &conditions)
{
    return conditions.selectRoots(solveEquation(equation, var));
}
