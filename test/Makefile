LIB := libpgf2webp.so

CXXFLAGS ?= -fPIC -g -fno-omit-frame-pointer -Wall -Wno-deprecated -std=c++11
PGF_FLAGS := $(shell pkg-config --cflags --libs libpgf)
WEBP_FLAGS := $(shell pkg-config --cflags --libs libwebp)

pgf2webp: ${LIB}
	$(CXX) ${CXXFLAGS} -I.. -L. -lpgf2webp -o $@ main.cpp

${LIB}: ../pgf2webp.cpp
	$(CXX) ${CXXFLAGS} ${PGF_FLAGS} ${WEBP_FLAGS} -shared -o $@ ../pgf2webp.cpp

TEST_FILES := big_buck_bunny.pgf webp_logo.pgf

.PHONY: check
check: pgf2webp
	for f in ${TEST_FILES}; do \
	LD_LIBRARY_PATH="$(shell pwd):${LD_LIBRARY_PATH}" ./pgf2webp $$f $$(echo $$f | sed 's,.pgf,.webp,'); \
	done
	sha1sum -c expected_sha1.txt

.PHONY: clean
clean:
	rm -f ${LIB} pgf2webp $(patsubst %.pgf,%.webp,${TEST_FILES})
