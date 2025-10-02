#pragma once
//STD Library
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <functional>
#include <stdlib.h>

//Platform specific
#ifdef _WIN32
#include <windows.h>
#endif

//My Sources
#include "Debug.hpp"
#include "TimeManager.hpp"
#include "DataIOHandler.hpp"
#include "ftd2xx.h"