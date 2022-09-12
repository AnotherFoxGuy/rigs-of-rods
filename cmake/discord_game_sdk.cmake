if (USE_DISCORDGAMESDK)
    include(FetchContent)

    FetchContent_Declare(discord_game_sdk
            URL https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip
            URL_MD5 f86f15957cc9fbf04e3db10462027d58
            )
    FetchContent_MakeAvailable(discord_game_sdk)


    file(GLOB_RECURSE src_files CONFIGURE_DEPENDS "${discord_game_sdk_SOURCE_DIR}/cpp/*.cpp" "${discord_game_sdk_SOURCE_DIR}/cpp/*.h")
    add_library(internal_discord_game_sdk STATIC ${src_files})
    if (WIN32)
        target_link_libraries(internal_discord_game_sdk PUBLIC "${discord_game_sdk_SOURCE_DIR}/lib/x86_64/discord_game_sdk.dll.lib")
    else()
        target_link_libraries(internal_discord_game_sdk PUBLIC "${discord_game_sdk_SOURCE_DIR}/lib/x86_64/discord_game_sdk.so")
    endif ()
    target_include_directories(internal_discord_game_sdk PUBLIC "${discord_game_sdk_SOURCE_DIR}/cpp/")
endif ()