/*****************************************************************************
 **
 ** Copyright (C) 2018 Fabian Schweinfurth
 ** Contact: autoshift <at> derfabbi.de
 **
 ** This file is part of autoshift
 **
 ** autoshift is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** autoshift is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with autoshift.  If not, see <http://www.gnu.org/licenses/>.
 **
 *****************************************************************************/

#pragma once

#include <QTimer>
#include <QEventLoop>

#define ENUM_FUNCS(name, first, ...)                              \
  typedef name ## _enum::name ## _enum name;                      \
  inline bool is_ ## name (int i) {                               \
    return (i >= ((int) name::first) && i < ((int) name::SIZE));  \
  }                                                               \
  inline name to ## name(int i) {                                 \
    if (!is_ ## name(i)) return name::NONE;                       \
    return (name)i;                                               \
  }                                                               \
  inline std::string s ## name(name i) {                                                 \
    static std::vector<std::string> names;                        \
    if (names.empty()) {                                          \
    char str[] { #first ", " #__VA_ARGS__ ", SIZE, NONE"};                            \
    for(char* token = std::strtok(&str[0], ", "); token != 0x0; token = std::strtok(0x0, ", ")) { \
        names.push_back(token);                                        \
    }                                                                  \
    }                                                             \
      return names[i];                                            \
  }

// scoped enum that is explicitly convertible to int
#define FENUM(name, first, ...)                 \
  namespace name ## _enum {                     \
    enum name ## _enum {                        \
      first,                                    \
        __VA_ARGS__  ,                          \
        SIZE,                                   \
        NONE                                    \
        };                                      \
  }                                             \
  ENUM_FUNCS(name, first, __VA_ARGS__)

/*
FENUM(Foo,
      ASD,
      QWE)
=>
  namespace Foo_enum {
    enum Foo_enum {
      ASD,
      QWE
    };
  }
typedef Foo_enum::Foo_enum Foo;
*/

#define FENUM_T(name, type, first, ...)         \
  namespace name ## _enum {                     \
    enum name ## _enum : type {                 \
      first,                                    \
      __VA_ARGS__,                              \
        SIZE,                                   \
        NONE                                    \
        };                                      \
  }                                             \
  ENUM_FUNCS(name, first, __VA_ARGS__)

/**
 * Wait for signal to fire
 *
 * @args obj The Object this signal belongs to.
 * @args signal The Signal.
 * @args ms Miliseconds timeout.
 *
 * @return Whether the signal did fire or timeout occurred
 */
template<class T, typename FUNC>
bool wait(const T* obj, FUNC signal, int ms = 5000)
{
  QTimer timer;
  timer.setSingleShot(true);

  // TODO timer that fires every 100ms and calls main eventloop
  QEventLoop loop;
  QObject::connect(obj, signal, &loop, &QEventLoop::quit);
  QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
  timer.start(ms);
  loop.exec();

  bool ret = timer.isActive();
  timer.stop();
  return ret;
}