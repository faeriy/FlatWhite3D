# Детальне пояснення Event.h

## 1. Enum (Переліки)

### `enum class EventType`
```cpp
enum class EventType {
    None = 0,
    WindowClose, WindowResize, ...
};
```

**Що це таке:**
- `enum class` - це типобезпечний перелік (C++11)
- Кожне значення - це окремий тип, який не можна автоматично перетворити на int
- `None = 0` - явно встановлює значення 0
- Наступні значення автоматично отримують 1, 2, 3, 4...

**Приклад використання:**
```cpp
EventType type = EventType::WindowClose;
// type має значення 1
```

### `enum EventCategory`
```cpp
enum EventCategory {
    None = 0,
    EventCategoryApplication = BIT(0),  // = 1 (біт 0)
    EventCategoryInput = BIT(1),        // = 2 (біт 1)
    EventCategoryKeyboard = BIT(2),     // = 4 (біт 2)
    EventCategoryMouse = BIT(3),        // = 8 (біт 3)
    EventCategoryMouseButton = BIT(4)   // = 16 (біт 4)
};
```

**Що таке BIT(n):**
- `BIT(n)` зазвичай визначається як `(1 << n)` (зсув вліво на n бітів)
- Це створює значення з одним встановленим бітом:
  - `BIT(0)` = `1` = `0001` (бінарно)
  - `BIT(1)` = `2` = `0010` (бінарно)
  - `BIT(2)` = `4` = `0100` (бінарно)
  - `BIT(3)` = `8` = `1000` (бінарно)

**Навіщо це потрібно:**
- Можна комбінувати кілька категорій в одне число через оператор `|` (OR):
  ```cpp
  int flags = EventCategoryKeyboard | EventCategoryMouse;
  // flags = 4 | 8 = 12 (бінарно: 1100)
  ```
- Можна перевіряти наявність категорії через оператор `&` (AND):
  ```cpp
  if (flags & EventCategoryKeyboard) {
      // категорія Keyboard присутня
  }
  ```

---

## 2. Макроси #define

### `EVENT_CLASS_TYPE(type)`
```cpp
#define EVENT_CLASS_TYPE(type) \
    static EventType GetStaticType() { return EventType::##type; }\
    virtual EventType GetEventType() const override { return GetStaticType(); }\
    virtual const char* GetName() const override { return #type; }
```

**Детальний розбір:**

1. **`##` (Token Pasting Operator)**
   - Склеює два токени в один
   - `EventType::##type` стає `EventType::KeyPressed`, якщо `type = KeyPressed`
   - Приклад: `EVENT_CLASS_TYPE(KeyPressed)` → `EventType::KeyPressed`

2. **`#type` (Stringizing Operator)**
   - Перетворює ім'я параметра в рядок
   - `#type` стає `"KeyPressed"`, якщо `type = KeyPressed`
   - Приклад: `EVENT_CLASS_TYPE(KeyPressed)` → `"KeyPressed"`

3. **`\` (Line Continuation)**
   - Продовжує макрос на наступному рядку
   - Дозволяє писати багаторядкові макроси

4. **`static` метод**
   - Викликається без створення об'єкта: `KeyPressedEvent::GetStaticType()`
   - Використовується для перевірки типу на етапі компіляції

5. **`virtual ... override`**
   - Перевизначає віртуальну функцію з базового класу `Event`
   - `override` гарантує, що така функція існує в базовому класі

**Приклад використання:**
```cpp
class KeyPressedEvent : public Event {
public:
    EVENT_CLASS_TYPE(KeyPressed)  // Розгортається в:
    // static EventType GetStaticType() { return EventType::KeyPressed; }
    // virtual EventType GetEventType() const override { return GetStaticType(); }
    // virtual const char* GetName() const override { return "KeyPressed"; }
};
```

### `EVENT_CLASS_CATEGORY(category)`
```cpp
#define EVENT_CLASS_CATEGORY(category) \
    virtual int GetCategoryFlags() const override { return category; }
