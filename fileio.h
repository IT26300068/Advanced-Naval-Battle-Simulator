#ifndef FILEIO_H
#define FILEIO_H

#include "battlefield.h"

void saveBattlefield(
    const Battlefield *bf,
    const char title[]
);

void saveText(const char text[]);
void showStatistics(void);

#endif