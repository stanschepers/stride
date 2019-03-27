set(SOURCE_FILES
        source/main.cpp
        )

set(HEADER_FILES
        )

set(UI_FILES
        )

set(QML_FILES
        QML/main.qml
        )

#qt5_wrap_ui(ui_wrap ${project_ui})
#qt5_wrap_cpp(moc_sources ${project_headers})

add_executable(StrideVisualization
        ${SOURCE_FILES}
        ${HEADER_FILES}
#        ${ui_wrap}
#        ${moc_sources}
        )

target_link_libraries(StrideVisualization PUBLIC Qt5::Widgets Qt5::Core Qt5::Quick libstride)

install(FILES ${QML_FILES} DESTINATION ${BIN_INSTALL_LOCATION}/Qt5Stride)