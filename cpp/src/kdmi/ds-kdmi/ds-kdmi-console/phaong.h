
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef PHAONG__H
#define PHAONG__H

#include <functional>

#include "accessors.h"

#define PASTE_EXPRESSION(...) __VA_ARGS__

template<typename T, typename Numeric_Index_type = signed short>
struct phaong_galaxy
{
 typedef T hyponode_value_type;
 typedef T index_type;
 typedef T type_descriptor_type;
 typedef Numeric_Index_type numeric_index_type;
};

template<typename GALAXY_Type>
class phaong
{
public:

 typedef typename GALAXY_Type::hyponode_value_type hyponode_value_type;
 typedef typename GALAXY_Type::index_type index_type;
 typedef typename GALAXY_Type::numeric_index_type numeric_index_type;
 typedef typename GALAXY_Type::type_descriptor_type type_descriptor_type;

 struct Hyponode
 {
  hyponode_value_type hypoval;
  type_descriptor_type type_descriptor;
 };

 struct Hypocell
 {
  Hyponode* hyponodes;
  Hypocell* next;
 };

 class Hypernode
 {
  friend class phaong;
  Hypocell* first_cell_;
  numeric_index_type size_;
  type_descriptor_type type_descriptor_;

 public:

  Hypernode(Hypocell* first_cell, numeric_index_type size,
    type_descriptor_type type_descriptor)
   :  first_cell_(first_cell), size_(size),
      type_descriptor_(type_descriptor)
  {
  }

  numeric_index_type cell_size()
  {
   if(size_ < 0)
     return -size_;
   return size_;
  }

  bool fixed_size()
  {
   return size_ > 0;
  }

 };

 struct Hyperedge
 {
  Hypernode* head;
  Hypernode* tail;
  Hypernode* extra;
 };

private:

 std::function<void(phaong&, Hypernode*)> node_add_function_;
 std::function<void(phaong&, Hyperedge*)> edge_add_function_;
 void* user_data_;

 Hyponode* get_hyponode(Hypernode* hn, numeric_index_type ind)
 {
  int cell_index = ind % hn->cell_size();
  int cell_order = hn->fixed_size()? 0 :
    ind / hn->cell_size();

  Hypocell* hc = hn->first_cell_;

  if(cell_order > 0)
  {
   int cc = 0;
   while(cc < cell_order)
   {
    if(!hc->next)
    {
     Hyponode* hns = new Hyponode[hn->cell_size()];
     Hypocell* nhc = new Hypocell{hns, nullptr};
     hc->next = nhc;
    }
    hc = hc->next;
    ++cc;
   }
  }
  return &(hc->hyponodes)[cell_index];
 }

 void _set_data(Hypernode* hn, numeric_index_type ind,
   hyponode_value_type& val, type_descriptor_type* type_descriptor)
 {
  Hyponode* ho = get_hyponode(hn, ind);

  ho->hypoval = val;
  ho->type_descriptor = *type_descriptor;
 }

 Hypernode* _new_hypernode(numeric_index_type size,
   type_descriptor_type& type_descriptor)
 {
  numeric_index_type sz = size;
  if(sz < 0) sz = -sz;
  Hyponode* hns = new Hyponode[sz];
  Hypocell* hc = new Hypocell{hns, nullptr};
  Hypernode* result = new Hypernode(hc, size, type_descriptor);
  if(node_add_function_)
  {
   node_add_function_(*this, result);
  }
  return result;
 }

public:

 phaong() : node_add_function_(nullptr), edge_add_function_(nullptr),
   user_data_(nullptr)
 {}

 ACCESSORS(PASTE_EXPRESSION(std::function<void(phaong&, Hypernode*)>) ,node_add_function)
 ACCESSORS(PASTE_EXPRESSION(std::function<void(phaong&, Hyperedge*)>) ,edge_add_function)
 ACCESSORS(void* ,user_data)

 template<typename T>
 T* user_data_as()
 {
  return (T*) user_data_;
 }


 Hypernode* new_hypernode(numeric_index_type size,
   type_descriptor_type type_descriptor = type_descriptor_type())
 {
  return _new_hypernode(size, type_descriptor);
 }

 Hypernode* new_hypernode(numeric_index_type size,
   type_descriptor_type& type_descriptor)
 {
  return _new_hypernode(size, type_descriptor);
 }

 void set_data(Hypernode* hn, numeric_index_type ind,
   hyponode_value_type val, type_descriptor_type type_descriptor)
 {
  _set_data(hn, ind, val, &type_descriptor);
 }

 void set_data(Hypernode* hn, numeric_index_type ind,
   hyponode_value_type val, type_descriptor_type& type_descriptor)
 {
  _set_data(hn, ind, val, &type_descriptor);
 }

 void set_data(Hypernode* hn, numeric_index_type ind,
   hyponode_value_type& val, type_descriptor_type type_descriptor)
 {
  _set_data(hn, ind, val, &type_descriptor);
 }

 void set_data(Hypernode* hn, numeric_index_type ind,
   hyponode_value_type& val, type_descriptor_type& type_descriptor)
 {
  _set_data(hn, ind, val, &type_descriptor);
 }

 void set_data(Hypernode* hn, numeric_index_type ind,
   hyponode_value_type val)
 {
  _set_data(hn, ind, val, nullptr);
 }

 void set_data(Hypernode* hn, numeric_index_type ind,
   hyponode_value_type& val)
 {
  _set_data(hn, ind, val, nullptr);
 }

 void get_data(Hypernode* hn, numeric_index_type ind,
   std::function<void(hyponode_value_type&)> fn)
 {
  Hyponode* ho = get_hyponode(hn, ind);
  fn(ho->hypoval);
 }

 Hyperedge* new_hyperedge(Hypernode* hnh, Hypernode* hnt, Hypernode* hne = nullptr)
 {
  Hyperedge* result = new Hyperedge{hnh, hnt, hne};
  if(edge_add_function_)
  {
   edge_add_function_(*this, result);
  }
  return result;
 }


};

#endif //  PHAONG__H
