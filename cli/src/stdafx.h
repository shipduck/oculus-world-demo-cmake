// Ŭnicode please 
#include "Kernel/OVR_Types.h"
#define SR_USE_PCH 1

#define NOMINMAX 

#if SR_USE_PCH
#include <float.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>
#include <locale.h>

#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <climits>
#include <cstdarg>
#include <cstring>
#include <cstdint>
#include <cfloat>

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

#ifdef OVR_OS_WIN32
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#include <xinput.h>
#endif

#else
#error "NEED PCH"
#endif