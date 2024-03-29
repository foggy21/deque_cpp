#pragma once
#include <iterator>
#include <memory>
#include <stdexcept>
#include <utility>

namespace fefu_laboratory_two {
template <typename T>
class Allocator {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;

  Allocator() noexcept = default;

  Allocator(const Allocator& other) noexcept = default;

  template <class U>
  Allocator(const Allocator<U>& other) noexcept;

  ~Allocator() = default;

  pointer allocate(size_type mem_size = 1) {
      return static_cast<pointer>(operator new(mem_size * sizeof(value_type)));
  }

  void deallocate(pointer p, size_type n) noexcept {
      ::operator delete(p, n);
  }

  //[[nodiscard]] std::allocation_result<T*> allocate_at_least(
  // std::size_t n ); // TODO For extra points
};

template <typename ValueType>
struct Node
{
    ValueType value;
    Node<ValueType>* next;
    Node<ValueType>* prev;

    Node(ValueType value) {
        this->prev = nullptr;
        this->next = nullptr;
        this->value = value;
    }
};

template <typename ValueType>
class Deque_iterator {
 public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = ValueType;
  using difference_type = std::ptrdiff_t;
  using pointer = ValueType*;
  using reference = ValueType&;

  Deque_iterator() noexcept = default;

  Deque_iterator(const Deque_iterator& other) noexcept {
      data = other.data;
  };

  Deque_iterator& operator=(const Deque_iterator& other) {
      data = other.data;
      return *data;
  };

  ~Deque_iterator() = default;

  friend void swap(Deque_iterator<ValueType>& it1, Deque_iterator<ValueType>& it2) {
      Deque_iterator<ValueType> itTemp = it2;
      it2 = it1;
      it1 = itTemp;
      delete itTemp;
  }

  friend bool operator==(const Deque_iterator<ValueType>& it1, const Deque_iterator<ValueType>& it2) {
      Node<Deque_iterator<ValueType>> node1(it1);
      Node<Deque_iterator<ValueType>> node2(it2);
      while (node1.next != nullptr && node2.next != nullptr) {
          if (node1.value == node2.value) {
              node1 = node1.next;
              node2 = node2.next;
          }
          else {
              return false;
          }
          if ((node1 == nullptr && node2 != nullptr) ||
              (node1 != nullptr && node2 == nullptr)) {
              return false;
          }
      }
      return true;
  }
  friend bool operator!=(const Deque_iterator<ValueType>& it1, const Deque_iterator<ValueType>& it2) {
      Node<Deque_iterator<ValueType>> node1(it1);
      Node<Deque_iterator<ValueType>> node2(it2);
      while (node1.next != nullptr && node2.next != nullptr) {
          if (node1.value == node2.value) {
              node1 = node1.next;
              node2 = node2.next;
          }
          else {
              return true;
          }
          if ((node1 == nullptr && node2 != nullptr) ||
              (node1 != nullptr && node2 == nullptr)) {
              return true;
          }
      }
      return false;
  }

  reference operator*() const {
      reference ref = *data->value;
      return ref;
  }
  pointer operator->() const {
      return static_cast<pointer>(&data->value);
  }

  Deque_iterator& operator++() {
      data = data->next;
      return *data;
  };
  Deque_iterator operator++(int) {
      data = data->next;
      return *data;
  }

  Deque_iterator& operator--() {
      data = data->prev;
      return *data;
  }
  Deque_iterator operator--(int) {
      data = data->prev;
      return *data;
  }

  Deque_iterator operator+(const difference_type& other) const {
      int count = static_cast<int>(other);
      while (count > 0) {
          data = data->next;
          if (data->next == nullptr) {
              break;
          }
          count--;
      }
      return *data;
  }
  Deque_iterator& operator+=(const difference_type& other) {
      int count = static_cast<int>(other);
      while (count > 0) {
          data = data->next;
          if (data->next == nullptr) {
              break;
          }
          count--;
      }
      return *data;
  }

  Deque_iterator operator-(const difference_type& other) const {
      int count = static_cast<int>(other);
      while (count > 0) {
          data = data->prev;
          if (data->prev == nullptr) {
              break;
          }
          count--;
      }
      return *data;
  }
  Deque_iterator& operator-=(const difference_type& other) {
      int count = static_cast<int>(other);
      while (count > 0) {
          data = data->prev;
          if (data->prev == nullptr) {
              break;
          }
          count--;
      }
      return *data;
  }

  difference_type operator-(const Deque_iterator& other) const {
      difference_type count = 0;

      if (data < other.data) {
          while (data->value != other.data->value) {
              data = data->next;
              count++;
          }
          return count;
      }
      else if (data > other.data) {
          while (data->value != other.data->value) {
              data = data->prev;
              count++;
          }
          return count;
      }
      return count;
  }

  reference operator[](const difference_type& other) {
      int count = static_cast<int>(other);
      if (count >= 0) {
          while (other > 0) {
              data = data->next;
              if (data->next == nullptr) {
                  break;
              }
              count--;
          }
      }
      else {
          while (count < 0) {
              data = data->prev;
              if (data->prev == nullptr) {
                  break;
              }
              count++;
          }
      }
      reference ref = *data;
      return ref;
  }

