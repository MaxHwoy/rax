#pragma once

// objective templated definitions

#include <type_traits>

#define where_fundamental(T) typename std::enable_if<std::is_fundamental<T>::value>::type* = nullptr
#define where_function(T) typename std::enable_if<std::is_function<T>::value>::type* = nullptr
#define where_class(T) typename std::enable_if<std::is_class<T>::value>::type* = nullptr
#define where_enum(T) typename std::enable_if<std::is_enum<T>::value>::type* = nullptr

#define where_const(T) typename std::enable_if<std::is_const<T>::value>::type* = nullptr
#define where_final(T) typename std::enable_if<std::is_final<T>::value>::type* = nullptr

#define where_integral(T) typename std::enable_if<std::is_integral<T>::value>::type* = nullptr
#define where_floating(T) typename std::enable_if<std::is_floating_point<T>::value>::type* = nullptr
#define where_string(T) typename std::enable_if<std::is_same<T, rax::string>::value>::type* = nullptr

#define where_object(T) typename std::enable_if<std::is_object<T>::value>::type* = nullptr
#define where_pointer(T) typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr
#define where_reference(T) typename std::enable_if<std::is_reference<T>::value>::type* = nullptr
