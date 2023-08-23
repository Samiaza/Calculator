#include "mainwindow.h"

#include "./ui_mainwindow.h"

namespace smartcalc {

namespace view {

MainWindow::MainWindow(controller::CalcController *controller, QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow), controller_(controller) {
  ui_->setupUi(this);
  ui_->lineEditRes->setPlaceholderText("0.0");
  ui_->lineEditMain->setPlaceholderText("Enter expression");
  ui_->lineEditX->setPlaceholderText("0.0");
  ui_->lineEditX->setValidator(
      new QDoubleValidator(-std::numeric_limits<double>::max(),
                           std::numeric_limits<double>::max(), 307, this));
  ui_->lineEditXmin->setValidator(new QDoubleValidator(kMinX_, kMaxX_, 7));
  ui_->lineEditXmax->setValidator(new QDoubleValidator(kMinX_, kMaxX_, 7));
  ui_->lineEditYmin->setValidator(new QDoubleValidator(kMinY_, kMaxY_, 7));
  ui_->lineEditYmax->setValidator(new QDoubleValidator(kMinY_, kMaxY_, 7));

  connect(ui_->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), this,
          SLOT(xRangeChanged(QCPRange)));

  connect(ui_->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), this,
          SLOT(yRangeChanged(QCPRange)));

  ui_->customPlot->xAxis->setRange(-2.5, 2.5);
  ui_->customPlot->yAxis->setRange(-2.5, 2.5);
  ui_->customPlot->setInteraction(QCP::iRangeDrag, true);
  ui_->customPlot->setInteraction(QCP::iRangeZoom, true);
  ui_->customPlot->addGraph();
  ui_->customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
  ui_->customPlot->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssTriangleInverted, 2));
  ui_->customPlot->graph(0)->setPen(QColor(255, 0, 0));
  ui_->customPlot->graph(0)->setAntialiased(false);

  ui_->dateEditStart->setDate(QDate::currentDate());
  ui_->dateEditRefill->setDate(QDate::currentDate());
  ui_->dateEditOut->setDate(QDate::currentDate());

  ui_->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
  ui_->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::on_pushButtonCalc_clicked() {
  double var_x = ui_->lineEditX->text().toDouble();
  model::arg_type args;
  args.push_back(var_x);
  model::arg_type vals = controller_->CalculateMath(expression_, args);
  ui_->lineEditRes->setText(vals.size() ? QString::number(vals[0]) : "Error");
}

void MainWindow::on_pushButtonPlot_clicked() {
  if (expression_.size() && controller_->CheckMathExpression(expression_)) {
    double x_min = ui_->customPlot->xAxis->range().lower;
    double x_range = ui_->customPlot->xAxis->range().size();
    double step_x = x_range / (kNumberDots_ - 1);
    model::arg_type args(kNumberDots_);
    for (size_t i = 0; i < kNumberDots_; ++i) {
      args[i] = x_min + i * step_x;
    }
    model::arg_type vals = controller_->CalculateMath(args);
    QVector<double> x = QVector<double>(args.begin(), args.end());
    QVector<double> y = QVector<double>(vals.begin(), vals.end());
    ui_->customPlot->graph(0)->setData(x, y);
    ui_->customPlot->replot();
  }
}

void MainWindow::on_pushButtonClear_clicked(bool checked) {
  ui_->lineEditMain->clear();
}

void MainWindow::on_lineEditXmin_editingFinished() {
  double tmp = ui_->lineEditXmin->text().toDouble();
  ui_->customPlot->xAxis->setRangeLower(tmp);
  ui_->customPlot->replot();
}

void MainWindow::on_lineEditXmax_editingFinished() {
  double tmp = ui_->lineEditXmax->text().toDouble();
  ui_->customPlot->xAxis->setRangeUpper(tmp);
  ui_->customPlot->replot();
}

void MainWindow::on_lineEditYmin_editingFinished() {
  double tmp = ui_->lineEditYmin->text().toDouble();
  ui_->customPlot->yAxis->setRangeLower(tmp);
  ui_->customPlot->replot();
}

void MainWindow::on_lineEditYmax_editingFinished() {
  double tmp = ui_->lineEditYmax->text().toDouble();
  ui_->customPlot->yAxis->setRangeUpper(tmp);
  ui_->customPlot->replot();
}

