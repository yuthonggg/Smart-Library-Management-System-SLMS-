#ifndef QUEUE_H
#define QUEUE_H

#include "LinkedList.h"
#include <iostream>

using namespace std;


template <typename T> class Queue {
private:
  Node<T> *front;
  Node<T> *rear;

public:
  Queue() : front(nullptr), rear(nullptr) {}

  ~Queue() {
    while (!isEmpty()) {
      dequeue();
    }
  }

  void enqueue(T value) {
    Node<T> *newNode = new Node<T>(value);
    if (rear == nullptr) {
      front = rear = newNode;
      return;
    }
    rear->next = newNode;
    rear = newNode;
  }

  void dequeue() {
    if (isEmpty())
      return;
    Node<T> *temp = front;
    front = front->next;
    if (front == nullptr) {
      rear = nullptr;
    }
    delete temp;
  }

  T peek() const {
    if (!isEmpty())
      return front->data;

    return T();
  }

  bool isEmpty() const { return front == nullptr; }

  Node<T> *getFront() const { return front; }

  int size() const {
    int count = 0;
    Node<T> *temp = front;
    while (temp != nullptr) {
      count++;
      temp = temp->next;
    }
    return count;
  }
};

#endif
