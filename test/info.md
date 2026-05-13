## Покрытие тестами

Все тесты используют Google Test Framework

### Resource Tests — 1 тест
- DoubledTarget — проверка удвоения ценности целевого ресурса для всех типов

### Dungeon Tests — 3 теста
- ShortestPathMultiHop — кратчайший путь через несколько комнат с tie-break по min id
- NearestUnvisitedFarAway — поиск ближайшей непосещённой комнаты через BFS
- ResetRestoresOriginal — восстановление состояния подземелья после мутации

### Parser Tests — 4 теста
- EmptyInput — пустой ввод возвращает ошибку
- ValidShortRoom0 — корректный ввод с сокращённым форматом комнаты 0
- InvalidRoom0NonzeroResources — ненулевые ресурсы в комнате 0 считаются ошибкой
- ParseValid — парсинг всех имён ресурсов (iron, gold, gems, exp)

### Simulator Tests — 2 теста
- MultipleCollectsInRoom — несколько сборов в одной комнате, проверка формата вывода
- DeathOutputWhenNotReturned — вывод при смерти бота (dead / you're lose)

### AliceBot Tests — 2 теста
- TwoRoomsMinimal — минимальный сценарий: go → collect → go, проверка действий
- ReturnsToRoom0 — последнее действие бота всегда возвращает в комнату 0

### Main Tests (интеграционные) — 4 теста
- NoArgs — запуск без аргументов возвращает код 1
- NonexistentFile — запуск с несуществующим файлом возвращает код 1
- TZExampleValidFull — полный пример из ТЗ (5 комнат, 6 еды, gems): проверка всего result.txt
- TZExampleInvalidFull — полный пример из ТЗ (pipe-разделитель): проверка строки ошибки в result.txt

## Итого: 16 тестов (12 юнит + 4 интеграционных)