  friend bool operator<(const Deque_iterator<ValueType>& it1,
      const Deque_iterator<ValueType>& it2) {
      while (it2.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it2.data == nullptr && (it1.data != nullptr && it1.data == nullptr)) return false;
      }
      return true;
  }
  friend bool operator<=(const Deque_iterator<ValueType>& it1,
      const Deque_iterator<ValueType>& it2) {
      while (it2.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it2.data == nullptr && it1.data != nullptr) return false;
      }
      return true;
  }
  friend bool operator>(const Deque_iterator<ValueType>& it1,
      const Deque_iterator<ValueType>& it2) {
      while (it1.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it1.data == nullptr && (it2.data != nullptr || it2.data == nullptr)) return false;
      }
      return true;
  }
  friend bool operator>=(const Deque_iterator<ValueType>& it1,
      const Deque_iterator<ValueType>& it2) {
      while (it1.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it1.data == nullptr && it2.data != nullptr) return false;
      }
      return true;
  }
  // operator<=> will be handy
  Node<ValueType>* data;
};

template <typename ValueType>
class Deque_const_iterator {
    // Shouldn't give non const references on value
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using pointer = const ValueType*;
    using reference = const ValueType&;

    Deque_const_iterator() noexcept = default;
    Deque_const_iterator(const Deque_const_iterator& other) noexcept {
        *this = other;
    }
    Deque_const_iterator(const Deque_iterator<ValueType>& other) noexcept {
        Node<Deque_iterator<ValueType>> it(other);
        data = it;
    }

    Deque_const_iterator& operator=(const Deque_const_iterator& other) {
        data = other.data;
        return *data;
    }
    Deque_const_iterator& operator=(const Deque_iterator<ValueType>& other) {
        data = other.data;
        return *data;
    }

    ~Deque_const_iterator() = default;

    friend void swap(Deque_const_iterator<ValueType>& it1,
        Deque_const_iterator<ValueType>& it2) {
        Deque_const_iterator<ValueType> itTemp = it2;
        it2 = it1;
        it1 = itTemp;
        delete itTemp;
    }

    friend bool operator==(const Deque_const_iterator<ValueType>& it1,
        const Deque_const_iterator<ValueType>& it2) {
        return it1.data == it2.data;
    }
    friend bool operator!=(const Deque_const_iterator<ValueType>& it1,
        const Deque_const_iterator<ValueType>& it2) {
        return it1.data != it2.data;
    }

    reference operator*() const {
        reference ref = data->value;
        return ref;
    }
    pointer operator->() const {
        return static_cast<pointer>(&data->value);
    }

    Deque_const_iterator& operator++() {
        data = data->next;
        return *data;
    }
    Deque_const_iterator operator++(int) {
        data = data->next;
        return *this;
    }

    Deque_const_iterator& operator--() {
        data = data->prev;
        return *data;
    }
    Deque_const_iterator operator--(int) {
        data = data->prev;
        return *data;
    }

    Deque_const_iterator operator+(const difference_type& other) const {
        int count = other;
        while (count > 0) {
            data = data->next;
            if (data->next == nullptr) {
                break;
            }
            count--;
        }
        return *data;
    }
    Deque_const_iterator& operator+=(const difference_type& other) {
        int count = other;
        while (count > 0) {
            data = data->next;
            if (data->next == nullptr) {
                break;
            }
            count--;
        }
        return *data;
    }

    Deque_const_iterator operator-(const difference_type& other) const {
        int count = other;
        while (count > 0) {
            data = data->prev;
            if (data->prev == nullptr) {
                break;
            }
            count--;
        }
        return *data;
    }
    Deque_const_iterator& operator-=(const difference_type& other) {
        int count = other;
        while (count > 0) {
            data = data->prev;
            if (data->prev == nullptr) {
                break;
            }
            count--;
        }
        return *data;
    }

    difference_type operator-(const Deque_const_iterator& other) const {
        difference_type count = 0;

        if (data < other.data) {
            while (data->value != other.data->value) {
                data = data->next;
                count++;
            }
            return count;
        }
        else if (data > other.data) {
            while (data->value != other.data->value) {
                data = data->prev;
                count++;
            }
            return count;
        }
        return count;
    }

  reference operator[](const difference_type& other) {
      int count = other;
      if (count >= 0) {
          while (count > 0) {
              data = data->next;
              if (data->next == nullptr) {
                  break;
              }
              count--;
          }
      }
      else {
          while (count < 0) {
              data = data->prev;
              if (data->prev == nullptr) {
                  break;
              }
              count++;
          }
      }
      reference ref = *data;
      return ref;
  }

  friend bool operator<(const Deque_const_iterator<ValueType>& it1,
      const Deque_const_iterator<ValueType>& it2) {
      while (it2.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it2.data == nullptr && (it1.data != nullptr && it1.data == nullptr)) return false;
      }
      return true;
  }
  friend bool operator<=(const Deque_const_iterator<ValueType>& it1,
      const Deque_const_iterator<ValueType>& it2) {
      while (it2.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it2.data == nullptr && it1.data != nullptr) return false;
      }
      return true;
  }
  friend bool operator>(const Deque_const_iterator<ValueType>& it1,
      const Deque_const_iterator<ValueType>& it2) {
      while (it1.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it1.data == nullptr && (it2.data != nullptr || it2.data == nullptr)) return false;
      }
      return true;
  }
  friend bool operator>=(const Deque_const_iterator<ValueType>& it1,
      const Deque_const_iterator<ValueType>& it2) {
      while (it1.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it1.data == nullptr && it2.data != nullptr) return false;
      }
      return true;
  }
  // operator<=> will be handy

  Node<ValueType>* data;
};

