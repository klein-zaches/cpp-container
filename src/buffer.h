#ifndef BOOK_BUFFER_H_
#define BOOK_BUFFER_H_
#include <stdexcept>
#include <iterator>
#include "common.h"

namespace incremental_book
{
template <class T, class TInner>
class Buffer
{
public:
  class Iterator
  {
  public:
    typedef Iterator self_type;
    typedef T value_type;
    typedef T &reference;
    typedef T *pointer;
    typedef std::forward_iterator_tag iterator_category;
    typedef int difference_type;
    Iterator() : Iterator(nullptr, 0, 0, 0) {}
    Iterator(pointer ptr, buffer_size_t valid_items, buffer_size_t filter, buffer_size_t index) : valid_items_count_(valid_items),
                                                                                                  filter_(filter),
                                                                                                  iterated_(1),
                                                                                                  head_(ptr)
    {
      current_item_ = index & filter;
      ptr_ = head_ + current_item_;
    }

    self_type operator++()
    {
      next();
      return *this;
    }
    self_type operator++(int junk)
    {
      self_type current_state = *this;
      next();
      return current_state;
    }
    reference operator*() { return *ptr_; }
    pointer operator->() { return ptr_; }
    bool operator==(const self_type &rhs) { return ptr_ == rhs.ptr_; }
    bool operator!=(const self_type &rhs) { return ptr_ != rhs.ptr_; }

  private:
    pointer ptr_;
    pointer head_;
    buffer_size_t filter_;
    buffer_size_t valid_items_count_;
    buffer_size_t current_item_;
    buffer_size_t iterated_;
    void next()
    {
      if (iterated_ == valid_items_count_)
      {
        ptr_ = nullptr;
      }
      else
      {
        do
        {
          current_item_ = (current_item_ + 1) & filter_;
          ptr_ = head_ + current_item_;
        } while (ptr_->is_empty());
        ++iterated_;
      }
    }
  };

  Buffer(int exponent_of_2)
  {
    max_size_ = 1ULL << exponent_of_2;
    filter_ = max_size_ - 1;
    head_ = max_size_ >> 1;
    data_ = new T[max_size_];
    size_ = 0;
  }

  ~Buffer() { delete[] data_; }
  void add(TInner item)
  {
    auto position = get_item_position(item);
    if (data_[position].is_empty())
    {
      ++size_;
    }

    if (position != head_)
    {
      if (item.price > data_[head_].price)
      {
        // when head_ is moved we also should clean all the "far" levels between previous head_ and new one
        auto index = (head_ + max_size_ - 1) & filter_;
        do
        {
          if (data_[index].is_empty() == false)
          {
            data_[index].clear();
          }
          //to loop
          index = (max_size_ - 1 + index) & filter_;
        } while (index != position);
        head_ = position;
      }
    }

    data_[position].add(item);
  }
  void remove(TInner item)
  {
    auto position = get_item_position(item);
    if (data_[position].is_empty())
    {
      throw std::invalid_argument("cannot remove order from an empty level");
    }
  }
  Iterator begin() { return Iterator(data_, size_, filter_, head_); }
  Iterator end() { return Iterator(); }
  buffer_size_t max_size() { return max_size_; }
  buffer_size_t size() { return size_; }

private:
  buffer_size_t max_size_;
  buffer_size_t size_;
  buffer_size_t filter_;
  buffer_size_t head_;
  T *data_;

  buffer_size_t get_item_position(TInner item)
  {
    if (size_ > 0)
    {
      auto diff = data_[head_].price - item.price;
      auto position = head_ + max_size_ + diff;
      return position & filter_;
    }
    return head_;
  }
};
}
#endif
