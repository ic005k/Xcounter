#ifndef DLGSTEPS_H
#define DLGSTEPS_H

#include <QAccelerometer>
#include <QDialog>

namespace Ui {
class dlgSteps;
}

class dlgSteps : public QDialog {
  Q_OBJECT

 public:
  explicit dlgSteps(QWidget *parent = nullptr);
  ~dlgSteps();
  Ui::dlgSteps *ui;

 protected:
  void keyReleaseEvent(QKeyEvent *event) override;

 private slots:
  void on_btnBack_clicked();

  void on_btnPause_clicked();

  void on_btnReset_clicked();

 private:
};

#endif  // DLGSTEPS_H
