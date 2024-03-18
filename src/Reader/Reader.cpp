#include "Reader.h"

#include <QKeyEvent>

#include "src/MainWindow.h"
#include "ui_MainWindow.h"

extern MainWindow* mw_one;
extern Method* m_Method;
extern QString iniFile, iniDir, privateDir;
extern bool zh_cn, isAndroid, isIOS, isEBook, isReport, isDark;
extern int fontSize;

bool isOpen = false;
bool isEpub, isText, isPDF;
QStringList readTextList, htmlFiles, ncxList;
QString strOpfPath, fileName, ebookFile, strTitle, catalogueFile;
int iPage, sPos, totallines;
int baseLines = 20;
int htmlIndex = 0;
int minBytes = 20000;
int maxBytes = 40000;

Reader::Reader(QWidget* parent) : QDialog(parent) {
  this->installEventFilter(this);

  mw_one->ui->lblTitle->hide();
  mw_one->ui->frameReaderFun2->hide();
  mw_one->ui->btnBackward->hide();
  mw_one->ui->btnForward->hide();
  mw_one->ui->textBrowser->hide();
  mw_one->ui->lblCataInfo->hide();
  mw_one->ui->lblCataInfo->adjustSize();
  mw_one->ui->lblCataInfo->setWordWrap(true);

  mw_one->ui->textBrowser->horizontalScrollBar()->hide();
  mw_one->ui->textBrowser->verticalScrollBar()->hide();

  QPalette pt = palette();
  pt.setBrush(QPalette::Text, Qt::black);
  pt.setBrush(QPalette::Base, QColor(235, 235, 235));
  pt.setBrush(QPalette::Highlight, Qt::red);
  pt.setBrush(QPalette::HighlightedText, Qt::white);
  mw_one->ui->textBrowser->setPalette(pt);

  mw_one->ui->btnPageNext->setStyleSheet("border:none");
  mw_one->ui->btnPageUp->setStyleSheet("border:none");
  mw_one->ui->btnSelText->setStyleSheet("border:none");

  mw_one->ui->btnPageNext->hide();
  mw_one->ui->btnPageUp->hide();
  mw_one->ui->btnSelText->hide();
  mw_one->ui->btnPages->setStyleSheet(
      "color: rgb(0, 0, 0);background-color: rgb(254, 234, 112);border: "
      "0px solid "
      "rgb(255,0,0);border-radius: 4px;"
      "font-weight: bold;");

  QFont f = this->font();
  f.setPointSize(10);
  f.setBold(true);
  mw_one->ui->btnPages->setFont(f);
}

Reader::~Reader() {}

bool Reader::eventFilter(QObject* obj, QEvent* evn) {
  if (evn->type() == QEvent::KeyRelease) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(evn);
    if (keyEvent->key() == Qt::Key_Back) {
    }
  }

  return QWidget::eventFilter(obj, evn);
}

void Reader::keyReleaseEvent(QKeyEvent* event) { Q_UNUSED(event); }

void Reader::on_btnOpen_clicked() {
  openfile =
      QFileDialog::getOpenFileName(this, tr("Knot"), "", tr("Txt Files (*.*)"));

  if (!QFileInfo(openfile).exists()) return;

#ifdef Q_OS_ANDROID
  openfile = m_Method->getRealPathFile(openfile);
#endif

  startOpenFile(openfile);
}

void Reader::setReaderStyle() {
  QColor textColor, baseColor;

  if (readerStyle == "1") {
    mw_one->ui->qwReader->rootContext()->setContextProperty("backImgFile",
                                                            "/res/b.png");
    mw_one->ui->qwReader->rootContext()->setContextProperty("myTextColor",
                                                            "#664E30");

    mw_one->m_ReaderSet->ui->btnStyle3->setStyleSheet(
        "color: #00C78C;background-color: rgb(0, 0, 0);border: 2px solid "
        "rgb(0,0,255);border-radius: 4px;");

    mw_one->m_ReaderSet->ui->btnStyle1->setStyleSheet(
        "color: rgb(102, 78, 48);background-color: rgb(240, 222, 198);border: "
        "2px solid "
        "rgb(255,0,0);border-radius: 4px;");
    mw_one->m_ReaderSet->ui->btnStyle2->setStyleSheet(
        "color: rgb(0, 0, 0);background-color: rgb(255, 255, 255);border: 2px "
        "solid "
        "rgb(0,0,255);border-radius: 4px;");

    textColor = QColor(102, 78, 48);
    baseColor = QColor(240, 222, 198);
  }

  if (readerStyle == "2") {
    mw_one->ui->qwReader->rootContext()->setContextProperty("backImgFile", "");
    mw_one->ui->qwReader->rootContext()->setContextProperty("myTextColor",
                                                            "#000000");

    mw_one->m_ReaderSet->ui->btnStyle3->setStyleSheet(
        "color: #00C78C;background-color: rgb(0, 0, 0);border: 2px solid "
        "rgb(0,0,255);border-radius: 4px;");

    mw_one->m_ReaderSet->ui->btnStyle1->setStyleSheet(
        "color: rgb(102, 78, 48);background-color: rgb(240, 222, 198);border: "
        "2px solid "
        "rgb(0,0,255);border-radius: 4px;");
    mw_one->m_ReaderSet->ui->btnStyle2->setStyleSheet(
        "color: rgb(0, 0, 0);background-color: rgb(255, 255, 255);border: 2px "
        "solid "
        "rgb(255,0,0);border-radius: 4px;");

    textColor = QColor(0, 0, 0);
    baseColor = QColor(255, 255, 255);
  }

  if (readerStyle == "3") {
    mw_one->ui->qwReader->rootContext()->setContextProperty("backImgFile",
                                                            "/res/b3.png");
    mw_one->ui->qwReader->rootContext()->setContextProperty("myTextColor",
                                                            "#2E8B57");

    mw_one->m_ReaderSet->ui->btnStyle3->setStyleSheet(
        "color: #00C78C;background-color: rgb(0, 0, 0);border: 2px solid "
        "rgb(255,0,0);border-radius: 4px;");

    mw_one->m_ReaderSet->ui->btnStyle1->setStyleSheet(
        "color: rgb(102, 78, 48);background-color: rgb(240, 222, 198);border: "
        "2px solid "
        "rgb(0,0,255);border-radius: 4px;");
    mw_one->m_ReaderSet->ui->btnStyle2->setStyleSheet(
        "color: rgb(0, 0, 0);background-color: rgb(255, 255, 255);border: 2px "
        "solid "
        "rgb(0,0,255);border-radius: 4px;");

    textColor = QColor(46, 139, 87);
    baseColor = QColor(0, 0, 0);
  }

  QPalette pt = palette();
  pt.setBrush(QPalette::Text, textColor);
  pt.setBrush(QPalette::Base, baseColor);
  pt.setBrush(QPalette::Highlight, Qt::red);
  pt.setBrush(QPalette::HighlightedText, Qt::white);
  mw_one->ui->textBrowser->setPalette(pt);
}

