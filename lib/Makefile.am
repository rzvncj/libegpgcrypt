AM_CPPFLAGS = $(GPGME_CFLAGS)


lib_LTLIBRARIES = libegpgcrypt.la

libegpgcrypt_la_SOURCES = init.cpp safe_egpg_object.cpp data_buffer.cpp \
	file_data_buffer.cpp memory_data_buffer.cpp context.cpp crypto.cpp \
	patterns.cpp keys.cpp

noinst_HEADERS = init.hpp patterns.hpp

libegpgcrypt_la_LIBADD = $(GPGME_LIBS)

