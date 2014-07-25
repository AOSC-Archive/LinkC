SET(SRC_LIST    main.c
                network/network.c
                network/linkc_TCP_system/linkc_TCP_io.c
                network/package/package_ctl.c
                network/p2p/conn_list.c
                network/p2p/p2p_helper.c
                network/p2p/list.c
                error/linkc_error.c
                db/linkc_db.c
                core/linkc_server.c
                )

ADD_EXECUTABLE (Server ${SRC_LIST})
SET(EXECUTABLE_OUTPUT_PATH ../)
TARGET_LINK_LIBRARIES (Server pthread sqlite3 ssl crypto )

