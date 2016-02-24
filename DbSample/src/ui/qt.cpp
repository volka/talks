#include "ui/qt.h"

#include <QGuiApplication>
#include <QtQuick/QQuickView>

namespace notes {
namespace ui {

QtClient::~QtClient() {}

QtClient::QtClient(std::shared_ptr<notes::db::NotebookDatabase> &db, int argc,
                   char **args)
    : db_(db), args_size_(argc),
      // will probably modify args
      app_(new QGuiApplication(args_size_, args)) {}

int QtClient::run() {
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///qml/qtclient.qml"));
    view.show();

    return app_->exec();
}

} // ns ui
} // ns notes
