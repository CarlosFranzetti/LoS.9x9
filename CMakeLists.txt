cmake_minimum_required(VERSION 3.22)

project(LoS9x9 VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/JUCE)

juce_add_gui_app(LoS9x9
    PRODUCT_NAME "LoS9x9"
    COMPANY_NAME "LosFiesta"
)

juce_generate_juce_header(LoS9x9)

target_sources(LoS9x9
    PRIVATE
        Source/Main.cpp
        Source/Engine.cpp
        Source/Engine.h
        Source/Sequencer.cpp
        Source/Sequencer.h
        Source/Samples.cpp
        Source/Samples.h
)

target_compile_definitions(LoS9x9
    PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:LoS9x9,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="${PROJECT_VERSION}"
)

target_link_libraries(LoS9x9
    PRIVATE
        juce::juce_audio_utils
        juce::juce_audio_devices
        juce::juce_audio_basics
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_gui_extra
)
