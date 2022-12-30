#include "LoadPic.h"

#include "src/mainwindow.h"
#include "ui_LoadPic.h"
#include "ui_mainwindow.h"
extern MainWindow* mw_one;
extern QString picfile;

dlgLoadPic::dlgLoadPic(QWidget* parent)
    : QDialog(parent), ui(new Ui::dlgLoadPic) {
  ui->setupUi(this);

  mw_one->ui->f_ImgFun->setObjectName("myframe");
  mw_one->ui->f_ImgFun->setStyleSheet(
      "QFrame#myframe{border-image:url(:/res/b.png)}");

  this->installEventFilter(this);

  this->layout()->setContentsMargins(1, 1, 1, 1);
  this->layout()->setSpacing(3);

  setModal(true);
}

dlgLoadPic::~dlgLoadPic() { delete ui; }

void dlgLoadPic::initMain(QString imgFile) {
  if (mw_one->isReaderVisible) mw_one->ui->frameReader->hide();
  if (mw_one->isMemoVisible) mw_one->ui->frameMemo->hide();

  mw_one->ui->qw_Img->rootContext()->setContextProperty("myW", mw_one->width());
  mw_one->ui->qw_Img->rootContext()->setContextProperty("myH",
                                                        mw_one->height());

  mw_one->ui->f_ImgView->show();

  QImage img(imgFile);

  mw_one->ui->qw_Img->rootContext()->setContextProperty("imgW",
                                                        img.width() * 2);
  mw_one->ui->qw_Img->rootContext()->setContextProperty("imgH",
                                                        img.height() * 2);
  mw_one->ui->qw_Img->rootContext()->setContextProperty("imgFile", imgFile);
  mw_one->ui->qw_Img->setSource(
      QUrl(QStringLiteral("qrc:/src/qmlsrc/imgview.qml")));
}

bool dlgLoadPic::eventFilter(QObject* watch, QEvent* evn) {
  if (evn->type() == QEvent::KeyPress) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(evn);
    if (keyEvent->key() == Qt::Key_Back) {
      return true;
    }
  }

  return QWidget::eventFilter(watch, evn);
}