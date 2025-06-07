#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <sstream>

template <typename T>
struct ForwardListNode
{
    T data;
    ForwardListNode* next = nullptr;
    ForwardListNode() = default;
    ForwardListNode(const T& data_) : data(data_) {}
    ForwardListNode(T&& data_) : data(std::move(data_)) {}
    ForwardListNode(const ForwardListNode& other) : data(other.data)
    {
        if (other.next)
        {
            next = new ForwardListNode(*other.next);
        }
    }
    ForwardListNode(ForwardListNode&& other) noexcept
        : data(std::move(other.data))
    {
        if (other.next)
        {
            next = other.next;
            other.next = nullptr;
        }
    }
    ForwardListNode(const T& data_, ForwardListNode* next_)
        : data(data_), next(next_)
    {
    }
    ForwardListNode(T&& data_, ForwardListNode* next_)
        : data(std::move(data_)), next(next_)
    {
    }
};

template <typename T>
void create_list(ForwardListNode<T>*& head, const std::vector<T>& data)
{
    head = nullptr;
    if (data.empty()) return;

    ForwardListNode<T>* current = nullptr;
    for (const auto& item : data)
    {
        ForwardListNode<T>* new_node = new ForwardListNode<T>(item);
        if (!head)
        {
            head = new_node;
            current = head;
        }
        else
        {
            current->next = new_node;
            current = new_node;
        }
    }
}

template <typename T>
void delete_list(ForwardListNode<T>*& head)
{
    ForwardListNode<T>* current = head;
    while (current)
    {
        ForwardListNode<T>* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

template <typename T>
void print_list(const ForwardListNode<T>* head, const std::ostream& os)
{
    std::ostringstream& oss = dynamic_cast<std::ostringstream&>(const_cast<std::ostream&>(os));
    const ForwardListNode<T>* current = head;
    while (current)
    {
        oss << current->data;
        if (current->next)
        {
            oss << " ";
        }
        current = current->next;
    }
}

template <typename T>
void reverse_list(ForwardListNode<T>*& head)
{
    ForwardListNode<T>* prev = nullptr;
    ForwardListNode<T>* current = head;
    ForwardListNode<T>* next = nullptr;

    while (current)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
}

template <typename T>
void remove_duplicates(ForwardListNode<T>*& head)
{
    if (!head || !head->next) return;

    ForwardListNode<T>* current = head;
    while (current)
    {
        ForwardListNode<T>* runner = current;
        while (runner->next)
        {
            if (runner->next->data == current->data)
            {
                ForwardListNode<T>* temp = runner->next;
                runner->next = runner->next->next;
                delete temp;
            }
            else
            {
                runner = runner->next;
            }
        }
        current = current->next;
    }
}