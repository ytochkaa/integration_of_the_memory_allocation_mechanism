# Интеграция механизма выделения памяти

Лабораторная работа №2. Интеграция аллокатора с фиксированными блоками (Allocator-master)
в SAT-решатель на основе алгоритма DPLL. Реализован паттерн Стратегия для выбора
переменной ветвления. Проведён статический анализ кода инструментом clang-tidy.

## UML-диаграмма классов

```mermaid
classDiagram
    class Allocator {
        -size_t m_blockSize
        -UINT m_maxObjects
        -Block* m_pHead
        +Allocate(size_t) void*
        +Deallocate(void*)
        +GetBlockCount() UINT
        +GetBlocksInUse() UINT
        +GetAllocations() UINT
        +GetDeallocations() UINT
    }

    class AllocatorPool {
        -CHAR m_memory[]
    }

    class IStrategy {
        <<interface>>
        +ChooseColumn(BoolEquation) int
    }

    class MostConstrainedStrategy {
        +ChooseColumn(BoolEquation) int
    }

    class FirstFreeColumnStrategy {
        +ChooseColumn(BoolEquation) int
    }

    class BBV {
        -byte* vec
        -int size
        -int len
        +getSize() int
        +getWeight() int
    }

    class BoolInterval {
        +BBV vec
        +BBV dnc
        +getValue(int) char
        +setValue(char, int)
        +isOrthogonal(BoolInterval) bool
        +mergeInterval(BoolInterval) BoolInterval
        +DECLARE_ALLOCATOR
    }

    class BoolEquation {
        +BoolInterval** cnf
        +BoolInterval* root
        +int cnfSize
        +int count
        +BBV mask
        +shared_ptr~IStrategy~ BranchStrategy
        +CheckRules() int
        +Simplify(int, char)
        +ChooseColForBranching() int
        +setStrategy(shared_ptr~IStrategy~) bool
        +DECLARE_ALLOCATOR
    }

    class NodeBoolTree {
        +NodeBoolTree* lt
        +NodeBoolTree* rt
        +BoolEquation* eq
        +DECLARE_ALLOCATOR
    }

    class ListNode {
        +int value
        +ListNode* next
        +DECLARE_ALLOCATOR
    }

    AllocatorPool --|> Allocator
    MostConstrainedStrategy ..|> IStrategy
    FirstFreeColumnStrategy ..|> IStrategy

    BoolInterval *-- BBV : vec, dnc
    BoolEquation *-- BBV : mask
    BoolEquation o-- BoolInterval : cnf[], root
    BoolEquation --> IStrategy : BranchStrategy
    NodeBoolTree --> BoolEquation : eq
    NodeBoolTree --> NodeBoolTree : lt, rt
    ListNode --> ListNode : next

    BoolInterval ..> Allocator : DECLARE_ALLOCATOR
    BoolEquation ..> Allocator : DECLARE_ALLOCATOR
    NodeBoolTree ..> Allocator : DECLARE_ALLOCATOR
    ListNode ..> Allocator : DECLARE_ALLOCATOR
```

## Структура проекта

- `Allocator-master/` — аллокатор с фиксированными блоками, эксперименты
- `SAT_DPLL/` — DPLL-решатель булевых уравнений с интегрированным аллокатором
- `analysis_report.txt` — отчёт по статическому анализу clang-tidy
