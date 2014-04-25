
find_package(Threads REQUIRED)

add_definitions(" -std=c++11  -DHAVE_PTHREADS  ")

if ( BUILD_SNPRINTF )
    SET (SNPRINTF_SOURCES
        snprintf.cc 
        snprintf.h c
        defs.h
    )
endif ( BUILD_SNPRINTF )


set( UTIL_INCLUDES
    bitop.h
    bitop_funcs.h
    segment_mem.h
    sflsq.h
    sfmemcap.h
    sfportobject.h
    snort_bounds.h
    stats.h
)

ADD_LIBRARY( utils STATIC
    ${UTIL_INCLUDES}
    ${SNPRINTF_SOURCES}
    asn1.cc 
    asn1.h
    dyn_array.cc
    dyn_array.h
    ring.h 
    ring_logic.h
    segment_mem.cc 
    sf_email_attach_decode.cc 
    sf_email_attach_decode.h
    sf_base64decode.cc 
    sf_base64decode.h
    sflsq.cc 
    sfmemcap.cc 
    sfportobject.cc 
    sfsnprintfappend.cc 
    sfsnprintfappend.h
    strvec.cc 
    strvec.h
    stats.cc 
    util.cc 
    util.h
    util_jsnorm.cc 
    util_jsnorm.h
    util_math.cc 
    util_math.h
    util_net.cc 
    util_net.h
    util_str.cc 
    util_str.h
    util_utf.cc 
    util_utf.h
    util_unfold.cc 
    util_unfold.h
)


set_default_visibility_compile_flag(utils)

target_link_libraries(utils
    log
    ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties( utils PROPERTIES
    COMPILE_FLAGS "-std=c++11 -DHAVE_PTHREADS  -pthread"
    LINK_FLAGS "-pthread -lpthread pthreads -lpthreads"
)

install (FILES ${UTIL_INCLUDES}
    DESTINATION "${INCLUDE_INSTALL_PATH}/utils"
)



