find_program(MSGFMT_EXE
        msgfmt
        PATHS ${BIN_DIR}/gettext/bin)


file(GLOB PO_FILES "${TMP_LANG_DIR}/translations/rigs-of-rods.rorpot/*.po")

foreach (po_file ${PO_FILES})
    get_filename_component(filename ${po_file} NAME_WE)

    message("Now compiling ${filename}")
    file(MAKE_DIRECTORY ${LANG_DIR}/${filename}/LC_MESSAGES/)
    execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy ${po_file} ${LANG_DIR}/${filename}/LC_MESSAGES/ror.po
            COMMAND ${MSGFMT_EXE} -o ${LANG_DIR}/${filename}/LC_MESSAGES/ror.mo ${LANG_DIR}/${filename}/LC_MESSAGES/ror.po
    )
    file(REMOVE ${LANG_DIR}/${filename}/LC_MESSAGES/ror.po)
endforeach ()