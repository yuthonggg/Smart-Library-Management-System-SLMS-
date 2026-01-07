#include "Library.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

// Constants
const int FINE_PER_DAY = 1; // RM1 per day
const int BORROW_DURATION_DAYS = 7;


string toLower(const string &s) {
  string result = s;
  transform(result.begin(), result.end(), result.begin(), ::tolower);
  return result;
}

bool equalsIgnoreCase(const string &a, const string &b) {
  return toLower(a) == toLower(b);
}

bool containsIgnoreCase(const string &haystack, const string &needle) {
  return toLower(haystack).find(toLower(needle)) != string::npos;
}

Library::Library() : currentUser(nullptr) {
  loadBooks();
  loadUsers();
}

Library::~Library() {
  saveBooks();
  saveUsers();
  for (Person *p : users) {
    delete p;
  }
}


Book *Library::findBookByID(string id) {
  for (auto &b : books) {
    if (equalsIgnoreCase(b.getBookID(), id))
      return &b;
  }
  return nullptr;
}

Person *Library::findUserByID(string id) {
  for (auto *p : users) {
    if (equalsIgnoreCase(p->getID(), id))
      return p;
  }
  return nullptr;
}


void Library::saveBooks() {
  ofstream out("books.txt");
  if (!out)
    return;

  for (const auto &b : books) {
    out << b.getBookID() << "|" << b.getTitle() << "|" << b.getAuthor() << "|"
        << b.getGenre() << "|" << b.getIsBorrowed() << "|"
        << b.getBorrowedByMemberID() << "|" << b.getDueDate() << "\n";
  }
  out.close();
}

void Library::loadBooks() {
  ifstream in("books.txt");
  if (!in)
    return;

  string line;
  while (getline(in, line)) {
    stringstream ss(line);
    string segment;
    vector<string> parts;

    while (getline(ss, segment, '|')) {
      parts.push_back(segment);
    }

    if (parts.size() >= 7) {
      Book b(parts[1], parts[2], parts[3], parts[0]);
      if (parts[4] == "1") {

        time_t due = stoll(parts[6]);
        b.borrowBook(parts[5], due);
      }
      books.push_back(b);
    }
  }
  in.close();
}

void Library::saveUsers() {
  ofstream out("users.txt");
  if (!out)
    return;

  for (Person *p : users) {
    out << p->getRole() << "|" << p->getID() << "|" << p->getName() << "|"
        << p->getEmail();
    if (p->getRole() == "Member") {
      Member *m = dynamic_cast<Member *>(p);

      out << "|HISTORY_START";
      Node<BorrowRecord> *curr = m->getBorrowHistory().getHead();
      while (curr) {
        out << "|" << curr->data.bookID << "," << curr->data.borrowDate << ","
            << curr->data.dueDate << "," << curr->data.returnDate << ","
            << curr->data.isReturned;
        curr = curr->next;
      }
      out << "|HISTORY_END";
    }
    out << "\n";
  }
  out.close();
}

void Library::loadUsers() {
  ifstream in("users.txt");
  if (!in) {
    // Create default librarian
    users.push_back(new Librarian("Admin", "admin", "admin@lib.com"));
    return;
  }

  string line;
  while (getline(in, line)) {
    stringstream ss(line);
    string segment;
    vector<string> parts;

    size_t pos = 0;
    string role, id, name, email;

    while (getline(ss, segment, '|')) {
      parts.push_back(segment);
    }

    if (parts.size() < 4)
      continue;

    role = parts[0];
    id = parts[1];
    name = parts[2];
    email = parts[3];

    Person *p = nullptr;
    if (role == "Librarian") {
      p = new Librarian(name, id, email);
    } else if (role == "Member") {
      Member *m = new Member(name, id, email);


      if (parts.size() > 5 && parts[4] == "HISTORY_START") {
        for (size_t i = 5; i < parts.size(); i++) {
          if (parts[i] == "HISTORY_END")
            break;

          stringstream hss(parts[i]);
          string hseg;
          vector<string> hparts;
          while (getline(hss, hseg, ','))
            hparts.push_back(hseg);

          if (hparts.size() == 5) {
            BorrowRecord rec(hparts[0], stoll(hparts[1]), stoll(hparts[2]));
            rec.returnDate = stoll(hparts[3]);
            rec.isReturned = (hparts[4] == "1");
            m->addBorrowRecord(rec);
          }
        }
      }
      p = m;
    }

    if (p)
      users.push_back(p);
  }
  in.close();

  bool hasLib = false;
  for (auto *u : users)
    if (u->getRole() == "Librarian")
      hasLib = true;
  if (!hasLib)
    users.push_back(new Librarian("Admin", "admin", "admin@lib.com"));
}



