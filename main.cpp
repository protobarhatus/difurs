#include "mainwindow.h"
#include <QApplication>
#include "algebra\algexpr.h"
#include "tests.h"
#include "algebra\some_algebra_expression_conversions.h"
#include "random"
#include "QDataStream"
#define IN_MOSCOW
#ifdef IN_MOSCOW
#include <G:\QTProjects\mo2\ExprMake\texpr_builder.h>
#include <G:\QTProjects\mo2\testpaintwidget.h>
#endif
#define A(x) AlgExpr(x)

int rand(int min, int max)
{
   static std::random_device rt;
     static std::mt19937_64 gen(rt());
     std::uniform_int_distribution<int> dist(min, max);

     return dist(rt);
}


int main(int argc, char *argv[])
{
    testAlgMod();
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
            // c = tan(x/2)*tan(x/2) + 2*tan(x) + tan(2*x);
        //d = tan(x/2)*tan(x/2) + tan(x);
        //  c = tan(x) + tan(2*x);
        //  c = c + d;
y = cos(x);
x = sin(x);
                        //c = (-1 + y + x + x*x + y*x)/y/(1 + y) + (x/y)*((-(x/y)*(x/y) + 3)/(-(x/y)*(x/y) + 1));
    c = (-1 + y + x + x*x + y*x)/y/(1 + y);
    d = (x/y)*((-(x/y)*(x/y) + 3)/(-(x/y)*(x/y) + 1));
    c = c + d;

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