template <class Iter>
class Deque_reverse_iterator {
 public:
  using iterator_type = Iter;
  using iterator_category =
      typename std::iterator_traits<Iter>::iterator_category;
  using value_type = typename std::iterator_traits<Iter>::value_type;
  using difference_type = typename std::iterator_traits<Iter>::difference_type;
  using pointer = typename std::iterator_traits<Iter>::pointer;
  using reference = typename std::iterator_traits<Iter>::reference;

  constexpr Deque_reverse_iterator() = default;

  constexpr explicit Deque_reverse_iterator(iterator_type x) {
      
  }

  template <class U>
  constexpr Deque_reverse_iterator(const Deque_reverse_iterator<U>& other) {
      static_assert(std::is_same_v<decltype(*std::declval<U>().data), decltype(data)>, "");
  }

  template <class U>
  Deque_reverse_iterator& operator=(const Deque_reverse_iterator<U>& other) {
      this->data = other.data;
  }

  iterator_type base() const {
      return *iter;
  }

  reference operator*() const {
      reference ref = *data;
      return ref;
  }

  pointer operator->() const {
      return static_cast<pointer>(&data->value);
  }

  reference operator[](difference_type n) {
      if (n >= 0) {
          while (n > 0) {
              data = data->prev;
              if (data->prev == nullptr) {
                  break;
              }
              n--;
          }
      } else {
          while (n < 0) {
              data = data->next;
              if (data->next == nullptr) {
                  break;
              }
              n++;
          }
      }
      reference ref = *data;
      return ref;
  }

  Deque_reverse_iterator& operator++() {
      data = data->prev;
      return *data;
  }
  Deque_reverse_iterator operator++(int) {
      data = data->prev;
      return *data;
  }

  Deque_reverse_iterator& operator--() {
      data = data->next;
      return *data;
  }
  Deque_reverse_iterator operator--(int) {
      data = data->next;
      return *data;
  }

  Deque_reverse_iterator operator+(difference_type n) const {
      while (n > 0) {
          data = data->prev;
          if (data->prev == nullptr) {
              break;
          }
          n--;
      }
      return *data;
  }
  Deque_reverse_iterator& operator+=(difference_type n) {
      while (n > 0) {
          data = data->prev;
          if (data->prev == nullptr) {
              break;
          }
          n--;
      }
      return *data;
  }