```

**Що робить:**
- Створює метод, який повертає категорію події
- `category` - це комбінація значень з `EventCategory` через `|`

**Приклад:**
```cpp
class KeyPressedEvent : public Event {
    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    // Створює метод, який повертає (EventCategoryKeyboard | EventCategoryInput)
};
```

---

## 3. Friend клас

```cpp
class Event {
    friend class EventDispatcher;
    // ...
protected:
    bool m_Handled = false;
};
```

**Що таке `friend`:**
- `friend` дає доступ до **приватних** і **захищених** членів класу
- `EventDispatcher` може читати і змінювати `m_Handled`, хоча він `protected`
- Це **односторонній доступ**: `EventDispatcher` бачить приватні поля `Event`, але не навпаки

**Навіщо це потрібно:**
- У рядку 70: `m_Event.m_Handled = func(*(T*)&m_Event);`
- `EventDispatcher` змінює `m_Handled` після обробки події
- Без `friend` це було б неможливо, бо `m_Handled` - захищений член

**Альтернативи:**
- Зробити `m_Handled` публічним (погіршує інкапсуляцію)
- Додати публічний метод `SetHandled()` (зайвий код)

---

## 4. Віртуальні функції (рядки 42-45)

```cpp
virtual EventType GetEventType() const = 0;
virtual const char* GetName() const = 0;
virtual int GetCategoryFlags() const = 0;
virtual std::string ToString() const { return GetName(); }
```

### `virtual`
- Дозволяє **поліморфізм** - виклик методу похідного класу через вказівник на базовий
- Приклад:
  ```cpp
  Event* e = new KeyPressedEvent();
  e->GetEventType();  // Викликає KeyPressedEvent::GetEventType()
  ```

### `= 0` (чисто віртуальна функція)
- Робить функцію **абстрактною** (не має реалізації)
- Робить клас **абстрактним** - його не можна створити напряму
- Похідні класи **обов'язково** повинні реалізувати цю функцію

### `const`
- Функція **не змінює** стан об'єкта
- Можна викликати на `const` об'єктах

### `override`
- Явно вказує, що функція **перевизначає** віртуальну функцію з базового класу
- Компілятор перевірить, що така функція існує в базовому класі
- Запобігає помилкам (наприклад, опечатка в імені)

### Різниця між `= 0` і реалізацією
- Перші три функції (`= 0`) - **обов'язкові** для реалізації в похідних класах
- `ToString()` має **реалізацію за замовчуванням**, але можна перевизначити

---

## 5. Шаблони (Templates) - рядки 57-58

```cpp
template<typename T>
using EventFn = std::function<bool(T&)>;
```

### `template<typename T>`
- Створює **параметризований тип**
- `T` - це тип-параметр, який буде підставлений під час використання
- Приклад: `EventFn<KeyPressedEvent>` - `T` стає `KeyPressedEvent`

### `using` (Type Alias)
- Створює **псевдонім** для типу (аналог `typedef`, але краще)
- `EventFn<T>` - це те саме, що `std::function<bool(T&)>`
- Просто коротший спосіб запису

### `std::function<bool(T&)>`
- **Функціональний об'єкт** з стандартної бібліотеки
- Може зберігати будь-яку функцію, лямбду, функтор, які:
  - Приймають `T&` (посилання на тип T)
  - Повертають `bool`

**Приклади використання:**
```cpp
// Звичайна функція
bool HandleKeyPress(KeyPressedEvent& e) {
    return true;
}
EventFn<KeyPressedEvent> handler1 = HandleKeyPress;

// Лямбда-функція
EventFn<KeyPressedEvent> handler2 = [](KeyPressedEvent& e) {
    std::cout << "Key pressed!" << std::endl;
    return true;
};

// Виклик
KeyPressedEvent event;
bool handled = handler2(event);  // викликає лямбду
```

**Навіщо це потрібно:**
- У `EventDispatcher::Dispatch()` (рядок 66) використовується для передачі обробника події
- Дозволяє передавати різні типи функцій (звичайні, лямбди, функтори) як один тип

---

## 6. Inline оператор << (рядки 79-83)

```cpp
inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    return os << e.ToString();
}
```

### `operator<<` (Перевантаження оператора)
- Перевантажує оператор `<<` для виводу в потік
- Дозволяє використовувати `std::cout << event` замість `std::cout << event.ToString()`

**Як працює:**
```cpp
Event* e = new KeyPressedEvent();
std::cout << *e;  // Викликає operator<<, який викликає e.ToString()
// Виведе: "KeyPressed"
```

### `inline`
- **Підказка компілятору** вставити код функції безпосередньо в місце виклику
- Може покращити продуктивність для маленьких функцій
- Компілятор може проігнорувати `inline`, якщо вважає за потрібне

### `std::ostream&` (повернення)
- Повертає посилання на потік
- Дозволяє **ланцюжок** викликів:
  ```cpp
  std::cout << event1 << " " << event2;
  // Спочатку викликається operator<< для event1,
  // потім для " ", потім для event2
  ```

### Чому функція в namespace, а не в класі?
- Оператори `<<` зазвичай визначаються **поза класом**
- Якщо б вона була в класі, синтаксис був би: `event << std::cout` (незручно)
- Поза класом: `std::cout << event` (природно)

---

## Загальна архітектура

**Як все працює разом:**

1. **Базовий клас `Event`** - абстрактний клас з віртуальними функціями
2. **Похідні класи** (наприклад, `KeyPressedEvent`) реалізують ці функції через макроси
3. **`EventDispatcher`** - диспетчер, який перевіряє тип події і викликає відповідний обробник
4. **Обробники** - функції типу `EventFn<T>`, які обробляють конкретні типи подій

**Приклад повного циклу:**
```cpp
// Створюємо подію
KeyPressedEvent event;

// Створюємо диспетчер
EventDispatcher dispatcher(event);

// Реєструємо обробник
dispatcher.Dispatch<KeyPressedEvent>([](KeyPressedEvent& e) {
    std::cout << "Key was pressed!" << std::endl;
    return true;  // подія оброблена
});

// Виводимо подію
std::cout << event << std::endl;  // "KeyPressed"
```

---

## Корисні посилання

- [C++ enum class](https://en.cppreference.com/w/cpp/language/enum)
- [C++ templates](https://en.cppreference.com/w/cpp/language/templates)
- [C++ virtual functions](https://en.cppreference.com/w/cpp/language/virtual)
- [C++ friend](https://en.cppreference.com/w/cpp/language/friend)
- [C++ operator overloading](https://en.cppreference.com/w/cpp/language/operators)

