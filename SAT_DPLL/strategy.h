#ifndef STRATEGY_H
#define STRATEGY_H

class BoolEquation;

class IStrategy{
public:
    virtual ~IStrategy() = default;
    virtual int ChooseColumn(BoolEquation &equation) const = 0;
};

class MostConstrainedStrategy : public IStrategy {
public:
    int ChooseColumn(BoolEquation &equation) const override;
};

class FirstFreeColumnStrategy : public IStrategy {
public:
    int ChooseColumn(BoolEquation &equation) const override;
};

#endif // STRATEGY_H
