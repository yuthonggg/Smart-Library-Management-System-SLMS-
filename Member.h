#ifndef MEMBER_H
#define MEMBER_H

#include "BorrowRecord.h"
#include "LinkedList.h"
#include "Person.h"


class Member : public Person {
private:
  LinkedList<BorrowRecord> borrowHistory;

public:
  Member(string name, string id, string email);

  string getRole() const override;
  void displayInfo() const override;

  void addBorrowRecord(BorrowRecord recor3d);
  LinkedList<BorrowRecord> &getBorrowHistory();
};

#endif
