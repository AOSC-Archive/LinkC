SET(SRC_LIST    main.c
                network/network.c
                network/linkc_TCP_system/linkc_TCP_io.c
                network/package/package_ctl.c
                error/linkc_error.c
                db/linkc_db.c
                )

ADD_EXECUTABLE (Server ${SRC_LIST})
SET(EXECUTABLE_OUTPUT_PATH ../)
TARGET_LINK_LIBRARIES (Server pthread sqlite3 ssl crypto)

