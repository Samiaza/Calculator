#ifndef CALCULATOR_VIEW_MAINWINDOW_H_
#define CALCULATOR_VIEW_MAINWINDOW_H_

#include <QMainWindow>

#include "calccontroller.h"
#include "qcustomplot.h"
#include "structs.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace smartcalc {

namespace view {

using namespace smartcalc::common;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow() = delete;
  MainWindow(MainWindow &&) = delete;
  MainWindow(MainWindow const &) = delete;
  MainWindow &operator=(MainWindow &&) = delete;
  MainWindow &operator=(MainWindow const &) = delete;
  explicit MainWindow(controller::CalcController *controller,
                      QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_pushButtonCalc_clicked();

  void on_pushButtonPlot_clicked();

  void on_lineEditXmin_editingFinished();

  void on_lineEditXmax_editingFinished();

  void on_lineEditYmin_editingFinished();

  void on_lineEditYmax_editingFinished();

  void xRangeChanged(const QCPRange &newRange);

  void yRangeChanged(const QCPRange &newRange);

  void setScale();

  void on_lineEditXmin_textChanged(const QString &arg1);

  void on_lineEditXmax_textChanged(const QString &arg1);

  void on_lineEditYmin_textChanged(const QString &arg1);

  void on_lineEditYmax_textChanged(const QString &arg1);

  void on_lineEditMain_textChanged(const QString &arg1);

  void on_pushButtonClear_clicked(bool checked);

  void on_pushButtonCrCalc_clicked();

  void on_pushButtonRefillAdd_clicked();

  void on_pushButtonRefillClear_clicked();

  void on_pushButtonRefillDel_clicked();

  void on_pushButtonOutAdd_clicked();

  void on_pushButtonOutDel_clicked();

  void on_pushButtonOutClear_clicked();

  void on_pushButtonDepCalc_clicked();

 private:
  static constexpr double kMinX_ = -1000000;
  static constexpr double kMaxX_ = 1000000;
  static constexpr double kMinY_ = -1000000;
  static constexpr double kMaxY_ = 1000000;
  static constexpr double kRangeX_ = 2000000;
  static constexpr double kRangeY_ = 2000000;
  static constexpr double kNumberDots_ = 100000;

  Ui::MainWindow *ui_;
  controller::CalcController *controller_;
  model::expr_type expression_;
};

}  // namespace view

}  // namespace smartcalc

#endif  // CALCULATOR_VIEW_MAINWINDOW_H_
