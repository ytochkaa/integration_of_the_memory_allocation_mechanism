#ifndef STRATEGY_H
#define STRATEGY_H

class BoolEquation;

class IStrategy{
public:
    virtual ~IStrategy() = default;
    virtual int ChooseColumn(BoolEquation &equation) const = 0;
};

#endif // STRATEGY_H
