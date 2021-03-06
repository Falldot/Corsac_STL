/**
 * corsac::STL
 *
 * compiler_traits.h
 *
 * Created by Falldot on 26.11.2021.
 * Copyright (c) 2021 Corsac. All rights reserved.
 */
#ifndef CORSAC_STL_COMPILER_TRAITS_H
#define CORSAC_STL_COMPILER_TRAITS_H

#pragma once

/**
 * CORSAC_ANALYSIS_ASSUME
 *
 * Исправления предупреждения анализа кода.
 */
#ifndef CORSAC_ANALYSIS_ASSUME
    #if defined(_MSC_VER)
        #define CORSAC_ANALYSIS_ASSUME(x) __analysis_assume(!!(x))
    #else
        #define CORSAC_ANALYSIS_ASSUME(x)
    #endif
#endif

/**
 * MSVC макросы для управления предупреждениями компилятора
 */
#ifndef CORSAC_DISABLE_WIN_VC_WARNING
    #if defined(_MSC_VER)
        #define CORSAC_DISABLE_WIN_VC_WARNING(w)  \
                        __pragma(warning(push,0))
    #else
        #define CORSAC_DISABLE_WIN_VC_WARNING(w)
    #endif
#endif

#ifndef CORSAC_DISABLE_VC_WARNING
    #if defined(_MSC_VER)
        #define CORSAC_DISABLE_VC_WARNING(w)  \
                    __pragma(warning(push))       \
                    __pragma(warning(disable:w))
    #else
        #define CORSAC_DISABLE_VC_WARNING(w)
    #endif
#endif

#ifndef CORSAC_RESTORE_VC_WARNING
    #if defined(_MSC_VER)
        #define CORSAC_RESTORE_VC_WARNING()   \
                    __pragma(warning(pop))
    #else
        #define CORSAC_RESTORE_VC_WARNING()
    #endif
#endif

/**
* CORSAC_COMPILER_INTMAX_SIZE
*
* Это связано с концепцией intmax_t uintmax_t, но доступно в форме препроцессора,
* а не во время компиляции. Во время компиляции вы можете использовать
* intmax_t и uintmax_t для использования фактических типов.
*/
#if defined(__GNUC__) && defined(__x86_64__)
    #define CORSAC_COMPILER_INTMAX_SIZE 16  // intmax_t is __int128_t (GCC extension) and is 16 bytes.
#else
    #define CORSAC_COMPILER_INTMAX_SIZE 8   // intmax_t is int64_t and is 8 bytes.
#endif

/**
* CORSAC_HAS_INCLUDE_AVAILABLE
*
* Используется для защиты от макроса CORSAC_HAS_INCLUDE() на компиляторах,
* которые не поддерживают указанную функцию.
*
* Пример использования:
*
* #if CORSAC_HAS_INCLUDE_AVAILABLE
*     #if CORSAC_HAS_INCLUDE("myinclude.h")
*         #include "myinclude.h"
*     #endif
* #endif
*/
#if !defined(CORSAC_HAS_INCLUDE_AVAILABLE)
    #define CORSAC_HAS_INCLUDE_AVAILABLE 1
#endif

/**
* CORSAC_LIKELY / CORSAC_UNLIKELY
*
* Определяется как макрос, который дает компилятору подсказку для предсказания ветвления.
* GCC дает вам возможность вручную указать компилятору результат сравнения,
* хотя часто лучше скомпилировать код с обратной связью по профилированию как в GCC
* (-fprofile-arcs), так и в VC++ (LTCG: PGO и т.д.) . Однако бывают случаи, когда вы очень
* уверены, что логическое выражение обычно оценивается как истинное или ложное и может
* помочь компилятору с помощью директивы явной ясности.
*
* Пример использования:
*    if(CORSAC_LIKELY(a == 0)) // Сообщите компилятору, что a обычно равно 0.
*       { ... }
*
* Example usage:
*    if(CORSAC_UNLIKELY(a == 0)) // Сообщите компилятору, что a обычно не равно 0.
*       { ... }
*/
#ifndef CORSAC_LIKELY
    #if (defined(CORSAC_COMPILER_CLANG) || defined(CORSAC_COMPILER_GCC))
        #if defined(__cplusplus)
            #define CORSAC_LIKELY(x)   __builtin_expect(!!(x), true)
            #define CORSAC_UNLIKELY(x) __builtin_expect(!!(x), false)
        #else
            #define CORSAC_LIKELY(x)   __builtin_expect(!!(x), 1)
            #define CORSAC_UNLIKELY(x) __builtin_expect(!!(x), 0)
        #endif
    #else
        #define CORSAC_LIKELY(x)   (x)
        #define CORSAC_UNLIKELY(x) (x)
    #endif
#endif

/**
* CORSAC_HAS_INCLUDE
*
* Может использоваться в выражениях #if и #elif для проверки существования заголовка,
* указанного в операнде. Если возможно, он принимает ненулевое значение,
* в противном случае - ноль. Операнд имеет ту же форму, что и файл в директиве include.
*
* Пример использования:
*
* #if CORSAC_HAS_INCLUDE("myinclude.h")
*     #include "myinclude.h"
* #endif
*
* #if CORSAC_HAS_INCLUDE(<myinclude.h>)
*     #include <myinclude.h>
* #endif
*/
#if !defined(CORSAC_HAS_INCLUDE)
    #define CORSAC_HAS_INCLUDE(x) __has_include(x)