  Deque_reverse_iterator operator-(difference_type n) const {
      while (n > 0) {
          data = data->next;
          if (data->next == nullptr) {
              break;
          }
          n--;
      }
      return *data;
  }
  Deque_reverse_iterator& operator-=(difference_type n) {
      while (n > 0) {
          data = data->next;
          if (data->next == nullptr) {
              break;
          }
          n--;
      }
      return *data;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator==(const Deque_reverse_iterator<Iterator1>& lhs,
      const Deque_reverse_iterator<Iterator2>& rhs) {
      return rhs.data == lhs.data;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator!=(const Deque_reverse_iterator<Iterator1>& lhs,
      const Deque_reverse_iterator<Iterator2>& rhs) {
      return rhs.data != lhs.data;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator>(const Deque_reverse_iterator<Iterator1>& it1,
      const Deque_reverse_iterator<Iterator2>& it2) {
      while (it1.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it1.data == nullptr && (it2.data != nullptr || it2.data == nullptr)) return false;
      }
      return true;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator<(const Deque_reverse_iterator<Iterator1>& it1,
      const Deque_reverse_iterator<Iterator2>& it2) {
      while (it2.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it2.data == nullptr && (it1.data != nullptr && it1.data == nullptr)) return false;
      }
      return true;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator<=(const Deque_reverse_iterator<Iterator1>& it1,
      const Deque_reverse_iterator<Iterator2>& it2) {
      while (it2.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it2.data == nullptr && it1.data != nullptr) return false;
      }
      return true;
  }

  template <class Iterator1, class Iterator2>
  friend bool operator>=(const Deque_reverse_iterator<Iterator1>& it1,
      const Deque_reverse_iterator<Iterator2>& it2) {
      while (it1.data != nullptr) {
          it1.data = it1.data->next;
          it2.data = it2.data->next;
          if (it1.data == nullptr && it2.data != nullptr) return false;
      }
      return true;
  }

  template <class IterT>
  friend Deque_reverse_iterator<IterT> operator+(
      typename Deque_reverse_iterator<IterT>::difference_type n,
      const Deque_reverse_iterator<IterT>& it) {
      return it.base() - n;
  }

  template <class Iterator>
  friend auto operator-(const Deque_reverse_iterator<Iterator>& lhs,
      const Deque_reverse_iterator<Iterator>& rhs) {
      difference_type count = 0;

      if (lhs < rhs) {
          while (rhs.data->value != lhs.data->value) {
              lhs.data = lhs.data->prev;
              count++;
          }
          return count;
      }
      else if (lhs > rhs) {
          while (rhs.data->value != lhs.data->value) {
              lhs.data = lhs.data->next;
              count++;
          }
          return count;
      }
      return count;
  }

  // operator <=> will be handy

  // friend constexpr std::iter_rvalue_reference_t<Iter> iter_move( const
  // std::reverse_iterator& i ); // For extra points

  // template<std::indirectly_swappable<Iter> Iter2>
  // friend constexpr void iter_swap(const reverse_iterator& x, const
  // std::reverse_iterator<Iter2>& y); // For extra points
  Node<Iter>* data;
private:
    iterator_type* iter;
};

template <class Iter>
Deque_reverse_iterator<Iter> make_reverse_iterator(Iter i) {
    return Deque_reverse_iterator<Iter>(i);
}

template <typename T, typename Allocator = Allocator<T>>
class Deque {
 public:
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = Deque_iterator<value_type>;
  using const_iterator = Deque_const_iterator<value_type>;
  using reverse_iterator = Deque_reverse_iterator<iterator>;
  using const_reverse_iterator = Deque_reverse_iterator<const_iterator>;

  /// @brief Default constructor. Constructs an empty container with a
  /// default-constructed allocator.
  Deque() = default;

  /// @brief Constructs an empty container with the given allocator
  /// @param alloc allocator to use for all memory allocations of this container
  explicit Deque(const Allocator& alloc) : alloc(alloc) {}

  /// @brief Constructs the container with count copies of elements with value
  /// and with the given allocator
  /// @param count the size of the container
  /// @param value the value to initialize elements of the container with
  /// @param alloc allocator to use for all memory allocations of this container
  Deque(size_type count, const T& value, const Allocator& alloc = Allocator()) : alloc(alloc) {
      for (size_type iter = 0; iter < count; ++iter) {
          this->push_front(value);
     }
  }

  /// @brief Constructs the container with count default-inserted instances of
  /// T. No copies are made.
  /// @param count the size of the container
  /// @param alloc allocator to use for all memory allocations of this container
  explicit Deque(size_type count, const Allocator& alloc = Allocator()) : alloc(alloc) {
      for (size_type iter = 0; iter < count; ++iter) {
          this->push_front(T{});
      }
  }

  /// @brief Constructs the container with the contents of the range [first,
  /// last).
  /// @tparam InputIt Input Iterator
  /// @param first, last 	the range to copy the elements from
  /// @param alloc allocator to use for all memory allocations of this container
  template <class InputIt>
  Deque(InputIt first, InputIt last, const Allocator& alloc = Allocator()) : alloc(alloc) {
      for (auto iter = first; iter != last; iter++) {
          Node<InputIt>* abc = new Node<InputIt>(iter);
          this->push_front(abc->value);
      }
  }

  /// @brief Copy constructor. Constructs the container with the copy of the
  /// contents of other.
  /// @param other another container to be used as source to initialize the
  /// elements of the container with
  Deque(const Deque& other) {
      for (Node<T>* iter = other.head; iter != nullptr; iter++) {
          this->push_front(iter->value);
      }
  }

  /// @brief Constructs the container with the copy of the contents of other,
  /// using alloc as the allocator.
  /// @param other another container to be used as source to initialize the
  /// elements of the container with
  /// @param alloc allocator to use for all memory allocations of this container
  Deque(const Deque& other, const Allocator& alloc) : alloc(alloc) {
      for (Node<T>* iter = other.head; iter != nullptr; iter++) {
          this->push_front(iter->value);
      }
  }

  /**
   * @brief Move constructor.
   *
   * Constructs the container with the contents of other using move semantics.
   * Allocator is obtained by move-construction from the allocator belonging to
   * other.
   *
   * @param other another container to be used as source to initialize the
   * elements of the container with
   */
  Deque(Deque&& other) {
      swap(other);
  }

  /**
   * @brief Allocator-extended move constructor.
   * Using alloc as the allocator for the new container, moving the contents
   * from other; if alloc != other.get_allocator(), this results in an
   * element-wise move.
   *
   * @param other another container to be used as source to initialize the
   * elements of the container with
   * @param alloc allocator to use for all memory allocations of this container
   */
  Deque(Deque&& other, const Allocator& alloc) : alloc(alloc) {
      swap(other);
  }

  /// @brief Constructs the container with the contents of the initializer list
  /// init.
  /// @param init initializer list to initialize the elements of the container
  /// with
  /// @param alloc allocator to use for all memory allocations of this container
  Deque(std::initializer_list<T> init, const Allocator& alloc = Allocator()) : alloc(alloc) {
      for (auto iter = init.begin(); iter != init.end(); ++iter) {
          this->push_front(*iter);
      }
  }

  /// @brief Destructs the deque.
  ~Deque() = default;

  /// @brief Copy assignment operator. Replaces the contents with a copy of the
  /// contents of other.
  /// @param other another container to use as data source
  /// @return *this
  Deque& operator=(const Deque& other) {
      if (this == &other)
          return *this;
      this->head = new Node<T>(other.head);
      Node<T>* temp = other.head->next;
      Node<T>* pointNew = nullptr;
      Node<T>* pointOld = head;
      while (temp)
      {
          pointNew->value = new Node<T>(temp->value);
          pointNew->prev = pointOld;
          pointOld->next = pointNew;
          pointOld = pointNew;
          temp = temp->next;
      }
      return *this;
  }

  /**
   * Move assignment operator.
   *
   * Replaces the contents with those of other using move semantics
   * (i.e. the data in other is moved from other into this container).
   * other is in a valid but unspecified state afterwards.
   *
   * @param other another container to use as data source
   * @return *this
   */
  Deque& operator=(Deque&& other) {
      Deque temp(std::move(other));
      std::swap(*this, other);
      return *this;
  }

  /// @brief Replaces the contents with those identified by initializer list
  /// ilist.
  /// @param ilist
  /// @return this
  Deque& operator=(std::initializer_list<T> ilist) {
      for (auto iter = ilist.begin(); iter < ilist.end(); iter++) {
          this->push_front(*iter);
      }
  }

  /// @brief Replaces the contents with count copies of value
  /// @param count
  /// @param value
  void assign(size_type count, const T& value) {
      for (size_type iter = 0; iter < count; iter++) {
          this->push_front(value);
      }
  }

  /// @brief Replaces the contents with copies of those in the range [first,
  /// last).
  /// @tparam InputIt
  /// @param first
  /// @param last
  template <class InputIt>
  void assign(InputIt first, InputIt last) {
      for (auto iter = first; iter != last; iter++) {
          Node<InputIt>* abc = new Node<InputIt>(iter);
          this->push_front(abc->value);
      }
  }

  /// @brief Replaces the contents with the elements from the initializer list
  /// ilis
  /// @param ilist
  void assign(std::initializer_list<T> ilist) {
      for (size_type iter = ilist.begin(); iter != ilist.end(); iter++) {
          this->push_front(*iter);
      }
  }

  /// @brief Returns the allocator associated with the container.
  /// @return The associated allocator.
  allocator_type get_allocator() const noexcept {
      return alloc;
  }

  /// ELEMENT ACCESS

  /// @brief Returns a reference to the element at specified location pos, with
  /// bounds checking. If pos is not within the range of the container, an
  /// exception of type std::out_of_range is thrown.
  /// @param pos position of the element to return
  /// @return Reference to the requested element.
  /// @throw std::out_of_range
  reference at(size_type pos) {
      if (pos > size_count-1) throw std::out_of_range;
      Node<T>* temp = head;
      for (size_type count = 0; count < pos; count++) {
          temp = temp->next;
      }
      reference ref = temp->value;
      return ref;
  }

  /// @brief Returns a const reference to the element at specified location pos,
  /// with bounds checking. If pos is not within the range of the container, an
  /// exception of type std::out_of_range is thrown.
  /// @param pos position of the element to return
  /// @return Const Reference to the requested element.
  /// @throw std::out_of_range
  const_reference at(size_type pos) const {
    if (pos > size_count - 1) throw std::out_of_range;
    Node<T>* temp = head;
    for (size_type count = 0; count < pos; count++) {
        temp = temp->next;
    }
    const_reference ref = temp->value;
    return ref;
  }

  /// @brief Returns a reference to the element at specified location pos. No
  /// bounds checking is performed.
  /// @param pos position of the element to return
  /// @return Reference to the requested element.
  reference operator[](size_type pos) {
    Node<T>* temp = head;
    for (size_type count = 0; count < pos; count++) {
        temp = temp->next;
    }
    reference ref = temp->value;
    return ref;
  }

  /// @brief Returns a const reference to the element at specified location pos.
  /// No bounds checking is performed.
  /// @param pos position of the element to return
  /// @return Const Reference to the requested element.
  const_reference operator[](size_type pos) const {
    Node<T>* temp = head;
    for (size_type count = 0; count < pos; count++) {
        temp = temp->next;
    }
    reference ref = temp->value;
    return ref;
  }

  /// @brief Returns a reference to the first element in the container.
  /// Calling front on an empty container is undefined.
  /// @return Reference to the first element
  reference front() {
    reference ref = this->head->value;
    return ref;
  }

  /// @brief Returns a const reference to the first element in the container.
  /// Calling front on an empty container is undefined.
  /// @return Const reference to the first element
  const_reference front() const {
    const_reference ref = this->head->value;
    return ref;
  }

  /// @brief Returns a reference to the last element in the container.
  /// Calling back on an empty container causes undefined behavior.
  /// @return Reference to the last element.
  reference back() {
    reference ref = this->tail->value;
    return ref;
  }

  /// @brief Returns a const reference to the last element in the container.
  /// Calling back on an empty container causes undefined behavior.
  /// @return Const Reference to the last element.
  const_reference back() const {
    const_reference ref = this->tail->value;
    return ref;
  }

  /// ITERATORS

  /// @brief Returns an iterator to the first element of the deque.
  /// If the deque is empty, the returned iterator will be equal to end().
  /// @return Iterator to the first element.
  iterator begin() noexcept {
    if (this->size() == 0) return end();
    Node<iterator>* temp = head;
    return temp;
  }

  /// @brief Returns an iterator to the first element of the deque.
  /// If the deque is empty, the returned iterator will be equal to end().
  /// @return Iterator to the first element.
  const_iterator begin() const noexcept {
    if (this->size() == 0) return end();
    Node<const_iterator>* temp = head;
    return temp;
  }

  /// @brief Same to begin()
  const_iterator cbegin() const noexcept {
    return begin();
  }

  /// @brief Returns an iterator to the element following the last element of
  /// the deque. This element acts as a placeholder; attempting to access it
  /// results in undefined behavior.
  /// @return Iterator to the element following the last element.
  iterator end() noexcept {
    iterator iter = tail;
    return iter->next;
  }

  /// @brief Returns an constant iterator to the element following the last
  /// element of the deque. This element acts as a placeholder; attempting to
  /// access it results in undefined behavior.
  /// @return Constant Iterator to the element following the last element.
  const_iterator end() const noexcept {
    const_iterator iter = tail;
    return iter.data->next;
  }

  /// @brief Same to end()
  const_iterator cend() const noexcept{
    return end();
  }

  /// @brief Returns a reverse iterator to the first element of the reversed
  /// deque. It corresponds to the last element of the non-reversed deque. If
  /// the deque is empty, the returned iterator is equal to rend().
  /// @return Reverse iterator to the first element.
  reverse_iterator rbegin() noexcept {
    if (this->size() == 0) return rend();
    Node<reverse_iterator>* temp = tail;
    return temp;
  }

  /// @brief Returns a const reverse iterator to the first element of the
  /// reversed deque. It corresponds to the last element of the non-reversed
  /// deque. If the deque is empty, the returned iterator is equal to rend().
  /// @return Const Reverse iterator to the first element.
  const_reverse_iterator rbegin() const noexcept {
    if (this->size() == 0) return rend();
    Node<const_reverse_iterator>* temp = tail;
    return temp;
  }

  /// @brief Same to rbegin()
  const_reverse_iterator crbegin() const noexcept {
    return rbegin();
  }

  /// @brief Returns a reverse iterator to the element following the last
  /// element of the reversed deque. It corresponds to the element preceding the
  /// first element of the non-reversed deque. This element acts as a
  /// placeholder, attempting to access it results in undefined behavior.
  /// @return Reverse iterator to the element following the last element.
  reverse_iterator rend() noexcept {
    Node<reverse_iterator>* iter = head;
    return iter->prev;
  }

  /// @brief Returns a const reverse iterator to the element following the last
  /// element of the reversed deque. It corresponds to the element preceding the
  /// first element of the non-reversed deque. This element acts as a
  /// placeholder, attempting to access it results in undefined behavior.
  /// @return Const Reverse iterator to the element following the last element.
  const_reverse_iterator rend() const noexcept {
    Node<const_reverse_iterator>* iter = head;
    return iter->prev;
  }

  /// @brief Same to rend()
  const_reverse_iterator crend() const noexcept {
    return rend();
  }

  /// CAPACITY

  /// @brief Checks if the container has no elements
  /// @return true if the container is empty, false otherwise
  bool empty() const noexcept {
      return size_count == 0;
  }

  /// @brief Returns the number of elements in the container
  /// @return The number of elements in the container.
  size_type size() const noexcept {
      return size_count;
  }

  /// @brief Returns the maximum number of elements the container is able to
  /// hold due to system or library implementation limitations
  /// @return Maximum number of elements.
  size_type max_size() const noexcept {
      return ULLONG_MAX;
  }

  /// @brief Requests the removal of unused capacity.
  /// It is a non-binding request to reduce the memory usage without changing
  /// the size of the sequence. All iterators and references are invalidated.
  /// Past-the-end iterator is also invalidated.
  void shrink_to_fit() {
      Node<T>* iter = head;
      for (size_type count = 0; count < size_count; ++count) {
          iter = iter->next;
      }
      tail = iter;
  }

  /// MODIFIERS

  /// @brief Erases all elements from the container.
  /// nvalidates any references, pointers, or iterators referring to contained
  /// elements. Any past-the-end iterators are also invalidated.
  void clear() noexcept {
      for (auto iter = cbegin(); iter != cend(); iter++) {
          this->erase(iter);
      }
  }

  /// @brief Inserts value before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param value element value to insert
  /// @return Iterator pointing to the inserted value.
  iterator insert(const_iterator pos, const T& value) {
      const_iterator posIter = this->cbegin();
      while (posIter != pos) posIter++;
      Node<const_iterator>* posNode(posIter);
      Node<iterator> insertedNode(value);
      if (posNode->prev != nullptr) {
          Node<const_iterator>* beforeNode = posNode->prev;
          insertedNode.next = posNode;
          insertedNode.prev = beforeNode;
          beforeNode->next = insertedNode;
          posNode->prev = insertedNode;
      }
      else {
          insertedNode.next = posNode;
          insertedNode.prev = nullptr;
          posNode->prev = insertedNode;
      }
      return insertedNode.value;
  }

  /// @brief Inserts value before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param value element value to insert
  /// @return Iterator pointing to the inserted value.
  iterator insert(const_iterator pos, T&& value) {
      const_iterator posIter = this->cbegin();
      while (posIter != pos) posIter++;
      Node<const_iterator>* posNode(posIter);
      Node<iterator> insertedNode(value);
      if (posNode->prev != nullptr) {
          Node<const_iterator>* beforeNode = posNode->prev;
          insertedNode.next = posNode;
          insertedNode.prev = beforeNode;
          beforeNode->next = insertedNode;
          posNode->prev = insertedNode;
      }
      else {
          insertedNode.next = posNode;
          insertedNode.prev = nullptr;
          posNode->prev = insertedNode;
      }
      return insertedNode.value;
  }

  /// @brief Inserts count copies of the value before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param count number of elements to insert
  /// @param value element value to insert
  /// @return Iterator pointing to the first element inserted, or pos if count
  /// == 0.
  iterator insert(const_iterator pos, size_type count, const T& value) {
      const_iterator posIter = this->cbegin();
      while (posIter != pos) posIter++;
      Node<const_iterator>* posNode(posIter);
      Node<iterator> insertedNode(value);
      for (size_type i = 0; i < count; ++i) {
        if (posNode->prev != nullptr) {
            Node<const_iterator>* beforeNode = posNode->prev;
            insertedNode.next = posNode;
            insertedNode.prev = beforeNode;
            beforeNode->next = insertedNode;
            posNode->prev = insertedNode;
        }
        else {
            insertedNode.next = posNode;
            insertedNode.prev = nullptr;
            posNode->prev = insertedNode;
        }
        posNode = posNode->prev;
      }
      return insertedNode.value;
  }

  /// @brief Inserts elements from range [first, last) before pos.
  /// @tparam InputIt Input Iterator
  /// @param pos iterator before which the content will be inserted.
  /// @param first,last the range of elements to insert, can't be iterators into
  /// container for which insert is called
  /// @return Iterator pointing to the first element inserted, or pos if first
  /// == last.
  template <class InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last) {
      auto iter = first;
      while (iter != last) {
          this->insert(pos, *iter);
          iter++;
      }
  }

  /// @brief Inserts elements from initializer list before pos.
  /// @param pos iterator before which the content will be inserted.
  /// @param ilist initializer list to insert the values from
  /// @return Iterator pointing to the first element inserted, or pos if ilist
  /// is empty.
  iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
      for (auto iter = ilist.begin(); iter != ilist.end(); iter++) {
          this->insert(pos, *iter);
      }
  }

