set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

SET(listVar "")
FOREACH(f ${VIS_SRC})
    LIST(APPEND listVar "../${f}")
ENDFOREACH(f)
SET(SOURCE_FILES "${listVar}")

#set(SOURCE_FILES ${VIS_SRC}
#        source/EpiOutput.h
#        source/EpiOutputReader.cpp
#        source/EpiOutputReader.h
#        source/EpiOutputReaderFactory.cpp
#        source/EpiOutputReaderFactory.h
#        source/EpiOutputJSONReader.cpp
#        source/EpiOutputJSONReader.h
#        )

set(QT_SOURCE_FILES
        source/strideVisualization.cpp
        source/MapController.cpp
        source/MapController.h
        )

set(UI_FILES
        )

set(QML_FILES
        QML/main.qml
        QML/location.qml
        QML/RoundMouseArea.qml
        QML/DataBarList.qml
        )

set_property(SOURCE source/strideVisualization.cpp PROPERTY SKIP_AUTOMOC ON)

#qt5_wrap_ui(ui_wrap ${project_ui})
#qt5_wrap_cpp(moc_sources ${project_headers})

add_executable(strideVisualization
        ${SOURCE_FILES}
        ${QT_SOURCE_FILES}
#        ${ui_wrap}
#        ${moc_sources}
        )

target_link_libraries(strideVisualization PUBLIC Qt5::Widgets Qt5::Core Qt5::Quick libstride)
install(FILES ${QML_FILES} DESTINATION ${BIN_INSTALL_LOCATION}/visualization)
install(TARGETS strideVisualization DESTINATION ${BIN_INSTALL_LOCATION}/visualization)
