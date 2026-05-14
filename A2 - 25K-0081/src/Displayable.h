#ifndef DISPLAYABLE_H
#define DISPLAYABLE_H

// Interface for objects that can be printed to console 
class Displayable {
public:
    virtual void show() const = 0; // Pure virtual function 
    virtual ~Displayable() {}
};

#endif