  /// @brief Inserts a new element into the container directly before pos.
  /// @param pos iterator before which the new element will be constructed
  /// @param ...args arguments to forward to the constructor of the element
  /// @return terator pointing to the emplaced element.
  template <class... Args>
  iterator emplace(const_iterator pos, Args&&... args);

  /// @brief Removes the element at pos.
  /// @param pos iterator to the element to remove
  /// @return Iterator following the last removed element.
  iterator erase(const_iterator pos) {
      iterator posIter = this->cbegin();
      while (posIter != pos) posIter++;
      Node<iterator>* posNode(posIter);
      if (posNode->prev != nullptr && posNode->next != nullptr) {
          Node<iterator>* beforeNode = posNode->prev;
          Node<iterator>* nextNode = posNode->next;
          beforeNode->next = nextNode;
          nextNode->prev = beforeNode;
          delete posNode;
          return *nextNode->next;
      }
      else if (posNode->prev == nullptr && posNode->next != nullptr) {
          Node<iterator>* nextNode = posNode->next;
          nextNode.prev = nullptr;
          delete posNode;
          return *nextNode->next->value;
      }
      else if (posNode->prev != nullptr && posNode->next == nullptr) {
          Node<iterator>* beforeNode = posNode->prev;
          beforeNode.next = nullptr;
          delete posNode;
          return *beforeNode->next->value;
      }
      else {
          delete posNode;
          return 0;
      }
  }