#endif

/**
* wchar_t
* Здесь мы определяем::
*    CORSAC_WCHAR_T_NON_NATIVE
*    CORSAC_WCHAR_SIZE = <sizeof(wchar_t)>
*/
#ifndef CORSAC_WCHAR_T_NON_NATIVE
    // Компиляторы, которые всегда реализуют wchar_t как собственный тип:
    //     COMCORSACU, новый SN и другие компиляторы на основе EDG.
    //     GCC
    //     Borland
    //     SunPro
    //     IBM Visual Age
    #if defined(CORSAC_COMPILER_MSVC) || (defined(CORSAC_COMPILER_CLANG) && defined(CORSAC_PLATFORM_WINDOWS))
        #ifndef _NATIVE_WCHAR_T_DEFINED
            #define CORSAC_WCHAR_T_NON_NATIVE 1
        #endif
    #endif

    #ifndef CORSAC_WCHAR_SIZE
        #if defined(__WCHAR_MAX__) // GCC определяет это для большинства платформ.
            #if (__WCHAR_MAX__ == 2147483647) || (__WCHAR_MAX__ == 4294967295)
                #define CORSAC_WCHAR_SIZE 4
            #elif (__WCHAR_MAX__ == 32767) || (__WCHAR_MAX__ == 65535)
                #define CORSAC_WCHAR_SIZE 2
            #elif (__WCHAR_MAX__ == 127) || (__WCHAR_MAX__ == 255)
                #define CORSAC_WCHAR_SIZE 1
            #else
                #define CORSAC_WCHAR_SIZE 4
            #endif
        #elif defined(CORSAC_PLATFORM_UNIX)
            // Стандартно для Unix wchar_t имеет значение int32_t или uint32_t.
            // Все версии GNUC по умолчанию используют 32-битный wchar_t, но CORSAC использовал
            // параметр командной строки GCC -fshort-wchar, чтобы установить его 16-битный.
            // Если вы знаете, что компилятор настроен на использование wchar_t, отличного
            // от значения по умолчанию, вам необходимо вручную определить CORSAC_WCHAR_SIZE для сборки.
            #define CORSAC_WCHAR_SIZE 4
        #else
            // В Windows стандартно,чтобы wchar_t был uint16_t.
            // GCC по умолчанию определяет wchar_t как int. Electronic Arts
            // стандартизировала wchar_t как 16-битное значение без знака на всех
            // консольных платформах. Учитывая, что в настоящее время нет известного
            // способа определить размер wchar_t во время препроцессора,
            // мы объявляем его равным 2, так как это стандарт Electronic Arts.
            // Если у вас есть CORSAC_WCHAR_SIZE! = Sizeof (wchar_t),
            // то ваш код может не работать, но он также не будет работать с
            // библиотеками wchar и данными из других частей CORSAC.
            // В GCC вы можете принудительно установить wchar_t в два байта
            // с помощью аргумента компилятора -fshort-wchar.
            #define CORSAC_WCHAR_SIZE 2
        #endif
    #endif
#endif

#if defined(CORSAC_COMPILER_GCC) || defined(__xlC__) || defined(__clang__)
    #define CORSAC_ALIGN_OF(type) ((size_t)__alignof__(type))
    #define CORSAC_ALIGN(n) __attribute__((aligned(n)))
    #define CORSAC_PREFIX_ALIGN(n)
    #define CORSAC_POSTFIX_ALIGN(n) __attribute__((aligned(n)))
    #define CORSAC_ALIGNED(variable_type, variable, n) variable_type variable __attribute__((aligned(n)))
    #define CORSAC_PACKED __attribute__((packed))

#elif defined(CORSAC_COMPILER_INTEL) || defined(CS_UNDEFINED_STRING) || (defined(CORSAC_COMPILER_MSVC))
    #define CORSAC_ALIGN_OF(type) ((size_t)__alignof(type))
    #define CORSAC_ALIGN(n) __declspec(align(n))
    #define CORSAC_PREFIX_ALIGN(n) CORSAC_ALIGN(n)
    #define CORSAC_POSTFIX_ALIGN(n)
    #define CORSAC_ALIGNED(variable_type, variable, n) CORSAC_ALIGN(n) variable_type variable
    #define CORSAC_PACKED

#elif defined(CORSAC_COMPILER_ARM)
    #define CORSAC_ALIGN_OF(type) ((size_t)__ALIGNOF__(type))
    #define CORSAC_ALIGN(n) __align(n)
    #define CORSAC_PREFIX_ALIGN(n) __align(n)
    #define CORSAC_POSTFIX_ALIGN(n)
    #define CORSAC_ALIGNED(variable_type, variable, n) __align(n) variable_type variable
    #define CORSAC_PACKED __packed
#endif

/**
 * Unused
 *
 * Сообщяем компилятору что переменную мы больше не используем.
 */
#define CORSAC_UNUSED(x) ((void)(x))

#endif //CORSAC_STL_COMPILER_TRAITS_H
