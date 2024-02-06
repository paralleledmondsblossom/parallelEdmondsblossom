#ifndef ENUMS_H
#define ENUMS_H

// Explicitly specify underlying type as char
typedef enum : short int {
    EvenLabel,
    OddLabel,
    UnreachedLabel
} Label;

#endif