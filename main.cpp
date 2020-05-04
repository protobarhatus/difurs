#include "mainwindow.h"
#include <QApplication>
#include "algebra\algexpr.h"
#include "tests.h"
#include "algebra\some_algebra_expression_conversions.h"
#include "random"
#include "QDataStream"
//#define IN_MOSCOW
#ifdef IN_MOSCOW
#include <G:\QTProjects\mo2\ExprMake\texpr_builder.h>
#include <G:\QTProjects\mo2\testpaintwidget.h>
#endif
#define A(x) AlgExpr(x)
#define x2 pow(x, 2)
#define x3 pow(x, 3)
#define x4 pow(x, 4)
int rand(int min, int max)
{
   static std::random_device rt;
     static std::mt19937_64 gen(rt());
     std::uniform_int_distribution<int> dist(min, max);

     return dist(rt);
}


int main(int argc, char *argv[])
{
   // testAlgMod();
    qDebug() << "#########";
    AlgExpr x = var();

    AlgExpr y = var();
    AlgExpr z = var();

    AlgExpr a = var();
    AlgExpr b = var();
    AlgExpr c = var();
    AlgExpr d = var();

    AlgExpr t = var();

    //c = (a + b) / 2;
   // TExpr_Builder builder;
   // auto expr = builder.BuildExpr("x^2+x+1");

//TODO: разобраться с ситуациями по типу
    //log(tan(x)) - (log(sin(x)) - log(cos(x))
    //2log(cos(x)) - log(1-sin(x)^2)
    //ТАКЖЕ, если  синусы и косинусы раскрываются по сумме всегда сразу, то мы лишаемся
    //возможности взять интегралы вида 1/(sin(ax+b)cos(ax+d)), 1/(sin(ax+b)^2cos(ax+d)) и т. д.
    //это надо исправить

   c = sqrt(x*x);

    //c = ;
   // c.out();

    qDebug();
    qDebug() << "######################################################";
    qDebug() << c.toString();


    QApplication sdsd(argc, argv);
#ifdef IN_MOSCOW
    TestPaintWidget widg;
    widg.setFontSize(18);

    widg.setExpr(c.toString());
    widg.show();
#endif
    MainWindow w;


    return sdsd.exec();
}
