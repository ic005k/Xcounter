#ifndef DLGLEFT_H
#define DLGLEFT_H

#include <QDialog>

namespace Ui {
class dlgLeft;
}

class dlgLeft : public QDialog
{
    Q_OBJECT

public:
    explicit dlgLeft(QWidget *parent = nullptr);
    ~dlgLeft();

private:
    Ui::dlgLeft *ui;
};

#endif // DLGLEFT_H
