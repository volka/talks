#include "ui/cli.h"

#include <iostream>

notes::ui::CliClient::~CliClient() {}

void notes::ui::CliClient::run() {
  bool abort = false;
  while (!abort) {
    printMenu();
    abort = processInput();
  }
}

void notes::ui::CliClient::printMenu() {
  using namespace std;
  cout << "--- Notes Main Menu ---" << endl << endl;
  cout << "a: Add Note" << endl;
  cout << "l: List Notes" << endl;
  cout << "s: Search Notes" << endl;
  cout << "d: Delete Note" << endl;
  cout << "q: Quit" << endl;
  cout << ">> ";
}

bool notes::ui::CliClient::processInput() {
  char input;
  std::cin >> input;
  switch (input) {
  case 'a':
    addNote();
    break;
  case 'l':
    listNotes();
    break;
  case 's':
    searchNotes();
    break;
  case 'd':
    deleteNote();
    break;
  case 'q':
    return true;
    break;
  default:
    std::cout << "Invalid input, please try again ..." << endl;
  }
}
