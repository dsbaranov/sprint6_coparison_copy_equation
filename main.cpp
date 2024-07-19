#include <algorithm>
#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

template <typename Type>
class SingleLinkedList {
  // Узел списка
  struct Node {
    Node() = default;
    Node(const Type &val, Node *next) : value(val), next_node(next) {}
    Type value;
    Node *next_node = nullptr;
  };

  template <typename ValueType>
  class BasicIterator {
    friend class SingleLinkedList;
    // Конвертирующий конструктор итератора из указателя на узел списка
    explicit BasicIterator(Node *node) : node_(node) {}

   public:
    // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого
    // итератора

    // Категория итератора — forward iterator
    // (итератор, который поддерживает операции инкремента и многократное
    // разыменование)
    using iterator_category = std::forward_iterator_tag;
    // Тип элементов, по которым перемещается итератор
    using value_type = Type;
    // Тип, используемый для хранения смещения между итераторами
    using difference_type = std::ptrdiff_t;
    // Тип указателя на итерируемое значение
    using pointer = ValueType *;
    // Тип ссылки на итерируемое значение
    using reference = ValueType &;

    BasicIterator() = default;

    // Конвертирующий конструктор/конструктор копирования
    // При ValueType, совпадающем с Type, играет роль копирующего конструктора
    // При ValueType, совпадающем с const Type, играет роль конвертирующего
    // конструктора
    BasicIterator(const BasicIterator<Type> &other) noexcept {
      node_ = other.node_;
    }

    // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при
    // наличии пользовательского конструктора копирования, явно объявим оператор
    // = и попросим компилятор сгенерировать его за нас
    BasicIterator &operator=(const BasicIterator &rhs) = default;

    // Оператор сравнения итераторов (в роли второго аргумента выступает
    // константный итератор) Два итератора равны, если они ссылаются на один и
    // тот же элемент списка либо на end()
    [[nodiscard]] bool operator==(
        const BasicIterator<const Type> &rhs) const noexcept {
      return (node_ == rhs.node_);
    }

    // Оператор проверки итераторов на неравенство
    // Противоположен !=
    [[nodiscard]] bool operator!=(
        const BasicIterator<const Type> &rhs) const noexcept {
      return (node_ != rhs.node_);
    }

    // Оператор сравнения итераторов (в роли второго аргумента итератор)
    // Два итератора равны, если они ссылаются на один и тот же элемент списка
    // либо на end()
    [[nodiscard]] bool operator==(
        const BasicIterator<Type> &rhs) const noexcept {
      return (node_ == rhs.node_);
    }

    // Оператор проверки итераторов на неравенство
    // Противоположен !=
    [[nodiscard]] bool operator!=(
        const BasicIterator<Type> &rhs) const noexcept {
      return node_ != rhs.node_;
    }

    // Оператор прединкремента. После его вызова итератор указывает на следующий
    // элемент списка Возвращает ссылку на самого себя Инкремент итератора, не
    // указывающего на существующий элемент списка, приводит к неопределённому
    // поведению
    BasicIterator &operator++() noexcept {
      node_ = node_->next_node;
      return *this;
    }

    // Оператор постинкремента. После его вызова итератор указывает на следующий
    // элемент списка Возвращает прежнее значение итератора Инкремент итератора,
    // не указывающего на существующий элемент списка, приводит к
    // неопределённому поведению
    BasicIterator operator++(int) noexcept {
      BasicIterator this_prev(node_);
      node_ = node_->next_node;
      return this_prev;
    }

    // Операция разыменования. Возвращает ссылку на текущий элемент
    // Вызов этого оператора у итератора, не указывающего на существующий
    // элемент списка, приводит к неопределённому поведению
    [[nodiscard]] reference operator*() const noexcept { return node_->value; }

    // Операция доступа к члену класса. Возвращает указатель на текущий элемент
    // списка Вызов этого оператора у итератора, не указывающего на существующий
    // элемент списка, приводит к неопределённому поведению
    [[nodiscard]] pointer operator->() const noexcept { return &node_->value; }

   private:
    Node *node_ = nullptr;
  };

 public:
  using value_type = Type;
  using reference = value_type &;
  using const_reference = const value_type &;
  // Итератор, допускающий изменение элементов списка
  using Iterator = BasicIterator<Type>;
  // Константный итератор, предоставляющий доступ для чтения к элементам списка
  using ConstIterator = BasicIterator<const Type>;

