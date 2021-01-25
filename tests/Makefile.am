AM_CPPFLAGS = $(GPGME_PTHREAD_CFLAGS)
AM_LDFLAGS = -no-install

noinst_PROGRAMS = buffers_test encrypt decrypt query_keys \
	import_keys sign verify

buffers_test_SOURCES = buffers_test.cpp
encrypt_SOURCES = encrypt.cpp
decrypt_SOURCES = decrypt.cpp
query_keys_SOURCES = query_keys.cpp
import_keys_SOURCES = import_keys.cpp
sign_SOURCES = sign.cpp
verify_SOURCES = verify.cpp

LDADD = ../lib/libegpgcrypt.la
