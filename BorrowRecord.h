#ifndef BORROWRECORD_H
#define BORROWRECORD_H

#include <ctime>
#include <string>


using namespace std;

struct BorrowRecord {
  string bookID;
  time_t borrowDate;
  time_t dueDate;
  time_t returnDate;
  bool isReturned;

  BorrowRecord(string bID, time_t bDate, time_t dDate)
      : bookID(bID), borrowDate(bDate), dueDate(dDate), returnDate(0),
        isReturned(false) {}

  // Default constructor for Node creation if needed
  BorrowRecord()
      : bookID(""), borrowDate(0), dueDate(0), returnDate(0),
        isReturned(false) {}
};

#endif // BORROWRECORD_H
