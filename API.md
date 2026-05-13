# Описание API и классов
Все в пространстве имён `alice_sandbox_calc`.

**Resource** (`resource.hpp`)
- `enum class Resource : int { Iron = 0, Gold = 1, Gems = 2, Exp = 3 }` — перечисление ресурсов
- `const int kResourceCount = 4` — количество типов ресурсов
- `const int kBaseValue[kResourceCount]` — базовые ценности: iron=7, gold=11, gems=23, exp=1
- `const char* const kResourceName[kResourceCount]` — строковые имена ресурсов
- `int eff_value(Resource r, Resource demand)` — возвращает ценность ресурса `r`; если `r == demand`, ценность удваивается

**Room** (`room.hpp`)
- `struct Room` — комната подземелья
  - `int m_id` — номер комнаты (по умолчанию -1)
  - `std::vector<int> m_neighbors` — список номеров смежных комнат
  - `int m_resources[kResourceCount]` — количество каждого ресурса в комнате

**Dungeon** (`dungeon.hpp`)
Хранит граф комнат в двух копиях: оригинал (неизменяемый) и рабочая (мутабельная).
- Конструктор: `explicit Dungeon(std::vector<Room> rooms)` — создаёт подземелье, сохраняя оригинал и рабочую копию
- `int room_count() const` — количество комнат
- `const Room& get_room(int id) const` — доступ к комнате (только чтение)
- `Room& mutable_room(int id)` — доступ к комнате (чтение/запись)
- `std::vector<Room>& mutable_rooms()` — доступ ко всем комнатам (чтение/запись)
- `void reset()` — восстанавливает рабочую копию из оригинала
- `std::vector<int> shortest_path(int from, int to, const std::vector<bool>& allowed) const` — BFS-поиск кратчайшего пути между комнатами; `allowed` ограничивает множество допустимых комнат; при равных путях выбирается комната с минимальным номером
- `std::vector<int> path_to_nearest_unvisited(int from, const std::vector<bool>& visited) const` — BFS-поиск ближайшей непосещённой комнаты; при равных расстояниях выбирается комната с минимальным номером

**Actions** (`bot.hpp`)
Действия бота представлены через `std::variant`:
- `struct GoAction { int m_room; }` — перемещение в комнату
- `struct CollectAction { Resource m_resource; }` — сбор ресурса
- `using Action = std::variant<GoAction, CollectAction>`

**IBot** (`bot.hpp`)
Абстрактный интерфейс бота:
- `virtual ~IBot() = default`
- `virtual std::vector<Action> run(Dungeon& dungeon, int food, Resource demand) = 0` — запускает алгоритм бота на подземелье с заданным запасом еды и целевым ресурсом; возвращает последовательность действий

**AliceBot** (`alice-bot.hpp`)
Реализация алгоритма Алисы:
- `class AliceBot : public IBot`
- `std::vector<Action> run(Dungeon& dungeon, int food, Resource demand) override` — двухфазный алгоритм:
  - Фаза исследования (`floor(food/2)` шагов): идёт в смежную непосещённую комнату с минимальным номером, собирает ресурс с максимальной ценностью
  - Фаза возвращения (`ceil(food/2)` шагов): возвращается кратчайшим путём, при излишке еды собирает ресурсы по убыванию ценности

**Parser** (`parser.hpp`)
- `struct ParsedInput` — результат парсинга:
  - `Dungeon m_dungeon` — подземелье
  - `int m_food` — запас еды
  - `Resource m_demand` — целевой ресурс
- `std::optional<Resource> parse_resource(std::string_view s)` — парсит имя ресурса; возвращает `nullopt` при ошибке
- `const char* resource_to_string(Resource r)` — строковое представление ресурса
- `std::variant<ParsedInput, std::string> parse_input(std::istream& in)` — парсит входные данные из потока; при ошибке возвращает строку с некорректными данными
Валидация включает:
- Диапазоны N=[1,255], M=[2,255], ресурсы=[0,255]
- Симметричность рёбер (если есть переход 3→4, должен быть 4→3)
- Отсутствие петель (ребро из комнаты в саму себя)
- Отсутствие ведущих нулей в числах
- Комната 0 не содержит ресурсов (допускается сокращённый формат)

**Simulator** (`simulator.hpp`)
- `void simulate(const std::vector<Action>& actions, Dungeon& dungeon, Resource demand, std::ostream& out)` — воспроизводит действия бота на подземелье и записывает результат в поток:
  - `go <номер>` — перемещение
  - `state <номер> <iron> <gold> <gems> <exp>` — состояние комнаты (собранные ресурсы отображаются как `_`)
  - `collect <ресурс>` — сбор ресурса
  - `result <iron> <gold> <gems> <exp> <ценность>` — итог при возвращении в комнату 0
  - `dead` / `you're lose: <iron> <gold> <gems> <exp> <ценность>` — при смерти бота