  /// @brief Removes the elements in the range [first, last).
  /// @param first,last range of elements to remove
  /// @return Iterator following the last removed element.
  iterator erase(const_iterator first, const_iterator last) {
      iterator result;
      for (auto iter = first; iter != last; iter++) {
          result = this->erase(iter);
      }
      return result;
  }

  /// @brief Appends the given element value to the end of the container.
  /// The new element is initialized as a copy of value.
  /// @param value the value of the element to append
  void push_back(const T& value) {
      size_count++;
      if (tail != nullptr) {
          Node<T>* temp = new Node<T>(value);
          temp.prev = tail;
          tail->next = temp;
          tail = temp;
      }
      else {
          tail = new Node<T>(value);
          head = tail;
      }
  }

  /// @brief Appends the given element value to the end of the container.
  /// Value is moved into the new element.
  /// @param value the value of the element to append
  void push_back(T&& value) {
      size_count++;
      if (tail != nullptr) {
          Node<T>* temp = new Node<T>(value);
          temp->prev = tail;
          tail->next = temp;
          tail = temp;
      }
      else {
          tail = new Node<T>(value);
          head = tail;
      }
  }

  /// @brief Appends a new element to the end of the container.
  /// @param ...args arguments to forward to the constructor of the element
  /// @return A reference to the inserted element.
  template <class... Args>
  reference emplace_back(Args&&... args);