void Reader::startOpenFile(QString openfile) {
  if (isReport) return;

  setPdfViewVisible(false);

  if (!mw_one->initMain) {
    saveReader();
    if (isText || isEpub) {
      savePageVPos();
    }
  }

  isEpub = false;
  isText = false;
  isPDF = false;

  setReaderStyle();

  if (QFile(openfile).exists()) {
    isEBook = true;
    strTitle = "";
    catalogueFile = "";
    mw_one->ui->btnPages->setText("1\n1");
    mw_one->ui->progReader->setValue(0);
    mw_one->ui->btnReader->setEnabled(false);
    mw_one->ui->frameReaderFun->setEnabled(false);

    mw_one->ui->lblTitle->hide();
    mw_one->ui->lblBookName->setText("");
    mw_one->ui->lblBookName->setWordWrap(true);
    mw_one->ui->btnCatalogue->hide();
    mw_one->ui->qwCata->hide();
    mw_one->ui->lblCataInfo->hide();
    mw_one->ui->qwReader->show();

    QString bookName;

#ifdef Q_OS_ANDROID
    QString name;
    name = getUriRealPath(openfile);
    QStringList lista = name.split("/");
    bookName = lista.at(lista.count() - 1);
#else
    QFileInfo fi(openfile);
    bookName = fi.fileName();
#endif

    ebookFile = openfile;
    strTitle =
        bookName + "    " + mw_one->getFileSize(QFile(ebookFile).size(), 2);

#ifdef Q_OS_LINUX
#endif

#ifdef Q_OS_MACOS
#endif

#ifdef Q_OS_WIN
    QString strZip, strExec, strUnzip, tagDir;
    tagDir = privateDir + "temp/";
    strZip = privateDir + "temp.zip";
    QTextEdit* txtEdit = new QTextEdit();
    strUnzip = qApp->applicationDirPath() + "/unzip.exe";
    strUnzip = "\"" + strUnzip + "\"";
    strZip = "\"" + strZip + "\"";
    strExec = privateDir;
    strExec = "\"" + strExec + "\"";
    QString strCommand1;
    QString strx = "\"" + tagDir + "\"";
    strCommand1 = strUnzip + " -o " + strZip + " -d " + strx;
    txtEdit->append(strCommand1);
    QString fileName = privateDir + "unbook.bat";
    mw_one->TextEditToFile(txtEdit, fileName);

    openFile(ebookFile);
    mw_one->readEBookDone();
#else
    mw_one->m_ReadTWThread->quit();
    mw_one->m_ReadTWThread->wait();

    mw_one->showProgress();

    mw_one->myReadEBookThread->start();
#endif

  } else
    return;
}

void Reader::openFile(QString openfile) {
  isOpen = false;

  qDebug() << "Starting to open files...";

  if (QFile(openfile).exists()) {
    readTextList.clear();
    readTextList = readText(openfile);
    if (readTextList.count() <= 0) return;
    QString strHead = readTextList.at(0);

    if (strHead.trimmed().mid(0, 2) == "PK") {
      QString dirpath;
      dirpath = privateDir + "temp/";
      deleteDirfile(dirpath);

      QString temp = privateDir + "temp.zip";
      QFile::remove(temp);
      if (!QFile::copy(openfile, temp)) {
        QMessageBox box;
        box.setText(openfile + "\n!=\n" + temp);
        box.exec();
      }

#ifdef Q_OS_MACOS
      QProcess* pro = new QProcess;
      pro->execute("unzip", QStringList() << "-o" << temp << "-d" << dirpath);
      pro->waitForFinished();
#endif

#ifdef Q_OS_LINUX
      QProcess* pro = new QProcess;
      pro->execute("unzip", QStringList() << "-o" << temp << "-d" << dirpath);
      pro->waitForFinished();

#endif

#ifdef Q_OS_WIN
      QString fileName = privateDir + "unbook.bat";
      QProcess* pro = new QProcess;
      pro->execute("cmd.exe", QStringList() << "/c" << fileName);
      pro->waitForFinished();

#endif

#ifdef Q_OS_ANDROID

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
      QAndroidJniObject javaZipFile = QAndroidJniObject::fromString(temp);
      QAndroidJniObject javaZipDir = QAndroidJniObject::fromString(dirpath);
      QAndroidJniObject m_activity = QAndroidJniObject::fromString("Unzip");
      m_activity.callStaticMethod<void>(
          "com.x/MyActivity", "Unzip",
          "(Ljava/lang/String;Ljava/lang/String;)V",
          javaZipFile.object<jstring>(), javaZipDir.object<jstring>());
#else
      QJniObject javaZipFile = QJniObject::fromString(temp);
      QJniObject javaZipDir = QJniObject::fromString(dirpath);
      QJniObject m_activity = QJniObject::fromString("Unzip");
      m_activity.callStaticMethod<void>(
          "com.x/MyActivity", "Unzip",
          "(Ljava/lang/String;Ljava/lang/String;)V",
          javaZipFile.object<jstring>(), javaZipDir.object<jstring>());
#endif

#endif

      qDebug() << openfile << dirpath;

      QString strFullPath;
      QString str0 = dirpath + "META-INF/container.xml";
      if (!QFile(str0).exists()) {
        qDebug() << "====== isEpub == false ======";
        return;
      } else {
        isEpub = true;
      }

      QStringList conList = readText(str0);
      for (int i = 0; i < conList.count(); i++) {
        QString str = conList.at(i);
        if (str.contains("full-path")) {
          QStringList list1 = str.split(" ");
          if (list1.count() > 0) {
            for (int j = 0; j < list1.count(); j++) {
              QString str1 = list1.at(j);
              if (str1.contains("full-path")) {
                QStringList list2 = str1.split("\"");
                if (list2.count() > 0) {
                  strFullPath = list2.at(1);
                }
              }
            }
          }
        }
      }

      QString strOpfFile = dirpath + strFullPath;
      QFileInfo fi(strOpfFile);
      strOpfPath = fi.path() + "/";
      QStringList opfList = readText(strOpfFile);
      htmlFiles.clear();

      if (opfList.count() > 1) {
        for (int i = 0; i < opfList.count(); i++) {
          QString str0 = opfList.at(i);
          str0 = str0.trimmed();

          if (str0.contains("idref=") && str0.mid(0, 8) == "<itemref") {
            QString idref = get_idref(str0);

            QString qfile;
            qfile = strOpfPath + get_href(idref, opfList);
            QFileInfo fi(qfile);
            if (fi.exists() && !htmlFiles.contains(qfile)) {
              if (fi.size() <= minBytes) {
                htmlFiles.append(qfile);
              } else {
                SplitFile(qfile);
              }
            }
          }
        }
      }

      qDebug() << strFullPath << htmlFiles;
      if (htmlFiles.count() == 0) {
        qDebug() << "====== htmlFiles Count== 0 ======";
        return;
      } else {
        QFile(strOpfPath + "main.css").remove();
        QFile::copy(":/res/main.css", strOpfPath + "main.css");
        ncx2html();
      }

    } else if (strHead.trimmed().toLower().contains("pdf")) {
      isPDF = true;

    } else {  // txt file
      isText = true;
      iPage = 0;
      sPos = 0;

      totallines = readTextList.count();
    }

    fileName = openfile;

    isOpen = true;

  }  // end file exists
}

QString Reader::get_idref(QString str0) {
  QString idref;
  str0 = str0.trimmed();
  if (str0.contains("idref=") && str0.mid(0, 8) == "<itemref") {
    QString str1 = str0;

    str1 = str1.replace("<", "");
    str1 = str1.replace("/>", "");
    QStringList list = str1.split(" ");
    for (int i = 0; i < list.count(); i++) {
      QString str2 = list.at(i);
      if (str2.contains("idref=")) {
        str2 = str2.replace("idref=", "");
        str2 = str2.replace("\"", "");
        str2 = str2.trimmed();
        idref = str2;
        break;
      }
    }
  }

  return idref;
}

