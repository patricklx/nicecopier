#############################################################################
# Makefile for building: NiceCopier
# Generated by qmake (3.0) (Qt 5.4.0)
# Project:  NiceCopier.pro
# Template: app
# Command: E:\programming\tools\qt\qt-32\qtbase\bin\qmake.exe -spec win32-g++ -o Makefile NiceCopier.pro
#############################################################################

MAKEFILE      = Makefile

first: release
install: release-install
uninstall: release-uninstall
QMAKE         = E:\programming\tools\qt\qt-32\qtbase\bin\qmake.exe
DEL_FILE      = del
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
COPY          = copy /y
COPY_FILE     = $(COPY)
COPY_DIR      = xcopy /s /q /y /i
INSTALL_FILE  = $(COPY_FILE)
INSTALL_PROGRAM = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = del
SYMLINK       = copy /y
DEL_DIR       = rmdir
MOVE          = move
SUBTARGETS    =  \
		release \
		debug


release: FORCE
	$(MAKE) -f $(MAKEFILE).Release
release-make_first: FORCE
	$(MAKE) -f $(MAKEFILE).Release 
release-all: FORCE
	$(MAKE) -f $(MAKEFILE).Release all
release-clean: FORCE
	$(MAKE) -f $(MAKEFILE).Release clean
release-distclean: FORCE
	$(MAKE) -f $(MAKEFILE).Release distclean
release-install: FORCE
	$(MAKE) -f $(MAKEFILE).Release install
release-uninstall: FORCE
	$(MAKE) -f $(MAKEFILE).Release uninstall
debug: FORCE
	$(MAKE) -f $(MAKEFILE).Debug
debug-make_first: FORCE
	$(MAKE) -f $(MAKEFILE).Debug 
debug-all: FORCE
	$(MAKE) -f $(MAKEFILE).Debug all
debug-clean: FORCE
	$(MAKE) -f $(MAKEFILE).Debug clean
debug-distclean: FORCE
	$(MAKE) -f $(MAKEFILE).Debug distclean
debug-install: FORCE
	$(MAKE) -f $(MAKEFILE).Debug install
debug-uninstall: FORCE
	$(MAKE) -f $(MAKEFILE).Debug uninstall

