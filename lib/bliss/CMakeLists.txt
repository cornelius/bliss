set(bliss_LIB_SRCS
  bliss.cpp
)

kde4_add_library( bliss SHARED ${bliss_LIB_SRCS} )

target_link_libraries( bliss ${KDE4_KDEUI_LIBS} ${QT_QTCORE_LIBRARY} ${QT_QTXML_LIBRARY} )

set_target_properties( bliss PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )
install( TARGETS bliss ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES
  bliss.h
  bliss_export.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/bliss
)