void Library::run() {
  cout << "Welcome to Smart Library Management System!" << endl;
  while (true) {
    cout << "\n--- Login ---\n";
    cout << "1. Login\n2. Guest Search\n3. Exit\nChoice: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
      while (true) {
        string id;
        cout << "Enter User ID (or 'q' to go back): ";
        cin >> id;

        if (equalsIgnoreCase(id, "q")) {
          break;
        }

        Person *p = findUserByID(id);
        if (p) {
          currentUser = p;
          string role = p->getRole();
          if (role == "Librarian")
            librarianMenu();
          else if (role == "Member")
            memberMenu();
          currentUser = nullptr;
          break;
        } else {
          cout << "User not found! Try again or enter 'q' to go back." << endl;
        }
      }
    } else if (choice == 2) {
      Guest g;
      currentUser = &g;
      guestMenu();
      currentUser = nullptr;
    } else if (choice == 3) {
      break;
    } else {
      cout << "Invalid choice." << endl;
    }
  }
}

void Library::librarianMenu() {
  while (true) {
    cout << "\n--- Librarian Menu (" << currentUser->getName() << ") ---\n";
    cout << "1. Add Book\n2. Remove Book\n3. Register Member\n4. Remove "
            "Member\n";
    cout << "5. Display All Books\n6. View All Users\n7. View Book Queue\n8. "
            "View All Borrowed Books\n9. "
            "Logout\nChoice: ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
      addBook();
      break;
    case 2:
      removeBook();
      break;
    case 3:
      registerMember();
      break;
    case 4:
      removeMember();
      break;
    case 5:
      displayAllBooks();
      break;
    case 6:
      for (auto *u : users)
        u->displayInfo();
      break;
    case 7:
      viewBookQueue();
      break;
    case 8:
      viewAllBorrowedBooks();
      break;
    case 9:
      return;
    default:
      cout << "Invalid.\n";
    }
  }
}

void Library::memberMenu() {
  while (true) {
    cout << "\n--- Member Menu (" << currentUser->getName() << ") ---\n";
    cout << "1. Search Books\n2. Borrow Book\n3. Return Book\n";
    cout << "4. View My History\n5. Logout\nChoice: ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
      searchBooks();
      break;
    case 2:
      borrowBook();
      break;
    case 3:
      returnBook();
      break;
    case 4:
      viewMyBorrowedBooks();
      break;
    case 5:
      return;
    default:
      cout << "Invalid.\n";
    }
  }
}

void Library::guestMenu() {
  while (true) {
    cout << "\n--- Guest Menu ---\n";
    cout << "1. Search Books\n2. View All Books\n3. Back\nChoice: ";
    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
      searchBooks();
      break;
    case 2:
      displayAllBooks();
      break;
    case 3:
      return;
    default:
      cout << "Invalid.\n";
    }
  }
}

// ---------------- Operations ----------------

void Library::addBook() {
  string t, a, g, id;
  while (true) {
    cout << "Enter Book ID (or 'q' to cancel): ";
    cin >> id;
    if (equalsIgnoreCase(id, "q")) {
      cout << "Cancelled.\n";
      return;
    }
    if (findBookByID(id)) {
      cout << "ID already exists! Try again or enter 'q' to cancel.\n";
    } else {
      break;
    }
  }
  cout << "Enter Title: ";
  cin.ignore();
  getline(cin, t);
  cout << "Enter Author: ";
  getline(cin, a);
  cout << "Enter Genre: ";
  getline(cin, g);

  books.push_back(Book(t, a, g, id));
  cout << "Book added.\n";
}

