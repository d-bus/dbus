if (DBUS_ENABLE_EMBEDDED_TESTS)

set (NAME_TEST_DIR ../../../test/name-test)

add_definitions(${DBUS_INTERNAL_CLIENT_DEFINITIONS})

add_helper_executable(test-pending-call-dispatch ${NAME_TEST_DIR}/test-pending-call-dispatch.c ${DBUS_INTERNAL_LIBRARIES})
add_helper_executable(test-pending-call-timeout ${NAME_TEST_DIR}/test-pending-call-timeout.c ${DBUS_INTERNAL_LIBRARIES})
add_helper_executable(test-thread-init ${NAME_TEST_DIR}/test-threads-init.c ${DBUS_INTERNAL_LIBRARIES})
add_helper_executable(test-ids ${NAME_TEST_DIR}/test-ids.c ${DBUS_INTERNAL_LIBRARIES})
add_helper_executable(test-shutdown ${NAME_TEST_DIR}/test-shutdown.c dbus-testutils)
add_helper_executable(test-privserver-client ${NAME_TEST_DIR}/test-privserver-client.c dbus-testutils)
add_helper_executable(test-autolaunch ${NAME_TEST_DIR}/test-autolaunch.c dbus-testutils)
endif (DBUS_ENABLE_EMBEDDED_TESTS)