QString Reader::get_href(QString idref, QStringList opfList) {
  for (int i = 0; i < opfList.count(); i++) {
    QString str0 = opfList.at(i);
    str0 = str0.trimmed();
    if (str0.contains("href=") && str0.contains(idref) &&
        str0.mid(0, 5) == "<item" && str0.contains("htm")) {
      QString str1;
      for (int j = 0; j < str0.length(); j++) {
        if (str0.mid(j, 6) == "href=\"") {
          for (int m = j + 6; m < str0.length(); m++) {
            if (str0.mid(m, 1) == "\"") {
              str1 = str0.mid(j + 6, m - j - 6);
              qDebug() << "href=" << str1;
              return str1;
              break;
            }
          }
        }
      }
      break;
    }
  }

  return "";
}

QString Reader::getTextEditLineText(QTextEdit* txtEdit, int i) {
  QTextBlock block = txtEdit->document()->findBlockByNumber(i);
  txtEdit->setTextCursor(QTextCursor(block));
  QString lineText = txtEdit->document()->findBlockByNumber(i).text();
  return lineText;
}

void Reader::saveReader() {
  QSettings Reg(privateDir + "reader.ini", QSettings::IniFormat);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  Reg.setIniCodec("utf-8");
#endif

  Reg.setValue("/Reader/FileName", fileName);

  if (isText || isEpub) {
    Reg.setValue("/Reader/FontSize", mw_one->textFontSize);

    if (isText) {
      Reg.setValue("/Reader/iPage" + fileName, iPage - baseLines);
    }

    if (isEpub) Reg.setValue("/Reader/htmlIndex" + fileName, htmlIndex);

    // dir
    Reg.setValue("/Reader/" + fileName + "MainDirIndex", mainDirIndex);
  }

  if (isPDF) {
    Reg.setValue("/Reader/PdfPage" + fileName, getPdfCurrentPage());
    Reg.setValue("/Reader/PdfScale" + fileName, getScale());
  }

  // book list
  Reg.setValue("/Reader/BookCount", bookList.count());
  for (int i = 0; i < bookList.count(); i++) {
    Reg.setValue("/Reader/BookSn" + QString::number(i), bookList.at(i));
  }
}

void Reader::initReader() {
  QSettings Reg(privateDir + "reader.ini", QSettings::IniFormat);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  Reg.setIniCodec("utf-8");
#endif

  readerStyle = Reg.value("/Reader/Style", "1").toString();

  QFont font;
  int fsize = Reg.value("/Reader/FontSize", 18).toInt();
  mw_one->textFontSize = fsize;
  mw_one->ui->qwReader->rootContext()->setContextProperty("FontSize", fsize);
  font.setPointSize(fsize);
  font.setLetterSpacing(QFont::AbsoluteSpacing, 2);  // 字间距

  fileName = Reg.value("/Reader/FileName").toString();
  if (!QFile(fileName).exists() && zh_cn) fileName = ":/res/test.txt";

  startOpenFile(fileName);

  getBookList();
}

void Reader::getBookList() {
  QSettings Reg(privateDir + "reader.ini", QSettings::IniFormat);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  Reg.setIniCodec("utf-8");
#endif
  // book list
  int count = Reg.value("/Reader/BookCount", 0).toInt();
  bookList.clear();
  for (int i = 0; i < count; i++) {
    QString str = Reg.value("/Reader/BookSn" + QString::number(i)).toString();
    QStringList list = str.split("|");
    if (QFile(list.at(1)).exists()) bookList.append(str);
  }
}

void Reader::setQMLText(QString txt1) {
  mw_one->ui->qwReader->rootContext()->setContextProperty("isAni", false);

  // white-space: pre-wrap;
  // text-indent:40px;

  QStringList list = txt1.split("\n");
  QString str1 = "<html>\n<body>\n";
  QString str2 = "</body>\n</html>";
  QString qsShow;

  for (int i = 0; i < list.count(); i++) {
    qsShow = qsShow +
             "<p style='line-height:35px; width:100% ; text-indent:40px; '>" +
             list.at(i) + "</p>";
  }
  qsShow = str1 + qsShow + str2;

  currentTxt = qsShow;

  loadQMLText(currentTxt);

  setAni();
}

void Reader::loadQMLText(QString str) {
  if (isText || isEpub) {
    QQuickItem* root = mw_one->ui->qwReader->rootObject();
    QMetaObject::invokeMethod((QObject*)root, "loadText", Q_ARG(QVariant, str));
  }
}

void Reader::on_btnPageUp_clicked() {
  if (mw_one->isSelText) return;
  mw_one->ui->lblTitle->hide();

  isPageNext = false;

  savePageVPos();
  if (isText) {
    int count = iPage - baseLines;
    if (count <= 0) return;
    textPos = 0;
    QString txt1;

    for (int i = count - baseLines; i < count; i++) {
      iPage--;
      QString str = readTextList.at(i);
      if (str.trimmed() != "")
        txt1 = txt1 + readTextList.at(i) + "\n" + strSpace;
    }

    setQMLText(txt1);
  }

  if (isEpub) {
    htmlIndex--;
    if (htmlIndex < 0) htmlIndex = 0;
    processHtml(htmlIndex);
    currentHtmlFile = htmlFiles.at(htmlIndex);
    setQMLHtml(currentHtmlFile, "");
  }

  setPageVPos();
  showInfo();
}

void Reader::on_btnPageNext_clicked() {
  if (mw_one->isSelText) return;
  mw_one->ui->lblTitle->hide();

  isPageNext = true;

  savePageVPos();
  if (isText) {
    QString txt1;
    if (totallines > baseLines) {
      int count = iPage + baseLines;
      if (count > totallines) return;
      textPos = 0;

      for (int i = iPage; i < count; i++) {
        iPage++;
        QString str = readTextList.at(i);

        if (str.trimmed() != "") txt1 = txt1 + str + "\n" + strSpace;
      }
    } else {
      for (int i = 0; i < totallines; i++) {
        QString str = readTextList.at(i);
        if (str.trimmed() != "") txt1 = txt1 + str + "\n" + strSpace;
      }
    }

    setQMLText(txt1);
  }

  if (isEpub) {
    htmlIndex++;
    if (htmlIndex == htmlFiles.count()) htmlIndex = htmlFiles.count() - 1;
    processHtml(htmlIndex);
    currentHtmlFile = htmlFiles.at(htmlIndex);
    setQMLHtml(currentHtmlFile, "");
  }
  setPageVPos();
  showInfo();
}

void Reader::openCataList(QString htmlFile) {
  savePageVPos();
  mw_one->ui->lblCataInfo->hide();
  mw_one->ui->qwCata->hide();
  mw_one->ui->qwReader->show();

  initLink(htmlFile);
}

void Reader::initLink(QString htmlFile) {
  for (int i = 0; i < htmlFiles.count(); i++) {
    QString str = htmlFiles.at(i);
    QString str1 = htmlFile;
    QStringList list = htmlFile.split("#");
    if (list.count() == 2) str1 = list.at(0);
    QStringList list2 = str1.split("/");
    if (list2.count() > 0) {
      str1 = list2.at(list2.count() - 1);
    }

    if (str.contains(str1)) {
      setEpubPagePosition(i, htmlFile);
      break;
    }
  }
}

