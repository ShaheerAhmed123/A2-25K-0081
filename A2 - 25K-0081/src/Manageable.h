#ifndef MANAGEABLE_H
#define MANAGEABLE_H

// Interface for inventory management tasks
class Manageable {
public:
    virtual void add() = 0;
    virtual void del() = 0;
    virtual ~Manageable() {}
};

#endif