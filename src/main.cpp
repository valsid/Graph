#include "mainwindow.h"
#include <QApplication>

#include <random>
#include <ctime>

int rand_int(int min, int max)
{
    static std::mt19937 re(time(0));
    std::uniform_int_distribution<int> uid(min, max);
    return uid(re);
}

double normal_rand_number(double mean, double stddev)
{
    static std::mt19937 re(time(0));
    std::normal_distribution<double> uid(mean, stddev);
    return uid(re);
}

#include <QTime>
#include <QtMath>

qreal cLength(qreal dx, qreal dy)
{
//    return qSqrt(qPow(dx, 2.0) + qPow(dy, 2.0));
//    return qSqrt(dx * dx + dy * dy);
//    qreal tmp = dy / dx; return abs(dx) * qSqrt(1 + tmp * tmp);
    return dx / qCos(qAtan(dy / dx));
}

int main(int argc, char *argv[])
{
//    QTime t;
//    t.start();
//    clock_t t1 = clock();


//    qreal dx = 100;
//    qreal dy = 530;
//    QLineF line(0, 0, 100, 530);

//    qreal length = sqrt( dx*dx + dy*dy );
//    qreal length2 = dx / cos(atan(dy / dx));
//    qDebug() << length;
//    qDebug() << length2;
//    qDebug() << line.length();
//    if(length != length2) {
//        qDebug() << "length != length1" << length << length2;
//    }

//    for(int i = 0; i < 2147483647; i++) {
//        volatile qreal t = cLength(line.dx(), line.dy());
//        volatile qreal t = line.length();
//    }
//    clock_t t2 = clock();

//    qDebug() << t.elapsed();
//    qDebug() << t2 - t1;

/*
    const int arrLength = 10;
    int arr[arrLength] = {};
    for(int i = 0; i < 100000; i++) {
        int rNum = normal_rand_number(5, 1.9);
        if(rNum >= 0 && rNum <= 10) arr[rNum]++;
    }

    for(int i = 0; i < arrLength; i++) {
        qDebug() << arr[i];
    }
// */

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
