#include "client.h"

#include "ui/cli.h"
#include "ui/qt.h"

#include <memory>

namespace notes
{
namespace ui
{

Client::~Client() {}

std::unique_ptr<Client>
Client::create(const std::string &type,
               std::shared_ptr<db::NotebookDatabase> &db, int argc, char **args)
{
    if (type == Client::Types::cli) {
        return std::make_unique<CliClient>(db, argc, args);
    }
    if (type == Client::Types::qt) {
        return std::make_unique<QtClient>(db, argc, args);
    }
    throw UiException("Invalid UI type, i don't know how to " + type);
    return nullptr;
}

} // ns ui
} // ns notes
