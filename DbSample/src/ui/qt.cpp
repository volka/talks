#include "ui/qt.h"

#include <QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

namespace notes {
namespace ui {

QtClient::~QtClient() {}

QtClient::QtClient(std::shared_ptr<notes::db::NotebookDatabase> &db, int argc,
                   char **args)
    : db_(db), args_size_(argc),
      // will probably modify args
      app_(new QGuiApplication(args_size_, args)),
      mainView_(new QQmlApplicationEngine(QUrl("qrc:///qml/qtclient.qml")))
{
}

int QtClient::run() {
    return app_->exec();
}

} // ns ui
} // ns notes