  // Возвращает итератор, ссылающийся на первый элемент
  // Если список пустой, возвращённый итератор будет равен end()
  [[nodiscard]] Iterator begin() noexcept { return Iterator(head_.next_node); }
  // Возвращает итератор, указывающий на позицию, следующую за последним
  // элементом односвязного списка Разыменовывать этот итератор нельзя — попытка
  // разыменования приведёт к неопределённому поведению
  [[nodiscard]] Iterator end() noexcept {
    Iterator it(&head_);
    while (it.node_->next_node != nullptr) {
      ++it;
    }
    return ++it;
  }

  // Возвращает константный итератор, ссылающийся на первый элемент
  // Если список пустой, возвращённый итератор будет равен end()
  // Результат вызова эквивалентен вызову метода cbegin()
  [[nodiscard]] ConstIterator begin() const noexcept {
    return ConstIterator(head_.next_node);
  }

  // Возвращает константный итератор, указывающий на позицию, следующую за
  // последним элементом односвязного списка Разыменовывать этот итератор нельзя
  // — попытка разыменования приведёт к неопределённому поведению Результат
  // вызова эквивалентен вызову метода cend()
  [[nodiscard]] ConstIterator end() const noexcept {
    return ConstIterator(nullptr);
  }

  // Возвращает константный итератор, ссылающийся на первый элемент
  // Если список пустой, возвращённый итератор будет равен cend()
  [[nodiscard]] ConstIterator cbegin() const noexcept {
    return ConstIterator(head_.next_node);
  }

  // Возвращает константный итератор, указывающий на позицию, следующую за
  // последним элементом односвязного списка Разыменовывать этот итератор нельзя
  // — попытка разыменования приведёт к неопределённому поведению
  [[nodiscard]] ConstIterator cend() const noexcept { return this->end(); }

  SingleLinkedList() {
    head_.next_node = nullptr;
    size_ = 0;
  }

  // Возвращает количество элементов в списке
  [[nodiscard]] size_t GetSize() const noexcept { return size_; }

  // Сообщает, пустой ли список
  [[nodiscard]] bool IsEmpty() const noexcept {
    return head_.next_node == nullptr;
  }

  // Вставляет элемент value в начало списка за время O(1)
  void PushFront(const Type &value) {
    head_.next_node = new Node(value, head_.next_node);
    size_++;
  }

  // Очищает список за время O(N)
  void Clear() noexcept {
    while (head_.next_node != nullptr) {
      Node *target = head_.next_node;
      head_.next_node = target->next_node;
      delete target;
      size_--;
    }
  }

  ~SingleLinkedList() { Clear(); }

  SingleLinkedList(std::initializer_list<Type> values) {
    size_ = 0;
    auto it = values.end();
    do {
      it--;
      head_.next_node = new Node(*it, head_.next_node);
      size_++;

    } while (it != values.begin());
  }

  SingleLinkedList(const SingleLinkedList &other) {
    size_ = 0;
    try {
      SingleLinkedList tmp;
      auto other_it = other.begin();
      while (other_it != other.end()) {
        tmp.PushFront(*other_it);
        ++other_it;
      }
      auto tmp_it = tmp.begin();
      while (tmp_it != tmp.end()) {
        PushFront(*tmp_it);
        ++tmp_it;
      }
    } catch (const std::exception &e) {
            Clear();
      throw;
    }
  }

  void swap(SingleLinkedList &other) noexcept {
    std::swap(head_.next_node, other.head_.next_node);
    std::swap(size_, other.size_);
  }

  SingleLinkedList &operator=(const SingleLinkedList &rhs) {
    try {
      SingleLinkedList tmp(rhs);
      swap(tmp);
    } catch (const std::exception &e) {
      throw;
    }

    return *this;
  }

 private:
  // Фиктивный узел, используется для вставки "перед первым элементом"
  Node head_;
  size_t size_;
};

