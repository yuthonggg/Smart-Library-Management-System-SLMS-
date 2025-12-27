#include "Book.h"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

Book::Book(string title, string author, string genre, string bookID)
    : title(title), author(author), genre(genre), bookID(bookID),
      isBorrowed(false), borrowedByMemberID(""), dueDate(0) {}

string Book::getTitle() const { return title; }
string Book::getAuthor() const { return author; }
string Book::getGenre() const { return genre; }
string Book::getBookID() const { return bookID; }
bool Book::getIsBorrowed() const { return isBorrowed; }
string Book::getBorrowedByMemberID() const { return borrowedByMemberID; }
time_t Book::getDueDate() const { return dueDate; }

void Book::borrowBook(string memberID, time_t due) {
  isBorrowed = true;
  borrowedByMemberID = memberID;
  dueDate = due;
}

void Book::returnBook() {
  isBorrowed = false;
  borrowedByMemberID = "";
  dueDate = 0;
}

void Book::addReservation(string memberID) {
  reservationQueue.enqueue(memberID);
}

string Book::processReservation() {
  if (reservationQueue.isEmpty())
    return "";
  string nextMember = reservationQueue.peek();
  reservationQueue.dequeue();
  return nextMember;
}

bool Book::hasReservations() const { return !reservationQueue.isEmpty(); }

Queue<string> &Book::getReservationQueue() { return reservationQueue; }

void Book::displayInfo() const {
  cout << "ID: " << bookID << " | Title: " << title << " | Author: " << author
       << " | Genre: " << genre
       << " | Status: " << (isBorrowed ? "Borrowed" : "Available");
  if (isBorrowed) {
    cout << " (Due: " << ctime(&dueDate) << ")";
  }
  cout << endl;
  if (reservationQueue.size() > 0) {
    cout << "   Queue Size: " << reservationQueue.size() << endl;
  }
}

int Book::getReservationCount() const { return reservationQueue.size(); }

time_t Book::getEstimatedAvailabilityDate() const {
  if (!isBorrowed)
    return time(0);
  // 7 days in seconds = 7 * 24 * 60 * 60 = 604800
  return dueDate + (reservationQueue.size() * 604800);
}
