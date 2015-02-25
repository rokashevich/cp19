set(SDL2_DIR ${CMAKE_BINARY_DIR}/build)

ExternalProject_Add(SDL2
	PREFIX ${SDL2_DIR}
	CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
	DOWNLOAD_COMMAND ""
	SOURCE_DIR ../SDL
)