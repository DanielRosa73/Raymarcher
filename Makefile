CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -Isrc -fopenmp
LDFLAGS = -lm
SRCDIR = src
OBJDIR = obj
SRCS = $(shell find $(SRCDIR) -name "*.cpp")
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# Ensure the object directory exists
$(shell mkdir -p $(dir $(OBJS)) >/dev/null)

raymarcher: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJDIR)/**/*.o raymarcher
