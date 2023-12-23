#include "SyncInfo.h"

#include "MainWindow.h"
#include "ui_SyncInfo.h"

extern MainWindow* mw_one;

SyncInfo::SyncInfo(QWidget* parent) : QDialog(parent), ui(new Ui::SyncInfo) {
  ui->setupUi(this);
  this->installEventFilter(this);

  setWindowFlag(Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground);
  ui->frame->setStyleSheet(
      "#frame{background-color: rgb(255, 255, 255);border-radius:10px; "
      "border:1px solid gray;}");
  this->layout()->setContentsMargins(5, 5, 5, 5);

  setModal(true);
  QScroller::grabGesture(ui->textBrowser, QScroller::LeftMouseButtonGesture);
  ui->textBrowser->verticalScrollBar()->setStyleSheet(mw_one->vsbarStyleSmall);
  mw_one->setSCrollPro(ui->textBrowser);
  mw_one->set_btnStyle(this);
}

SyncInfo::~SyncInfo() { delete ui; }

void SyncInfo::on_btnClose_clicked() {
  mw_one->m_NotesList->initNotesList();
  mw_one->m_NotesList->initRecycle();

  // load note
  mw_one->m_NotesList->currentMDFile = mw_one->m_NotesList->getCurrentMDFile();
  mw_one->m_Notes->MD2Html(mw_one->m_NotesList->currentMDFile);
  mw_one->m_Notes->loadMemoQML();
  mw_one->m_Notes->setVPos();

  close();
  ui->textBrowser->clear();
  infoList.clear();
}

void SyncInfo::init() {
  setGeometry(mw_one->geometry().x(), mw_one->geometry().y(), mw_one->width(),
              mw_one->height() / 2);
}

bool SyncInfo::eventFilter(QObject* watch, QEvent* evn) {
  if (evn->type() == QEvent::KeyRelease) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(evn);
    if (keyEvent->key() == Qt::Key_Back) {
      return true;
    }
  }

  return QWidget::eventFilter(watch, evn);
}

void SyncInfo::runSync(QString path) {
  qDebug() << QTime::currentTime().toString() + "  Start Sync..." << path
           << "isSelf=" << mw_one->isSelf;
  if (!mw_one->isSelf && !mw_one->initMain) {
    QString info = QDateTime::currentDateTime().toString() + "\n" +
                   tr("The data update is complete.") + "\n";
    if (!infoList.contains(info)) {
      infoList.append(info);
      ui->textBrowser->append(info);
    }
    init();
    show();
  }
}
