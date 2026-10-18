cmake_minimum_required(VERSION 3.22)

project(CipherLab VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# JUCE is expected to be present as the existing Git submodule at ./JUCE.
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/JUCE/CMakeLists.txt")
    message(FATAL_ERROR "JUCE submodule was not found. Run: git submodule update --init --recursive")
endif()

add_subdirectory(JUCE)

juce_add_gui_app(CipherLab
    PRODUCT_NAME "CipherLab"
    COMPANY_NAME "CipherLab"
    BUNDLE_ID "com.cipherlab.app"
)

target_sources(CipherLab PRIVATE
    source/Main.cpp
    source/MainComponent.cpp
    source/MainComponent.h
    source/ciphers/SimpleSubstitution.cpp
    source/ciphers/SimpleSubstitution.h
    source/ciphers/DoubleTransposition.cpp
    source/ciphers/DoubleTransposition.h
    source/ciphers/OneTimePad.cpp
    source/ciphers/OneTimePad.h
    source/utils/Comparison.h
)

target_compile_definitions(CipherLab PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:CipherLab,JUCE_PRODUCT_NAME>"
    JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:CipherLab,JUCE_VERSION>"
)

target_link_libraries(CipherLab PRIVATE
    juce::juce_gui_extra
    juce::juce_graphics
    juce::juce_events
    juce::juce_core
    juce::juce_data_structures
    juce::juce_audio_basics
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)

include(CTest)
if(BUILD_TESTING)
    add_executable(CipherLabTests
        tests/CipherTests.cpp
        source/ciphers/SimpleSubstitution.cpp
        source/ciphers/DoubleTransposition.cpp
        source/ciphers/OneTimePad.cpp
    )
    target_include_directories(CipherLabTests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/source")
    add_test(NAME CipherLabTests COMMAND CipherLabTests)
endif()
