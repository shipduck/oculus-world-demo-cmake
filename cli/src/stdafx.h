// Ŭnicode please 
#define SR_USE_PCH 1

#define NOMINMAX 

#if SR_USE_PCH
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <climits>
#include <cstdarg>
#include <cstring>
#include <cstdint>

#include <typeinfo>

#include <locale>   // wstring_convert
#include <codecvt>  // codecvt_utf8
#include <iostream> // cout
#include <string>   // stoi and u32string

#include <sstream>
#include <random>

#include <array>
#include <list>
#include <tuple>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <memory>
#include <deque>

#include <unordered_map>
#include <unordered_set>

#include <type_traits>

// system
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// libovr
#include <OVR.h>

// loki
#include "Typelist.h"
#include "HierarchyGenerators.h"

// boost
#include <boost/format.hpp>
#include <boost/bimap.hpp>
#include <boost/assign.hpp>

// for unit test
#include <gtest/gtest.h>

#else
#error "NEED PCH"
#endif