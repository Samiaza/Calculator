#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QLocale::setDefault(QLocale(QLocale::C));
  setenv("LC_NUMERIC", "C", 1);
  QApplication app(argc, argv);
  smartcalc::model::CalcCore calccore;
  smartcalc::controller::CalcController calccontroller(&calccore);
  smartcalc::view::MainWindow mainwindow(&calccontroller);
  mainwindow.show();
  return app.exec();
}
