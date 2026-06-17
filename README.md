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
        -UINT m_blockCnt
        -UINT m_blocksInUse
        -UINT m_allocations
        -UINT m_deallocations
        +Allocate(size_t) void*
        +Deallocate(void*)
        +GetBlockSize() size_t
        +GetBlockCount() UINT
        +GetBlocksInUse() UINT
        +GetAllocations() UINT
        +GetDeallocations() UINT
    }

    class AllocatorPool~T, Objects~ {
        -CHAR m_memory[]
    }

    class X {
        -byte* ptr
        -int index
        +X()
        +X(byte* vec, int k)
        +operator=(int k) X
        +operator int()
        +operator=(X& v) X
    }

    class BBV {
        -byte* vec
        -int size
        -int len
        +BBV()
        +BBV(int size)
        +BBV(const char* str)
        +BBV(BBV& V)
        +Init(const char* str) void
        +Set0(int k) void
        +Set1(int k) void
        +operator=(BBV& V) BBV
        +operator==(BBV& V) bool
        +operator|(BBV& V) BBV
        +operator&(BBV& V) BBV
        +operator~() BBV
        +operator[](int k) X
        +getWeight() int
        +getSize() int
    }

    class BoolInterval {
        +BBV vec
        +BBV dnc
        +BoolInterval(size_t len)
        +BoolInterval(const char* vec, const char* dnc)
        +BoolInterval(const char* vector)
        +BoolInterval(BBV& vec, BBV& dnc)
        +setInterval(BBV& vec, BBV& dnc) void
        +length() int
        +rang() int
        +getValue(int ix) char
        +setValue(char value, int ix) void
        +isOrthogonal(BoolInterval& ibv) bool
        +isEqualComponent(BoolInterval& ibv) bool
        +mergeInterval(BoolInterval& ibv) BoolInterval&
        +isIntersection(BoolInterval& ibv) bool
        +isAbsorb(BoolInterval& ibv) bool
        +DECLARE_ALLOCATOR
    }

    class BoolEquation {
        +BoolInterval** cnf
        +BoolInterval* root
        +int cnfSize
        +int count
        +BBV mask
        +shared_ptr~IStrategy~ BranchStrategy
        +BoolEquation(BoolInterval**, BoolInterval*, int, int, BBV)
        +BoolEquation(BoolEquation& equation)
        +CheckRules() int
        +Rule1Row1(BoolInterval*) bool
        +Rule2RowNull(BoolInterval*) bool
        +Rule3ColNull(BBV) void
        +Rule4Col0(BBV) bool
        +Rule5Col1(BBV) bool
        +Simplify(int, char) void
        +ChooseColForBranching() int
        +setStrategy(shared_ptr~IStrategy~) bool
        +DECLARE_ALLOCATOR
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
        +NodeBoolTree(BoolEquation*)
        +NodeBoolTree(const NodeBoolTree&)
        +DECLARE_ALLOCATOR
    }

    class ListNode {
        +int value
        +ListNode* next
        +ListNode(int v)
        +DECLARE_ALLOCATOR
    }

    AllocatorPool --|> Allocator
    IStrategy <|.. MostConstrainedStrategy
    IStrategy <|.. FirstFreeColumnStrategy

    BBV --> X : использует
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
