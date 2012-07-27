BINARY   = p9c

CXX      = g++
MKDIR    = mkdir
RM       = rm

CXXFLAGS = -g

SRCS     = $(shell find sources -name '*.cc')
HDRS     = $(shell find includes -name '*.hh')
OBJS     = $(addprefix build/objects/,$(SRCS:.cc=.o))
DEPS     = $(addprefix build/dependencies/,$(SRCS:.cc=.d) $(HDRS:.hh=.d))

PURPLE   = $(shell printf "\033[35m")
CYAN     = $(shell printf "\033[36m")
GREEN    = $(shell printf "\033[32m")
BROWN    = $(shell printf "\033[33m")
EOS      = $(shell printf "\033[00m")

CXXFLAGS += $(shell llvm-config --cxxflags) -fexceptions -std=c++11 -I./libp9/includes -I./includes -I.
LDFLAGS  += $(shell llvm-config --ldflags) $(shell llvm-config --libs core jit native) -L./libp9/build -lP9Engine -lP9Parse

all: $(BINARY)
	@printf "Compilation done, output is build/${BINARY}\n"

$(BINARY): build/$(BINARY)

-include $(DEPS)

build/$(BINARY): $(OBJS) libp9/build/libP9Parse.a libp9/build/libP9Engine.a
	@printf "%s# Linking final executable.%s\n" "${PURPLE}" "${EOS}"
	@${CXX} -o build/${BINARY} ${OBJS} ${LDFLAGS}

libp9/build/libP9Parse.a libp9/build/libP9Engine: build-libp9 ;

build-libp9:
	@$(MAKE) -s -C libp9 libP9Parse.a libP9Engine.a

$(DEPS): build/dependencies/%.d: %.cc
	@printf "%s+ Generating dependency file for %s.%s\n" "${GREEN}" "${<}" "${EOS}"
	@${MKDIR} -p "$(dir ${@})"
	@${CXX} ${CXXFLAGS} -MM -MG -MT "$(patsubst build/dependencies/%,build/objects/%,$(@:.d=.o))" "${<}" > $(@)

$(OBJS): build/objects/%.o: %.cc
	@printf "%s+ Compiling %s.%s\n" "${GREEN}" "${<}" "${EOS}"
	@${MKDIR} -p "$(dir ${@})"
	@${CXX} ${CXXFLAGS} -c -o "${@}" "${<}"

clean:
	@printf "%s- Removing temporary files.%s\n" "${BROWN}" "${EOS}"
	@${RM} -rf build/generated
	@${RM} -rf build/objects

clean-dependencies:
	@printf "%s- Removing dependencies listing.%s\n" "${BROWN}" "${EOS}"
	@${RM} -rf build/dependencies

fclean: clean
	@printf "%s- Removing binary files.%s\n" "${BROWN}" "${EOS}"
	@${RM} -rf build/${BINARY}

re: clean-dependencies fclean
	@$(MAKE) --no-print-directory all

.PHONY: $(BINARY) all clean fclean re clean-depends build-libp9
