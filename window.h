#pragma once

#include <atomic>

void createWindow();
void windowMessageLoop();
void redrawWindow();

extern std::atomic_bool done;