void Library::removeBook() {
  while (true) {
    string id;
    cout << "Enter Book ID to remove (or 'q' to cancel): ";
    cin >> id;
    if (equalsIgnoreCase(id, "q")) {
      cout << "Cancelled.\n";
      return;
    }

    bool found = false;
    for (auto it = books.begin(); it != books.end(); ++it) {
      if (equalsIgnoreCase(it->getBookID(), id)) {
        books.erase(it);
        cout << "Book removed.\n";
        return;
      }
    }
    cout << "Book not found. Try again or enter 'q' to cancel.\n";
  }
}

void Library::registerMember() {
  string name, id, email;
  while (true) {
    cout << "Enter New Member ID (or 'q' to cancel): ";
    cin >> id;
    if (equalsIgnoreCase(id, "q")) {
      cout << "Cancelled.\n";
      return;
    }
    if (findUserByID(id)) {
      cout << "ID already exists! Try again or enter 'q' to cancel.\n";
    } else {
      break;
    }
  }
  cout << "Enter Name: ";
  cin.ignore();
  getline(cin, name);
  cout << "Enter Email: ";
  cin >> email;

  users.push_back(new Member(name, id, email));
  cout << "Member registered.\n";
}

void Library::removeMember() {
  while (true) {
    string id;
    cout << "Enter Member ID to remove (or 'q' to cancel): ";
    cin >> id;
    if (equalsIgnoreCase(id, "q")) {
      cout << "Cancelled.\n";
      return;
    }
    for (auto it = users.begin(); it != users.end(); ++it) {
      if (equalsIgnoreCase((*it)->getID(), id)) {
        delete *it;
        users.erase(it);
        cout << "User removed.\n";
        return;
      }
    }
    cout << "User not found. Try again or enter 'q' to cancel.\n";
  }
}

void Library::displayAllBooks() {
  for (const auto &b : books)
    b.displayInfo();
}

void Library::searchBooks() {
  string query;
  cout << "Enter Search Query (Title/Author/Genre): ";
  cin.ignore();
  getline(cin, query);

  bool found = false;
  for (const auto &b : books) {
    if (containsIgnoreCase(b.getTitle(), query) ||
        containsIgnoreCase(b.getAuthor(), query) ||
        containsIgnoreCase(b.getGenre(), query)) {
      b.displayInfo();
      found = true;
    }
  }
  if (!found)
    cout << "No books found.\n";
}

void Library::borrowBook() {
  if (currentUser->getRole() != "Member")
    return;

  while (true) {
    string bid;
    cout << "Enter Book ID to borrow (or 'q' to cancel): ";
    cin >> bid;

    if (equalsIgnoreCase(bid, "q")) {
      cout << "Cancelled.\n";
      return;
    }

    Book *b = findBookByID(bid);
    if (!b) {
      cout << "Book not found. Try again or enter 'q' to cancel.\n";
      continue;
    }

    if (b->getIsBorrowed()) {
      cout << "Book is currently borrowed by " << b->getBorrowedByMemberID()
           << ".\n";
      cout << "There are " << b->getReservationCount() << " people waiting.\n";
      time_t est = b->getEstimatedAvailabilityDate();
      cout << "Estimated availability if you join: "
           << ctime(&est);
      cout << "Add to reservation queue? (y/n): ";
      char c;
      cin >> c;
      if (c == 'y' || c == 'Y') {
        b->addReservation(currentUser->getID());
        cout << "Reservation added position.\n";
      }
      return;
    }

    // Check if reserved by someone else
    if (b->hasReservations()) {
      string nextID = b->getReservationQueue().peek();
      if (nextID != currentUser->getID()) {
        cout << "This book is reserved for member " << nextID << ".\n";
        cout << "Adding you to the queue.\n";
        b->addReservation(currentUser->getID());
        return;
      } else {

        b->getReservationQueue().dequeue();
      }
    }

    // Proceed to borrow
    time_t now = time(0);
    time_t due = now + (BORROW_DURATION_DAYS * 24 * 60 * 60);

    b->borrowBook(currentUser->getID(), due);

    // Add to history
    // Since currentUser is Member* (checked via role), we cast
    Member *m = dynamic_cast<Member *>(currentUser);
    m->addBorrowRecord(BorrowRecord(bid, now, due));

    cout << "Book borrowed successfully! Due: " << ctime(&due);
    return;
  }
}

