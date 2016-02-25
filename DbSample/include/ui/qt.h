#pragma once

#include "client.h"

class QGuiApplication;
class QQmlApplicationEngine;

namespace notes {
namespace ui {

class QtClient : public Client {

  public:
    QtClient(std::shared_ptr<db::NotebookDatabase> &db, int argc, char **args);
    virtual ~QtClient();

    virtual int run();

  private:
    std::shared_ptr<db::NotebookDatabase> db_;
    const std::vector<char *> args_;
    int args_size_; // QApplication needs an int&
    std::unique_ptr<QGuiApplication> app_;
    std::unique_ptr<QQmlApplicationEngine> mainView_;
};
} // ui
} // notes
