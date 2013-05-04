set(bliss_bin_SRCS
  viewpositions.cpp
  storage.cpp
  history.cpp
  itemplacer.cpp
  delayedsignal.cpp
  menuhandleitem.cpp
  listitem.cpp
  newlistdialog.cpp
  droptargetitem.cpp
  groupaddersidebaritem.cpp
  storagefile.cpp
  storagegit.cpp
  hidinglineedit.cpp
  menuhandler.cpp
  todohandleitem.cpp
  matchlist.cpp
  searchresultview.cpp
  searchedit.cpp
  fanmenuitem.cpp
  buttonitem.cpp
  groupadderitem.cpp
  historyview.cpp
  overview.cpp
  roundedrectitem.cpp
  settingswidget.cpp
  magicmenuitem.cpp
  mainmenuitem.cpp
  newgroupdialog.cpp
  labelitem.cpp
  fanmenuelement.cpp
  fanmenu.cpp
  todoitem.cpp
  groupview.cpp
  newtododialog.cpp
  mainwindow.cpp
  main.cpp
  mainview.cpp
  mainmodel.cpp
  blissitemmodel.cpp
  blissallitemmodel.cpp
  blisstodoitemmodel.cpp
  trackinggraphicsview.cpp
)

kde4_add_kcfg_files(bliss_bin_SRCS settings.kcfgc )

kde4_add_executable(bliss_bin ${bliss_bin_SRCS})
set_target_properties( bliss_bin PROPERTIES OUTPUT_NAME bliss )

target_link_libraries(bliss_bin bliss gitdata ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QT_QTWEBKIT_LIBRARY})

install(TARGETS bliss_bin ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( FILES bliss.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES bliss.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )
install( FILES blissui.rc  DESTINATION  ${DATA_INSTALL_DIR}/bliss )
