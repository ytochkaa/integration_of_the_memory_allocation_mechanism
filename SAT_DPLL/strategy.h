#ifndef STRATEGY_H
#define STRATEGY_H

class BoolEquation;

// Паттерн "Стратегия": правило выбора переменной ветвления вынесено из решателя,
// поэтому новое правило добавляется новым классом без изменения BoolEquation.
class IStrategy{
public:
    virtual ~IStrategy() = default; // стратегия удаляется через указатель на интерфейс
    // Возвращает индекс столбца для ветвления или -1, если свободных столбцов нет.
    virtual int ChooseColumn(BoolEquation &equation) const = 0;
};

// Столбец с минимальным числом символов '-' (переменная входит в большинство дизъюнкций).
class MostConstrainedStrategy : public IStrategy {
public:
    int ChooseColumn(BoolEquation &equation) const override;
};

// Первый свободный столбец слева направо.
class FirstFreeColumnStrategy : public IStrategy {
public:
    int ChooseColumn(BoolEquation &equation) const override;
};

#endif // STRATEGY_H