void Reader::setEpubPagePosition(int index, QString htmlFile) {
  savePageVPos();
  htmlIndex = index;
  processHtml(index);
  currentHtmlFile = htmlFiles.at(index);

  if (htmlFile.contains("#")) {
    QStringList list = htmlFile.split("#");
    QString html = list.at(0);
    QString skipid = list.at(1);
    QString strfile;
    int count0 = 0;
    for (int i = 0; i < 500; i++) {
      strfile = htmlFiles.at(index + i);
      QString buffers = mw_one->loadText(strfile);
      if (buffers.contains(skipid)) {
        html = strfile;
        count0 = i;
        qDebug() << "html=" << html << "count0=" << count0;
        break;
      }
    }
    htmlIndex = index + count0;
    processHtml(htmlIndex);

    for (int i = 0; i < ncxList.count(); i++) {
      QString str = ncxList.at(i);
      QStringList l0 = str.split("===");
      QString strfile = l0.at(1);
      if (strfile.trimmed() == htmlFile) {
        strFind = l0.at(0);
        strFind = strFind.trimmed();
        break;
      }
    }

    setQMLHtml(html, skipid);

  } else {
    setQMLHtml(currentHtmlFile, "");
    setPageVPos();
  }

  showInfo();
}

void Reader::processHtml(int index) {
  if (!isEpub) return;

  QString hf = htmlFiles.at(index);

  QTextEdit* text_edit = new QTextEdit;

  QString strHtml = mw_one->loadText(hf);
  strHtml = strHtml.replace("　", " ");

  strHtml = strHtml.replace("</p>", "</p>\n");
  strHtml = strHtml.replace("/>", "/>\n");

  strHtml = strHtml.replace("><", ">\n<");

  strHtml = strHtml.replace("<img", "\n<img");

  strHtml = strHtml.replace(".css", "");
  strHtml = strHtml.replace("font-family:", "font0-family:");

  // QString space0, mystyle;
  // space0 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
  // mystyle = " style='line-height:35px; width:100% ; text-indent:40px; ' ";

  text_edit->setPlainText(strHtml);

  QPlainTextEdit* plain_edit = new QPlainTextEdit;

  for (int i = 0; i < text_edit->document()->lineCount(); i++) {
    QString str = getTextEditLineText(text_edit, i);
    str = str.trimmed();

    if (str.contains("</head>")) {
      QString css =
          "<link href=\"../main.css\" rel=\"stylesheet\" type=\"text/css\" "
          "/>";
      css.replace("../", "file://" + strOpfPath);
      plain_edit->appendPlainText(css);
      plain_edit->appendPlainText("</head>");
    } else {
      if (str.trimmed() != "") {
        if (str.contains("<image") && str.contains("xlink:href=")) {
          str.replace("xlink:href=", "src=");
          str.replace("<image", "<img");
          str.replace("height", "height1");
          str.replace("width", "width1");
        }

        if (str.mid(0, 4) == "<img") {
          QString str1 = str;
          QStringList list = str1.split(" ");
          QString strSrc;
          for (int k = 0; k < list.count(); k++) {
            QString s1 = list.at(k);
            if (s1.contains("src=")) {
              strSrc = s1;
              break;
            }
          }
          strSrc = strSrc.replace("src=", "");
          strSrc = strSrc.replace("/>", "");

          QString strimg = strSrc;
          strimg = strimg.replace("\"", "");
          QString imgFile = strOpfPath + strimg;
          imgFile = imgFile.replace("../", "");

          // qDebug() << "imgFile=" << imgFile;

          bool isCover = false;
          if (imgFile.toLower().contains("cover")) {
            isCover = true;
          }
          int nw = 0;
          if (isCover) {
            nw = mw_one->ui->qwReader->width() - 25;
          } else {
            nw = mw_one->ui->qwReader->width() - 104;
          }
          QString strw = " width = " + QString::number(nw);
          QImage img(imgFile);

          if (!isCover) {
            if (img.width() >= nw) {
              str = str.replace(" width = ", " width1 = ");
              str = str.replace("/>", strw + " />");
              str = "<a href=" + strSrc + ">" + str + " </a>";
            } else {
              str = "<a href=" + strSrc + ">" + str + "</a>";
            }
          }

          if (isCover) {
            str = str.replace(" width = ", " width1 = ");
            str = str.replace("/>", strw + " />");
            str = str + " </a>";
          }

          // qDebug() << "strSrc=" << strSrc << str;

          str = str.replace("width=", "width1=");
          str = str.replace("height=", "height1=");
        }

        plain_edit->appendPlainText(str);
      }
    }
  }

  TextEditToFile(plain_edit, hf);
  delete plain_edit;
}

void Reader::setQMLHtml(QString htmlFile, QString skipID) {
  mw_one->ui->qwReader->rootContext()->setContextProperty("isAni", false);
  QQuickItem* root = mw_one->ui->qwReader->rootObject();
#ifdef Q_OS_WIN
  QString htmlBuffer = mw_one->loadText(htmlFile);
  QMetaObject::invokeMethod((QObject*)root, "loadHtmlBuffer",
                            Q_ARG(QVariant, htmlBuffer));
#else

  QMetaObject::invokeMethod((QObject*)root, "loadHtml",
                            Q_ARG(QVariant, htmlFile), Q_ARG(QVariant, skipID));
#endif

  mw_one->m_ReaderSet->ui->lblInfo->setText(
      tr("Info") + " : " + htmlFile + "  " +
      mw_one->getFileSize(QFile(htmlFile).size(), 2));

  qDebug() << "setQMLHtml:Html File=" << htmlFile;

  if (skipID != "") {
    setHtmlSkip(htmlFile, skipID);
  }

  setAni();
}

void Reader::setAni() {
  if (isPageNext)
    mw_one->ui->qwReader->rootContext()->setContextProperty("aniW",
                                                            mw_one->width());
  else
    mw_one->ui->qwReader->rootContext()->setContextProperty("aniW",
                                                            -mw_one->width());
  mw_one->ui->qwReader->rootContext()->setContextProperty("toW", 0);
  mw_one->ui->qwReader->rootContext()->setContextProperty("isAni", true);
}

QStringList Reader::readText(QString textFile) {
  QStringList list, list1;

  if (QFile(textFile).exists()) {
    QFile file(textFile);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
      qDebug() << tr("Cannot read file %1:\n%2.")
                      .arg(QDir::toNativeSeparators(textFile),
                           file.errorString());

    } else {
      QString text;
      QByteArray buff = file.readAll();
      text = GetCorrectUnicode(buff);

      text.replace("/>", "/>\n");
      text.replace("<", "\n<");
      list = text.split("\n");
      for (int i = 0; i < list.count(); i++) {
        QString str = list.at(i);
        str = str.trimmed();
        if (str != "") list1.append(str);
      }
    }
    file.close();
  }

  return list1;
}

QString Reader::GetCorrectUnicode(const QByteArray& text) {
  QTextCodec::ConverterState state;
  QTextCodec* codec = QTextCodec::codecForName("UTF-8");
  QString strtext = codec->toUnicode(text.constData(), text.size(), &state);
  if (state.invalidChars > 0) {
    strtext = QTextCodec::codecForName("GBK")->toUnicode(text);
  } else {
    strtext = text;
  }
  return strtext;
}

void Reader::closeEvent(QCloseEvent* event) {
  Q_UNUSED(event);
  saveReader();
  savePageVPos();
}

void Reader::paintEvent(QPaintEvent* event) { Q_UNUSED(event); }

