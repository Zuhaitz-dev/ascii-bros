CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -Isrc
LDFLAGS  = -no-pie

SRCDIR   = src
BUILDDIR = build
SRCS     = $(wildcard $(SRCDIR)/*.cpp)
OBJS     = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))
TARGET   = ascii-bros

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR) $(TARGET)

extract-levels:
	@if [ -z "$(SMB_DISASM)" ]; then \
		echo "Set SMB_DISASM to your local SMBDIS-style disassembly path"; \
		echo "Example: make extract-levels SMB_DISASM=../smb1/src/levels/levels.asm"; \
		exit 1; \
	fi
	python3 tools/extract_smb_levels.py "$(SMB_DISASM)" --out levels
