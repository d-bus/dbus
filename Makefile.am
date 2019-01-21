SUBDIRS=dbus bus tools test doc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = dbus-1.pc

cmakeconfigdir = $(libdir)/cmake/DBus1
cmakeconfig_DATA = cmake/DBus1Config.cmake       \
                   cmake/DBus1ConfigVersion.cmake

EXTRA_DIST =			\
	autogen.sh		\
	CONTRIBUTING.md		\
	dbus-1.pc.in		\
	cleanup-man-pages.sh    \
        ChangeLog.pre-1-0       \
        NEWS.pre-1-0            \
	ChangeLog.pre-1-2       \
        NEWS.pre-1-2		\
	README.cmake		\
	README.valgrind		\
	README.win		\
	README.wince		\
	README.cygwin		\
	README.launchd		\
	cmake/CMakeLists.txt \
	cmake/ConfigureChecks.cmake \
	cmake/DBus1Config.cmake.in \
	cmake/DBus1Config.pkgconfig.in \
	cmake/DBus1ConfigVersion.cmake.in \
	cmake/bus \
	cmake/bus-test.bat.cmake \
	cmake/config.h.cmake \
	cmake/dbus \
	cmake/dbus-env.bat.cmake \
	cmake/doc \
	cmake/i686-w64-mingw32.cmake \
	cmake/modules \
	cmake/test \
	cmake/tools \
	$(NULL)

all-local: Doxyfile

update-authors:
	git shortlog -s -e | cut -c 8- | sort > AUTHORS

DISTCHECK_CONFIGURE_FLAGS = \
	--enable-xml-docs \
	--with-systemdsystemunitdir=$$dc_install_base/$(systemdsystemunitdir)

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

# Add rules for code-coverage testing, as defined by AX_CODE_COVERAGE
include $(top_srcdir)/aminclude_static.am
