#ifndef DLGTODO_H
#define DLGTODO_H

#include <QCheckBox>
#include <QDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QListWidgetItem>
#include <QTextEdit>
#include <QToolButton>

namespace Ui {
class dlgTodo;
}

class dlgTodo : public QDialog {
  Q_OBJECT

 public:
  explicit dlgTodo(QWidget *parent = nullptr);
  Ui::dlgTodo *ui;
  ~dlgTodo();

  bool isToday = false;
  void setAlartTop(QListWidgetItem *item);
  QListWidgetItem *minAlartItem;
  void init_Items();
  QString alarmStyle =
      "QLabel{background:rgb(112,128,105);color:rgb(255,255,255);border:2px;"
      "border-radius:4px;padding:2px 4px;}";
  QString alarmStyleToday =
      "QLabel{background:rgb(255,11,25);color:white;border:2px;"
      "border-radius:4px;padding:2px 4px;}";
  QString alarmStyleTomorrow =
      "QLabel{background:rgb(245,245,25);color:black;border:2px;"
      "border-radius:4px;padding:2px 4px;}";
  void add_Item(QString, QString, bool);
  static void saveTodo();
  QString styleDark =
      "#listWidget::item {background-color: #393d49;color: #ffffff;border: "
      "transparent;padding: 8px; height: 65;}"
      "#listWidget::item:hover {background-color: #4e5465;}"
      "#listWidget::item:selected {border-left: 5px solid #009688;}";

  QLabel *getTimeLabel(int);
  void on_Alarm();
  QLabel *getMainLabel(int);
  void startTimerAlarm(QString text);
  void stopTimerAlarm();

  void sendMsgAlarm(QString text);

  qlonglong getSecond(QString strDateTime);
  void refreshAlarm();
  int getEditTextHeight(QTextEdit *edit);
  QLabel *getTimeLabel(QListWidgetItem *item);
  QLabel *getMainLabel(QListWidgetItem *item);
  void insertItem(QString strTime, QString strText, int curIndex);
  int getCurrentIndex();
  QString getItemTime(int index);
  QString getItemTodoText(int index);
  void delItem(int index);
  void setCurrentIndex(int index);
  void setHighPriority(bool isBool);

  void addItem(QString strTime, QString strText);
  int getCount();

  void modifyTime(int index, QString strTime);
  void modifyTodoText(int index, QString strTodoText);
 public slots:
  void on_btnOK_clicked();
  void on_btnCancel_clicked();
  void isAlarm(int index);

 protected:
  void keyReleaseEvent(QKeyEvent *event) override;
  void closeEvent(QCloseEvent *event) override;
  bool eventFilter(QObject *watch, QEvent *evn) override;
 public slots:
  void on_btnBack_clicked();

  void on_btnAdd_clicked();

  void on_listWidget_itemClicked(QListWidgetItem *item);

  void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

  void on_listWidget_currentRowChanged(int currentRow);

  void on_btnModify_clicked();

  void on_btnHigh_clicked();

  void on_btnLow_clicked();

  void on_btnSetTime_clicked();

  void on_btnRecycle_clicked();

  void on_btnReturn_clicked();

  void on_btnClear_clicked();

  void on_btnRestore_clicked();

  void on_btnDel_clicked();

  void on_textEdit_textChanged();

  void reeditText();
  private:
  QListWidgetItem *editItem;
  void add_ItemSn(int index);
  QLabel *lblModi;
  QTextEdit *editModi;
  bool isModi = false;
  bool isWeekValid(QString lblDateTime, QString strDate);
  bool isTomorrow = false;
};
#endif  // DLGTODO_H
