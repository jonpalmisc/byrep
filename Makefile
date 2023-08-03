PREFIX		?= /usr/local

CXXFLAGS	?= -std=c++20
CXXFLAGS	+= -Wall -Wextra -Wpedantic -Werror

ifneq ($(DEBUG),)
  CXXFLAGS	+= -g -fsanitize=address
else
  CXXFLAGS	+= -O2
endif

# ===----------------------------------------------------------------------=== #

BYREP_T		:= byrep
BYREP_S		:= $(wildcard *.cpp)
BYREP_O		:= $(BYREP_S:.cpp=.o)
BYREP_D		:= $(BYREP_O:.o=.d)

.PHONY: all
all: $(BYREP_T)

$(BYREP_T): $(BYREP_O)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -o $@ -c $<

-include $(BYREP_D)

# ===----------------------------------------------------------------------=== #

.PHONY: install
install: $(BYREP_T)
	install $(BYREP_T) $(PREFIX)/bin/byrep

.PHONY: clean
clean:
	rm -fr $(BYREP_T) $(BYREP_O) $(BYREP_D)
