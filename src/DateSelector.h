#ifndef DATESELECTOR_H
#define DATESELECTOR_H

#include <QDialog>
#include <QKeyEvent>

#include "RollingBox.h"

namespace Ui {
class DateSelector;
}

class DateSelector : public QDialog {
  Q_OBJECT

 public:
  explicit DateSelector(QWidget *parent = nullptr);
  ~DateSelector();
  Ui::DateSelector *ui;

  RollingBox *rboxYear, *rboxMonth, *rboxDay;

  int dateFlag = 0; /*1=btnYeat 2=btnMonth 3=btnStartDate 4=btnEnDate*/

  void init();

  void initStartEndDate(QString flag);

 protected:
  bool eventFilter(QObject *watch, QEvent *evn) override;

 private slots:
  void on_hsYear_valueChanged(int value);

  void on_hsMonth_valueChanged(int value);

  void on_hsDay_valueChanged(int value);

  void on_btnOk_clicked();

 private:
  void initRBox(RollingBox *rbox, int w);
};

#endif  // DATESELECTOR_H