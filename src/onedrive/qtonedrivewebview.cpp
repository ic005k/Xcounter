#include "qtonedrivewebview.h"

#include <QContextMenuEvent>
#include <QMenu>

QtOneDriveWebView::QtOneDriveWebView(QWidget *parent)
    : QWebEngineView(parent) {}

void QtOneDriveWebView::contextMenuEvent(QContextMenuEvent *event) {
  QMenu *menu = new QMenu(this);
  menu->addAction(pageAction(QWebEnginePage::Copy));
  menu->addAction(pageAction(QWebEnginePage::Paste));
  menu->exec(mapToGlobal(event->pos()));
}
