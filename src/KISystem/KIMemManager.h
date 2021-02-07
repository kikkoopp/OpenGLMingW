//
// Created by Kriko on 2021/2/6.
//

#ifndef OPENGLEXAMPLE_KIMEMMANAGER_H
#define OPENGLEXAMPLE_KIMEMMANAGER_H

#include <windows.h>
#include <new>
#include "KISystem.h"
#include "KISynchronize.h"
#define VS_NEW new
#define VS_DELETE delete
#define USE_STL_TYPE_TRAIT
#ifdef USE_STL_TYPE_TRAIT
#include <type_traits>
#endif // USE_STL_TYPE_TRAIT
#include <dbghelp.h>

namespace KIEngine {
    class KIMemManager {

    };
}


#endif //OPENGLEXAMPLE_KIMEMMANAGER_H