void MainWindow::xRangeChanged(const QCPRange &newRange) {
  QCPRange fixedRange(newRange);
  if (newRange.lower < kMinX_) fixedRange.lower = kMinX_;
  if (newRange.upper > kMaxX_) fixedRange.upper = kMaxX_;
  ui_->customPlot->xAxis->setRange(fixedRange);
  ui_->lineEditXmin->setText(QString::number(fixedRange.lower));
  ui_->lineEditXmax->setText(QString::number(fixedRange.upper));
  MainWindow::setScale();
}

void MainWindow::yRangeChanged(const QCPRange &newRange) {
  QCPRange fixedRange(newRange);
  if (newRange.lower < kMinY_) fixedRange.lower = kMinY_;
  if (newRange.upper > kMaxY_) fixedRange.upper = kMaxY_;
  ui_->customPlot->yAxis->setRange(fixedRange);
  ui_->lineEditYmin->setText(QString::number(fixedRange.lower));
  ui_->lineEditYmax->setText(QString::number(fixedRange.upper));
  MainWindow::setScale();
}

void MainWindow::setScale() {
  double rangeX, rangeY;
  rangeX = ui_->customPlot->xAxis->range().size();
  rangeY = ui_->customPlot->yAxis->range().size();
  if (rangeX < rangeY) {
    ui_->labelScaleX->setText(QString::number(rangeY / rangeX));
    ui_->labelScaleY->setText("1");
  } else if (rangeX > rangeY) {
    ui_->labelScaleY->setText(QString::number(rangeX / rangeY));
    ui_->labelScaleX->setText("1");
  } else {
    ui_->labelScaleX->setText("1");
    ui_->labelScaleY->setText("1");
  }
}

void MainWindow::on_lineEditXmin_textChanged(const QString &arg1) {
  if (ui_->lineEditXmin->hasAcceptableInput()) {
    ui_->lineEditXmin->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
  } else {
    ui_->lineEditXmin->setStyleSheet("background-color: rgba(127, 0, 0, 127);");
  }
}

void MainWindow::on_lineEditXmax_textChanged(const QString &arg1) {
  if (ui_->lineEditXmax->hasAcceptableInput()) {
    ui_->lineEditXmax->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
  } else {
    ui_->lineEditXmax->setStyleSheet("background-color: rgba(127, 0, 0, 127);");
  }
}

void MainWindow::on_lineEditYmin_textChanged(const QString &arg1) {
  if (ui_->lineEditYmin->hasAcceptableInput()) {
    ui_->lineEditYmin->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
  } else {
    ui_->lineEditYmin->setStyleSheet("background-color: rgba(127, 0, 0, 127);");
  }
}

void MainWindow::on_lineEditYmax_textChanged(const QString &arg1) {
  if (ui_->lineEditYmax->hasAcceptableInput()) {
    ui_->lineEditYmax->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
  } else {
    ui_->lineEditYmax->setStyleSheet("background-color: rgba(127, 0, 0, 127);");
  }
}

void MainWindow::on_lineEditMain_textChanged(const QString &arg1) {
  expression_ = ui_->lineEditMain->text()
                    .toLower()
                    .replace(" ", "")
                    .toUtf8()
                    .toStdString();
  if (controller_->CheckMathExpression(expression_)) {
    ui_->lineEditMain->setStyleSheet(
        "border: 2px solid black; background-color: white; "
        "selection-background-color: green; selection-color: white; "
        "border-radius: 5px;");
  } else {
    ui_->lineEditMain->setStyleSheet(
        "border: 2px solid black; background-color: "
        "rgba(127, 0 ,0 ,65); selection-background-color: green; "
        "selection-color: white; border-radius: 5px;");
  }
}

void MainWindow::on_pushButtonCrCalc_clicked() {
  credit_input cr_input = {0};
  cr_input.amount = ui_->doubleSpinBoxCrAmount->value();
  cr_input.rate = ui_->doubleSpinBoxCrPercent->value();
  cr_input.term = ui_->spinBoxMounthsCr->value();
  cr_input.type = ui_->comboBoxPaymentType->currentIndex() == 0
                      ? credit_t::kAnnuit
                      : credit_t::KDiff;
  credit_output cr_output = controller_->CalculateCredit(cr_input);
  ui_->lineEditInterestVal->setText(
      QString::number(cr_output.interest, 'f', 2));
  ui_->lineEditTotalVal->setText(QString::number(cr_output.total, 'f', 2));
  ui_->tableWidget->clearContents();
  ui_->tableWidget->setRowCount(0);
  ui_->tableWidget->verticalHeader()->setVisible(false);
  for (size_t i = 0; i < cr_output.payments.size(); i++) {
    ui_->tableWidget->insertRow(i);
    ui_->tableWidget->setItem(i, 0,
                              new QTableWidgetItem(QString::number(i + 1)));
    ui_->tableWidget->item(i, 0)->setTextAlignment(Qt::AlignCenter);
    ui_->tableWidget->setItem(
        i, 1,
        new QTableWidgetItem(QString::number(cr_output.payments[i], 'f', 2)));
    ui_->tableWidget->item(i, 1)->setTextAlignment(Qt::AlignCenter);
  }
}

