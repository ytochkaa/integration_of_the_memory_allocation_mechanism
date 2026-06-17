# Интеграция механизма выделения памяти

Лабораторная работа №2. Интеграция аллокатора с фиксированными блоками (Allocator-master)
в SAT-решатель на основе алгоритма DPLL. Реализован паттерн Стратегия для выбора
переменной ветвления. Проведён статический анализ кода инструментом clang-tidy.

## UML-диаграмма классов

```mermaid
classDiagram
direction LR
    class Allocator {
        +Allocate(size_t) void*
        +Deallocate(void*)
        +GetAllocations() UINT
        +GetDeallocations() UINT
    }

    class AllocatorPool~T, Objects~

    class X {
        -byte* ptr
        -int index
    }

    class BBV {
        -byte* vec
        -int size
        -int len
        +getWeight() int
        +getSize() int
    }

    class BoolInterval {
        +BBV vec
        +BBV dnc
        +getValue(int ix) char
        +setValue(char value, int ix) void
        +isOrthogonal(BoolInterval&) bool
        +mergeInterval(BoolInterval&) BoolInterval&
    }

    class BoolEquation {
        +BoolInterval** cnf
        +BoolInterval* root
        +int cnfSize
        +int count
        +BBV mask
        +shared_ptr~IStrategy~ BranchStrategy
        +CheckRules() int
        +Simplify(int, char) void
        +ChooseColForBranching() int
        +setStrategy(shared_ptr~IStrategy~) bool
    }

    class IStrategy {
        <<interface>>
        +ChooseColumn(BoolEquation&) int
    }

    class MostConstrainedStrategy {
        +ChooseColumn(BoolEquation&) int
    }

    class FirstFreeColumnStrategy {
        +ChooseColumn(BoolEquation&) int
    }

    class NodeBoolTree {
        +NodeBoolTree* lt
        +NodeBoolTree* rt
        +BoolEquation* eq
    }

    class ListNode {
        +int value
        +ListNode* next
    }

    AllocatorPool --|> Allocator
    IStrategy <|-- MostConstrainedStrategy
    IStrategy <|-- FirstFreeColumnStrategy

    BBV ..> X
    BoolInterval *-- BBV
    BoolEquation *-- BBV
    BoolEquation o-- BoolInterval
    BoolEquation --> BoolInterval
    BoolEquation --> IStrategy
    NodeBoolTree --> BoolEquation

    BoolInterval ..> Allocator
    BoolEquation ..> Allocator
    NodeBoolTree ..> Allocator
    ListNode ..> Allocator
```

## Структура проекта

- `Allocator-master/` — аллокатор с фиксированными блоками, эксперименты
- `SAT_DPLL/` — DPLL-решатель булевых уравнений с интегрированным аллокатором
- `analysis_report.txt` — отчёт по статическому анализу clang-tidy
