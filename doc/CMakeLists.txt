#
# api documentation
#
find_package(Doxygen)

# build doc always
add_custom_target(doc ALL)

if(DOXYGEN_EXECUTABLE)
  option(DBUS_ENABLE_DOXYGEN_DOCS "build DOXYGEN documentation (requires Doxygen)" ON)
endif(DOXYGEN_EXECUTABLE)

if(DBUS_ENABLE_DOXYGEN_DOCS)
    set(top_srcdir ${CMAKE_SOURCE_DIR})
    configure_file(../Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile )
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/api/html/index.html
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    add_custom_target(apidoc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    add_dependencies(doc apidoc)
    add_custom_target(devhelp2
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/api/html/index.html ${CMAKE_CURRENT_BINARY_DIR}/dbus.devhelp2
    )
    add_dependencies(doc devhelp2)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/api/html/ DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/dbus/api)
endif(DBUS_ENABLE_DOXYGEN_DOCS)

find_program(XSLTPROC_EXECUTABLE xsltproc)
if(XSLTPROC_EXECUTABLE)
    message(STATUS "${XSLTPROC_EXECUTABLE} found")
    set(DBUS_HAVE_XSLTPROC 1)
endif()

if(DBUS_HAVE_XSLTPROC)
    add_custom_command(
        OUTPUT dbus.devhelp2
        COMMAND ${XSLTPROC_EXECUTABLE} -o dbus.devhelp2 ${CMAKE_CURRENT_SOURCE_DIR}/doxygen_to_devhelp.xsl api/xml/index.xml
        DEPENDS doxygen_to_devhelp.xsl api/html/index.html
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dbus.devhelp2 DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/dbus)
endif()

#
# docbook based documentation
# 
find_package(DocBookXSL)

if(DOCBOOKXSL_DIR AND XSLTPROC_EXECUTABLE)
    option(DBUS_ENABLE_XML_DOCS "build XML documentation" ON)
    set(DBUS_XML_DOCS_ENABLED 1)
    message(STATUS "xsltproc docbook generator found")
    add_custom_target(xmldoc)
    add_dependencies(doc xmldoc)
endif()

if(DBUS_ENABLE_XML_DOCS)

macro(DOCBOOK _sources _format)
    get_filename_component(_infile ${_sources} ABSOLUTE)
    get_filename_component(_name ${_infile} NAME)
    if(${_format} STREQUAL "man")
        string(REPLACE ".xml" "" _outname ${_name})
        set(STYLESHEET "${DOCBOOKXSL_DIR}/manpages/docbook.xsl")
    else()
        string(REPLACE ".xml" ".html" _outname ${_name})
        set(STYLESHEET "${DOCBOOKXSL_DIR}/html/docbook.xsl")
    endif()
    set(_outfile ${CMAKE_CURRENT_BINARY_DIR}/${_outname})
    if(EXISTS ${_sources})
        if(XSLTPROC_EXECUTABLE)
            add_custom_target(${_outname}
                ${XSLTPROC_EXECUTABLE} --output ${_outfile} --nonet --xinclude  --param passivetex.extensions '1' ${STYLESHEET} ${_infile}
                DEPENDS ${_infile}
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            )
        endif()
        add_dependencies(xmldoc ${_outname})
        if(${_format} STREQUAL "man")
            install(FILES ${_outfile} DESTINATION ${CMAKE_INSTALL_DATADIR}/man/man1)
        else()
            install(FILES ${_outfile} DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/dbus)
        endif()
    else()
        message(STATUS "skipping xml doc generating for ${_infile}, file not found")
    endif()
endmacro(DOCBOOK)

### copy tests to builddir so that generated tests and static tests 
### are all in one place.
### todo how to add more filetypes 
macro(COPYDIR _src _type)
    foreach(FILE_TYPE ${_type})
        foreach(DIR ${_src})
            file(GLOB FILES "${DIR}/${FILE_TYPE}" )
            file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${DIR})
            foreach(FILE ${FILES})
                get_filename_component(FILENAME ${FILE} NAME)
                set(TARGET ${CMAKE_BINARY_DIR}/${DIR}/${FILENAME})
                configure_file(${FILE} ${TARGET} COPYONLY)
                if(CONFIG_VERBOSE)
                    message("FROM: ${FILE}\nTO: ${TARGET}\n")
                endif(CONFIG_VERBOSE)
            endforeach(FILE)
        endforeach(DIR)
    endforeach(FILE_TYPE)
endmacro(COPYDIR)

COPYDIR(doc *.png)
COPYDIR(doc *.svg)

docbook(${CMAKE_CURRENT_SOURCE_DIR}/dbus-test-plan.xml html)
docbook(${CMAKE_CURRENT_SOURCE_DIR}/dbus-tutorial.xml html)
docbook(${CMAKE_CURRENT_SOURCE_DIR}/dbus-specification.xml html)
docbook(${CMAKE_CURRENT_SOURCE_DIR}/dbus-faq.xml html)

configure_file(dbus-cleanup-sockets.1.xml.in ${CMAKE_CURRENT_BINARY_DIR}/dbus-cleanup-sockets.1.xml)
configure_file(dbus-daemon.1.xml.in ${CMAKE_CURRENT_BINARY_DIR}/dbus-daemon.1.xml)
configure_file(dbus-launch.1.xml.in ${CMAKE_CURRENT_BINARY_DIR}/dbus-launch.1.xml)
configure_file(dbus-monitor.1.xml.in ${CMAKE_CURRENT_BINARY_DIR}/dbus-monitor.1.xml)
configure_file(dbus-send.1.xml.in ${CMAKE_CURRENT_BINARY_DIR}/dbus-send.1.xml)
configure_file(dbus-test-tool.1.xml.in ${CMAKE_CURRENT_BINARY_DIR}/dbus-test-tool.1.xml)
configure_file(dbus-update-activation-environment.1.xml.in ${CMAKE_CURRENT_BINARY_DIR}/dbus-update-activation-environment.1.xml)
configure_file(dbus-uuidgen.1.xml.in ${CMAKE_CURRENT_BINARY_DIR}/dbus-uuidgen.1.xml)
if(UNIX)
    docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-daemon.1.xml man)
    docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-monitor.1.xml man)
    docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-send.1.xml man)
    docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-test-tool.1.xml man)
    docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-launch.1.xml man)
    docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-uuidgen.1.xml man)
    docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-cleanup-sockets.1.xml man)
    docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-update-activation-environment.1.xml man)
endif()
docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-daemon.1.xml html)
docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-monitor.1.xml html)
docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-send.1.xml html)
docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-test-tool.1.xml html)
docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-launch.1.xml html)
docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-uuidgen.1.xml html)
docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-cleanup-sockets.1.xml html)
docbook(${CMAKE_CURRENT_BINARY_DIR}/dbus-update-activation-environment.1.xml html)

#
# handle html index file
#
configure_file(index.html.in ${CMAKE_CURRENT_BINARY_DIR}/index.html)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/index.html DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/dbus)

set(EXTRA_DIST
    busconfig.dtd
    introspect.dtd
    introspect.xsl
)

install(FILES ${EXTRA_DIST} DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/dbus)

endif()
