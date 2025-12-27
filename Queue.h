#ifndef QUEUE_H
#define QUEUE_H

#include "LinkedList.h"
#include <iostream>

using namespace std;

// We can composition or inheritance, or just implement separate node logic.
// Using the Node from LinkedList.h is convenient.

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
    // Should handle empty case, maybe throw or return default
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

#endif // QUEUE_H
