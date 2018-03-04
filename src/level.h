#ifndef BOOK_LEVEL_H_
#define BOOK_LEVEL_H_

#include <list>
namespace incremental_book
{
template <class TInner>
class Level
{
  public:
  void clear(){
      data_.clear();
  }
    void add(TInner item)
    {
        price = item.price;
        data_.push_back(item);
    }

    void remove(TInner item)
    {
        for (auto i = std::begin(data_); i != std::end(data_); i++)
        {
            if (i->order_id == item.order_id)
            {
                data_.erase(i);
                break;
            }
        }
    }

    int price;
    bool is_empty() { return data_.size() == 0;}
    std::list<TInner> data_;
};

class Order
{
  public:
    Order(int p, long id) : price(p), order_id(id) {}
    int  price;
    long  order_id;
};
}

#endif