void MainWindow::on_pushButtonRefillAdd_clicked() {
  double refill = ui_->doubleSpinBoxRefillVal->value();
  QString str = ui_->dateEditRefill->date().toString("dd.MM.yyyy\t") +
                QString::number(refill, 'f', 2);
  ui_->listWidgetRefills->addItem(str);
}

void MainWindow::on_pushButtonRefillDel_clicked() {
  if (ui_->listWidgetRefills->selectionModel()->selection().isEmpty() ==
      false) {
    int index = ui_->listWidgetRefills->selectionModel()
                    ->selection()
                    .indexes()
                    .front()
                    .row();
    ui_->listWidgetRefills->model()->removeRow(index);
  }
}

void MainWindow::on_pushButtonRefillClear_clicked() {
  ui_->listWidgetRefills->clear();
}

void MainWindow::on_pushButtonOutAdd_clicked() {
  double out = -ui_->doubleSpinBoxOutVal->value();
  QString str = ui_->dateEditOut->date().toString("dd.MM.yyyy\t") +
                QString::number(out, 'f', 2);
  ui_->listWidgetOuts->addItem(str);
}

void MainWindow::on_pushButtonOutDel_clicked() {
  if (ui_->listWidgetOuts->selectionModel()->selection().isEmpty() == false) {
    int index = ui_->listWidgetOuts->selectionModel()
                    ->selection()
                    .indexes()
                    .front()
                    .row();
    ui_->listWidgetOuts->model()->removeRow(index);
  }
}

void MainWindow::on_pushButtonOutClear_clicked() {
  ui_->listWidgetOuts->clear();
}

void MainWindow::on_pushButtonDepCalc_clicked() {
  deposit_input dep_input;
  QDate date = ui_->dateEditStart->date();
  dep_input.start_date = {static_cast<size_t>(date.day()),
                          static_cast<size_t>(date.month()),
                          static_cast<size_t>(date.year())};
  dep_input.amount = ui_->doubleSpinBoxDepAmount->value();
  dep_input.term = ui_->spinBoxMounthsDep->value();
  dep_input.dep_rate = ui_->doubleSpinBoxDepPercent->value();
  dep_input.tax_rate = ui_->doubleSpinBoxTaxPercent->value();
  dep_input.payment_type =
      static_cast<dep_payment_t>(ui_->comboBoxPaymentFreq->currentIndex());
  dep_input.is_capitalize = ui_->pushButtonCapitalization->isChecked();
  int refills_count = ui_->listWidgetRefills->count();
  for (int i = 0; i < refills_count; ++i) {
    QString refillstr = ui_->listWidgetRefills->item(i)->text();
    QStringList tmplist = refillstr.split('\t');
    QDate refilldate = QDate::fromString(tmplist[0], "dd.MM.yyyy");
    double refillval = tmplist[1].toDouble();
    dep_input.ops.push_back({Date{static_cast<size_t>(refilldate.day()),
                                  static_cast<size_t>(refilldate.month()),
                                  static_cast<size_t>(refilldate.year())},
                             refillval});
  }
  int outs_count = ui_->listWidgetOuts->count();
  for (int i = 0; i < outs_count; ++i) {
    QString outstr = ui_->listWidgetOuts->item(i)->text();
    QStringList tmplist = outstr.split('\t');
    QDate outdate = QDate::fromString(tmplist[0], "dd.MM.yyyy");
    double outval = tmplist[1].toDouble();
    dep_input.ops.push_back({Date{static_cast<size_t>(outdate.day()),
                                  static_cast<size_t>(outdate.month()),
                                  static_cast<size_t>(outdate.year())},
                             outval});
  }
  deposit_output dep_output = controller_->CalculateDeposit(dep_input);
  ui_->lineEditInterestDepVal->setText(
      QString::number(dep_output.interest, 'f', 2));
  ui_->lineEditTaxVal->setText(QString::number(dep_output.tax, 'f', 2));
  ui_->lineEditDepositVal->setText(QString::number(dep_output.deposit, 'f', 2));
}

}  // namespace view

}  // namespace smartcalc
