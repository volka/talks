#include "client.h"

#include "ui/cli.h"
#include "ui/qt.h"

#include <memory>

namespace notes {
namespace ui {

Client::~Client() {}

std::unique_ptr<Client>
Client::create(const std::string& type, std::shared_ptr<db::NotebookDatabase>& db)
{
    if (type == Client::Types::cli) {
        return std::make_unique<CliClient>(db);
    }
    if (type == Client::Types::qt) {

    }
    throw UiException("Invalid UI type, i don't know how to " + type);
    return nullptr;
}

} // ns ui
} // ns notes
