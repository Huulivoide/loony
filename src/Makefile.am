AM_CFLAGS = -Wall -Wextra -DSCRIPTDIR=\"$(datadir)/@PACKAGE@/lua\"
AM_LDFLAGS = -lm -lncurses -llua
bin_PROGRAMS = loony
loony_SOURCES = main.c \
				cursesio.c cursesio.h \
				inttrie.c inttrie.h \
				lua_api.c lua_api.h \
				textbuf.c textbuf.h \
				util.c util.h \
				window.c window.h