  /// @brief Removes the last element of the container.
  void pop_back() {
      if (tail) {
          Node<T>* pointTail = tail;
          tail = pointTail->prev;
          delete pointTail;
          --size_count;
      }
  }

  /// @brief Prepends the given element value to the beginning of the container.
  /// @param value the value of the element to prepend
  void push_front(const T& value) {
      size_count++;
      if (head != nullptr) {
          Node<T>* temp = new Node<T>(value);
          temp->next = head;
          head->prev = temp;
          head = temp;
      }
      else {
          head = new Node<T>(value);
          tail = head;
      }
  }

  /// @brief Prepends the given element value to the beginning of the container.
  /// @param value moved value of the element to prepend
  void push_front(T&& value) {
      size_count++;
      if (head != nullptr) {
          Node<T>* temp = new Node<T>(value);
          temp->next = head;
          head->prev = temp;
          head = temp;
      }
      else {
          head = new Node<T>(value);
          tail = head;
      }
  }

  /// @brief Inserts a new element to the beginning of the container.
  /// @param ...args arguments to forward to the constructor of the element
  /// @return A reference to the inserted element.
  template <class... Args>
  reference emplace_front(Args&&... args);

  /// @brief Removes the first element of the container.
  void pop_front() {
      if (head) {
          Node<T>* pointHead = head;
          head = head->next;
          delete pointHead;
          --size_count;
      }
  }