void Reader::goPostion() {
  if (isOpen) {
    QSettings Reg(privateDir + "reader.ini", QSettings::IniFormat);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    Reg.setIniCodec("utf-8");
#endif

    if (isText) {
      iPage = Reg.value("/Reader/iPage" + fileName, 0).toULongLong();
      on_btnPageNext_clicked();
    }

    if (isEpub) {
      htmlIndex = Reg.value("/Reader/htmlIndex" + fileName, 0).toInt();

      if (htmlIndex >= htmlFiles.count()) {
        htmlIndex = 0;
      }
      processHtml(htmlIndex);
      currentHtmlFile = htmlFiles.at(htmlIndex);
      setQMLHtml(currentHtmlFile, "");
      setPageVPos();
      showInfo();
    }

    if (isPDF) {
      if (!mw_one->isPdfNewMothod) {
        int page = Reg.value("/Reader/PdfPage" + fileName, 1).toInt();
        setPdfPage(page);

        qreal scale = Reg.value("/Reader/PdfScale" + fileName, 1).toReal();
        setPdfScale(scale);
      }
    }
  }
}

int Reader::deleteDirfile(QString dirName) {
  QDir directory(dirName);
  if (!directory.exists()) {
    return true;
  }

  QString srcPath = QDir::toNativeSeparators(dirName);
  if (!srcPath.endsWith(QDir::separator())) srcPath += QDir::separator();

  QStringList fileNames = directory.entryList(
      QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
  bool error = false;
  for (QStringList::size_type i = 0; i != fileNames.size(); ++i) {
    QString filePath = srcPath + fileNames.at(i);
    QFileInfo fileInfo(filePath);
    if (fileInfo.isFile() || fileInfo.isSymLink()) {
      QFile::setPermissions(filePath, QFile::WriteOwner);
      if (!QFile::remove(filePath)) {
        error = true;
      }
    } else if (fileInfo.isDir()) {
      if (!deleteDirfile(filePath)) {
        error = true;
      }
    }
  }

  if (!directory.rmdir(QDir::toNativeSeparators(directory.path()))) {
    error = true;
  }
  return !error;
}

void Reader::setFontSize(int textFontSize) {
  qreal pos1 = getVPos();
  qreal h1 = getVHeight();

  mw_one->ui->qwReader->rootContext()->setContextProperty("FontSize",
                                                          textFontSize);

  qreal h2 = getVHeight();
  qreal pos2 = getNewVPos(pos1, h1, h2);
  setVPos(pos2);
  textPos = pos2;
}

void Reader::TextEditToFile(QPlainTextEdit* txtEdit, QString fileName) {
  QFile* file;
  file = new QFile;
  file->setFileName(fileName);
  bool ok = file->open(QIODevice::WriteOnly | QIODevice::Text);
  if (ok) {
    QTextStream out(file);
    out << txtEdit->toPlainText();
    file->close();
    delete file;
  } else
    qDebug() << "Write failure!" << fileName;
}

void Reader::savePageVPos() {
  QSettings Reg(privateDir + "reader.ini", QSettings::IniFormat);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  Reg.setIniCodec("utf-8");
#endif
  textPos = getVPos();
  if (isEpub) {
    if (mw_one->ui->qwCata->isVisible()) {
      Reg.setValue("/Reader/vpos" + fileName + "  CataVPos", textPos);
      int index = m_Method->getCurrentIndexFromQW(mw_one->ui->qwCata);
      Reg.setValue("/Reader/vpos" + fileName + "  CataIndex", index);
    } else {
      if (htmlIndex >= 0)
        Reg.setValue("/Reader/vpos" + fileName + currentHtmlFile, textPos);
    }
  } else {
    Reg.setValue("/Reader/vpos" + fileName + QString::number(iPage), textPos);
  }
}

void Reader::setPageVPos() {
  QSettings Reg(privateDir + "reader.ini", QSettings::IniFormat);
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  Reg.setIniCodec("utf-8");
#endif
  if (isEpub) {
    if (mw_one->ui->qwCata->isVisible()) {
      textPos = Reg.value("/Reader/vpos" + fileName + "  CataVPos", 0).toReal();
      int index =
          Reg.value("/Reader/vpos" + fileName + "  CataIndex", 0).toReal();
      m_Method->setCurrentIndexFromQW(mw_one->ui->qwCata, index);
    } else {
      if (htmlIndex >= 0)
        textPos =
            Reg.value("/Reader/vpos" + fileName + currentHtmlFile, 0).toReal();
    }

  } else {
    textPos = Reg.value("/Reader/vpos" + fileName + QString::number(iPage), 0)
                  .toReal();
  }

  setVPos(textPos);
}

void Reader::setVPos(qreal pos) {
  QQuickItem* root;
  if (mw_one->ui->qwCata->isVisible())
    root = mw_one->ui->qwCata->rootObject();
  else
    root = mw_one->ui->qwReader->rootObject();

  QMetaObject::invokeMethod((QObject*)root, "setVPos", Q_ARG(QVariant, pos));
}

qreal Reader::getVPos() {
  QVariant itemCount;

  QQuickItem* root;
  if (mw_one->ui->qwCata->isVisible())
    root = mw_one->ui->qwCata->rootObject();
  else
    root = mw_one->ui->qwReader->rootObject();

  QMetaObject::invokeMethod((QObject*)root, "getVPos",
                            Q_RETURN_ARG(QVariant, itemCount));
  textPos = itemCount.toDouble();
  return textPos;
}

qreal Reader::getVHeight() {
  QVariant itemCount;
  QQuickItem* root = mw_one->ui->qwReader->rootObject();
  QMetaObject::invokeMethod((QObject*)root, "getVHeight",
                            Q_RETURN_ARG(QVariant, itemCount));
  textHeight = itemCount.toDouble();
  return textHeight;
}

qreal Reader::getNewVPos(qreal pos1, qreal h1, qreal h2) {
  qreal pos2;
  pos2 = pos1 * h2 / h1;
  return pos2;
}

void Reader::showInfo() {
  if (isText) {
    if (totallines > baseLines) {
      mw_one->ui->btnPages->setText(QString::number(iPage / baseLines) + "\n" +
                                    QString::number(totallines / baseLines));
      mw_one->ui->progReader->setMaximum(totallines / baseLines);
      mw_one->ui->progReader->setValue(iPage / baseLines);
    } else {
      mw_one->ui->progReader->setMaximum(100);
      mw_one->ui->progReader->setValue(0);
    }
  }

  if (isEpub) {
    mw_one->ui->btnPages->setText(QString::number(htmlIndex + 1) + "\n" +
                                  QString::number(htmlFiles.count()));
    mw_one->ui->progReader->setMaximum(htmlFiles.count());
    mw_one->ui->progReader->setValue(htmlIndex + 1);
  }

  mw_one->m_ReaderSet->updateProgress();
}

void Reader::SplitFile(QString qfile) {
  QTextEdit* text_edit = new QTextEdit;
  QPlainTextEdit* plain_edit = new QPlainTextEdit;
  QPlainTextEdit* plain_editHead = new QPlainTextEdit;

  QFileInfo fi(qfile);

  QString text = mw_one->loadText(qfile);
  text = text.replace("><", ">\n<");
  text = text.replace("</head>", "</head>\n");
  text_edit->setPlainText(text);
  int count = text_edit->document()->lineCount();
  for (int i = 0; i < count; i++) {
    QString str = getTextEditLineText(text_edit, i);
    plain_editHead->appendPlainText(str);
    if (str.trimmed() == "</head>") break;
  }

  int countHead = plain_editHead->document()->lineCount();
  int countBody = count - countHead;
  int n;
  qint64 bb = fi.size();
  if (bb > minBytes && bb < maxBytes)
    n = 2;
  else
    n = bb / minBytes;

  qDebug() << "size======" << bb << n;

  int split = countBody / n;
  int breakLine = 0;
  for (int x = 1; x < n + 1; x++) {
    if (x == 1) {
      // 1
      for (int i = 0; i < count; i++) {
        QString str = getTextEditLineText(text_edit, i);
        plain_edit->appendPlainText(str);
        if (i == countHead + split) {
          plain_edit->appendPlainText("</body>");
          plain_edit->appendPlainText("</html>");
          breakLine = i;
          break;
        }
      }

      QString file1 =
          qfile;  // fi.path() + "/" + fi.baseName() + "." + fi.suffix();
      TextEditToFile(plain_edit, file1);
      htmlFiles.append(file1);
    }

    // 2...n-1
    if (x > 1 && x < n) {
      plain_edit->clear();
      plain_edit->setPlainText(plain_editHead->toPlainText());
      plain_edit->appendPlainText("<body>");
      for (int i = breakLine + 1; i < count; i++) {
        QString str = getTextEditLineText(text_edit, i);
        plain_edit->appendPlainText(str);
        if (i == countHead + split * x) {
          plain_edit->appendPlainText("</body>");
          plain_edit->appendPlainText("</html>");
          breakLine = i;
          break;
        }
      }

      QString file2 = fi.path() + "/" + fi.baseName() + "_" +
                      QString::number(x - 1) + "." + fi.suffix();
      TextEditToFile(plain_edit, file2);
      htmlFiles.append(file2);
    }

    if (x == n) {
      // n
      plain_edit->clear();
      plain_edit->setPlainText(plain_editHead->toPlainText());
      plain_edit->appendPlainText("<body>");
      for (int i = breakLine + 1; i < count; i++) {
        QString str = getTextEditLineText(text_edit, i);
        plain_edit->appendPlainText(str);
      }

      QString filen = fi.path() + "/" + fi.baseName() + "_" +
                      QString::number(x - 1) + "." + fi.suffix();
      TextEditToFile(plain_edit, filen);
      htmlFiles.append(filen);
    }
  }

  delete plain_edit;
  delete plain_editHead;
}

QString Reader::getNCX_File(QString path) {
  QDir* dir = new QDir(path);
  QStringList filter;
  filter << "*.ncx";
  dir->setNameFilters(filter);
  QList<QFileInfo>* fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
  for (int i = 0; i < fileInfo->size(); i++) {
    if (fileInfo->at(i).exists()) {
      QString file = fileInfo->at(i).filePath();
      return file;
    }
  }
  return "";
}

void Reader::proceImg() {
  QString imgdir = strOpfPath + "Images";
  QDir dir0(imgdir);
  if (!dir0.exists()) imgdir = strOpfPath + "images";
  QDir dir2(imgdir);
  if (!dir2.exists()) imgdir = strOpfPath + "graphics";
  QDir dir1(imgdir);
  if (!dir1.exists()) imgdir = strOpfPath;
  qDebug() << "Image Dir : " << imgdir;

  QDir* dir = new QDir(imgdir);
  QStringList filter;
  filter << "*.png"
         << "*.jpg"
         << "*.jpeg"
         << "*.bmp"
         << "*.svg";
  dir->setNameFilters(filter);
  QList<QFileInfo>* fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
  for (int i = 0; i < fileInfo->size(); i++) {
    if (fileInfo->at(i).exists()) {
      QString file = fileInfo->at(i).filePath();
      QFileInfo fi(file);
      QString picFile = fi.path() + "/org-" + fi.fileName();
      QFile::copy(file, picFile);

      QImage img(file);
      double w, h;
      int new_w, new_h;
      w = img.width();
      h = img.height();
      double r = (double)w / h;
      if (w > mw_one->width() - 104 || file.contains("cover")) {
        new_w = mw_one->width() - 104;
        if (file.contains("cover")) new_w = mw_one->width() - 25;
        new_h = new_w / r;
        QPixmap pix;
        // pix = QPixmap::fromImage(img.scaled(new_w, new_h));
        pix = QPixmap::fromImage(img);
        pix = pix.scaled(new_w, new_h, Qt::KeepAspectRatio,
                         Qt::SmoothTransformation);
        pix.save(file);
      }
    }
  }

  QString strCover = getCoverPicFile(htmlFiles.at(0));
  qDebug() << "strCover=" << strCover << htmlFiles.at(0);
  if (QFile(strCover).exists()) {
    QImage img(strCover);
    double w, h;
    int new_w, new_h;
    w = img.width();
    h = img.height();
    double r = (double)w / h;
    new_w = mw_one->width() - 25;
    new_h = new_w / r;
    QPixmap pix;
    // pix = QPixmap::fromImage(img.scaled(new_w, new_h));
    pix = QPixmap::fromImage(img);
    pix =
        pix.scaled(new_w, new_h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    pix.save(strCover);
  }
}

QString Reader::getUriRealPath(QString uripath) {
#ifdef Q_OS_ANDROID
  QString realpath;

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  QAndroidJniObject javaUriPath = QAndroidJniObject::fromString(uripath);
  QAndroidJniObject m_activity = QtAndroid::androidActivity();
  QAndroidJniObject s = m_activity.callObjectMethod(
      "getUriPath", "(Ljava/lang/String;)Ljava/lang/String;",
      javaUriPath.object<jstring>());
#else
  QJniObject javaUriPath = QJniObject::fromString(uripath);
  QJniObject m_activity = QNativeInterface::QAndroidApplication::context();
  QJniObject s = m_activity.callObjectMethod(
      "getUriPath", "(Ljava/lang/String;)Ljava/lang/String;",
      javaUriPath.object<jstring>());
#endif

  realpath = s.toString();
  qDebug() << "RealPath" << realpath;
  return realpath;
#endif

  return uripath;
}

void Reader::getReadList() {
  for (int i = 0; i < bookList.count(); i++) {
    QString str = bookList.at(i);
    QStringList list = str.split("|");
    if (!QFile(list.at(1)).exists()) {
      bookList.removeAt(i);
      i--;
    }
  }

  if (bookList.count() == 0) return;

  setPdfViewVisible(false);
  m_Method->clearAllBakList(mw_one->ui->qwBookList);
  for (int i = 0; i < bookList.count(); i++) {
    QString str = bookList.at(i);
    QStringList listBooks = str.split("|");
    QString bookName = listBooks.at(0);
    QString bookPath = listBooks.at(1);
    QString suffix;
    if (bookName.toLower().contains(".txt")) {
      suffix = "txt";
    } else if (bookName.toLower().contains(".epub")) {
      suffix = "epub";
    } else if (bookName.toLower().contains(".pdf")) {
      suffix = "pdf";
    } else
      suffix = "none";

    m_Method->addItemToQW(mw_one->ui->qwBookList, bookName, bookPath, "",
                          suffix, 0);
  }

  for (int i = 0; i < bookList.count(); i++) {
    QString str = bookList.at(i);
    QStringList listBooks = str.split("|");
    if (listBooks.at(1) == fileName) {
      m_Method->setCurrentIndexFromQW(mw_one->ui->qwBookList, i);
      break;
    }
  }
}

void Reader::clearAllReaderRecords() {
  int count = m_Method->getCountFromQW(mw_one->ui->qwBookList);
  if (count == 0) return;

  m_Method->m_widget = new QWidget(mw_one);
  ShowMessage* m_ShowMsg = new ShowMessage(this);
  if (!m_ShowMsg->showMsg("Knot", tr("Clear all reading history") + " ? ", 2))
    return;

  m_Method->clearAllBakList(mw_one->ui->qwBookList);
  bookList.clear();
  QFile file(privateDir + "reader.ini");
  if (file.exists()) file.remove();
}

void Reader::openBookListItem() {
  int index = m_Method->getCurrentIndexFromQW(mw_one->ui->qwBookList);

  if (index < 0) return;

  QString str = bookList.at(index);
  QStringList listBooks = str.split("|");
  QString bookfile = listBooks.at(1);
  if (bookfile != fileName)
    startOpenFile(bookfile);
  else {
    if (isPDF) setPdfViewVisible(true);
  }

  mw_one->on_btnBackBookList_clicked();
}

void Reader::backDir() {
  if (!QFile(fileName).exists()) return;

  setEpubPagePosition(mainDirIndex, "");
  qDebug() << "mainDirIndex: " << mainDirIndex;
  if (mainDirIndex == 0) {
    on_btnPageNext_clicked();
    on_btnPageUp_clicked();

  } else {
    on_btnPageUp_clicked();
    on_btnPageNext_clicked();
  }
}

QString Reader::getCoverPicFile(QString htmlFile) {
  QStringList list = readText(htmlFile);
  QString str0, str1;
  for (int i = 0; i < list.count(); i++) {
    str0 = list.at(i);
    str0 = str0.trimmed();
    // qDebug() << "str0=" << str0;
    str0 = str0.replace("<image", "<img");
    str0 = str0.replace("xlink:href=", "src=");
    if (str0.contains("<img") && str0.contains("src=")) {
      for (int j = 0; j < str0.length(); j++) {
        if (str0.mid(j, 5) == "src=\"") {
          for (int m = j + 5; m < str0.length(); m++) {
            if (str0.mid(m, 1) == "\"") {
              str1 = str0.mid(j + 5, m - j - 5);
              qDebug() << "img src=" << strOpfPath + str1;
              str1 = str1.replace("../", "");
              return strOpfPath + str1;
              break;
            }
          }
          break;
        }
      }
      break;
    }
  }
  return "";
}

void Reader::setPdfViewVisible(bool vv) {
  if (mw_one->isPdfNewMothod) return;
  QQuickItem* root = mw_one->ui->qwPdf->rootObject();
  if (!mw_one->isPdfNewMothod)
    QMetaObject::invokeMethod((QObject*)root, "setViewVisible",
                              Q_ARG(QVariant, vv));
  else
    QMetaObject::invokeMethod((QObject*)root, "setPdfVisible",
                              Q_ARG(QVariant, vv));
}

int Reader::getPdfCurrentPage() {
  QVariant itemCount;
  QQuickItem* root = mw_one->ui->qwPdf->rootObject();
  QMetaObject::invokeMethod((QObject*)root, "getCurrentPage",
                            Q_RETURN_ARG(QVariant, itemCount));
  return itemCount.toInt();
}

qreal Reader::getScale() {
  QVariant itemCount;
  QQuickItem* root = mw_one->ui->qwPdf->rootObject();
  QMetaObject::invokeMethod((QObject*)root, "getScale",
                            Q_RETURN_ARG(QVariant, itemCount));
  return itemCount.toReal();
}

void Reader::setPdfPage(int page) {
  QQuickItem* root = mw_one->ui->qwPdf->rootObject();
  QMetaObject::invokeMethod((QObject*)root, "setPdfPage",
                            Q_ARG(QVariant, page));
}

void Reader::setPdfScale(qreal scale) {
  QQuickItem* root = mw_one->ui->qwPdf->rootObject();
  QMetaObject::invokeMethod((QObject*)root, "setPdfScale",
                            Q_ARG(QVariant, scale));
}

void Reader::setHideShowTopBar() {
  QQuickItem* root = mw_one->ui->qwPdf->rootObject();
  QMetaObject::invokeMethod((QObject*)root, "setHideShowTopBar");
}

// 拷贝文件夹：
bool Reader::copyDirectoryFiles(const QString& fromDir, const QString& toDir,
                                bool coverFileIfExist) {
  QDir sourceDir(fromDir);
  QDir targetDir(toDir);
  if (!targetDir.exists()) { /**< 如果目标目录不存在，则进行创建 */
    if (!targetDir.mkdir(targetDir.absolutePath())) return false;
  }

  QFileInfoList fileInfoList = sourceDir.entryInfoList();
  foreach (QFileInfo fileInfo, fileInfoList) {
    if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") continue;

    if (fileInfo.isDir()) { /**< 当为目录时，递归的进行copy */
      if (!copyDirectoryFiles(fileInfo.filePath(),
                              targetDir.filePath(fileInfo.fileName()),
                              coverFileIfExist))
        return false;
    } else { /**< 当允许覆盖操作时，将旧文件进行删除操作 */
      if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
        targetDir.remove(fileInfo.fileName());
      }

      /// 进行文件copy
      if (!QFile::copy(fileInfo.filePath(),
                       targetDir.filePath(fileInfo.fileName()))) {
        return false;
      }
    }
  }
  return true;
}

void Reader::on_hSlider_sliderReleased(int position) {
  mw_one->ui->lblTitle->hide();

  int max = 0;
  if (isText) max = totallines / baseLines;
  if (isEpub) max = htmlFiles.count();
  if (position >= max) position = max;
  sPos = position;
  getLines();
  setPageVPos();
}

void Reader::getLines() {
  QString qsShow;

  mw_one->m_ReaderSet->ui->hSlider->setMinimum(1);

  if (isText) {
    mw_one->m_ReaderSet->ui->hSlider->setMaximum(totallines / baseLines);
    mw_one->ui->btnPages->setText(
        QString::number(mw_one->m_ReaderSet->ui->hSlider->value()) + "\n" +
        QString::number(totallines / baseLines));
    iPage = (mw_one->m_ReaderSet->ui->hSlider->value() - 1) * baseLines;
    qDebug() << "iPage" << iPage << mw_one->m_ReaderSet->ui->hSlider->value();

    int count = iPage + baseLines;
    QString txt1;
    for (int i = iPage; i < count; i++) {
      iPage++;
      QString str = readTextList.at(i);
      if (str.trimmed() != "")
        txt1 = txt1 + readTextList.at(i).trimmed() + "\n" + strSpace;
    }

    qsShow =
        "<p style='line-height:32px; width:100% ; white-space: pre-wrap; "
        "'>" +
        txt1 + "</p>";
    setQMLText(qsShow);
  }

  if (isEpub) {
    mw_one->m_ReaderSet->ui->hSlider->setMaximum(htmlFiles.count());
    htmlIndex = sPos - 1;
    if (htmlIndex < 0) htmlIndex = 0;
    processHtml(htmlIndex);
    currentHtmlFile = htmlFiles.at(htmlIndex);
    setQMLHtml(currentHtmlFile, "");
  }
}

void Reader::showCatalogue() {
  savePageVPos();

  if (mw_one->ui->qwCata->isVisible()) {
    mw_one->ui->lblCataInfo->hide();
    mw_one->ui->qwCata->hide();
    mw_one->ui->qwReader->show();

  } else {
    mw_one->ui->qwReader->hide();
    mw_one->ui->lblCataInfo->show();
    mw_one->ui->qwCata->show();

    m_Method->clearAllBakList(mw_one->ui->qwCata);
    for (int i = 0; i < ncxList.count(); i++) {
      QString item = ncxList.at(i);
      QString str0, str1;
      str0 = item.split("===").at(0);
      str1 = item.split("===").at(1);
      m_Method->addItemToQW(mw_one->ui->qwCata, str0, str1, "", "", 0);
    }
  }

  setPageVPos();
  showInfo();
}

void Reader::ncx2html() {
  ncxList.clear();
  QString ncxFile;
  QStringList fileList;
  QStringList fmt;
  fmt.append("ncx");
  mw_one->m_NotesList->getAllFiles(strOpfPath, fileList, fmt);
  if (fileList.count() >= 1) {
    ncxFile = fileList.at(0);
  }

  if (!QFile(ncxFile).exists()) {
    return;
  }

  QPlainTextEdit* plain_edit = new QPlainTextEdit;
  plain_edit->appendPlainText("<html>");
  plain_edit->appendPlainText("<body>");
  plain_edit->appendPlainText("<style>.my-link {color: #336699;} </style>");

  QTextEdit* text_edit = new QTextEdit;
  QTextEdit* text_edit0 = new QTextEdit;
  QString strHtml0 = mw_one->loadText(ncxFile);
  strHtml0 = strHtml0.replace("　", " ");
  strHtml0 = strHtml0.replace("<docTitle>", "\n<docTitle>\n");
  strHtml0 = strHtml0.replace("</docTitle>", "\n</docTitle>\n");
  strHtml0 = strHtml0.replace("<navMap>", "\n<navMap>\n");
  strHtml0 = strHtml0.replace("</navMap>", "\n</navMap>\n");
  strHtml0 = strHtml0.replace("<navLabel>", "\n<navLabel>\n");
  strHtml0 = strHtml0.replace("</navLabel>", "\n</navLabel>\n");
  strHtml0 = strHtml0.replace("<navPoint", "\n<navPoint");
  strHtml0 = strHtml0.replace("</navPoint>", "\n</navPoint>\n");
  text_edit0->setPlainText(strHtml0);
  for (int i = 0; i < text_edit0->document()->lineCount(); i++) {
    QString str = getTextEditLineText(text_edit0, i);
    str = str.trimmed();
    if (str != "") {
      text_edit->append(str);
    }
  }

  for (int i = 0; i < text_edit->document()->lineCount(); i++) {
    QString str = getTextEditLineText(text_edit, i);
    str = str.trimmed();
    QString str0, str1, str2;
    bool isAdd = false;
    bool isAddTitle = false;

    if (str == "<docTitle>") {
      str0 = getTextEditLineText(text_edit, i + 1);
      str0.replace("<text>", "");
      str0.replace("</text>", "");
      str0 = str0.trimmed();
      isAddTitle = true;
    }

    if (str.contains("<docTitle><text>")) {
      str0 = str;
      str0.replace("<docTitle><text>", "");
      str0.replace("</text></docTitle>", "");
      str0 = str0.trimmed();
      isAddTitle = true;
    }

    if (isAddTitle) {
      mw_one->ui->lblCataInfo->setText(str0);
      qDebug() << str0;
      plain_edit->appendPlainText("<div>");
      plain_edit->appendPlainText("<h3>" + str0 + "</h3>");
      plain_edit->appendPlainText("</div>");

      plain_edit->appendPlainText("<ul>");
    }

    if (str == "<navLabel>") {
      str1 = getTextEditLineText(text_edit, i + 1);
      str1.replace("<text>", "");
      str1.replace("</text>", "");
      str1 = str1.trimmed();

      str2 = getTextEditLineText(text_edit, i + 3);
      str2.replace("<content src=", "");
      str2.replace("/>", "");
      str2.replace("\"", "");
      str2 = str2.trimmed();

      isAdd = true;
    }

    if (str.contains("<navLabel><text>")) {
      str1 = str;
      str1.replace("<navLabel><text>", "");
      str1.replace("</text></navLabel>", "");
      str1 = str1.trimmed();

      str2 = getTextEditLineText(text_edit, i + 1);
      str2.replace("<content src=", "");
      str2.replace("/>", "");
      str2.replace("\"", "");
      str2 = str2.trimmed();

      isAdd = true;
    }

    if (isAdd) {
      qDebug() << str1 << str2;

      plain_edit->appendPlainText("<div>");
      plain_edit->appendPlainText("<li><a href=" + strOpfPath + str2 +
                                  " class=my-link >" + str1 + "</a></li>");
      plain_edit->appendPlainText("</div>");

      ncxList.append(str1 + "===" + strOpfPath + str2);
    }
  }

  plain_edit->appendPlainText("</ul>");

  plain_edit->appendPlainText("</body>");
  plain_edit->appendPlainText("</html>");
  catalogueFile = strOpfPath + "catalogue.html";
  TextEditToFile(plain_edit, catalogueFile);
}

void Reader::setHtmlSkip(QString htmlFile, QString skipID) {
  QTextBrowser* textBrowser = new QTextBrowser();
  textBrowser->setFixedHeight(mw_one->ui->qwReader->height());
  textBrowser->setFixedWidth(mw_one->ui->qwReader->width());
  QFont font = mw_one->ui->qwReader->font();
  font.setPixelSize(mw_one->textFontSize);
  font.setFamily(mw_one->m_ReaderSet->ui->btnFont->font().family());
  font.setLetterSpacing(QFont::AbsoluteSpacing, 2);
  textBrowser->setFont(font);

  if (isEpub) {
    QString str = mw_one->loadText(htmlFile);
    str.replace("..", strOpfPath);
    QDir dir;
    dir.setCurrent(strOpfPath);
    textBrowser->setHtml(str);
  }

  qDebug() << "strFind=" << strFind << "skipID=" << skipID;

  for (int i = 0; i < 100; i++) {
    if (textBrowser->find(strFind)) {
      int curpos = textBrowser->textCursor().position();

      QTextCursor cursor;
      cursor = textBrowser->textCursor();
      cursor.setPosition(curpos);
      cursor.setPosition(curpos + 2, QTextCursor::KeepAnchor);
      textBrowser->setTextCursor(cursor);
      QString s0 = cursor.selectedText();

      qDebug() << "cursor pos=" << curpos << "s0=" << s0 << s0.trimmed();

      if (s0.trimmed().length() < 2) {
        mw_one->ui->qwReader->rootContext()->setContextProperty("nCursorPos",
                                                                curpos);
        if (curpos > 50) {
          qreal scrollPos = getVPos();
          scrollPos = scrollPos + textBrowser->height() / 2;
          setVPos(scrollPos);
        }
        break;
      }
    }
  }
}

QString Reader::getSkipText(QString htmlFile, QString skipID) {
  QStringList list = readText(htmlFile);
  QString hxHeight = "24";
  for (int i = 0; i < list.count(); i++) {
    QString item = list.at(i);
    if (item.contains(skipID)) {
      if (item.contains("h1")) hxHeight = "32";
      if (item.contains("h2")) hxHeight = "24";
      if (item.contains("h3")) hxHeight = "18";
      if (item.contains("h4")) hxHeight = "16";
      if (item.contains("h5")) hxHeight = "13";
      if (item.contains("h6")) hxHeight = "12";

      QStringList l0 = item.split(">");
      QString txt = l0.at(1);
      txt = txt.split(">").at(0);
      txt = txt.trimmed();

      if (txt == "") {
        item = list.at(i + 1);
        l0 = item.split(">");
        txt = l0.at(1);
        txt = txt.split("<").at(0);
        txt = txt.trimmed();
      }

      qDebug() << "skipID=" << skipID << "txt=" << txt << hxHeight;
      return txt + "===" + hxHeight;
      break;
    }
  }

  return "";
}
