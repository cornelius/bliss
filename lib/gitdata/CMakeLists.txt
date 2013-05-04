set(gitdata_LIB_SRCS
  command.cpp dir.cpp remote.cpp
)

kde4_add_library( gitdata SHARED ${gitdata_LIB_SRCS} )

target_link_libraries( gitdata ${KDE4_KDEUI_LIBS} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} )

set_target_properties( gitdata PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install( TARGETS gitdata ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES
  command.h dir.h remote.h
  gitdata_export.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/gitdata
)