  /// @brief Resizes the container to contain count elements.
  /// If the current size is greater than count, the container is reduced to its
  /// first count elements. If the current size is less than count, additional
  /// default-inserted elements are appended
  /// @param count new size of the container
  void resize(size_type count) {
      if (size_count == count) return;
      if (size_count > count) {
          while (size_count != count) pop_back();
      }
      else {
          while (size_count != count) push_back(T{});
      }
  }

  /// @brief Resizes the container to contain count elements.
  /// If the current size is greater than count, the container is reduced to its
  /// first count elements. If the current size is less than count, additional
  /// copies of value are appended.
  /// @param count new size of the container
  /// @param value the value to initialize the new elements with
  void resize(size_type count, const value_type& value) {
      if (size_count == count) return;
      if (size_count > count) {
          while (size_count != count) pop_back();
      }
      else {
          while (size_count != count) push_back(value);
      }
  }

  /// @brief Exchanges the contents of the container with those of other.
  /// Does not invoke any move, copy, or swap operations on individual elements.
  /// All iterators and references remain valid. The past-the-end iterator is
  /// invalidated.
  /// @param other container to exchange the contents with
  void swap(Deque& other) {
      std::swap(this->head, other.head);
      std::swap(this->tail, other.tail);
      std::swap(this->size_count, other.size_count);
  }

  /// COMPARISIONS

  /// @brief Checks if the contents of lhs and rhs are equal
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator==(const Deque<U, Alloc>& lhs,
      const Deque<U, Alloc>& rhs) {
      if (lhs.size_count != rhs.size_count) return false;
      for (auto liter = lhs.begin(), riter = rhs.begin(); liter != lhs.end() && riter != lhs.end(); liter++, riter++) {
          if (*liter != *riter) return false;
      }
      return true;
  }

  /// @brief Checks if the contents of lhs and rhs are not equal
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator!=(const Deque<U, Alloc>& lhs,
      const Deque<U, Alloc>& rhs) {
      if (lhs.size_count != rhs.size_count) return true;
      for (auto liter = lhs.begin(), riter = rhs.begin(); liter != lhs.end() && riter != lhs.end(); liter++, riter++) {
          if (*liter != *riter) return true;
      }
      return false;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator>(const Deque<U, Alloc>& lhs, const Deque<U, Alloc>& rhs) {
      if (lhs.size_count < rhs.size_count) return false;
      for (auto liter = lhs.begin(), riter = rhs.begin(); liter != lhs.end() && riter != lhs.end(); liter++, riter++) {
          if (*liter <= *riter) return false;
      }
      return true;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator<(const Deque<U, Alloc>& lhs, const Deque<U, Alloc>& rhs) {
      if (lhs.size_count > rhs.size_count) return false;
      for (auto liter = lhs.begin(), riter = rhs.begin(); liter != lhs.end() && riter != lhs.end(); liter++, riter++) {
          if (*liter >= *riter) return false;
      }
      return true;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator>=(const Deque<U, Alloc>& lhs,
      const Deque<U, Alloc>& rhs) {
      if (lhs.size_count < rhs.size_count) return false;
      for (auto liter = lhs.begin(), riter = rhs.begin(); liter != lhs.end() && riter != lhs.end(); liter++, riter++) {
          if (*liter < *riter) return false;
      }
      return true;
  }

  /// @brief Compares the contents of lhs and rhs lexicographically.
  /// @param lhs,rhs deques whose contents to compare
  template <class U, class Alloc>
  friend bool operator<=(const Deque<U, Alloc>& lhs,
      const Deque<U, Alloc>& rhs) {
      if (lhs.size_count > rhs.size_count) return false;
      for (auto liter = lhs.begin(), riter = rhs.begin(); liter != lhs.end() && riter != lhs.end(); liter++, riter++) {
          if (*liter > *riter) return false;
      }
      return true;
  }

  // operator <=> will be handy
  private:
      Node<T>* head;
      Node<T>* tail;
      size_type size_count = 0;
      allocator_type alloc;
};

/// NON-MEMBER FUNCTIONS

/// @brief  Swaps the contents of lhs and rhs.
/// @param lhs,rhs containers whose contents to swap
template <class T, class Alloc>
void swap(Deque<T, Alloc>& lhs, Deque<T, Alloc>& rhs) {
    std::swap(lhs.begin(), rhs.begin());
    std::swap(lhs.end(), rhs.end());
    std::swap(lhs.size(), rhs.size());
}

/// @brief Erases all elements that compare equal to value from the container.
/// @param c container from which to erase
/// @param value value to be removed
/// @return The number of erased elements.
template <class T, class Alloc, class U>
typename Deque<T, Alloc>::size_type erase(Deque<T, Alloc>& c, const U& value) {
    size_t count = 0;
    for (auto iter = c.begin(); iter != c.end(); iter++) {
        if (*iter == value) {
            c.erase(value);
            count++;
        }
    }
    return count;
}

/// @brief Erases all elements that compare equal to value from the container.
/// @param c container from which to erase
/// @param pred unary predicate which returns ​true if the element should be
/// erased.
/// @return The number of erased elements.
template <class T, class Alloc, class Pred>
typename Deque<T, Alloc>::size_type erase_if(Deque<T, Alloc>& c, Pred pred) {
    size_t count = 0;
    for (auto iter = c.begin(); iter != c.end(); iter++) {
        if (pred(*iter) == true) {
            c.erase(*iter);
            count++;
        }
    }
    return count;
}

}  // namespace fefu_laboratory_two