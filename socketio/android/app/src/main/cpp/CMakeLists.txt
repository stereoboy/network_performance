# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.
project("android-socket.io-cpp")

#set(ENV{PKG_CONFIG_PATH} ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
#
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(OpenSSL REQUIRED openssl)
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

# TODO or FIXME : workaround against find_package(OpenSSL REQUIRED)
#
# references
#  - https://cmake.org/cmake/help/v3.6/module/FindOpenSSL.html
#  - https://stackoverflow.com/questions/45958214/android-cmake-could-not-find-openssl
#
set(OPENSSL_ROOT_DIR ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
set(OPENSSL_FOUND ON)
set(OPENSSL_INCLUDE_DIR  ${OPENSSL_ROOT_DIR}/include ${CMAKE_SOURCE_DIR}/../jniLibs/openssl_src/include)
set(OPENSSL_CRYPTO_LIBRARY libcrypto.so)
set(OPENSSL_SSL_LIBRARY libssl.so)
set(OPENSSL_LIBRARIES ssl crypto)
set(OPENSSL_VERSION 1.1.1f)

find_package(OpenSSL REQUIRED)

set(OpenSSL_DIR ${CMAKE_SOURCE_DIR}/../jniLibs)
add_library(OpenSSL::SSL SHARED IMPORTED)
set_target_properties(OpenSSL::SSL PROPERTIES IMPORTED_LOCATION ${OPENSSL_ROOT_DIR}/libssl.so)
add_library(OpenSSL::Crypto SHARED IMPORTED)
set_target_properties(OpenSSL::Crypto PROPERTIES IMPORTED_LOCATION ${OPENSSL_ROOT_DIR}/libcrypto.so)

add_subdirectory(lib/socket.io-client-cpp)

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

target_include_directories( native-lib PUBLIC
                            lib/socket.io-client-cpp/src )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.
target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                        OpenSSL::SSL
                        OpenSSL::Crypto
                        sioclient_tls
        )