template <typename Type>
void swap(SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept {
  lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {
  bool result = true;
  if (lhs.GetSize() != rhs.GetSize()) {
    return false;
  }
  auto lhs_it = lhs.begin();
  auto rhs_it = rhs.begin();
  while (lhs_it != lhs.end()) {
    if (*lhs_it != *rhs_it) {
      result = false;
      break;
    }
    ++lhs_it;
    ++rhs_it;
  }
  return result;
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {
  return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type> &lhs,
               const SingleLinkedList<Type> &rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {
  return (lhs == rhs || lhs < rhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type> &lhs,
               const SingleLinkedList<Type> &rhs) {
  return !(lhs <= rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs,
                const SingleLinkedList<Type> &rhs) {
  return (lhs > rhs || lhs == rhs);
}

void Test0() {
  using namespace std;
  {
    const SingleLinkedList<int> empty_int_list;
    assert(empty_int_list.GetSize() == 0u);
    assert(empty_int_list.IsEmpty());
  }

  {
    const SingleLinkedList<string> empty_string_list;
    assert(empty_string_list.GetSize() == 0u);
    assert(empty_string_list.IsEmpty());
  }
}
void Test1() {
  // Шпион, следящий за своим удалением
  struct DeletionSpy {
    DeletionSpy() = default;
    explicit DeletionSpy(int &instance_counter) noexcept
        : instance_counter_ptr_(&instance_counter)  //
    {
      OnAddInstance();
    }
    DeletionSpy(const DeletionSpy &other) noexcept
        : instance_counter_ptr_(other.instance_counter_ptr_)  //
    {
      OnAddInstance();
    }
    DeletionSpy &operator=(const DeletionSpy &rhs) noexcept {
      if (this != &rhs) {
        auto rhs_copy(rhs);
        std::swap(instance_counter_ptr_, rhs_copy.instance_counter_ptr_);
      }
      return *this;
    }
    ~DeletionSpy() { OnDeleteInstance(); }

   private:
    void OnAddInstance() noexcept {
      if (instance_counter_ptr_) {
        ++(*instance_counter_ptr_);
      }
    }
    void OnDeleteInstance() noexcept {
      if (instance_counter_ptr_) {
        assert(*instance_counter_ptr_ != 0);
        --(*instance_counter_ptr_);
      }
    }

    int *instance_counter_ptr_ = nullptr;
  };

  // Проверка вставки в начало
  {
    SingleLinkedList<int> l;
    assert(l.IsEmpty());
    assert(l.GetSize() == 0u);

    l.PushFront(0);
    l.PushFront(1);
    assert(l.GetSize() == 2);
    assert(!l.IsEmpty());

    l.Clear();
    assert(l.GetSize() == 0);
    assert(l.IsEmpty());
  }

  // Проверка фактического удаления элементов
  {
    int item0_counter = 0;
    int item1_counter = 0;
    int item2_counter = 0;
    {
      SingleLinkedList<DeletionSpy> list;
      list.PushFront(DeletionSpy{item0_counter});
      list.PushFront(DeletionSpy{item1_counter});
      list.PushFront(DeletionSpy{item2_counter});

      assert(item0_counter == 1);
      assert(item1_counter == 1);
      assert(item2_counter == 1);
      list.Clear();
      assert(item0_counter == 0);
      assert(item1_counter == 0);
      assert(item2_counter == 0);

      list.PushFront(DeletionSpy{item0_counter});
      list.PushFront(DeletionSpy{item1_counter});
      list.PushFront(DeletionSpy{item2_counter});
      assert(item0_counter == 1);
      assert(item1_counter == 1);
      assert(item2_counter == 1);
    }
    assert(item0_counter == 0);
    assert(item1_counter == 0);
    assert(item2_counter == 0);
  }

  // Вспомогательный класс, бросающий исключение после создания N-копии
  struct ThrowOnCopy {
    ThrowOnCopy() = default;
    explicit ThrowOnCopy(int &copy_counter) noexcept
        : countdown_ptr(&copy_counter) {}
    ThrowOnCopy(const ThrowOnCopy &other)
        : countdown_ptr(other.countdown_ptr)  //
    {
      if (countdown_ptr) {
        if (*countdown_ptr == 0) {
          throw std::bad_alloc();
        } else {
          --(*countdown_ptr);
        }
      }
    }
    // Присваивание элементов этого типа не требуется
    ThrowOnCopy &operator=(const ThrowOnCopy &rhs) = delete;
    // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается
    // при каждом копировании. Как только обнулится, конструктор копирования
    // выбросит исключение
    int *countdown_ptr = nullptr;
  };

  {
    bool exception_was_thrown = false;
    // Последовательно уменьшаем счётчик копирований до нуля, пока не будет
    // выброшено исключение
    for (int max_copy_counter = 5; max_copy_counter >= 0; --max_copy_counter) {
      // Создаём непустой список
      SingleLinkedList<ThrowOnCopy> list;
      list.PushFront(ThrowOnCopy{});
      try {
        int copy_counter = max_copy_counter;
        list.PushFront(ThrowOnCopy(copy_counter));
        // Если метод не выбросил исключение, список должен перейти в новое
        // состояние
        assert(list.GetSize() == 2);
      } catch (const std::bad_alloc &) {
        exception_was_thrown = true;
        // После выбрасывания исключения состояние списка должно остаться
        // прежним
        assert(list.GetSize() == 1);
        break;
      }
    }
    assert(exception_was_thrown);
  }
}
void Test2() {
  // Итерирование по пустому списку
  {
    SingleLinkedList<int> list;
    // Константная ссылка для доступа к константным версиям begin()/end()
    const auto &const_list = list;

    // Итераторы begin и end у пустого диапазона равны друг другу
    assert(list.begin() == list.end());
    assert(const_list.begin() == const_list.end());
    assert(list.cbegin() == list.cend());
    assert(list.cbegin() == const_list.begin());
    assert(list.cend() == const_list.end());
  }

  // Итерирование по непустому списку
  {
    SingleLinkedList<int> list;
    const auto &const_list = list;

    list.PushFront(1);
    assert(list.GetSize() == 1u);
    assert(!list.IsEmpty());

    assert(const_list.begin() != const_list.end());
    assert(const_list.cbegin() != const_list.cend());
    assert(list.begin() != list.end());

    assert(const_list.begin() == const_list.cbegin());

    assert(*list.cbegin() == 1);
    *list.begin() = -1;
    assert(*list.cbegin() == -1);

    const auto old_begin = list.cbegin();
    list.PushFront(2);
    assert(list.GetSize() == 2);

    const auto new_begin = list.cbegin();
    assert(new_begin != old_begin);
    // Проверка прединкремента
    {
      auto new_begin_copy(new_begin);
      assert((++(new_begin_copy)) == old_begin);
    }
    // Проверка постинкремента
    {
      auto new_begin_copy(new_begin);
      assert(((new_begin_copy)++) == new_begin);
      assert(new_begin_copy == old_begin);
    }
    // Итератор, указывающий на позицию после последнего элемента, равен
    // итератору end()
    {
      auto old_begin_copy(old_begin);
      assert((++old_begin_copy) == list.end());
    }
  }
  // Преобразование итераторов
  {
    SingleLinkedList<int> list;
    list.PushFront(1);
    // Конструирование ConstIterator из Iterator
    SingleLinkedList<int>::ConstIterator const_it(list.begin());
    assert(const_it == list.cbegin());
    assert(*const_it == *list.cbegin());

    SingleLinkedList<int>::ConstIterator const_it1;
    // Присваивание ConstIterator'у значения Iterator
    const_it1 = list.begin();
    assert(const_it1 == const_it);
  }
  // Проверка оператора ->
  {
    using namespace std;
    SingleLinkedList<std::string> string_list;

    string_list.PushFront("one"s);
    assert(string_list.cbegin()->length() == 3u);
    string_list.begin()->push_back('!');
    assert(*string_list.begin() == "one!"s);
  }
}
void Test3() {
  // Проверка списков на равенство и неравенство
  {
    SingleLinkedList<int> list_1;
    list_1.PushFront(1);
    list_1.PushFront(2);

    SingleLinkedList<int> list_2;
    list_2.PushFront(1);
    list_2.PushFront(2);
    list_2.PushFront(3);

    SingleLinkedList<int> list_1_copy;
    list_1_copy.PushFront(1);
    list_1_copy.PushFront(2);

    SingleLinkedList<int> empty_list;
    SingleLinkedList<int> another_empty_list;

    // Список равен самому себе
    assert(list_1 == list_1);
    assert(empty_list == empty_list);

    // Списки с одинаковым содержимым равны, а с разным - не равны
    assert(list_1 == list_1_copy);
    assert(list_1 != list_2);
    assert(list_2 != list_1);
    assert(empty_list == another_empty_list);
  }

  // Обмен содержимого списков
  {
    SingleLinkedList<int> first;
    first.PushFront(1);
    first.PushFront(2);

    SingleLinkedList<int> second;
    second.PushFront(10);
    second.PushFront(11);
    second.PushFront(15);

    const auto old_first_begin = first.begin();
    const auto old_second_begin = second.begin();
    const auto old_first_size = first.GetSize();
    const auto old_second_size = second.GetSize();

    first.swap(second);

    assert(second.begin() == old_first_begin);
    assert(first.begin() == old_second_begin);
    assert(second.GetSize() == old_first_size);
    assert(first.GetSize() == old_second_size);

    // Обмен при помощи функции swap
    {
      using std::swap;

      // В отсутствие пользовательской перегрузки будет вызвана функция
      // std::swap, которая выполнит обмен через создание временной копии
      swap(first, second);

      // Убеждаемся, что используется не std::swap, а пользовательская
      // перегрузка

      // Если бы обмен был выполнен с созданием временной копии,
      // то итератор first.begin() не будет равен ранее сохранённому значению,
      // так как копия будет хранить свои узлы по иным адресам
      assert(first.begin() == old_first_begin);
      assert(second.begin() == old_second_begin);
      assert(first.GetSize() == old_first_size);
      assert(second.GetSize() == old_second_size);
    }
  }

  // Инициализация списка при помощи std::initializer_list
  {
    SingleLinkedList<int> list{1, 2, 3, 4, 5};
    assert(list.GetSize() == 5);
    assert(!list.IsEmpty());
    assert(std::equal(list.begin(), list.end(), std::begin({1, 2, 3, 4, 5})));
  }

  // Лексикографическое сравнение списков
  {
    using IntList = SingleLinkedList<int>;

    assert((IntList{1, 2, 3} < IntList{1, 2, 3, 1}));
    assert((IntList{1, 2, 3} <= IntList{1, 2, 3}));
    assert((IntList{1, 2, 4} > IntList{1, 2, 3}));
    assert((IntList{1, 2, 3} >= IntList{1, 2, 3}));
  }

  // Копирование списков
  {
    const SingleLinkedList<int> empty_list{};
    // Копирование пустого списка
    {
      auto list_copy(empty_list);
      assert(list_copy.IsEmpty());
    }

    SingleLinkedList<int> non_empty_list{1, 2, 3, 4};
    // Копирование непустого списка
    {
      auto list_copy(non_empty_list);

      assert(non_empty_list.begin() != list_copy.begin());
      assert(list_copy == non_empty_list);
    }
  }

  // Присваивание списков
  {
    const SingleLinkedList<int> source_list{1, 2, 3, 4};

    SingleLinkedList<int> receiver{5, 4, 3, 2, 1};
    receiver = source_list;
    assert(receiver.begin() != source_list.begin());
    assert(receiver == source_list);
  }

  // Вспомогательный класс, бросающий исключение после создания N-копии
  struct ThrowOnCopy {
    ThrowOnCopy() = default;
    explicit ThrowOnCopy(int &copy_counter) noexcept
        : countdown_ptr(&copy_counter) {}
    ThrowOnCopy(const ThrowOnCopy &other)
        : countdown_ptr(other.countdown_ptr)  //
    {
      if (countdown_ptr) {
        if (*countdown_ptr == 0) {
          throw std::bad_alloc();
        } else {
          --(*countdown_ptr);
        }
      }
    }
    // Присваивание элементов этого типа не требуется
    ThrowOnCopy &operator=(const ThrowOnCopy &rhs) = delete;
    // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается
    // при каждом копировании. Как только обнулится, конструктор копирования
    // выбросит исключение
    int *countdown_ptr = nullptr;
  };

  // Безопасное присваивание списков
  {
    SingleLinkedList<ThrowOnCopy> src_list;
    src_list.PushFront(ThrowOnCopy{});
    src_list.PushFront(ThrowOnCopy{});
    auto thrower = src_list.begin();
    src_list.PushFront(ThrowOnCopy{});

    int copy_counter =
        0;  // при первом же копировании будет выброшено исключение
    thrower->countdown_ptr = &copy_counter;

    SingleLinkedList<ThrowOnCopy> dst_list;
    dst_list.PushFront(ThrowOnCopy{});
    int dst_counter = 10;
    dst_list.begin()->countdown_ptr = &dst_counter;
    dst_list.PushFront(ThrowOnCopy{});

    try {
      dst_list = src_list;
      // Ожидается исключение при присваивании
      assert(false);
    } catch (const std::bad_alloc &) {
      // Проверяем, что состояние списка-приёмника не изменилось
      // при выбрасывании исключений
      assert(dst_list.GetSize() == 2);
      auto it = dst_list.begin();
      assert(it != dst_list.end());
      assert(it->countdown_ptr == nullptr);
      ++it;
      assert(it != dst_list.end());
      assert(it->countdown_ptr == &dst_counter);
      assert(dst_counter == 10);
    } catch (...) {
      // Других типов исключений не ожидается
      assert(false);
    }
  }
}

int main() {
  Test0();
  Test1();
  Test2();
  Test3();
}