void Library::returnBook() {
  while (true) {
    string bid;
    cout << "Enter Book ID to return (or 'q' to cancel): ";
    cin >> bid;

    if (equalsIgnoreCase(bid, "q")) {
      cout << "Cancelled.\n";
      return;
    }

    Book *b = findBookByID(bid);
    if (!b) {
      cout << "Book not found. Try again or enter 'q' to cancel.\n";
      continue;
    }

    if (!b->getIsBorrowed()) {
      cout << "Book was not borrowed. Try again or enter 'q' to cancel.\n";
      continue;
    }

    // Calculate Fine
    time_t now = time(0);
    int fine = 0;
    if (now > b->getDueDate()) {
      double diff = difftime(now, b->getDueDate());
      int daysOver = (int)(diff / (24 * 60 * 60));
      if (daysOver < 0)
        daysOver = 0; // Just in case
      fine = daysOver * FINE_PER_DAY;
      if (fine > 0)
        cout << "Book is Overdue! Fine: RM" << fine << endl;
    }

    // Update Member Record
    string borrowerID = b->getBorrowedByMemberID();
    Person *borrower = findUserByID(borrowerID);
    if (borrower && borrower->getRole() == "Member") {
      Member *m = dynamic_cast<Member *>(borrower);
      // Update the specific record in history
      // LinkedList searching is manual
      Node<BorrowRecord> *curr = m->getBorrowHistory().getHead();
      while (curr) {
        if (curr->data.bookID == bid && !curr->data.isReturned) {
          curr->data.isReturned = true;
          curr->data.returnDate = now;
          break;
        }
        curr = curr->next;
      }
    }

    b->returnBook();
    cout << "Book returned.\n";

    // Check Reservation
    if (b->hasReservations()) {
      cout << "Note: This book is reserved by "
           << b->getReservationQueue().peek() << ".\n";
    }
    return;
  }
}

void Library::viewMyBorrowedBooks() {
  if (currentUser->getRole() != "Member")
    return;
  currentUser->displayInfo();
}

int Library::calculateFine(time_t dueDate, time_t returnDate) {
  if (returnDate > dueDate) {
    double diff = difftime(returnDate, dueDate);
    int days = diff / (24 * 3600);
    return days * FINE_PER_DAY;
  }
  return 0;
}

void Library::viewBookQueue() {
  while (true) {
    string bid;
    cout << "Enter Book ID to view queue (or 'q' to cancel): ";
    cin >> bid;

    if (equalsIgnoreCase(bid, "q")) {
      cout << "Cancelled.\n";
      return;
    }

    Book *b = findBookByID(bid);
    if (!b) {
      cout << "Book not found. Try again or enter 'q' to cancel.\n";
      continue;
    }

    cout << "\n--- Reservation Queue for '" << b->getTitle() << "' ---\n";

    if (!b->hasReservations()) {
      cout << "No reservations for this book.\n";
      return;
    }

    Queue<string> &queue = b->getReservationQueue();
    Node<string> *curr = queue.getFront();
    int position = 1;

    while (curr != nullptr) {
      Person *p = findUserByID(curr->data);
      cout << position << ". Member ID: " << curr->data;
      if (p) {
        cout << " (" << p->getName() << ")";
      }
      cout << endl;
      curr = curr->next;
      position++;
    }

    cout << "Total in queue: " << queue.size() << endl;
    return;
  }
}

void Library::viewAllBorrowedBooks() {
  cout << "\n--- All Borrowed Books Details ---\n";
  bool found = false;
  for (const auto &b : books) {
    if (b.getIsBorrowed()) {
      found = true;
      cout << "Book ID: " << b.getBookID() << " | Title: " << b.getTitle();
      cout << "\n  Borrowed By: " << b.getBorrowedByMemberID();

      Person *borrower = findUserByID(b.getBorrowedByMemberID());
      if (borrower) {
        cout << " (" << borrower->getName() << ")";
      }


      time_t due = b.getDueDate();
      cout << "\n  Due Date: " << ctime(&due);
      cout << "-----------------------------------\n";
    }
  }
  if (!found) {
    cout << "No books are currently borrowed.\n";
  }
}
