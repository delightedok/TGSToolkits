set(_TGSTK_COMMS_SOURCE_FILES_CPP
    comm_logger.cpp
    comm_mems.cpp
)

set(TGSTK_COMMS_SOURCE_FILES_CPP )
add_head_foreach(TGSTK_COMMS_SOURCE_FILES_CPP "comms" ${_TGSTK_COMMS_SOURCE_FILES_CPP})