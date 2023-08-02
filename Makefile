CXXFLAGS	?= -std=c++20
CXXFLAGS	+= -Wall -Wextra -Wpedantic -Werror
CXXFLAGS	+= -g -O1

ifneq ($(ASAN),)
  CXXFLAGS	+= -fsanitize=address
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

.PHONY: clean
clean:
	rm -fr $(BYREP_T) $(BYREP_O) $(BYREP_D)