Makefile: NiceCopier.pro ../../../../tools/qt/qt-32/qtbase/mkspecs/win32-g++/qmake.conf ../../../../tools/qt/qt-32/qtbase/mkspecs/features/spec_pre.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/qdevice.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/device_config.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/common/shell-win32.conf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/qconfig.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_bootstrap_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_bootstrap_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_concurrent.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_concurrent_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_concurrent.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_core.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_core_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_core.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_gui.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_gui_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_gui.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_network.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_network_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_network.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_opengl.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_opengl_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_opengl.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_openglextensions.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_openglextensions_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_openglextensions.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_platformsupport_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_platformsupport_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_printsupport.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_printsupport_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_printsupport.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_sql.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_sql_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_sql.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_testlib.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_testlib_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_testlib.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_widgets.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_widgets_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_widgets.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_xml.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_lib_xml_private.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_lib_xml.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_plugin_qgenericbearer.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_plugin_qgenericbearer.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_plugin_qico.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_plugin_qico.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_plugin_qminimal.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_plugin_qminimal.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_plugin_qnativewifibearer.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_plugin_qnativewifibearer.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_plugin_qsqlite.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_plugin_qsqlite.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_plugin_qsqlodbc.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_plugin_qsqlodbc.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_plugin_qwindows.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_plugin_qwindows.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules-inst/qt_plugin_windowsprintersupport.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_plugin_windowsprintersupport.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_tool_moc.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_tool_qdoc.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_tool_qlalr.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_tool_rcc.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_tool_syncqt.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/modules/qt_tool_uic.pri \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/qt_functions.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/qt_config.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/win32/qt_config.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/win32-g++/qmake.conf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/spec_post.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/exclusive_builds.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/default_pre.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/win32/default_pre.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/resolve_config.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/exclusive_builds_post.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/default_post.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/win32/rtti.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/precompile_header.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/warn_on.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/qt.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/resources.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/moc.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/win32/opengl.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/uic.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/win32/windows.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/testcase_targets.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/exceptions.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/yacc.prf \
		../../../../tools/qt/qt-32/qtbase/mkspecs/features/lex.prf \
		NiceCopier.pro \
		E:/programming/tools/qt/qt-32/qtbase/lib/qtmain.prl \
		E:/programming/tools/qt/qt-32/qtbase/lib/Qt5Core.prl \
		E:/programming/tools/qt/qt-32/qtbase/lib/Qt5Widgets.prl \
		E:/programming/tools/qt/qt-32/qtbase/lib/Qt5Gui.prl \
		E:/programming/tools/qt/qt-32/qtbase/lib/qtharfbuzzng.prl \
		E:/programming/tools/qt/qt-32/qtbase/lib/Qt5Network.prl \
		E:/programming/tools/qt/qt-32/qtbase/lib/Qt5Xml.prl \
		E:/programming/tools/qt/qt-32/qtbase/plugins/platforms/qwindows.prl \
		E:/programming/tools/qt/qt-32/qtbase/lib/Qt5PlatformSupport.prl \
		E:/programming/tools/qt/qt-32/qtbase/plugins/imageformats/qico.prl \
		E:/programming/tools/qt/qt-32/qtbase/plugins/bearer/qgenericbearer.prl \
		E:/programming/tools/qt/qt-32/qtbase/plugins/bearer/qnativewifibearer.prl
	$(QMAKE) -spec win32-g++ -o Makefile NiceCopier.pro
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\spec_pre.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\qdevice.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\device_config.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\common\shell-win32.conf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\qconfig.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_bootstrap_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_bootstrap_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_concurrent.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_concurrent_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_concurrent.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_core.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_core_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_core.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_gui.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_gui_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_gui.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_network.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_network_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_network.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_opengl.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_opengl_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_opengl.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_openglextensions.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_openglextensions_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_openglextensions.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_platformsupport_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_platformsupport_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_printsupport.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_printsupport_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_printsupport.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_sql.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_sql_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_sql.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_testlib.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_testlib_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_testlib.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_widgets.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_widgets_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_widgets.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_xml.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_lib_xml_private.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_lib_xml.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_plugin_qgenericbearer.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_plugin_qgenericbearer.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_plugin_qico.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_plugin_qico.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_plugin_qminimal.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_plugin_qminimal.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_plugin_qnativewifibearer.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_plugin_qnativewifibearer.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_plugin_qsqlite.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_plugin_qsqlite.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_plugin_qsqlodbc.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_plugin_qsqlodbc.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_plugin_qwindows.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_plugin_qwindows.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules-inst\qt_plugin_windowsprintersupport.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_plugin_windowsprintersupport.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_tool_moc.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_tool_qdoc.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_tool_qlalr.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_tool_rcc.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_tool_syncqt.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\modules\qt_tool_uic.pri:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\qt_functions.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\qt_config.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\win32\qt_config.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\win32-g++\qmake.conf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\spec_post.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\exclusive_builds.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\default_pre.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\win32\default_pre.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\resolve_config.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\exclusive_builds_post.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\default_post.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\win32\rtti.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\precompile_header.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\warn_on.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\qt.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\resources.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\moc.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\win32\opengl.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\uic.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\win32\windows.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\testcase_targets.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\exceptions.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\yacc.prf:
..\..\..\..\tools\qt\qt-32\qtbase\mkspecs\features\lex.prf:
NiceCopier.pro:
E:/programming/tools/qt/qt-32/qtbase/lib/qtmain.prl:
E:/programming/tools/qt/qt-32/qtbase/lib/Qt5Core.prl:
E:/programming/tools/qt/qt-32/qtbase/lib/Qt5Widgets.prl:
E:/programming/tools/qt/qt-32/qtbase/lib/Qt5Gui.prl:
E:/programming/tools/qt/qt-32/qtbase/lib/qtharfbuzzng.prl:
E:/programming/tools/qt/qt-32/qtbase/lib/Qt5Network.prl:
E:/programming/tools/qt/qt-32/qtbase/lib/Qt5Xml.prl:
E:/programming/tools/qt/qt-32/qtbase/plugins/platforms/qwindows.prl:
E:/programming/tools/qt/qt-32/qtbase/lib/Qt5PlatformSupport.prl:
E:/programming/tools/qt/qt-32/qtbase/plugins/imageformats/qico.prl:
E:/programming/tools/qt/qt-32/qtbase/plugins/bearer/qgenericbearer.prl:
E:/programming/tools/qt/qt-32/qtbase/plugins/bearer/qnativewifibearer.prl:
qmake: FORCE
	@$(QMAKE) -spec win32-g++ -o Makefile NiceCopier.pro

qmake_all: FORCE

make_first: release-make_first debug-make_first FORCE
all: release-all debug-all FORCE
clean: release-clean debug-clean FORCE
distclean: release-distclean debug-distclean FORCE
	-$(DEL_FILE) Makefile
	-$(DEL_FILE) E:\programming\projects\nicecopier\application\NiceCopier\nicecopier_plugin_import.cpp

release-mocclean:
	$(MAKE) -f $(MAKEFILE).Release mocclean
debug-mocclean:
	$(MAKE) -f $(MAKEFILE).Debug mocclean
mocclean: release-mocclean debug-mocclean

release-mocables:
	$(MAKE) -f $(MAKEFILE).Release mocables
debug-mocables:
	$(MAKE) -f $(MAKEFILE).Debug mocables
mocables: release-mocables debug-mocables

check: first
FORCE:

$(MAKEFILE).Release: Makefile
$(MAKEFILE).Debug: Makefile
