#ifndef BOOK_H
#define BOOK_H

#include "Queue.h"
#include <iostream>
#include <string>

using namespace std;

class Book {
private:
  string title;
  string author;
  string genre;
  string bookID;
  bool isBorrowed;
  string borrowedByMemberID; // Who borrowed it?
  time_t dueDate;

  // Queue of Member IDs (strings)
  Queue<string> reservationQueue;

public:
  Book(string title, string author, string genre, string bookID);

  // Getters and Setters
  string getTitle() const;
  string getAuthor() const;
  string getGenre() const;
  string getBookID() const;
  bool getIsBorrowed() const;
  string getBorrowedByMemberID() const;
  time_t getDueDate() const;

  void borrowBook(string memberID, time_t due);
  void returnBook();

  // Reservation Management
  void addReservation(string memberID);
  string processReservation(); // Returns next member ID or empty
  bool hasReservations() const;
  Queue<string> &getReservationQueue(); // Helper if we need to inspect (mostly
                                        // we just process)
  int getReservationCount() const;
  time_t getEstimatedAvailabilityDate() const;

  void displayInfo() const;
};

#endif // BOOK_H
