cmake_minimum_required(VERSION 3.22)

project(RB338Renewed VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/JUCE)

juce_add_gui_app(RB338Renewed
    PRODUCT_NAME "RB338Renewed"
    COMPANY_NAME "LosFiesta"
)

juce_generate_juce_header(RB338Renewed)

target_sources(RB338Renewed
    PRIVATE
        Source/Main.cpp
        Source/Engine.cpp
        Source/Engine.h
        Source/Sequencer.cpp
        Source/Sequencer.h
        Source/Samples.cpp
        Source/Samples.h
)

target_compile_definitions(RB338Renewed
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:RB338Renewed,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="${PROJECT_VERSION}"
)

target_link_libraries(RB338Renewed
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_devices
        juce::juce_audio_basics
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_gui_extra
)
